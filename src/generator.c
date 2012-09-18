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
#include <time.h>
#include <string.h>
#include "cheetah.h"
#include "render.h"
#include "random-taus88.h"

#define MAGICK_CONSTANT 20

#define TEXPARAM(a) do {\
	glGenTextures(1, &bufid);\
	glBindTexture(GL_TEXTURE_2D, bufid);\
	if(linear)\
		TEX_LINEAR;\
	else \
		TEX_NEAREST;\
	glTexImage2D( GL_TEXTURE_2D, \
								0, \
								(a) ? GL_RGBA : GL_RGB, \
								w, \
								h, \
								0, \
								(a) ? GL_RGBA : GL_RGB, \
								GL_UNSIGNED_BYTE, \
								imageData.data);\
	glBindTexture(GL_TEXTURE_2D, 0);\
} while(0)

#define REPEAT \
for(j = h / 2; j < h; j++) \
	for(i = 0; i <= w / 2; i++) \
		*((int*)(buf + (j * h + i) * channels)) = *((int*)(buf + ((h - j - 1) * h + i) * channels)); \
for(j = 0; j < h; j++) \
	for(i = w / 2; i < w; i++) \
		if(channels == 3) \
		{ \
			*((int*)(buf + (j * h +          i)  * channels)) =  \
			*((int*)(buf + (j * h + (w - i - 1)) * channels)) & 0xffffff; \
		} \
		else \
		{ \
			*((int*)(buf + (j * h +          i)  * channels)) =  \
			*((int*)(buf + (j * h + (w - i - 1)) * channels)); \
		}

#define LOOP_CIRCLE(code) \
for(j = 0; j <= h / 2; j++) { \
	for(i = 0; i <= w / 2; i++) { code }} \
REPEAT

#define DISTANCE \
((i - w / 2 + 0.5) * (i - w / 2 + 0.5) / (float)(w * w) + \
 (j - h / 2 + 0.5) * (j - h / 2 + 0.5) / (float)(h * h))
void generateImageData(ImageData *ptr, int w, int h, const char *imageType) {
	int i, j, c, channels;
	char *buf = NULL;
	static rt88_state *taus88 = NULL;
	if(!taus88)
	{
		new(taus88, rt88_state, 1);
		rt88_init(taus88, time(0));
	}
	
	if(!ptr)
	{
		myError("generateImageData: empty pointer");
		return;
	}
	#define NEW3 channels = 3; new(buf, char, w * h * channels + 1);
	#define NEW4 channels = 4; new(buf, char, w * h * channels);
	if(strcmp(imageType, "dummy") == 0){
		NEW3
		memset(buf, 0, w * h * channels);
	}
	else if(strcmp(imageType, "dummy_alpha") == 0){
		NEW4
		memset(buf, 0, w * h * channels);
	}
	else if(strcmp(imageType, "noise") == 0){
		NEW3
		for(i = 0; i < w * h; i++)
			*((int*)(buf + i * channels)) = rt88_trand(taus88);
	}
	else if(strcmp(imageType, "noise_alpha") == 0){
		NEW4
		for(i = 0; i < w * h; i++)
			*((int*)(buf + i * channels)) = rt88_trand(taus88);
	}
	#define COLOR_LIGHT \
		if(channels==3) { \
			c = c | c << 8 | c << 16;\
			*((int*)(buf + (j * h + i) * channels)) = c; \
		} else { \
			c = 0xffffff | c<<24; \
			*((int*)(buf + (j * h + i) * channels)) = c; \
		}
	#define LIGHT c = 255 - 2 * 255 * sqrtf(DISTANCE);\
		if (c < 0) c = 0;
	else if(strcmp(imageType, "light") == 0) {
		NEW3
		LOOP_CIRCLE(LIGHT
			COLOR_LIGHT)
	}
	else if(strcmp(imageType, "light_alpha") == 0) {
		NEW4
		LOOP_CIRCLE(LIGHT
			COLOR_LIGHT)
	}
	#define LIGHTEXP c = 255 * expf (- DISTANCE * MAGICK_CONSTANT);
	else if(strcmp(imageType, "lightexp") == 0) {
		NEW3
		LOOP_CIRCLE(LIGHTEXP
			COLOR_LIGHT)
	}
	else if(strcmp(imageType, "lightexp_alpha") == 0) {
		NEW4
		LOOP_CIRCLE(LIGHTEXP
			COLOR_LIGHT)
	}
	#define CIRCLE_COLOR \
		c = (i - w / 2) * (i - w / 2) + \
		(j - h / 2) * (j - h / 2);\
		if(c > (w / 4 * w)) c = 0; else c = 0xffffffff;\
		*((int*)(buf + (j * h + i) * channels)) = c;
	else if(strcmp(imageType, "circle") == 0) {
		NEW3
		LOOP_CIRCLE(CIRCLE_COLOR)
	}
	else if(strcmp(imageType, "circle_alpha") == 0) {
		NEW4
		LOOP_CIRCLE(CIRCLE_COLOR)
	}
	else
	{
		myError("undefined generator type: %s", imageType);
		return;
	}
	ptr->w = w;
	ptr->h = h;
	ptr->channels = channels;
	ptr->data = buf;
}

void generateImage(Image *ptr, int w, int h, const char *imageType, const char *options) {
	static ImageData imageData;
	GLuint bufid = 0;
	ptr->id = 0;
	bool linear = 1;
	NEDED_INIT;
	if(ptr) {
		imageData.data = NULL;
		generateImageData(&imageData, w, h, imageType);
		if(imageData.data)
		{
			while(*options)
			{
				if(*options == 'n') linear = 0;
				options++;
			}
			TEXPARAM(imageData.channels == 4 ? 1 : 0);
			ptr->w = (float)w;
			ptr->h = (float)h;
			ptr->id = bufid;
		}
	}
	if(imageData.data) delete(imageData.data);
}