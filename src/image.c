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
	delete(myBuf);
	return img;
}

inline unsigned int loadImageTex(const char *options, unsigned char *img, int width, int height, int channels)
{
	unsigned int tex_id;
	NEDED_INIT;
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
 * @descr Load image from disc with specific options.
 * @group graphics/image
 * @var file name
 * @var string of options. This is depends on image loading module you use. Supported options:
 *  * _n_ - use nearest interpolation
 *  * _m_ - generate mip-maps (automatically sets mip-map interpolation)
 *  * _i_ - load instantly without delayed resource loader
 * @return Image object
 * */
void newImageOpt(Image* ptr, const char *name, const char *options) {
	int width, height, channels, instant = 0, i = 0;
	unsigned int tex_id;
	unsigned char *img;
	NEDED_INIT;
	if(!name)
	{
		MYERROR("empty filename");
		return;
	}
	if(options) while(options[i])
	{
		if(options[i] == 'i') instant = 1;
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
 * @descr Draw while image using 1x1 pixel quad. You may change quad size and position using transformations.
 * @group graphics/image
 * @var Image object
 * */
void imageDraw(Image * image) {
	imageBind(image);
	PUSH_QUAD(0,0,1,1,0,0,0);
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
 * @descr Draw while multitexture using 1x1 pixel quad. You may change quad size and position using transformations.
 * @group graphics/image
 * @var Multitexture object
 * */
void multitextureDraw(Multitexture * multitexture) {
	multitextureBind(multitexture);
	PUSH_QUAD(0,0,1,1,0,0,0);
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
 * @descr Draw part of multitexture using 1x1 pixel quad with texture coordinates. You may change quad size and position using transformations.
 * @group graphics/multitexture
 * @var Image object
 * @var x offset of texture
 * @var y offset of texture
 * @var width of texture
 * @var height of texture
 * */
void multitextureDrawq(Multitexture * multitexture, float qx, float qy, float qw, float qh) {
	multitextureBind(multitexture);
	PUSH_QUADT(0,0,1,1,0,0,0,qx, qy, qw, qh, multitexture->w, multitexture->h);
}

/**
 * @descr Draw part of multitexture of given size at a given position using 1x1 pixel quad with texture coordinates. You may change quad size and position using transformations.
 * @group graphics/multitexture
 * @var Image object
 * @var position of left top corner
 * @var position of left top corner
 * @var width of quad
 * @var height of quad
 * @var x offset of texture
 * @var y offset of texture
 * @var width of texture
 * @var height of texture
 * */
void multitextureDrawqxy(Multitexture * multitexture, float x, float y, float w, float h, float qx, float qy, float qw, float qh) {
	multitextureBind(multitexture);
	PUSH_QUADT(x,y,w,h,0,0,0,qx, qy, qw, qh, multitexture->w, multitexture->h);
}

/**
 * @descr Draw part of image using 1x1 pixel quad with texture coordinates. You may change quad size and position using transformations.
 * @group graphics/image
 * @var Image object
 * @var x offset of texture
 * @var y offset of texture
 * @var width of texture
 * @var height of texture
 * */
void imageDrawq(Image * image, float qx, float qy, float qw, float qh) {
	imageBind(image);
	PUSH_QUADT(0,0,1,1,0,0,0,qx, qy, qw, qh, image->w, image->h);
}

/**
 * @descr Draw part of image of given size at a given position using 1x1 pixel quad with texture coordinates. You may change quad size and position using transformations.
 * @group graphics/image
 * @var Image object
 * @var position of left top corner
 * @var position of left top corner
 * @var width of quad
 * @var height of quad
 * @var x offset of texture
 * @var y offset of texture
 * @var width of texture
 * @var height of texture
 * */
void imageDrawqxy(Image * image, float x, float y, float w, float h, float qx, float qy, float qw, float qh) {
	imageBind(image);
	PUSH_QUADT(x,y,w,h,0,0,0,qx, qy, qw, qh, image->w, image->h);
}

/**
 * @descr Draw part of image of given size at a given position using 1x1 pixel quad with texture coordinates. You may change quad size and position using transformations.
 * @group graphics/image
 * @var Image object
 * @var position of left top corner
 * @var position of left top corner
 * @var width of quad
 * @var height of quad
 * @var angle
 * @var origin x
 * @var origin y
 * @var x offset of texture
 * @var y offset of texture
 * @var width of texture
 * @var height of texture
 * */
void imageDrawqt(Image * image, float x, float y, float w, float h, float a, float ox, float oy, float qx, float qy, float qw, float qh) {
	imageBind(image);
	PUSH_QUADT(x,y,w,h,a,ox,oy,qx, qy, qw, qh, image->w, image->h);
}

/**
 * @descr Draw part of multitexture of given size at a given position using 1x1 pixel quad with texture coordinates. You may change quad size and position using transformations.
 * @group graphics/multitexture
 * @var Multitexture object
 * @var position of left top corner
 * @var position of left top corner
 * @var width of quad
 * @var height of quad
 * @var angle
 * @var origin x
 * @var origin y
 * @var x offset of texture
 * @var y offset of texture
 * @var width of texture
 * @var height of texture
 * */
void multitextureDrawqt(Multitexture * multitexture, float x, float y, float w, float h, float a, float ox, float oy, float qx, float qy, float qw, float qh) {
	multitextureBind(multitexture);
	PUSH_QUADT(x,y,w,h,a,ox,oy,qx, qy, qw, qh, multitexture->w, multitexture->h);
}

/**
 * @descr Set the current active texture for multitexturenig. Equivalent to glActiveTexture(GL_TEXTURE0 + i).
 * @group graphics/image
 * @var number of texture slot (min 0, max 7)
 * */
void activeTexture(int i) {
	glActiveTexture_(GL_TEXTURE0 + i);
}

/**
 * @descr Enable/disable smooth interpolation for image. Disabled filtering useful, if you want to fit image to pixel matrix. If this image will be scaled and/or rotated you must enable filtering (this is by defaults).
 * @group graphics/image
 * @var Image object
 * @var true means that filtering is enabled, false means that filtering is disabled
 * */
void imageFiltering(Image * img, bool enabled) {
	glBindTexture(GL_TEXTURE_2D, img->id);
	if(!enabled)
		TEX_NEAREST;
	else
		TEX_LINEAR;
}

void _newImageFromData(Image * ptr, ImageData * imgdata, const char *options) {
	unsigned int tex_id;
	if(!imgdata || !imgdata->data) MYERROR("invalid data");
	ptr->w = (float)imgdata->w;
	ptr->h = (float)imgdata->h;
	ptr->channels = imgdata->channels;
	tex_id = SOIL_internal_create_OGL_texture(
			(unsigned char*)imgdata->data, ptr->w, ptr->h, ptr->channels,
			0, SOIL_FLAG_TEXTURE_REPEATS,
			GL_TEXTURE_2D, GL_TEXTURE_2D,
			GL_MAX_TEXTURE_SIZE);
	ptr->id = tex_id;
	while(*options)
	{
		if(*options == 'n') TEX_NEAREST;
		options++;
	}
}

void deleteImage(Image * ptr) {
	//~ printf("%d\n", same_type_p(typeof(ptr)) == INTEGER_TYPE);
	#ifdef MEMORY_TEST
		printf("Freeing Image %d\n", ptr->id);
	#endif
	if(ptr && ptr->id > 1) glDeleteTextures(1, &ptr->id);
	
	//~ else MYERROR("Trying to free a null-image. Maybe, you did it manually?");
}
