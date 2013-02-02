/*******************************************************************************

Copyright (c) 2012-2013 Pavel Roschin (aka RPG) <rpg89@post.ru>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:  The above
copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

*******************************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cheetah.h"
#include "macros.h"
#include "render.h"
#include "vertex.h"
#include "SOIL/SOIL.h"

void delay();

typedef struct Resource {
	Image *image;
	unsigned char *data;
	char *name;
	char *options;
	int len;
} Resource;

#define QDATA Resource
	typedef struct node_t node_t, *node, *queue;
	struct node_t {QDATA val; node prev, next;};
#define QHEAD(q) q->prev
#define QTAIL(q) q->next
#define QEMPTY(q) !QHEAD(q)
queue newQueue();
queue resLoaderQueue;

Resource *resShared;

/*******************************************************************************
PRIVATE FUNCTIOS
*******************************************************************************/

static unsigned int loadImageTex(const char *options, unsigned char *img, int width, int height, int channels)
{
	unsigned int tex_id;
	int flags = SOIL_FLAG_TEXTURE_REPEATS;
	NEEDED_INIT;
	CHECK_OPTION(options, nearest);
	CHECK_OPTION(options, clamp);
	if(TRUE == clamp)
		flags = 0;
	tex_id = SOIL_direct_load_DDS_from_memory(img, 0, flags, 0);
	if(0 == tex_id)
		tex_id = SOIL_internal_create_OGL_texture(
			img, width, height, channels,
			0, flags,
			GL_TEXTURE_2D, GL_TEXTURE_2D,
			GL_MAX_TEXTURE_SIZE);
	if(TRUE == nearest)
		TEX_NEAREST;
	SOIL_free_image_data(img);
	prevImageId = 0;
	return tex_id;
}

/**
 * Threaded image loader
 * */

/* Load image in separate thread (if specified) */
static unsigned char * loadImageData(const char *name, int *width, int *height, int *channels)
{
	unsigned int file_size;
	unsigned char *img;
	unsigned char *myBuf;
	NEEDED_INIT;
	myBuf = loadfile(name, &file_size);
	ERROR_IF_NULL(myBuf);
	img = SOIL_load_image_from_memory(
				myBuf, sizeof(unsigned char) * file_size,
				width, height, channels,
				0 );
	if(img != myBuf)
		delete(myBuf);
	return img;
error:
	myError("cannot load image: empty file %s", name);
	return NULL;
}

queue newQueue()
{
	node q = malloc(sizeof(node_t));
	q->next = q->prev = 0;
	return q;
}

static void enqueue(queue q, QDATA n)
{
	//~ printf("%s\n", n.image->name);
	//~ SDL_mutexP(resQueueMutex);
	node nd = malloc(sizeof(node_t));
	nd->val = n;
	if (!QHEAD(q)) QHEAD(q) = nd;
	nd->prev = QTAIL(q);
	if (nd->prev) nd->prev->next = nd;
	QTAIL(q) = nd;
	nd->next = 0;
	//~ printf("%d\n", QEMPTY(resLoaderQueue));
	//~ SDL_mutexV(resQueueMutex);
}

static int dequeue(queue q, QDATA *val)
{
	//~ SDL_mutexP(resQueueMutex);
	node tmp = QHEAD(q);
	if (NULL == tmp)
		return 0;
	*val = tmp->val;
	QHEAD(q) = tmp->next;
	if (QTAIL(q) == tmp)
		QTAIL(q) = 0;
	free(tmp);
	//~ SDL_mutexV(resQueueMutex);
	return 1;
}

void resLoaderInit(bool resloader)
{
	resLoaderQueue = NULL;
	if(TRUE == resloader)
	{
		resLoaderQueue = newQueue();
		//~ resQueueMutex = SDL_CreateMutex();
		resShared = 0;
	}
}

//TODO
/**
 * Separate thread to load images
 * */
int resLoaderThread(void *unused)
{
	Resource r;
	//~ SDL_Event e;
	unsigned char *img;
	//~ unsigned char *myBuf;
	//~ bool empty;
	int width, height;
	while(1)
	{
		//~ SDL_mutexP(resQueueMutex);
		//~ empty = QEMPTY(resLoaderQueue)
		//~ SDL_mutexV(resQueueMutex);
		delay(10);
		if(!resShared && !QEMPTY(resLoaderQueue))
		{
			//~ printf("Queue: %d\n", QEMPTY(resLoaderQueue));
			//~ SDL_mutexP(resQueueMutex);
			dequeue(resLoaderQueue, &r);
			img = loadImageData(r.image->name, &width, &height, &r.image->channels);
			r.image->w = (float)width;
			r.image->h = (float)height;
			r.data = img;
			//~ printf("%s\n", r.image->name);
			resShared = &r;
			//~ SDL_Delay(5);
			//~ new(e, SDL_Event, 1);
			//~ e.type = SDL_USEREVENT;
			//~ e.user.code = 0;
			//~ e.user.data1 = (void*)&r;
			//~ SDL_PushEvent(&e);
		}
	}
}

/**
 * Runt in the mainthread to just copy image to GPU
 * */
void resLoaderMainThread()
{
	Resource * r;
	if(resShared)
	{
		r = resShared;
		r->image->id = loadImageTex(r->image->options, r->data, r->image->w, r->image->h, r->image->channels);
		delete(r->image->name);
		delete(r->image->options);
		resShared = NULL;
	}
}

static unsigned char * loadImageMask(const unsigned char * img, const char *name, int width, int height, int channels)
{
	char *mask_name;
	const int masklen = 5;
	unsigned char *mask_img;
	unsigned char *new_img;
	int img_len = width * height;
	int mask_w, mask_h, mask_channels;
	int mask_step, img_step;
	int x, y, j, i;
	/* gen mask name */
	char *pch = strrchr(name, '.');
	mask_name = (char*)malloc(strlen(name) + masklen + 1);
	strncpy(mask_name, name, pch - name);
	strcpy(mask_name + (pch - name), "_mask");
	strcpy(mask_name + (pch - name) + masklen, pch);
	/* try to load mask */
	mask_img = loadImageData(mask_name, &mask_w, &mask_h, &mask_channels);
	if(mask_img)
	{
		new_img = (unsigned char *)malloc(sizeof(unsigned char) * img_len * 4);
		ERROR_IF_NULL(new_img);
		for(x = y = i = j = mask_step = img_step = 0; i < img_len; i++)
		{
			new_img[j++] = img[img_step++];
			new_img[j++] = img[img_step++];
			new_img[j++] = img[img_step++];
			if(channels == 4)
				img_step++;
			new_img[j++] = 0xff - mask_img[(y * mask_h + x) * mask_channels];
			x++;
			if(x >= mask_h)
				x = 0;
			mask_step++;
			if(mask_step >= width)
			{
				x = mask_step = 0;
				y++;
				if(y >= mask_h)
					y = 0;
			}
		}
		free(mask_img);
		free(mask_name);
		return new_img;
	}
	free(mask_name);
	return NULL;
error:
	myError("Cannot create mask image!");
	return NULL;
}

static void imageCheckResLoader(Image * image)
{
	if(resLoaderQueue && image->id == null_texture && !image->queued)
	{
		Resource r;
		memset(&r, 0, sizeof(Resource));
		image->queued = 1;
		r.image = image;
		enqueue(resLoaderQueue, r);
	}
}

static void multitextureBind(Multitexture * multitexture)
{
	//~ if(prevImageId == image->id) return;
	FLUSH_BUFFER();
	Image * image;
	int i;
	for(i = 0; i < multitexture->size; i++)
	{
		image = multitexture->images[i];
		glActiveTexture_(GL_TEXTURE0 + i);
		imageCheckResLoader(image);
		if (image->id)
			glBindTexture(GL_TEXTURE_2D, image->id);
	}
	prevImageId = 0;
	glActiveTexture_(GL_TEXTURE0);
}

/*******************************************************************************
PUBLIC FUNCTIOS
*******************************************************************************/

/* Load image from disk */
void newImageOpt(Image *ptr, const char *name, const char *options) {
	int width, height, channels;
	unsigned int tex_id;
	unsigned char *img;
	unsigned char *img_mask;
	NEEDED_INIT_VOID;
	if(!name)
	{
		myError("empty filename");
		return;
	}
	CHECK_OPTION(options, instant);
	CHECK_OPTION(options, mask);
	if(FALSE == resLoaderQueue || TRUE == instant)
	{
		img = loadImageData(name, &width, &height, &channels);
		if(img == NULL)
		{
			myError("can't load image %s", name);
			return;
		}
		if(TRUE == mask)
		{
			img_mask = loadImageMask(img, name, width, height, channels);
			if(img_mask)
			{
				channels = 4;
				free(img);
				img = img_mask;
			}
		}
		tex_id = loadImageTex(options, img, width, height, channels);
		ptr->id = tex_id;
		ptr->w = (float)width;
		ptr->h = (float)height;
	}
	else
	{
		new(ptr->name, char, strlen(name)+1);
		new(ptr->options, char, strlen(options)+1);
		memcpy(ptr->name, name, strlen(name)+1);
		memcpy(ptr->options, options, strlen(options)+1);
		ptr->id = null_texture;
		ptr->w = 1;
		ptr->h = 1;
		ptr->queued = 0;
	}
}

/**
 * @descr Load image from raw memory. By default image must be 3 bytes per pixel RGB
 * @group image
 * @var width of image
 * @var height of image
 * @var string of options. This is depends on image loading module you use. Supported options:
 *  * _n_ - use nearest interpolation
 *  * _a_ - image with alpha (4 bytes per pixel)
 * @return Image object
 * @advanced
 * */
void newImageRaw(Image *ptr, int width, int height, const char *data, const char *options) {
	unsigned int tex_id;
	GLenum format = GL_RGB;
	const int level = 0;
	const int border = 0;
	NEEDED_INIT_VOID;
	CHECK_OPTION(options, nearest);
	CHECK_OPTION(options, alpha);
	CHECK_OPTION(options, clamp);
	if(TRUE == alpha)
		format = GL_RGBA;
	glGenTextures(1, &tex_id);
	TEXTURE_BIND(tex_id);
	if(TRUE == nearest)
		TEX_NEAREST;
	else
		TEX_LINEAR;
	if(TRUE == clamp)
		TEX_CLAMP;
	else
		TEX_REPEAT;
	glTexImage2D(GL_TEXTURE_2D, level, format, width, height, border,
		format, GL_UNSIGNED_BYTE, (void *)data);
	ptr->w = (float)width;
	ptr->h = (float)height;
	ptr->id = tex_id;
}

/**
 * @descr Bind Image object. Equivalent to glBindTexture.
 * @group image
 * @var Image object
 * @advanced
 * */
void imageBind(Image * image) {
	if(!image) return;
	imageCheckResLoader(image);
	TEXTURE_BIND(image->id);
}

/**
 * @descr Enable texturing. Equivalent to glEnable(GL_TEXTURE_2D).
 * @group image
 * @advanced
 * */
void enableTexture() {
	glEnable(GL_TEXTURE_2D);
}

/**
 * @descr Disable texturing. Equivalent to glDisable(GL_TEXTURE_2D).
 * @group image
 * @advanced
 * */
void disableTexture() {
	glDisable(GL_TEXTURE_2D);
}

void imageDrawxy(Image * image, float x, float y, float w, float h) {
	imageBind(image);
	PUSH_QUAD(x,y,w,h,0,0,0);
}

void imageDrawt(Image * image, float x, float y, float w, float h, float a, float ox, float oy) {
	imageBind(image);
	PUSH_QUAD(x,y,w,h,a,ox,oy);
}

void imageDrawqxy(Image * image, float x, float y, float w, float h, float qx, float qy, float qw, float qh) {
	imageBind(image);
	PUSH_QUADT(x,y,w,h,0,0,0,qx, qy, qw, qh, image->w, image->h);
}

void imageDrawqt(Image * image, float x, float y, float w, float h, float qx, float qy, float qw, float qh, float a, float ox, float oy) {
	imageBind(image);
	PUSH_QUADT(x,y,w,h,a,ox,oy,qx, qy, qw, qh, image->w, image->h);
}

#define borderImageDrawInternal(borderImage, x, y, w, h, a, ox, oy) do {\
	imageBind(borderImage->image);\
	float ow = borderImage->image->w;\
	float oh = borderImage->image->h;\
	float t  = borderImage->top;\
	float r  = borderImage->right;\
	float b  = borderImage->bottom;\
	float l  = borderImage->left;\
	if(t > 0.0)\
	{\
		PUSH_QUADT(x,  y,  l,          t,          a, ox,         oy,          0,       0,       l,            t,          ow, oh);\
		PUSH_QUADT(x,  y,  w - l - r,  t,          a, ox - l,     oy,          l,       0,       ow - l - r,   t,          ow, oh);\
		PUSH_QUADT(x,  y,  r,          t,          a, ox - w + r, oy,          ow - r,  0,       r,            t,          ow, oh);\
	}\
	PUSH_QUADT(x,    y,      l,      h - t - b,  a, ox,         oy - t,      0,       t,        l,           oh - t - b, ow, oh);\
	if(FALSE == borderImage->borderOnly)\
		PUSH_QUADT(x,  y,  w - l - r,  h - t - b,  a, ox - l,     oy - t,      l,       t,        ow - l - r,  oh - t - b, ow, oh);\
	PUSH_QUADT(x,    y,  r,          h - t - b,  a, ox - w + r, oy - t,      ow - r,  t,        r,           oh - t - b, ow, oh);\
	if(b > 0.0)\
	{\
		PUSH_QUADT(x,  y,  l,          b,          a, ox,         oy - h + b,  0,       oh - b,   l,           b,          ow, oh);\
		PUSH_QUADT(x,  y,  w - l - r,  b,          a, ox - l,     oy - h + b,  l,       oh - b,   ow - l - r,  b,          ow, oh);\
		PUSH_QUADT(x,  y,  r,          b,          a, ox - w + r, oy - h + b,  ow - r,  oh - b,   r,           b,          ow, oh);\
	}\
} while(0)

void borderImageDrawt(BorderImage * borderImage, float x, float y, float w, float h, float a, float ox, float oy) {
	borderImageDrawInternal(borderImage, x, y, w, h, a, ox, oy);
}

void borderImageDrawxy(BorderImage * borderImage, float x, float y, float w, float h) {
	borderImageDrawInternal(borderImage, x, y, w, h, 0, 0, 0);
}

void multitextureDrawxy(Multitexture * multitexture, float x, float y, float w, float h) {
	multitextureBind(multitexture);
	PUSH_QUAD(x,y,w,h,0,0,0);
}

void multitextureDrawt(Multitexture * multitexture, float x, float y, float w, float h, float a, float ox, float oy) {
	multitextureBind(multitexture);
	PUSH_QUAD(x,y,w,h,a,ox,oy);
}

void multitextureDrawqxy(Multitexture * multitexture, float x, float y, float w, float h, float qx, float qy, float qw, float qh) {
	multitextureBind(multitexture);
	PUSH_QUADT(x,y,w,h,0,0,0,qx, qy, qw, qh, multitexture->w, multitexture->h);
}

void multitextureDrawqt(Multitexture * multitexture, float x, float y, float w, float h, float qx, float qy, float qw, float qh, float a, float ox, float oy) {
	multitextureBind(multitexture);
	PUSH_QUADT(x,y,w,h,a,ox,oy,qx, qy, qw, qh, multitexture->w, multitexture->h);
}

//~ /**
 //~ * @descr Set the current active texture for multitexturenig. Equivalent to glActiveTexture(GL_TEXTURE0 + i).
 //~ * @group image
 //~ * @var number of texture slot (min 0, max 7)
 //~ * */
//~ void activeTexture(int i) {
	//~ glActiveTexture_(GL_TEXTURE0 + i);
//~ }

//~ /**
 //~ * @descr Enable/disable smooth interpolation for image. Disabled filtering useful, if you want to fit image to pixel matrix. If this image will be scaled and/or rotated you must enable filtering (default). Must be called before texture loading.
 //~ * @group image
 //~ * @var Image object
 //~ * @var true means that filtering is enabled, false means that filtering is disabled
 //~ * */
//~ void imageFiltering(Image * img, bool enabled) {
	//~ glBindTexture(GL_TEXTURE_2D, img->id);
	//~ if(!enabled)
		//~ TEX_NEAREST;
	//~ else
		//~ TEX_LINEAR;
//~ }

//~ void _newImageFromData(Image * ptr, ImageData * imgdata, const char *options) {
	//~ unsigned int tex_id;
	//~ if(!imgdata || !imgdata->data) MYERROR("invalid data");
	//~ ptr->w = (float)imgdata->w;
	//~ ptr->h = (float)imgdata->h;
	//~ ptr->channels = imgdata->channels;
	//~ tex_id = SOIL_internal_create_OGL_texture(
			//~ (unsigned char*)imgdata->data, ptr->w, ptr->h, ptr->channels,
			//~ 0, SOIL_FLAG_TEXTURE_REPEATS,
			//~ GL_TEXTURE_2D, GL_TEXTURE_2D,
			//~ GL_MAX_TEXTURE_SIZE);
	//~ ptr->id = tex_id;
	//~ while(*options)
	//~ {
		//~ if(*options == 'n') TEX_NEAREST;
		//~ options++;
	//~ }
//~ }



/**
 * @descr Free GPU memory occupied by Image.
 * @var Image object
 * @advanced
 * */
void deleteImage(Image * ptr) {
	//~ printf("%d\n", same_type_p(typeof(ptr)) == INTEGER_TYPE);
	#ifdef MEMORY_TEST
		printf("Freeing Image %d\n", ptr->id);
	#endif
	if(ptr && ptr->id > 1) glDeleteTextures(1, &ptr->id);

	//~ else MYERROR("Trying to free a null-image. Maybe, you did it manually?");
}

