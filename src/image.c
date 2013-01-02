/*******************************************************************************

Copyright (c) 2012 Pavel Roschin (aka RPG) <rpg89@post.ru>

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
#include "cheetah.h"
#include "SOIL/SOIL.h"
#include "render.h"

inline unsigned int loadImageTex(const char *options, unsigned char *img, int width, int height, int channels)
{
	unsigned int tex_id;
	NEDED_INIT;
	tex_id = SOIL_direct_load_DDS_from_memory(img,0,0,0);
	if(!tex_id)
		tex_id = SOIL_internal_create_OGL_texture(
			img, width, height, channels,
			0, SOIL_FLAG_TEXTURE_REPEATS,
			GL_TEXTURE_2D, GL_TEXTURE_2D,
			GL_MAX_TEXTURE_SIZE);
	while(*options)
	{
		if(*options == 'n') TEX_NEAREST;
		options++;
	}
	SOIL_free_image_data(img);
	return tex_id;
}

/**
 * Threaded image loader
 * */

/* Load image in separate thread (if specified) */
inline unsigned char * loadImageData(const char *name, int *width, int *height, int *channels)
{
	unsigned int file_size;
	unsigned char *img;
	unsigned char *myBuf;
	NEDED_INIT;
	myBuf = loadfile(name, &file_size);
	if(!myBuf)
	{
		MYERROR("cannot load image: empty file %s", name);
		return NULL;
	}
	img = SOIL_load_image_from_memory(
				myBuf, sizeof(unsigned char) * file_size,
				width, height, channels,
				0 );
	if(img != myBuf)
		delete(myBuf);
	return img;
}

inline queue newQueue()
{
	node q = malloc(sizeof(node_t));
	q->next = q->prev = 0;
	return q;
}

inline void enqueue(queue q, QDATA n)
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
 
inline int dequeue(queue q, QDATA *val)
{
	//~ SDL_mutexP(resQueueMutex);
	node tmp = QHEAD(q);
	if (!tmp) return 0;
	*val = tmp->val;
	//~ printf("%s\n", tmp->val.image->name);
	QHEAD(q) = tmp->next;
	if (QTAIL(q) == tmp) QTAIL(q) = 0;
	free(tmp);
	//~ SDL_mutexV(resQueueMutex);
	return 1;
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
		SDL_Delay(10);
		//~ if(QEMPTY(resLoaderQueue)) printf(": %d\n", QEMPTY(resLoaderQueue));
		if(!resShared&&!QEMPTY(resLoaderQueue))
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
	//~ unsigned int millis;
	if(resShared) {
		r = resShared;
		//~ millis = globalTime;
		r->image->id = loadImageTex(r->image->options, r->data, r->image->w, r->image->h, r->image->channels);
		//~ printf("Delayed resource loader: loaded %s with %d ms\n", r->image->name, SDL_GetTicks() - millis);
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
		if(!new_img)
		{
			MYERROR("Cannot create mask image!");
			return NULL;
		}
		for(x = y = i = j = mask_step = img_step = 0; i < img_len; i++)
		{
			new_img[j++] = img[img_step++];
			new_img[j++] = img[img_step++];
			new_img[j++] = img[img_step++];
			if(channels == 4) img_step++;
			new_img[j++] = 0xff - mask_img[(y * mask_h + x) * mask_channels];
			x++;
			if(x >= mask_h) x = 0;
			mask_step++;
			if(mask_step >= width)
			{
				x = mask_step = 0;
				y++;
				if(y >= mask_h) y = 0;
			}
		}
		free(mask_img);
		free(mask_name);
		return new_img;
	}
	free(mask_name);
	return NULL;
}

/**
 * @descr Load image from disc with specific options.
 * @group graphics/image
 * @var file name
 * @var string of options. This is depends on image loading module you use. Supported options:
 *  * _n_ - use nearest interpolation
 *  * _m_ - generate mip-maps (automatically sets mip-map interpolation)
 *  * _i_ - load instantly without delayed resource loader
 * @return Image object
 * */
void newImageOpt(Image *ptr, const char *name, const char *options) {
	int width, height, channels, i = 0;
	bool mask = 0, instant = 0;
	unsigned int tex_id;
	unsigned char *img;
	unsigned char *img_mask;
	NEDED_INIT_VOID;
	if(!name)
	{
		MYERROR("empty filename");
		return;
	}
	if(options) while(options[i])
	{
		if(options[i] == 'i') instant = 1;
		if(options[i] == 'm') mask = 1;
		i++;
	}
	if(!resLoaderQueue||instant)
	{
		img = loadImageData(name, &width, &height, &channels);
		if(img == NULL)
		{
			MYERROR("can't load image %s", name);
			return;
		}
		if(mask)
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
		//~ new(ptr, Image, 1);
		ptr->id = tex_id;
		ptr->w = (float)width;
		ptr->h = (float)height;
	}
	else
	{
		//~ new(ptr, Image, 1);
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

//~ /**
 //~ * @descr Load image from disc.
 //~ * @group graphics/image
 //~ * @var file name
 //~ * @return Image object
 //~ * */
//~ Image *newImage(const char *name) {
	//~ return newImageOpt(name, "");
//~ }

//~ #endif

inline void imageCheckResLoader(Image * image) {
	if(resLoaderQueue && image->id == null_texture && !image->queued)
	{
		Resource r;
		image->queued = 1;
		r.image = image;
		enqueue(resLoaderQueue, r);
	}
}

/**
 * @descr Bind Image object. Equivalent to glBindTexture.
 * @group graphics/image
 * @var Image object
 * */
inline void imageBind(Image * image) {
	if(!image) return;
	imageCheckResLoader(image);
	TEXTURE_BIND(image->id);
}

/**
 * @descr Enable texturing. Equivalent to glEnable(GL_TEXTURE_2D).
 * @group graphics/image
 * */
void enableTexture() {
	glEnable(GL_TEXTURE_2D);
}

/**
 * @descr Disable texturing. Equivalent to glDisable(GL_TEXTURE_2D).
 * @group graphics/image
 * */
void disableTexture() {
	glDisable(GL_TEXTURE_2D);
}

/**
 * @descr Draw image of given size at a given position.
 * @group graphics/image
 * @var Image object
 * */
void imageDrawxy(Image * image, float x, float y, float w, float h) {
	imageBind(image);
	PUSH_QUAD(x,y,w,h,0,0,0);
}

/**
 * @descr Draw image with full translation.
 * @group graphics/image
 * @var Image object
 * */
void imageDrawt(Image * image, float x, float y, float w, float h, float a, float ox, float oy) {
	imageBind(image);
	PUSH_QUAD(x,y,w,h,a,ox,oy);
}

/**
 * @descr Draw part of image of given size at a given position.
 * @group graphics/image
 * @var Image object
 * @var position of left top corner
 * @var position of left top corner
 * @var width of quad (actual drawing size)
 * @var height of quad  (actual drawing size)
 * @var x offset of texture
 * @var y offset of texture
 * @var width of texture (stretchig relative to width of quad)
 * @var height of texture (stretchig relative to height of quad)
 * */
void imageDrawqxy(Image * image, float x, float y, float w, float h, float qx, float qy, float qw, float qh) {
	imageBind(image);
	PUSH_QUADT(x,y,w,h,0,0,0,qx, qy, qw, qh, image->w, image->h);
}

/**
 * @descr Draw part of image of given size at a given position with transformations.
 * @group graphics/image
 * @var Image object
 * @var position of left top corner
 * @var position of left top corner
 * @var width of quad (actual drawing size)
 * @var height of quad  (actual drawing size)
 * @var x offset of texture
 * @var y offset of texture
 * @var width of texture (stretchig relative to width of quad)
 * @var height of texture (stretchig relative to height of quad)
 * @var angle (relative to origin)
 * @var origin x
 * @var origin y
 * */
void imageDrawqt(Image * image, float x, float y, float w, float h, float qx, float qy, float qw, float qh, float a, float ox, float oy) {
	imageBind(image);
	PUSH_QUADT(x,y,w,h,a,ox,oy,qx, qy, qw, qh, image->w, image->h);
}

bool borderImageDrawCentralPart = TRUE;

void imageDrawBorderCenter(bool drawCenter) {
	borderImageDrawCentralPart = drawCenter;
}

/**
 * @descr Draw image sliced on 9 parts by 4 lines: top, bottom, left and right
 * @group graphics/image
 * @var Image object
 * @var position of left top corner
 * @var position of left top corner
 * @var summary width of object
 * @var summary height of object
 * @var slice top
 * @var slice right
 * @var slice bottom
 * @var slice left
 * */
void imageDrawBorder(Image * image, float x, float y, float w, float h, float t, float r, float b, float l) {
	imageBind(image);
	float ow = image->w;
	float oh = image->h;
	if(t > 0.0)
	{
		PUSH_QUADT(x,          y,          l,          t,          0, 0, 0,  0,       0,       l,            t,          ow, oh);
		PUSH_QUADT(x + l,      y,          w - l - r,  t,          0, 0, 0,  l,       0,       ow - l - r,    t,          ow, oh);
		PUSH_QUADT(x + w - r,  y,          r,          t,          0, 0, 0,  ow - r,  0,       r,            t,          ow, oh);
	}

	PUSH_QUADT(x,            y + t,      l,          h - t - b,  0, 0, 0,  0,       t,        l,           oh - t - b, ow, oh);
	if(borderImageDrawCentralPart)
		PUSH_QUADT(x + l,      y + t,      w - l - r,  h - t - b,  0, 0, 0,  l,       t,        ow - l - r,  oh - t - b, ow, oh);
	PUSH_QUADT(x + w - r,    y + t,      r,          h - t - b,  0, 0, 0,  ow - r,  t,        r,           oh - t - b, ow, oh);
	if(b > 0.0)
	{
		PUSH_QUADT(x,          y + h - b,  l,          b,          0, 0, 0,  0,       oh - b,   l,           b,          ow, oh);
		PUSH_QUADT(x + l,      y + h - b,  w - l - r,  b,          0, 0, 0,  l,       oh - b,   ow - l - r,  b,          ow, oh);
		PUSH_QUADT(x + w - r,  y + h - b,  r,          b,          0, 0, 0,  ow - r,  oh - b,   r,           b,          ow, oh);
	}
}

void initMultitexture(Multitexture * multitexture) {
	new(multitexture->images, Image*, multitexture->size);
}

void deleteMultitexture(Multitexture * multitexture) {
	delete(multitexture->images);
}

inline void multitextureBind(Multitexture * multitexture) {
	//~ if(prevImageId == image->id) return;
	FLUSH_BUFFER();
	Image * image;
	int i;
	for(i = 0; i < multitexture->size; i++)
	{
		image = multitexture->images[i];
		glActiveTexture_(GL_TEXTURE0 + i);
		imageCheckResLoader(image);
		if (image->id) glBindTexture(GL_TEXTURE_2D, image->id);
	}
	glActiveTexture_(GL_TEXTURE0);
}

/**
 * @descr Draw multitexture of given size at a given position.
 * @group graphics/image
 * @var Multitexture object
 * */
void multitextureDrawxy(Multitexture * multitexture, float x, float y, float w, float h) {
	multitextureBind(multitexture);
	PUSH_QUAD(x,y,w,h,0,0,0);
}

/**
 * @descr Draw multitexture with full translation.
 * @group graphics/image
 * @var Multitexture object
 * */
void multitextureDrawt(Multitexture * multitexture, float x, float y, float w, float h, float a, float ox, float oy) {
	multitextureBind(multitexture);
	PUSH_QUAD(x,y,w,h,a,ox,oy);
}

/**
 * @descr Draw part of multitexture of given size at a given position.
 * @group graphics/multitexture
 * @var Multitexture object
 * @var position of left top corner
 * @var position of left top corner
 * @var width of quad (actual drawing size)
 * @var height of quad  (actual drawing size)
 * @var x offset of texture
 * @var y offset of texture
 * @var width of texture (stretchig relative to width of quad)
 * @var height of texture (stretchig relative to height of quad)
 * */
void multitextureDrawqxy(Multitexture * multitexture, float x, float y, float w, float h, float qx, float qy, float qw, float qh) {
	multitextureBind(multitexture);
	PUSH_QUADT(x,y,w,h,0,0,0,qx, qy, qw, qh, multitexture->w, multitexture->h);
}

/**
 * @descr Draw part of multitexture of given size at a given position using 1x1 pixel quad with texture coordinates. You may change quad size and position using transformations.
 * @group graphics/multitexture
 * @var Multitexture object
 * @var position of left top corner
 * @var position of left top corner
 * @var width of quad (actual drawing size)
 * @var height of quad  (actual drawing size)
 * @var x offset of texture
 * @var y offset of texture
 * @var width of texture (stretchig relative to width of quad)
 * @var height of texture (stretchig relative to height of quad)
 * @var angle (relative to origin)
 * @var origin x
 * @var origin y
 * */
void multitextureDrawqt(Multitexture * multitexture, float x, float y, float w, float h, float qx, float qy, float qw, float qh, float a, float ox, float oy) {
	multitextureBind(multitexture);
	PUSH_QUADT(x,y,w,h,a,ox,oy,qx, qy, qw, qh, multitexture->w, multitexture->h);
}

//~ /**
 //~ * @descr Set the current active texture for multitexturenig. Equivalent to glActiveTexture(GL_TEXTURE0 + i).
 //~ * @group graphics/image
 //~ * @var number of texture slot (min 0, max 7)
 //~ * */
//~ void activeTexture(int i) {
	//~ glActiveTexture_(GL_TEXTURE0 + i);
//~ }

//~ /**
 //~ * @descr Enable/disable smooth interpolation for image. Disabled filtering useful, if you want to fit image to pixel matrix. If this image will be scaled and/or rotated you must enable filtering (default). Must be called before texture loading.
 //~ * @group graphics/image
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

void deleteImage(Image * ptr) {
	//~ printf("%d\n", same_type_p(typeof(ptr)) == INTEGER_TYPE);
	#ifdef MEMORY_TEST
		printf("Freeing Image %d\n", ptr->id);
	#endif
	if(ptr && ptr->id > 1) glDeleteTextures(1, &ptr->id);
	
	//~ else MYERROR("Trying to free a null-image. Maybe, you did it manually?");
}
