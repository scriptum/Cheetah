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
#include "render.h"
#include "cconfig.h"
#include "cvertex.h"
#include "crandom.h"

#include <SDL.h>

#include "test.h"

SDL_Window *screen = NULL;
SDL_GLContext glcontext;

bool clearScreenFlag = 1;

int resLoaderThread(void *unused);
void initRenderer(void);
void resLoaderInit(bool resloader);
extern void resLoaderMainThread(void);

void resetView(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, h, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void resetViewDefault(void)
{
	int w, h;
	RETURN_IF_NULL(screen);
	SDL_GetWindowSize(screen, &w, &h);
	resetView(w, h);
}

CHEETAH_EXPORT void setWindowSize(int w, int h)
{
	RETURN_IF_NULL(screen);
	SDL_SetWindowSize(screen, w, h);
}

/* Create window and initialize all OpenGL's stuff. */
CHEETAH_EXPORT bool cheetahInit(const char *appName, const char *options)
{
	unsigned flags = SDL_WINDOW_OPENGL;
	bool firstrun = FALSE;
	bool vsync;
	int width = 800, height = 600;
	int count = sscanf(options, "%11dx%11d", &width, &height);
	if(1 == count)
	{
		height = width;
	}
	if(width <= 0)
	{
		width = 800;
	}
	if(height <= 0)
	{
		height = 600;
	}
	CHECK_OPTION(options, fullscreen);
	CHECK_OPTION(options, resizable);
	CHECK_OPTION(options, novsync);
	vsync = !novsync;
	CHECK_OPTION(options, resloader);
	CHECK_OPTION(options, depth);
	CHECK_OPTION(options, stencil);
	CHECK_OPTION(options, noframe);
	const int bpp = 32;
	if(TRUE == fullscreen)
	{
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	else
	{
		if(TRUE == resizable)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}
	}
	if(TRUE == noframe)
	{
		flags |= SDL_WINDOW_BORDERLESS;
	}
	if(NULL == screen)
	{
		if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			myError("couldn't init SDL: %s", SDL_GetError());
			return FALSE;
		}
		screen = SDL_CreateWindow(appName ? appName : "Cheetah Engine",
		                          SDL_WINDOWPOS_CENTERED,
		                          SDL_WINDOWPOS_CENTERED,
		                          width, height, flags);
		if(NULL == screen)
		{
			myError("couldn't set %dx%dx%d video mode: %s",
			        width, height, bpp, SDL_GetError());
			return FALSE;
		}
		glcontext = SDL_GL_CreateContext(screen);
		SDL_GL_SetSwapInterval(vsync);
		if(TRUE == depth)
		{
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		}
		if(TRUE == stencil)
		{
			SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		}
		firstrun = TRUE;
		/*Set screen auto-scale properties*/
		screenScale.origWidth = (float)width;
		screenScale.origHeight = (float)height;
		screenScale.autoScale = TRUE;
		screenScale.autoScaleFont = TRUE;
		screenScale.scaleX = 1.0f;
		screenScale.scaleY = 1.0f;
		screenScale.offsetX = 0.0f;
		screenScale.offsetY = 0.0f;
		screenScale.aspect = screenScale.origWidth / screenScale.origHeight;
	}

	if(TRUE == firstrun)
	{
		initRenderer();
		/* set background color */
		glClearColor(0.0, 0.0, 0.0, 1.0);
		/* enable blending */
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		/* DO NOT TOUCH!!!*/
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		//~ glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
		if(TRUE == resloader)
		{
			resLoaderInit(resloader);
			SDL_CreateThread(resLoaderThread, NULL, NULL);
		}
		glGenTextures(1, &null_texture);
		glBindTexture(GL_TEXTURE_2D, null_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, "\0\0\0\0");

		globalTimers.gameSpeed = 1.0;
		globalTimers.rescaleTime = 0;
		globalTimers.resizeDelay = 100;
		globalTimers.time = 0;
		globalTimers.timed = 0.0;
		globalTimers.timeOffsetd = 0.0;
	}

	resetViewDefault();
	//~ glDepthRange(-10000,10000);
	if(TRUE == firstrun)
	{
		/* Texture for drawing device independent anti-aliased rectangle:
		 * 0000
		 * 0110
		 * 0110
		 * 0000
		 * */
		glGenTextures(1, &rect_texture);
		glBindTexture(GL_TEXTURE_2D, rect_texture);
		TEX_CLAMP;
		TEX_LINEAR;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		             "\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377"
		             "\377\377\377\377\377\377\377\377\377\377\0\377\377\377\0\377\377\377\377"
		             "\377\377\377\377\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0"
		             "\377\377\377\0");

		/* create two main vertex buffers */
		vertexCounter = 0;
		new(texCoord, float, VERTEX_BUFFER_LIMIT * VERTICLES_PER_SPRITE);
		new(vertexCoord, float, VERTEX_BUFFER_LIMIT * VERTICLES_PER_SPRITE);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
#ifdef COLOR_ARRAYS
		new(colorArray, unsigned char, 2 * VERTEX_BUFFER_LIMIT * VERTICLES_PER_SPRITE);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, colorArray);
#endif
		glVertexPointer(2, GL_FLOAT, 0, vertexCoord);
		glTexCoordPointer(2, GL_FLOAT, 0, texCoord);

		/* init random generator */
		random_hash_seed((uint32_t)time(NULL));
	}

	return TRUE;
}

/**
 * @descr Check, if screen exists. Useful if you have windowless version of your application, e.g. server.
 * @group graphics/window
 * @return true if screen was initialized
 * */
CHEETAH_EXPORT bool isInit(void)
{
	return screen != NULL;
}

/**
 * @descr Get window's width.
 * @group graphics/window
 * */
CHEETAH_EXPORT void getWindowSize(int *w, int *h)
{
	if(NULL == screen)
	{
		*w = *h = 0;
		return;
	}
	SDL_GetWindowSize(screen, w, h);
}

/**
 * @descr Swap buffers and present graphics. This function calls automatically every frame by default.
 * @group graphics/window
 * @advanced
 * */
CHEETAH_EXPORT void swapBuffers(void)
{
	FLUSH_BUFFER();
	SDL_GL_SwapWindow(screen);
}

/**
 * @descr Set window's title.
 * @group graphics/window
 * @var text to replace the caption
 * @see init
 * */
CHEETAH_EXPORT void setTitle(const char *text)
{
	SDL_SetWindowTitle(screen, text);
}

/**
 * @descr Get list of possible screen modes. You need this if you want to run application in fullscreen mode. This function gives you direct access to SDL's structure.
 * @group graphics/window
 * @return array of pointers to SDL_Rect structure.
 * @advanced
 * */
/*SDL_Rect **getModesSDL(void) {
	NEEDED_INIT;
	SDL_Rect **modes = SDL_ListModes(0, SDL_OPENGL | SDL_FULLSCREEN);
	if(modes == (SDL_Rect **)0 || modes == (SDL_Rect **)-1)
		return NULL;
	return modes;
}*/

/**
 * @descr Toggle auto-clear screen. Disable auto-clear if you want to do it manually or you have fullscreen background.
 * @group graphics/drawing
 * @var enable or disable
 * @see clear
 * */
CHEETAH_EXPORT void clearScreen(bool enabled)
{
	clearScreenFlag = enabled;
}

/**
 * @descr Enable or disable autoscale. Autoscale allows you to draw stuff in the fixed coordinates, and engine automatically translates all coordinates if window changes his size. Is you want to control screen size yourself, disable this.
 * @group graphics/drawing
 * @var enable or disable autoscale
 * */
CHEETAH_EXPORT void autoScale(bool enableAutoScale)
{
	screenScale.autoScale = enableAutoScale;
}

static void doAutoScale(void)
{
	if(screenScale.autoScale)
	{
		glLoadIdentity();
		glTranslatef(screenScale.offsetX, screenScale.offsetY, 0);
		glScalef(screenScale.scaleX, screenScale.scaleY, 1);
	}
}

CHEETAH_EXPORT void prepare(void)
{
	unsigned int delta = globalTimers.time;
	globalTimers.time = SDL_GetTicks();
	delta = globalTimers.time - delta;
	globalTimers.timeOffsetd += (1.0 - globalTimers.gameSpeed) * delta / 1000.0;
	globalTimers.timed = globalTimers.time / 1000.0;
	globalTimers.gameTimed = globalTimers.timed - globalTimers.timeOffsetd;
	dbgvv("time: %f\ngame time: %f\noffset: %f", globalTimers.timed, globalTimers.gameTimed, globalTimers.timeOffsetd);
	doAutoScale();
	resLoaderMainThread();
	if(globalTimers.rescaleTime && globalTimers.time > globalTimers.rescaleTime)
	{
		int w, h;
		SDL_GetWindowSize(screen, &w, &h);
		// SDL_SetVideoMode(screen->w, screen->h, 32, screen->flags);
		resetView(w, h);
		doAutoScale();
		globalTimers.rescaleTime = 0;
	}
	if(clearScreenFlag)
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

void recomputeScreenScale(float w, float h)
{
	if(!screenScale.autoScale)
	{
		return;
	}
	screenScale.aspect = w / h;
	if(screenScale.aspect > screenScale.origWidth / screenScale.origHeight)
	{
		screenScale.scaleX = screenScale.scaleY = h / screenScale.origHeight;
		screenScale.offsetX = floorf((w - screenScale.origWidth * screenScale.scaleX) * 0.5f);
		screenScale.offsetY = 0;
	}
	else
	{
		screenScale.scaleX = screenScale.scaleY = w / screenScale.origWidth;
		screenScale.offsetY = floorf((h - screenScale.origHeight * screenScale.scaleY) * 0.5f);
		screenScale.offsetX = 0;
	}
}

CHEETAH_EXPORT void sdlquit()
{
	if(screen)
	{
		SDL_DestroyWindow(screen);
		SDL_GL_DeleteContext(glcontext);
	}
	SDL_Quit();
}
