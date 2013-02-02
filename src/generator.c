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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "cheetah.h"
#include "macros.h"
#include "render.h"
#include "random-taus88.h"

#define MAGICK_CONSTANT 20

void newImageRaw(Image *, int, int, const char *, const char *);

inline void repeat(int w, int h, int channels, char *buf) {
	int i, j;
	for(j = h / 2; j < h; j++)
		for(i = 0; i <= w / 2; i++)
			*((int*)(buf + (j * h + i) * channels)) =
			*((int*)(buf + ((h - j - 1) * h + i) * channels));
	for(j = 0; j < h; j++)
	    for(i = w / 2; i < w; i++)
		if(channels == 3)
		{
		    *((int*)(buf + (j * h +          i)  * channels)) |=
		    *((int*)(buf + (j * h + (w - i - 1)) * channels)) & 0xffffff;
		}
		else
		{
		    *((int*)(buf + (j * h +          i)  * channels)) =
		    *((int*)(buf + (j * h + (w - i - 1)) * channels));
		}
}

#define LOOP_CIRCLE(code)                                                      \
for(j = 0; j <= h / 2; j++) {                                                  \
    for(i = 0; i <= w / 2; i++) { code }}                                      \
repeat(w, h, channels, buf);

inline float distance(int i, int j, int w, int h) {
	return ((i - w / 2 + 0.5) * (i - w / 2 + 0.5) / (float)(w * w) +
		(j - h / 2 + 0.5) * (j - h / 2 + 0.5) / (float)(h * h));
}

static void generateImageData(ImageData *ptr, int w, int h, const char *imageType, bool alpha) {
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
    #define NEW do {                                                           \
        if(alpha) {                                                            \
            channels = 4; new(buf, char, w * h * channels);                    \
        }                                                                      \
        else {                                                                 \
            channels = 3; new0(buf, char, w * h * channels + 1);               \
        }                                                                      \
	} while(0)
	if(strcmp(imageType, "dummy") == 0) {
		NEW;
		memset(buf, 0xff, w * h * channels);
	}
	else if(strcmp(imageType, "noise") == 0){
		NEW;
		for(i = 0; i < w * h; i++)
			*((int*)(buf + i * channels)) = rt88_trand(taus88);
	}
    #define COLOR_LIGHT                                                        \
        if(channels == 3) {                                                    \
            c = c | c << 8 | c << 16;                                          \
            *((int*)(buf + (j * h + i) * channels)) |= c;                      \
        } else {                                                               \
            c = 0xffffff | c << 24;                                            \
            *((int*)(buf + (j * h + i) * channels)) = c;                       \
		}
	else if(strcmp(imageType, "light") == 0) {
		NEW;
		LOOP_CIRCLE(
			c = 255 - 2 * 255 * sqrtf(distance(i, j, w, h));
			if (c < 0)
				c = 0;
			COLOR_LIGHT
		)
	}
	else if(strcmp(imageType, "lightexp") == 0) {
		NEW;
		LOOP_CIRCLE(
			c = 255 * expf (- distance(i, j, w, h) * MAGICK_CONSTANT);
			COLOR_LIGHT
		)
	}
	else if(strcmp(imageType, "circle") == 0) {
		NEW;
		LOOP_CIRCLE(
			c = (i - w / 2) * (i - w / 2) + (j - h / 2) * (j - h / 2);
			if(c > (w / 4 * w))
				c = 0;
			else
				c = 0xffffffff;
			*((int*)(buf + (j * h + i) * channels)) = c;
		)
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
	ptr->id = 0;
	NEEDED_INIT_VOID;
	if(ptr) {
		imageData.data = NULL;
		CHECK_OPTION(options, alpha);
		generateImageData(&imageData, w, h, imageType, alpha);
		if(imageData.data)
			newImageRaw(ptr, w, h, imageData.data, options);
	}
	if(imageData.data) delete(imageData.data);
}
