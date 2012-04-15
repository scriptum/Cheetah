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
#include <string.h>
#include "cheetah.h"
#include "render.h"


static unsigned int g_seed; 

inline void fast_srand(int seed) 
{
	g_seed = seed;
}
inline int fastrand()
{
	g_seed = (214013*g_seed+2531011);
	return (g_seed>>16)&0x7FFF;
}

#define TEXPARAM(a) do {\
	glGenTextures(1, &bufid);\
	glBindTexture(GL_TEXTURE_2D, bufid);\
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);\
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);\
	glTexImage2D(GL_TEXTURE_2D, 0, a?GL_RGBA:GL_RGB, w, h, 0, a?GL_RGBA:GL_RGB, GL_UNSIGNED_BYTE, buf);\
	glBindTexture(GL_TEXTURE_2D, 0);\
} while(0)

void generateImage(Image *ptr, int w, int h, const char *imageType) {
	char *buf = NULL;
	GLuint bufid;
	int i, j, c;
	if(strcmp(imageType, "dummy") == 0){
		new(buf, char, w * h * 3);
		memset(buf, 0, w * h * 3);
		TEXPARAM(0);
	}
	else if(strcmp(imageType, "dummy_alpha") == 0){
		new(buf, char, w * h * 4);
		memset(buf, 0, w * h * 4);
		TEXPARAM(1);
	}
	else if(strcmp(imageType, "noise") == 0){
		new(buf, char, w * h * 3 + 1);
		for(i = 0; i < w * h; i++)
			*((int*)(buf + i * 3)) = rand();
		//~ *((int*)(&buf[w * h * 3])) = fastrand();
		TEXPARAM(0);
	}
	else if(strcmp(imageType, "noise_alpha") == 0){
		new(buf, char, w * h * 4);
		for(i = 0; i < w * h; i++)
			*((int*)(buf + (i<<2))) = rand();
		TEXPARAM(1);
	}
	else if(strcmp(imageType, "light") == 0) {
		new(buf, char, w * h * 3);
		for(j = 0; j < h; j++)
		{
			for(i = 0; i < w; i++)
			{
				c = 255 - 2 * 255 *
				sqrtf(
					(i - w/2) * (float)(i - w/2) / (float)(w*w)+ 
					(j - h/2) * (float)(j - h/2) / (float)(h*h)
				);
				if (c < 0) c = 0;
				c = c | c << 8 | c << 16;
				*((int*)(buf + (j * h + i) * 4)) = c;
			}
		}
		TEXPARAM(0);
	}
	else if(strcmp(imageType, "light_alpha") == 0) {
		new(buf, char, w * h * 4);
		for(j = 0; j < h; j++)
		{
			for(i = 0; i < w; i++)
			{
				c = 255 - 2 * 255 *
				sqrtf(
					(i - w/2) * (float)(i - w/2) / (float)(w*w)+ 
					(j - h/2) * (float)(j - h/2) / (float)(h*h)
				);
				if (c < 0) c = 0;
				c = 0xffffff | c<<24;
				*((int*)(buf + (j * h + i) * 4)) = c;
			}
		}
		TEXPARAM(1);
	}
	else
	{
		myError("undefined generator type: %s", imageType);
	}
	if(ptr) {
		ptr->w = (float)w;
		ptr->h = (float)h;
		ptr->id = bufid;
	}
	if(buf) delete(buf);
}