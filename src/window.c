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

#include "cheetah.h"
#include "render.h"
#include "config.h"

SDL_Surface *screen = NULL;

/**
 * @descr Create window and initialize all OpenGL's stuff. You MUST call this before any graphics function, e.g. cheetah.newImage. You may call this function again to re-size window, change application title, toggle fullscreen. Other options are ignored.
 * @group graphics/window
 * @var application's title shown in titlebar
 * @var width of the window
 * @var height of the window
 * @var bits per pixel (8, 16, 32, usually 32)
 * @var string of options. Supported options:
 *  * _f_ - fullscreen
 *  * _r_ - allow to re-size window
 *  * _l_ - use delayed resource loader
 *  * _v_ - enable vertical sync (recommend)
 *  * _d_ - enable depth buffer (usually 2D apps do not need this)
 *  * _s_ - enable stencil buffer (usually 2D apps do not need this)
 * @return true if success
 * */
bool init(const char * appName, unsigned int width, unsigned int height, int bpp, const char * attr) {
	bool fullscreen = 0;
	bool resizable = 0;
	int vsync = 0;
	bool firstrun = 0;
	bool depth = 0;
	bool stencil = 0;
	bool loader = 0;
	char ch;
	Uint32 flags = SDL_OPENGL | SDL_DOUBLEBUF;
	
	
	while(*attr)
	{
		ch = *attr;
		if(ch == 'f') fullscreen = 1;
		if(ch == 'r') resizable = 1;
		if(ch == 'v') vsync = 1;
		if(ch == 'd') depth = 1;
		if(ch == 's') stencil = 1;
		if(ch == 'l') loader = 1;
		attr++;
	}
	if (fullscreen) {
		flags |= SDL_FULLSCREEN;
	} else if (resizable) {
		flags |= SDL_RESIZABLE;
	}
	
	if(width < 1) width = 1;
	if(height < 1) height = 1;
	
	if (screen == NULL) {
		if ( SDL_Init ( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER ) != 0 )
			return 0;
		atexit(SDL_Quit);
		SDL_EnableUNICODE(1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, vsync);
		if(depth) SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		if(stencil) SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		firstrun = 1;
		/*Set screen auto-scale properties*/
		screenScale.origWidth = width;
		screenScale.origHeight = height;
		screenScale.autoScale = 1;
		screenScale.autoScaleFont = 1;
		screenScale.scaleX = 1.0f;
		screenScale.scaleY = 1.0f;
		screenScale.offsetX = 0.0f;
		screenScale.offsetY = 0.0f;
		screenScale.aspect = (float)width/height;
	}
	if(appName) SDL_WM_SetCaption (appName, appName);
	screen = SDL_SetVideoMode(width, height, bpp, flags);
	if (screen == NULL)
		myError("couldn't set %dx%dx%d video mode: %s",
								width, height, bpp, SDL_GetError());

	if (firstrun) {
		initRenderer();
		/* set background color */
		glClearColor( 0, 0, 0, 1);
		/* set line antialiasing */
		//~ glEnable(GL_POINT_SMOOTH);
		//~ glEnable(GL_LINE_SMOOTH);
		/* enable blending */
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		/* DO NOT TOUCH!!!*/
		glDisable(GL_CULL_FACE); 
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		//~ glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
		resLoaderQueue = NULL;
		if(loader)
		{
			resLoaderQueue = newQueue();
			resQueueMutex = SDL_CreateMutex();
			resShared = 0;
			SDL_CreateThread(resLoaderThread, (void *)NULL);
			glGenTextures(1, &null_texture);
			glBindTexture(GL_TEXTURE_2D, null_texture);
			//~ TEX_CLAMP;
			//~ TEX_LINEAR;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, "\0\0\0\0");
		}
		//~ glEnable(GL_ALPHA_TEST);
		//~ glAlphaFunc(GL_GREATER,0.1f);
	}

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//~ glDepthRange(-10000,10000);
	if(firstrun) {
		quadlist = glGenLists(1);
		glNewList(quadlist, GL_COMPILE);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(0, 0);
		glTexCoord2f(0, 1); glVertex2f(0, 1);
		glTexCoord2f(1, 1); glVertex2f(1, 1);
		glTexCoord2f(1, 0); glVertex2f(1, 0);
		glEnd();
		glEndList();
		pointlist = glGenLists(1);
		glNewList(pointlist, GL_COMPILE);
		glBegin(GL_POINTS);
		glVertex2f(0, 0);
		glEnd();
		glEndList();
		
		/*two vertex buffers*/
		vertexCounter = 0;
		new(texCoord, float, VERTEX_BUFFER_LIMIT * 8);
		new(vertexCoord, float, VERTEX_BUFFER_LIMIT * 8);
		verAlloc = VERTEX_BUFFER_LIMIT;
		
		//~ static float tex[] = {0,0,0,1,1,1,1,0};
		//~ static float ver[] = {0,0,0,1,1,1,1,0};
		//~ glGenBuffers_(1, &vboVer);
		//~ glBindBuffer_(GL_ARRAY_BUFFER_ARB, vboVer);
		//~ glBufferData_(GL_ARRAY_BUFFER_ARB, sizeof(Point)*4, (void*)ver, GL_STATIC_DRAW_ARB);
		//~ glGenBuffers_(1, &vboTex);
		//~ glBindBuffer_(GL_ARRAY_BUFFER_ARB, vboTex);
		//~ glBufferData_(GL_ARRAY_BUFFER_ARB, sizeof(Point)*4, (void*)tex, GL_STATIC_DRAW_ARB);
		//~ glBindBuffer_(GL_ARRAY_BUFFER_ARB, 0);
		glGenTextures(1, &rect_texture);
		glBindTexture(GL_TEXTURE_2D, rect_texture);
		TEX_CLAMP;
		TEX_LINEAR;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		"\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377"
		"\377\377\377\377\377\377\377\377\377\377\0\377\377\377\0\377\377\377\377"
		"\377\377\377\377\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0"
		"\377\377\377\0");
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		// fix vertex pointers to one memory area
		glVertexPointer(2, GL_FLOAT, 0, vertexCoord);
		glTexCoordPointer(2, GL_FLOAT, 0, texCoord);
		//~ glBindTexture(GL_TEXTURE_2D, 0);
	}
	return 1;
}

/**
 * @descr Check, if screen exists. Useful if you have windowless version of y our application, e.g. server.
 * @group graphics/window
 * @return true if screen was initialized
 * */
bool isInit() {
	return screen != NULL;
}

/**
 * @descr Get window's width
 * @group graphics/window
 * @return width of the window
 * @see getWindowHeight
 * */
int getWindowWidth() {
	return screen->w;
}

/**
 * @descr Get window's height
 * @group graphics/window
 * @return height of the window
 * @see getWindowWidth
 * */
int getWindowHeight() {
	return screen->h;
}

/**
 * @descr Swap buffers and present graphics
 * @group graphics/window
 * */
void swapBuffers() {
	FLUSH_BUFFER();
	SDL_GL_SwapBuffers();
}

/**
 * @descr Set window's title
 * @group graphics/window
 * @var text to replace the caption
 * @see init
 * */
void caption(const char * text) {
	SDL_WM_SetCaption(text, text);
}

/**
 * @descr Get list of possible screen modes. You need this if you want to run application in fullscreen mode.
 * @group graphics/window
 * @return array of pointers to SDL_Rect structure.
 * */
SDL_Rect ** getModes() {
	SDL_Rect ** modes = SDL_ListModes(0, SDL_OPENGL | SDL_FULLSCREEN);
	if(modes == (SDL_Rect **)0 || modes == (SDL_Rect **)-1)
		return 0;
	return modes;
}

int (*showCursor)(int mode) = &SDL_ShowCursor;
int (*grabCursor)(int mode) = &SDL_WM_GrabInput;