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
#include "SOIL/SOIL.h"
#include "render.h"
//~ void stackdumper(int param)
//~ {
	//~ void *array[10];
	//~ size_t size;
	//~ char **strings;
	//~ size_t i;
//~ 
	//~ size = backtrace(array, 10);
	//~ strings = backtrace_symbols (array, size);
//~ 
	//~ printf ("Obtained %zd stack frames.\n", size);
//~ 
	//~ for (i = 0; i < size; i++)
		//~ printf ("%s\n", strings[i]);
//~ 
	//~ free (strings);
//~ }

//~ #if 0
void colorMask(bool r, bool g, bool b, bool a) {
	glColorMask(r,g,b,a);
}
//~ #endif

GLuint quadlist, pointlist, null_texture;

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
	Uint32 flags = SDL_OPENGL;
	
	
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
		//~ SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
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
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		/* enable blending */
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		/* enable backface culling */
		glDisable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDisable(GL_DEPTH_TEST);
		resLoaderQueue = NULL;
		if(loader)
		{
			resLoaderQueue = newQueue();
			resQueueMutex = SDL_CreateMutex();
			resShared = 0;
			SDL_CreateThread(resLoaderThread, (void *)NULL);
			glGenTextures(1, &null_texture);
			glBindTexture(GL_TEXTURE_2D, null_texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, "\0\0\0\0");
		}
	}

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDepthRange(-10000,10000);
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
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
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

/**
 * @descr Shows the cursor
 * @group graphics/window
 * */
void showCursor() {
	SDL_ShowCursor(SDL_ENABLE);
}

/**
 * @descr Hides the cursor
 * @group graphics/window
 * */
void hideCursor() {
	SDL_ShowCursor(SDL_DISABLE);
}

/**
 * @descr Enables depth test. Useless if you didn't pass 'd' option to cheetah.init. Equivalent to glEnable(GL_DEPTH_TEST);
 * @group graphics/drawing
 * @see disableDepthTest
 * */
void enableDepthTest() {
	glEnable(GL_DEPTH_TEST);
}

/**
 * @descr Disables depth test. Useless if you didn't pass 'd' option to cheetah.init. Equivalent to glDisable(GL_DEPTH_TEST);
 * @group graphics/drawing
 * @see enableDepthTest
 * */
void disableDepthTest() {
	glDisable(GL_DEPTH_TEST);
}

/**
 * @descr Enables stencil test. Useless if you didn't pass 'd' option to cheetah.init. Equivalent to glEnable(GL_STENCIL_TEST);
 * @group graphics/drawing
 * @see disableStencilTest
 * */
void enableStencilTest() {
	glEnable(GL_STENCIL_TEST);
}

/**
 * @descr Disables stencil test. Useless if you didn't pass 'd' option to cheetah.init. Equivalent to glDisable(GL_STENCIL_TEST);
 * @group graphics/drawing
 * @see enableStencilTest
 * */
void disableStencilTest() {
	glDisable(GL_STENCIL_TEST);
}

/**
 * @descr Gets the number of milliseconds past from the execution time. Equivalent to SDL_GetTicks();
 * @group graphics/timer
 * */
unsigned int getTicks() {
	return SDL_GetTicks();
}

/**
 * @descr Gets the time in seconds past from the execution time.
 * @group graphics/timer
 * */
double getTime() {
	return (double)SDL_GetTicks()/1000;
}

/**
 * @descr Do nothing some time.
 * @group graphics/timer
 * @var delay in milliseconds (1/1000 s)
 * @see sleep
 * */
void delay(unsigned int ms) {
	return SDL_Delay(ms);
}

/**
 * @descr Do nothing some time.
 * @group graphics/timer
 * @var delay in seconds
 * @see delay
 * */
void sleep(unsigned int sec) {
	return SDL_Delay(sec*1000);
}


//~ void translate(double translateX, double translateY) {
	//~ glTranslated(translateX, translateY, 0);
//~ }

/**
 * @descr Move object relatively to the current matrix position.
 * @group graphics/drawing
 * @var x coordinate
 * @var y coordinate
 * @see scale rotate translateObject
 * */
void move(double translateX, double translateY) {
	glTranslated(translateX, translateY, 0);
}

/**
 * @descr Scale object relatively to the current matrix size (initially, matrix has size 1x1 pixel).
 * @group graphics/drawing
 * @var x scale
 * @var y scale
 * @see move rotate translateObject
 * */
void scale(double scaleX, double scaleY) {
	glScaled(scaleX, scaleY, 1);
}

/**
 * @descr Rotate object relatively to the current matrix angle.
 * @group graphics/drawing
 * @var angle
 * @see move scale translateObject
 * */
void rotate(double angle) {
	glRotated(angle, 0, 0, 1);
}

/**
 * @descr Move, rotate and scale object relatively to it's center (origin)
 * @group graphics/drawing
 * @var x coordinate
 * @var y coordinate
 * @var angle
 * @var x scale
 * @var y scale
 * @var x offset (center)
 * @var y offset (center)
 * @see move scale rotate
 * */
void translateObject(double x, double y, double angle, double width, double height, double origin_x, double origin_y) {
	if(x || y) glTranslated(x, y, 0);
	//~ glTranslated(origin_x, origin_y, 0);
	if(angle) glRotated(angle, 0, 0, 1);
	if(width != 1.0 || height != 1.0) glScalef(width, height, 1);
	if(origin_x || origin_y) glTranslated(-origin_x/width, -origin_y/height, 0);
	
	//glTranslated(ox, oy, 0);
}

/**
 * @descr Enable or disable autoscale. Autoscale allows you to draw stuff in the fixed coordinates, and engine automatically translates all coordinates if window changes his size. Is you want to control screen size yourself, disable this.
 * @group graphics/drawing
 * @var enable or disable autoscale
 * */
void autoScale(bool autoScale) {
	screenScale.autoScale = autoScale;
}

//~ void printMatrix(GLdouble *m)
//~ {
	//~ int i, j;
	//~ for (i = 0; i < 4; i++)
	//~ {
		//~ for (j = 0; j < 4; j++)
		//~ {
			//~ printf("%E\t", *(m+i*4+j));
		//~ }
		//~ putc('\n', stdout);
	//~ }
//~ }

void doAutoScale() {
	glLoadIdentity();
	glTranslatef(screenScale.offsetX, screenScale.offsetY, 0);
	glScalef(screenScale.scaleX, screenScale.scaleY, 1);
	//~ glPushMatrix();
	//~ GLdouble projMatrix[16];
	//~ GLdouble modelMatrix[16];
	//~ glGetDoublev(GL_PROJECTION, projMatrix);
	//~ printMatrix(projMatrix);
	//~ glGetDoublev(GL_MODELVIEW, modelMatrix);
	//~ printMatrix(modelMatrix);
	//~ putc('\n', stdout);
	//~ glPopMatrix();
}

/**
 * @descr Enable or disable blending. Drawing without blending usually faster, but textures with alpha-channel will be poor. Blending is enabled by defaults.
 * @group graphics/drawing
 * @var enable or disable blending
 * @see enableBlend disableBlend
 * */
void blend(bool blend) {
	if(blend) glEnable(GL_BLEND);
	else glDisable(GL_BLEND);
}

/**
 * @descr Enable blending.
 * @group graphics/drawing
 * @see blend
 * */
void enableBlend() {
	glEnable(GL_BLEND);
}

/**
 * @descr Disable blending.
 * @group graphics/drawing
 * @see blend
 * */
void disableBlend() {
	glDisable(GL_BLEND);
}

/**
 * @descr Push the transformation matrix. Equivalent to glPushMatrix();
 * @group graphics/drawing
 * @see pop reset
 * */
void push() {
	glPushMatrix();
	if (glGetError() == GL_STACK_OVERFLOW)
		myError("No more free slots to save the view.");
}

/**
 * @descr Pop the transformation matrix. Equivalent to glPopMatrix();
 * @group graphics/drawing
 * @see push reset
 * */
void pop() {
	glPopMatrix();
	if (glGetError() == GL_STACK_UNDERFLOW)
		myError("No saved view was found.");
}

/**
 * @descr Reset the transformation matrix. Equivalent to glLoadIdentity();
 * @group graphics/drawing
 * @see pop push
 * */
void reset() {
	glLoadIdentity();
}

/**
 * @descr Draw line.
 * @group graphics/drawing
 * @var x start
 * @var y start
 * @var x end
 * @var y end
 * @see color lineWidth smooth
 * */
void line(double x1, double y1, double x2, double y2) {
	glBegin(GL_LINES);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glEnd();
}

/**
 * @descr Draw rectangle. Note, if you bind texture or shader, filled rectangle may be shaded or textured.
 * @group graphics/drawing
 * @var is rectangle filled
 * @see color rectanglexy imageDraw framebufferDraw
 * */
void rectangle(bool filled) {
	if(filled)
		glCallList(quadlist);
	else
	{
		glBegin(GL_LINE_LOOP);
		glVertex2f(0, 0);
		glVertex2f(1, 0);
		glVertex2f(1, 1);
		glVertex2f(0, 1);
		glEnd();
	}
}

/**
 * @descr Draw rectangle of given size at a given position. Note, if you bind texture or shader, filled rectangle may be shaded or textured.
 * @group graphics/drawing
 * @var position of left top corner
 * @var position of left top corner
 * @var width of quad
 * @var height of quad
 * @var is rectangle filled
 * @see color rectangle imageDraw framebufferDraw
 * */
void rectanglexy(float x, float y, float w, float h, bool filled) {
	if(filled)
	{
		glBegin(GL_QUADS);
		glVertex2f(x,   y);
		glVertex2f(x+w, y);
		glVertex2f(x+w, y+h);
		glVertex2f(x,   y+h);
		glEnd();
	}
	else
	{
		glBegin(GL_LINE_LOOP);
		glVertex2f(x,   y);
		glVertex2f(x+w, y);
		glVertex2f(x+w, y+h);
		glVertex2f(x,   y+h);
		glEnd();
	}
}

/**
 * @descr Draw circle.
 * @group graphics/drawing
 * @var radius
 * @var segments (more is slower and better)
 * @var is circle filled
 * @see color circlexy
 * */
void circle(double rad, double segments, bool filled) {
	int i;
	const double DBLPI = 3.1415926 * 2;
	GLdouble angle;
	glBegin(filled ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
	int max = segments;
	for (i = 0; i <= max; i++)
	{
		angle = DBLPI / segments * (double)i;
		glVertex2d(sin(angle) * rad, cos(angle) * rad);
	}
	glEnd();
}

/**
 * @descr Draw circle at a given position.
 * @group graphics/drawing
 * @var position of circle center
 * @var position of circle center
 * @var radius
 * @var segments (more is slower and better)
 * @var is circle filled
 * @see color circle
 * */
void circlexy(float x, float y, double rad, double segments, bool filled) {
	int i;
	const double DBLPI = 3.1415926 * 2;
	GLdouble angle;
	glBegin(filled ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
	int max = segments;
	for (i = 0; i <= max; i++)
	{
		angle = DBLPI / segments * (double)i;
		glVertex2d(sin(angle) * rad + x, cos(angle) * rad + y);
	}
	glEnd();
}

/**
 * @descr Draw point.
 * @group graphics/drawing
 * @see color pointxy pointSize smooth getPointSize
 * */
void point() {
	glCallList(pointlist);
}

/**
 * @descr Draw point at a given position.
 * @group graphics/drawing
 * @var position
 * @var position
 * @see color point pointSize smooth getPointSize
 * */
void pointxy(float x, float y) {
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}

/**
 * @descr Sets the current point size. Not all platforms support point size correctly.
 * @group graphics/drawing
 * @var point size
 * @see point getPointSize
 * */
void pointSize(float size) {
	glPointSize(size);
}

/**
 * @descr Gets the current point size.
 * @group graphics/drawing
 * @return point size
 * @see point pointSize
 * */
double getPointSize() {
	double s;
	glGetDoublev(GL_POINT_SIZE, &s);
	return s;
}

/**
 * @descr Sets the line width. Not all platforms support line width correctly.
 * @group graphics/drawing
 * @var line size
 * @see line getLineWidth
 * */
void lineWidth(float width) {
	glLineWidth(width);
}

/**
 * @descr Gets the line width.
 * @group graphics/drawing
 * @return line size
 * @see line lineWidth
 * */
double getLineWidth() {
	double s;
	glGetDoublev(GL_LINE_WIDTH, &s);
	return s;
}

/**
 * @descr Enables or disables lines and points smoothing. Not all platforms support this.
 * @group graphics/drawing
 * @var enable or disable smoothing
 * @see line point
 * */
void smooth(bool smooth) {
	if(smooth) {
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
	} else {
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_LINE_SMOOTH);
	}
}

/**
 * @descr Sets the color.
 * @group graphics/drawing
 * @var red (0 - 255)
 * @var green (0 - 255)
 * @var blue (0 - 255)
 * @var alpha (0 - 255)
 * @see colorf
 * */
void color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	glColor4ub(r,g,b,a);
}

/**
 * @descr Sets the color, float.
 * @group graphics/drawing
 * @var red (0 - 1) float
 * @var green (0 - 1) float
 * @var blue (0 - 1) float
 * @var alpha (0 - 1) float
 * @see colorf
 * */
void colorf(float r, float g, float b, float a) {
	glColor4f(r,g,b,a);
}

/**
 * @descr Sets the color for clear screen.
 * @group graphics/drawing
 * @var red
 * @var green
 * @var blue
 * @var alpha
 * @see clear
 * */
void clearColor(float r, float g, float b, float a) {
	glClearColor(r,g,b,a);
}

/**
 * @descr Sets the blending mode. Blending modes allow you to create some cool effects.
 * @group graphics/drawing
 * @var One of possible blending modes. 0 is defaults. Use pre-defined blending modes:
 *  * cheetah.blendAlpha or 0 - defaults
 *  * cheetah.blendMultiplicative
 *  * cheetah.blendAdditive
 *  * cheetah.blendSubstractive
 *  * cheetah.blendMask - render inverse alpha channel
 *  * cheetah.blendScreen - as photoshop blend mode
 *  * cheetah.blendDetail - interesting effect, allows to use gray detail textures
 * */
void blendMode(int mode) {
	switch(mode) {
		case blend_alpha:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case blend_multiplicative:
			glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case blend_additive:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		case blend_screen:
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
			break;
		case blend_substractive:
			glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
			break;
		case blend_detail:
			glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
			break;
		case blend_mask:
			glBlendFunc(GL_ZERO, GL_SRC_ALPHA);
			break;
	}
}

/**
 * @descr Specify the equation used for both the RGB blend equation and the Alpha blend equation. Equivalent to glBlendEquation(mode);
 * @group graphics/drawing
 * @var Specifies how source and destination colors are combined. It must be:
 *  * cheetah.GL_FUNC_ADD
 *  * cheetah.GL_FUNC_SUBTRACT
 *  * cheetah.GL_FUNC_REVERSE_SUBTRACT
 *  * cheetah.GL_MIN
 *  * cheetah.GL_MAX
 * */
void blendEquation(int mode) {
	glBlendEquation_(mode);
}

/**
 * @descr Specify pixel arithmetic. Equivalent to glBlendFunc(sourcefactor, destinationfactor);
 * @group graphics/drawing
 * @var Specifies how the red, green, blue, and alpha source blending factors are computed. The following symbolic constants are accepted: 
 *  * cheetah.GL_ZERO
 *  * cheetah.GL_ONE
 *  * cheetah.GL_DST_COLOR
 *  * cheetah.GL_ONE_MINUS_DST_COLOR
 *  * cheetah.GL_SRC_ALPHA
 *  * cheetah.GL_ONE_MINUS_SRC_ALPHA
 *  * cheetah.GL_DST_ALPHA
 *  * cheetah.GL_ONE_MINUS_DST_ALPHA
 *  * cheetah.GL_SRC_ALPHA_SATURATE
 * @var Specifies how the red, green, blue, and alpha destination blending factors are computed. Eight symbolic constants are accepted: 
 *  * cheetah.GL_ZERO
 *  * cheetah.GL_ONE
 *  * cheetah.GL_SRC_COLOR
 *  * cheetah.GL_ONE_MINUS_SRC_COLOR
 *  * cheetah.GL_SRC_ALPHA
 *  * cheetah.GL_ONE_MINUS_SRC_ALPHA
 *  * cheetah.GL_DST_ALPHA
 *  * cheetah.GL_ONE_MINUS_DST_ALPHA
 * */
void blendFunc(int sourcefactor, int destinationfactor) {
	glBlendFunc(sourcefactor, destinationfactor);
}

/**
 * @descr Clear screen. Usually used in lQuery.addhook(cheetah.clear). Slow. Do not use it if you have in your game image background.
 * @group graphics/drawing
 * @see clearColorDepth clearColorStencil clearDepth clearStencil
 * */
void clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}

/**
 * @descr Clear screen and depth buffer. Usually used in lQuery.addhook(cheetah.clearColorDepth). Slow.
 * @group graphics/drawing
 * @see clear clearColorStencil clearDepth clearStencil
 * */
void clearColorDepth() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * @descr Clear screen and stencil buffer. Usually used in lQuery.addhook(cheetah.clearColorStencil). Slow.
 * @group graphics/drawing
 * @see clear clearColorDepth clearDepth clearStencil
 * */
void clearColorStencil() {
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

/**
 * @descr Clear depth buffer. Usually used in lQuery.addhook(cheetah.clearDepth). Slow.
 * @group graphics/drawing
 * @see clear clearColorDepth clearColorStencil clearStencil
 * */
void clearDepth() {
	glClear(GL_DEPTH_BUFFER_BIT);
}

/**
 * @descr Clear stencil buffer. Usually used in lQuery.addhook(cheetah.clearStencil). Slow.
 * @group graphics/drawing
 * @see clear clearColorDepth clearColorStencil clearDepth
 * */
void clearStencil() {
	glClear(GL_STENCIL_BUFFER_BIT);
}

void stencilFunc(int func, int ref, unsigned int mask) {
	glStencilFunc(func, ref, mask);
}

void stencilOp(int fail, int zfail, int zpass) {
	glStencilOp(fail, zfail, zpass);
}

void drawToStencil() {
	glStencilFunc (GL_NEVER, 0x0, 0x1);
	glStencilOp(GL_REPLACE,GL_KEEP,GL_REPLACE);
}

void drawUsingStencil() {
	glStencilFunc (GL_EQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
}

//~ #if 0


inline unsigned char * loadImageData(const char *name, int *width, int *height, int *channels)
{
	unsigned int file_size;
	unsigned char *img;
	unsigned char *myBuf;
	myBuf = loadfile(name, &file_size);
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
	tex_id = SOIL_internal_create_OGL_texture(
			img, width, height, channels,
			0, SOIL_FLAG_TEXTURE_REPEATS,
			GL_TEXTURE_2D, GL_TEXTURE_2D,
			GL_MAX_TEXTURE_SIZE);
	//~ 
	while(*options)
	{
		if(*options == 'n') {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
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
	char ch;
	int width, height, channels, repeat = 1, instant = 0;
	unsigned int tex_id;
	unsigned char *img;
	unsigned char *myBuf;
	if(!screen)
	{
		myError("Call init function before!");
		return;
	}
	while(*options)
	{
		if(*options == 'i') {instant = 1; break;}
		options++;
	}
	if(!resLoaderQueue||instant)
	{
		img = loadImageData(name, &width, &height, &channels);
		if(img == NULL)
		{
			myError("can't load image %s", name);
			return;
		}
		tex_id = loadImageTex(options, img, width, height, channels);
		//~ new(ptr, Image, 1);
		ptr->id = tex_id;
		ptr->w = width;
		ptr->h = height;
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

/**
 * @descr Bind Image object. Equivalent to glBindTexture.
 * @group graphics/image
 * @var Image object
 * */
inline void imageBind(Image * image) {
	if(resLoaderQueue&&image->id==null_texture&&!image->queued)
	{
		Resource r;
		image->queued = 1;
		r.image = image;
		enqueue(resLoaderQueue, r);
	}
	glBindTexture(GL_TEXTURE_2D, image->id);
}

/**
 * @descr Enable texturing. Equivalent to glEnable(GL_TEXTURE_2D).
 * @group graphics/image
 * */
void enableTexture2D() {
	glEnable(GL_TEXTURE_2D);
}

/**
 * @descr Disable texturing. Equivalent to glDisable(GL_TEXTURE_2D).
 * @group graphics/image
 * */
void disableTexture2D() {
	glDisable(GL_TEXTURE_2D);
}

/**
 * @descr Draw while image using 1x1 pixel quad. You may change quad size and position using transformations.
 * @group graphics/image
 * @var Image object
 * */
void imageDraw(Image * image) {
	imageBind(image);
	glEnable(GL_TEXTURE_2D);
	glCallList(quadlist);
	glDisable(GL_TEXTURE_2D);
}

/*TODO: Заюзать шейдры для передачи координат, а выводить квад листом. В шрифтах аналогично*/
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
	glEnable(GL_TEXTURE_2D);
	qx = qx/(float)image->w;
	qy = qy/(float)image->h;
	qw = qw ? qx + qw/(float)image->w : 1;
	qh = qh ? qy + qh/(float)image->h : 1;
	glBindTexture(GL_TEXTURE_2D, image->id);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(qx, qy);   glVertex2i(0, 0);
	glTexCoord2f(qx, qh);   glVertex2i(0, 1);
	glTexCoord2f(qw, qh);   glVertex2i(1, 1);
	glTexCoord2f(qw, qy);   glVertex2i(1, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
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
	glEnable(GL_TEXTURE_2D);
	qx = qx/(float)image->w;
	qy = qy/(float)image->h;
	qw = qw ? qx + qw/(float)image->w : 1;
	qh = qh ? qy + qh/(float)image->h : 1;
	glBegin(GL_QUADS);
	glTexCoord2f(qx, qy);   glVertex2f(x, y);
	glTexCoord2f(qx, qh);   glVertex2f(x, y+h);
	glTexCoord2f(qw, qh);   glVertex2f(x+w, y+h);
	glTexCoord2f(qw, qy);   glVertex2f(x+w, y);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

/**
 * @descr Set the current active texture for multitexturenig. Equivalent to glActiveTexture(GL_TEXTURE0 + i).
 * @group graphics/image
 * @var number of texture slot (min 0, max 7)
 * */
void activeTexture(int i) {
	glActiveTexture_(GL_TEXTURE0 + i);
}

//~ /**
 //~ * @descr Delete image and free memory. 
 //~ * @group graphics/image
 //~ * @var Image object
 //~ * */
void deleteImage(Image * ptr) {
	if(ptr) glDeleteTextures(1, &ptr->id);
	else myError("Trying to free a null-image. Maybe, you did it manually?");
}

Vbo * newVbo(Point * data, Point * tex, unsigned int count) {
	Vbo *ptr;
	new(ptr, Vbo, 1);
	ptr->count = count;
	glGenBuffers_(1, &ptr->id);
	glBindBuffer_(GL_ARRAY_BUFFER_ARB, ptr->id);
	glBufferData_(GL_ARRAY_BUFFER_ARB, sizeof(Point)*4*count, (void*)data, GL_STATIC_DRAW_ARB);
	glGenBuffers_(1, &ptr->tex);
	glBindBuffer_(GL_ARRAY_BUFFER_ARB, ptr->tex);
	glBufferData_(GL_ARRAY_BUFFER_ARB, sizeof(Point)*4*count, (void*)tex, GL_STATIC_DRAW_ARB);
	return ptr;
}

void vboDraw(Vbo * ptr) {
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY); 
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
	glBindBuffer_(GL_ARRAY_BUFFER_ARB, ptr->id);
	glVertexPointer(2, GL_FLOAT, 0, (char *) NULL);
	glBindBuffer_(GL_ARRAY_BUFFER_ARB, ptr->tex);
	glTexCoordPointer(2, GL_FLOAT, 0, (char *) NULL);
	glDrawArrays(GL_QUADS, 0, ptr->count*4); 
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

Vbo * newVboPoints(Point * data, unsigned int count) {
	Vbo *ptr;
	new(ptr, Vbo, 1);
	ptr->count = count;
	int i;
	if(supported.PS)
	{
		if(supported.VBO)
		{
			glGenBuffers_(1, &ptr->id);
			glBindBuffer_(GL_ARRAY_BUFFER_ARB, ptr->id);
			glBufferData_(GL_ARRAY_BUFFER_ARB, sizeof(Point)*count, (void*)data, GL_STATIC_DRAW_ARB);
		}
		else
		{
			printf("No VBO support: rendering VBO's using lists.\n");
			ptr->id = glGenLists(1);
			glNewList(ptr->id, GL_COMPILE);
			glBegin(GL_POINTS);
			for (i = 0; i < count; i++)
				glVertex2f(data[i].x, data[i].y);
			glEnd();
			glEndList();
		}
	}
	else
	{
		ptr->data = data;
		printf("No Point Sprite support: falling down to quads.\n");
	}
	
	return ptr;
}
void vboDrawSprites(Vbo * ptr, Image * img, float size) {
	int i;
	glEnable(GL_TEXTURE_2D);
	imageBind(img);
	if(supported.PS)
	{
		glPointSize(size);
		glEnable(GL_POINT_SPRITE);
		glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
		
		if(supported.VBO)
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glBindBuffer_(GL_ARRAY_BUFFER_ARB, ptr->id);
			glVertexPointer(2, GL_FLOAT, 0, (char *) NULL);
			glDrawArrays(GL_POINTS, 0, ptr->count); 
			glDisableClientState(GL_VERTEX_ARRAY);
		}
		else
			glCallList(ptr->id);
	}
	else
	{
		for (i = 0; i < ptr->count; i++)
		{
			glPushMatrix();
			glTranslatef(ptr->data[i].x - size/2, ptr->data[i].y - size/2, 0.0);
			glScalef(size, size, 1);
			glCallList(quadlist);
			glPopMatrix();
		}
	}
	
	glDisable(GL_TEXTURE_2D);
}

Vbo * newVboPoints3(Point3 * data, unsigned int count) {
	Vbo *ptr;
	new(ptr, Vbo, 1);
	ptr->count = count;
	int i;
	glGenBuffers_(1, &ptr->id);
	glBindBuffer_(GL_ARRAY_BUFFER_ARB, ptr->id);
	glBufferData_(GL_ARRAY_BUFFER_ARB, sizeof(Point3)*count, (void*)data, GL_STATIC_DRAW_ARB);
	return ptr;
}
void vboDrawSprites3(Vbo * ptr, Image * img, float size) {
	glEnable(GL_TEXTURE_2D);
	imageBind(img);
	glPointSize(size);
	glEnable(GL_POINT_SPRITE);
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer_(GL_ARRAY_BUFFER_ARB, ptr->id);
	glVertexPointer(3, GL_FLOAT, 0, (char *) NULL);
	glDrawArrays(GL_POINTS, 0, ptr->count); 
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glDisable(GL_TEXTURE_2D);
}

void deleteVbo(Vbo * ptr) {
	if(ptr) {
		glDeleteBuffers_(1, &ptr->id);
	}
	else myError("Trying to free a null-VBO. Maybe, you did it manually?");
}
