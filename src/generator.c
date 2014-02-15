/*******************************************************************************

Copyright (c) 2012-2014 Pavel Roschin (aka RPG) <rpg89@post.ru>

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
#include "cmacros.h"
#include "render.h"
#include "crandom.h"
#include "test.h"

#define MAGICK_CONSTANT 20.0f

void newImageRaw(Image *, int, int, const char *, const char *);

static inline void repeat(int w, int h, int channels, char *buf)
{
	int i, j;
	for(j = h / 2; j < h; j++)
	{
		for(i = 0; i <= w / 2; i++)
		{
			*((int *)(buf + (j * h + i) * channels)) = *((int *)(buf + ((h - j - 1) * h + i) * channels));
		}
	}
	for(j = 0; j < h; j++)
	{
		for(i = w / 2; i < w; i++)
		{
			if(channels == 3)
			{
				*((int *)(buf + (j * h + i)  * channels)) |= *((int *)(buf + (j * h + (w - i - 1)) * channels)) & 0xffffff;
			}
			else
			{
				*((int *)(buf + (j * h + i)  * channels)) = *((int *)(buf + (j * h + (w - i - 1)) * channels));
			}
		}
	}
}

#define LOOP_CIRCLE(code)                                                      \
for(j = 0; j <= h / 2; j++) {                                                  \
    for(i = 0; i <= w / 2; i++) { code }}                                      \
repeat(w, h, channels, buf);

static inline float distance(int i, int j, int w, int h)
{
	return (((float)i - (float)w / 2 + 0.5f) * ((float)i - (float)w / 2 + 0.5f) / (float)(w * w) +
	        ((float)j - (float)h / 2 + 0.5f) * ((float)j - (float)h / 2 + 0.5f) / (float)(h * h));
}

CHEETAH_EXPORT static void generateImageData(ImageData *ptr, int w, int h, const char *imageType, bool alpha)
{
	int i, j, channels;
	int32_t c;
	float buff;
	char *buf = NULL;

	if(!ptr)
	{
		myError("generateImageData: empty pointer");
		return;
	}

#define NEW do {                                                       \
		if(alpha) {                                                    \
			channels = 4; new(buf, char, w * h * channels);        \
		}                                                              \
		else {                                                         \
			channels = 3; new0(buf, char, w * h * channels + 1);   \
		}                                                              \
	} while(0)

	if(strcmp(imageType, "dummy") == 0)
	{
		NEW;
		memset(buf, 0xff, (size_t)(w * h * channels));
	}
	else if(strcmp(imageType, "noise") == 0)
	{
		NEW;
		for(i = 0; i < w * h; i++)
		{
			*((unsigned *)(buf + i * channels)) = rand128();
		}
	}

#define COLOR_LIGHT                                                            \
	if(channels == 3) {                                                    \
		c = c | c << 8 | c << 16;                                      \
		*((int*)(buf + (j * h + i) * channels)) |= c;                  \
	} else {                                                               \
		c = 0xffffff | c << 24;                                        \
		*((int*)(buf + (j * h + i) * channels)) = c;                   \
	}

	else if(strcmp(imageType, "light") == 0)
	{
		NEW;
		LOOP_CIRCLE(
		        buff = 2.0f * 255.0f * sqrtf(distance(i, j, w, h));
		        c = 255 - (int)buff;
		        if(c < 0)
		        c = 0;
		        COLOR_LIGHT
		)
	}
	else if(strcmp(imageType, "lightexp") == 0)
	{
		NEW;
		LOOP_CIRCLE(
		        buff = 255.0f * expf(-distance(i, j, w, h) * MAGICK_CONSTANT);
		        c = (int)buff;
		        COLOR_LIGHT
		)
	}
	else if(strcmp(imageType, "circle") == 0)
	{
		NEW;
		LOOP_CIRCLE(
		        c = (i - w / 2) * (i - w / 2) + (j - h / 2) * (j - h / 2);
		        if(c > (w * w / 4))
		        c = 0;
		        else
		{
			c = (int32_t)0xffffffff;
			}
		* ((int *)(buf + (j * h + i) * channels)) = c;
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

CHEETAH_EXPORT void generateImage(Image *ptr, int w, int h, const char *imageType, const char *options)
{
	static ImageData imageData;
	ptr->id = 0;
	NEEDED_INIT_VOID;
	if(ptr)
	{
		imageData.data = NULL;
		CHECK_OPTION(options, alpha);
		generateImageData(&imageData, w, h, imageType, alpha);
		if(imageData.data)
		{
			newImageRaw(ptr, w, h, imageData.data, options);
		}
	}
	if(imageData.data)
	{
		delete(imageData.data);
	}
}
