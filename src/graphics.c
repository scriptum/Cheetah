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
//~ #include "SOIL/SOIL.h"
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

GLuint quadlist, pointlist;


/**
 * @descr Create window and initialize all OpenGL's stuff. You MUST call this before any graphics function, e.g. cheetah.newImage. You may call this function again to re-size window, change application title, toggle fullscreen. Other options are ignored.
 * @group graphics/window
 * @var application's title shown in titlebar
 * @var width of the window
 * @var height of the window
 * @var bits per pixel (8, 16, 32, usually 32)
 * @var string of options. Supported options:
 *  * _f_ - fullscreen
 *  * _r_ - allow to resize window
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
	char ch;
	Uint32 flags = SDL_OPENGL;

	while(*attr)
	{
		ch = *attr;
		//~ printf("%d %d\n", ch, 'v');
		if(ch == 'f') fullscreen = 1;
		if(ch == 'r') resizable = 1;
		if(ch == 'v') vsync = 1;
		if(ch == 'd') depth = 1;
		if(ch == 's') stencil = 1;
		//if(ch == 'm') flags |= SDL_WINDOW_MAXIMIZED;
		attr++;
	}
//~ printf("%d %d\n", vsync, 'v');
	if (fullscreen) {
		flags |= SDL_FULLSCREEN;
	} else if (resizable) {
		flags |= SDL_RESIZABLE;
	}
	
	if(width < 1) width = 1;
	if(height < 1) height = 1;
	
	if (screen == NULL) {
		if ( SDL_Init ( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) != 0 )
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
void setWindowCaption(const char * text){
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
	return SDL_Delay(sec);
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
void setAutoScale(bool autoScale) {
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
 * @see color setLineWidth setSmooth
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
 * @see color imageDraw framebufferDraw
 * */
void rectangle(bool filled) {
	if(filled)
		glCallList(quadlist);
	else
	{
		glBegin(GL_LINE_LOOP);
		glVertex2d(0, 0);
		glVertex2d(1, 0);
		glVertex2d(1, 1);
		glVertex2d(0, 1);
		glEnd();
	}
}

/**
 * @descr Draw circle.
 * @group graphics/drawing
 * @var radius
 * @var segments (more is slower and better)
 * @var is circle filled
 * @see color
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
 * @descr Draw point.
 * @group graphics/drawing
 * @see color setPointSize setSmooth getPointSize
 * */
void point() {
	glCallList(pointlist);
}

/**
 * @descr Gets the current point size.
 * @group graphics/drawing
 * @return point size
 * @see point setPointSize
 * */
double getPointSize() {
	double s;
	glGetDoublev(GL_POINT_SIZE, &s);
	return s;
}

/**
 * @descr Sets the current point size. Not all platforms support point size correctly.
 * @group graphics/drawing
 * @var point size
 * @see point getPointSize
 * */
void setPointSize(float size) {
	glPointSize(size);
}

/**
 * @descr Sets the line width. Not all platforms support line width correctly.
 * @group graphics/drawing
 * @var line size
 * @see line getLineWidth
 * */
void setLineWidth(float width) {
	glLineWidth(width);
}

/**
 * @descr Gets the line width.
 * @group graphics/drawing
 * @return line size
 * @see line setLineWidth
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
void setSmooth(bool smooth) {
	if(smooth) {
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
	} else {
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_LINE_SMOOTH);
	}
}

/**
 * @descr Sets the color. Faster than setColor.
 * @group graphics/drawing
 * @var red
 * @var green
 * @var blue
 * @var alpha
 * @see setColor
 * */
void color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	glColor4ub(r,g,b,a);
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
void setClearColor(float r, float g, float b, float a) {
	glClearColor(r,g,b,a);
}

/**
 * @descr Sets the blending mode. Blending modes allow you to create some cool effects.
 * @group graphics/drawing
 * @var One of possible blending modes. 0 is defaults. Use pre-defined blending modes:
 * cheetah.blendAlpha or 0 - defaults
 * cheetah.blendMultiplicative
 * cheetah.blendAdditive
 * cheetah.blendSubstractive
 * cheetah.blendScreen - as photoshop blend mode
 * cheetah.blendDetail - interesting effect, allows to use gray detail textures
 * */
void setBlendMode(int mode) {
	if(mode == blend_substractive) {
		glBlendEquation_(GL_FUNC_REVERSE_SUBTRACT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glBlendEquation_(GL_FUNC_ADD);
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
			case blend_detail:
				glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
				break;
		}
	}
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

void setStencilFunc(int func, int ref, unsigned int mask) {
	glStencilFunc(func, ref, mask);
}

void setStencilOp(int fail, int zfail, int zpass) {
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

#if 0

/**
 * @descr Load image from disc.
 * @group graphics/image
 * @var file name
 * @return Image object
 * */
Image *newImage(const char *name) {
	return newImageOpt(name, "");
}

/**
 * @descr Load image from disc with specific options.
 * @group graphics/image
 * @var file name
 * @var string of options. This is depends on image loading module you use. Supported options:
 *  * _n_ - use nearest interpolation
 *  * _m_ - generate mip-maps (automatically sets mip-map interpolation)
 * @return Image object
 * */
Image *newImageOpt(const char *name, const char *options) {
	Image *ptr;
	char ch;
	if(!screen)
	{
		myError("Call init function before!");
		return NULL;
	}
	int width, height, channels, repeat = 1;
	unsigned int tex_id;
	unsigned char* img;
	unsigned int file_size;
	unsigned char * myBuf = loadfile(name, &file_size);
	img = SOIL_load_image_from_memory(
				myBuf, sizeof(unsigned char) * file_size,
				&width, &height, &channels,
				0 );
	if( NULL == img ) {
		myError("can't load image %s", name);
		return 0;
	}
	tex_id = SOIL_internal_create_OGL_texture(
			img, width, height, channels,
			0, SOIL_FLAG_TEXTURE_REPEATS,
			GL_TEXTURE_2D, GL_TEXTURE_2D,
			GL_MAX_TEXTURE_SIZE );
	
	while(*options)
	{
		ch = *options;
		if(ch == 'n') {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		options++;
	}
	SOIL_free_image_data( img );
	ptr = (Image *)malloc(sizeof(Image));
	ptr->id = tex_id;
	ptr->w = width;
	ptr->h = height;
	return ptr;
}

#endif

/**
 * @descr Bind Image object. Equivalent to glBindTexture.
 * @group graphics/image
 * @var Image object
 * */
void imageBind(Image * image) {
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
	glBindTexture(GL_TEXTURE_2D, image->id);
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
	glBindTexture(GL_TEXTURE_2D, image->id);
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
 * @descr Set the current active texture for multitexturenig. Equivalent to glActiveTexture(GL_TEXTURE0 + i).
 * @group graphics/image
 * @var number of texture slot (min 0, max 7)
 * */
void activeTexture(int i) {
	glActiveTexture_(GL_TEXTURE0 + i);
}

/**
 * @descr Delete image and free memory.
 * @group graphics/image
 * @var Image object
 * */
void deleteImage(Image * ptr) {
	if(ptr)
		glDeleteTextures(1, &ptr->id);
	else myError("Trying to free a null-image. Maybe, you did it manually?");
}

/**
 * @descr Enable/disable smooth interpolation for image. Disabled filtering useful, if you want to fit image to pixel matrix. If this image will be scaled and/or rotated you must enable filtering (this is by defaults).
 * @group graphics/image
 * @var Image object
 * @var true means that filtering is enabled, false means that filtering is disabled
 * */
void imageFiltering(Image * img, bool enabled) {
	glBindTexture(GL_TEXTURE_2D, img->id);
	if(enabled)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}

int checkFramebufferStatus()
{
	GLenum status;
	status = (GLenum) glCheckFramebufferStatus_(GL_FRAMEBUFFER_EXT);
	switch(status) {
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			return 1;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			myError("Framebuffer incomplete, incomplete attachment\n");
			return 0;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			myError("Unsupported framebuffer format\n");
			return 0;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			myError("Framebuffer incomplete, missing attachment\n");
			return 0;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			myError("Framebuffer incomplete, attached images must have same dimensions\n");
			return 0;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			myError("Framebuffer incomplete, attached images must have same format\n");
			return 0;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			myError("Framebuffer incomplete, missing draw buffer\n");
			return 0;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			myError("Framebuffer incomplete, missing read buffer\n");
			return 0;
	}
	return 0;
}

/**
 * @descr Create framebuffer object. Note, that not all video drivers support this. It's recommend to check returning value using cheetah.inPointer and check framebuffer support using cheetah.supported.FBO.
 * @group graphics/framebuffer
 * @var width
 * @var height
 * @var string of options. Supported options:
 *  * _a_ - enable alpha channel
 *  * _n_ - enable smooth interpolation
 *  * _r_ - repeat as texture (not all faramebuffers need this)
 *  * _1_ - create 8 bits (1 byte) per channel framebuffer (default)
 *  * _2_ - create 16 bits (2 byte) per channel framebuffer (slow), not all systems support this
 *  * _4_ - create 32 bits (4 byte) per channel framebuffer (very SLOW), use only if you know, that you doing, not all systems support this
 * @return Framebuffer object
 * */
Framebuffer * newFramebuffer(unsigned int width, unsigned int height, const char * options) {
	//unsigned int percision, bool alpha, bool interpolation, bool repeat) {
	unsigned int percision = 8;
	bool alpha = 0;
	bool interpolation = 1;
	bool repeat = 0;
	Image *ptr;
	Framebuffer *fboptr;
	GLint current_fbo;
	GLenum internal, format;
	bool status;
	char ch;
	
	if(!screen)
	{
		myError("Call init function before!");
		return 0;
	}
	
	if(!supported.FBO) {
		myError("Framebuffers are not supported on this machine. You'd better to check it in script (cheetah.supported)");
		return 0;
	}
	
	while(*options)
	{
		ch = *options;
		options++;
		if(ch == 'a') {alpha = 1; continue;}
		if(ch == 'n') {interpolation = 0; continue;}
		if(ch == 'r') {repeat = 1; continue;}
		if(ch == '4') {percision = 32; continue;}
		if(ch == '2') {percision = 16; continue;}
		if(ch == '1') {percision = 8; continue;}
	}
	
	if(percision == 32) {
		internal = alpha ? GL_RGBA32F_ARB : GL_RGB32F_ARB;
		format = GL_FLOAT;
	}
	else if(percision == 16) {
		internal = alpha ? GL_RGBA16F_ARB : GL_RGB16F_ARB;
		format = GL_HALF_FLOAT_ARB;
	}
	else {
		//~ if(percision != 8) myError("Invalid parameter in framebuffer's percision (8 expected, got %d). Using 8bit framebuffer.", percision);
		internal = alpha ? GL_RGBA : GL_RGB;
		format = GL_UNSIGNED_BYTE;
	}
	
	//save current fbo
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING_EXT, &current_fbo);
	
	ptr = (Image*)malloc(sizeof(Image));
	fboptr = (Framebuffer*)malloc(sizeof(Framebuffer));
	ptr->w = width;
	ptr->h = height;
	//~ GLuint depthbuffer;
	//~ glGenRenderbuffers_(1, &depthbuffer);
			//~ glBindRenderbuffer_(GL_RENDERBUFFER_EXT, depthbuffer);
			//~ glRenderbufferStorage_(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT16, width, height);
			//~ glBindRenderbuffer_(GL_RENDERBUFFER_EXT, 0);
			
	// generate texture save target
	glGenTextures(1, &ptr->id);
	glBindTexture(GL_TEXTURE_2D, ptr->id);
	
	if(interpolation) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	
	if(repeat) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, internal, width, height, 0, GL_RGBA, format, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// create framebuffer
	glGenFramebuffers_(1, &fboptr->id);
	glBindFramebuffer_(GL_FRAMEBUFFER_EXT, fboptr->id);
	glFramebufferTexture2D_(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
			GL_TEXTURE_2D, ptr->id, 0);
	//~ glFramebufferRenderbuffer_(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
					//~ GL_RENDERBUFFER_EXT, depthbuffer);
	status = checkFramebufferStatus();

	// unbind framebuffer
	glBindFramebuffer_(GL_FRAMEBUFFER_EXT, (GLuint)current_fbo);
		
	if (status) {
		fboptr->image = ptr;
		return fboptr;
	}
	else {
		glDeleteTextures(1, &ptr->id);
		glDeleteFramebuffers_(1, &fboptr->id);
		free(ptr);
		free(fboptr);
		return 0;
	}
}

/**
 * @descr Bind framebuffer object. Means, that now all graphics will be rendered to this framebuffer.
 * @group graphics/framebuffer
 * @var Framebuffer object
 * */
void framebufferBind(Framebuffer * ptr) {
	glBindFramebuffer_(GL_FRAMEBUFFER_EXT, ptr->id);
	glViewport( 0, 0, ptr->image->w, ptr->image->h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, ptr->image->w, 0, ptr->image->h, -1, 1 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

/**
 * @descr Unbind framebuffer object. Means, that now all graphics will be rendered to default screen. This function unbinds the current framebuffer object.
 * @group graphics/framebuffer
 * */
void framebufferUnbind() {
	glBindFramebuffer_(GL_FRAMEBUFFER_EXT, 0);
	glViewport( 0, 0, screen->w, screen->h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, screen->w, screen->h, 0, -1, 1 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

/**
 * @descr Draw framebuffer. Same as draw Image.
 * @group graphics/framebuffer
 * @var Framebuffer object
 * @see imageDraw
 * */
void framebufferDraw(Framebuffer * ptr) {
	imageDraw(ptr->image);
}

/**
 * @descr Draw part of framebuffer with texture coordinates.
 * @group graphics/framebuffer
 * @var Framebuffer object
 * @var x offset of framebuffer's texture
 * @var y offset of framebuffer's texture
 * @var width of framebuffer's texture
 * @var height of framebuffer's texture
 * @see imageDrawq
 * */
void framebufferDrawq(Framebuffer * ptr, float qx, float qy, float qw, float qh) {
	imageDrawq(ptr->image, qx, qy, qw, qh);
}

/**
 * @descr Delete framebuffer and free memory.
 * @group graphics/framebuffer
 * @var Framebuffer object
 * */
void deleteFramebuffer(Framebuffer * ptr) {
	if(ptr) {
		glDeleteTextures(1, &ptr->image->id);
		glDeleteFramebuffers_(1, &ptr->id);
		free(ptr->image);
	}
	else myError("Trying to free a null-framebuffer. Maybe, you did it manually?");
}

Vbo * newVbo(Point * data, Point * tex, unsigned int count) {
	Vbo * ptr = (Vbo*)malloc(sizeof(Vbo));
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
	Vbo * ptr = (Vbo*)malloc(sizeof(Vbo));
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
	glBindTexture(GL_TEXTURE_2D, img->id);
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
	Vbo * ptr = (Vbo*)malloc(sizeof(Vbo));
	ptr->count = count;
	int i;
	glGenBuffers_(1, &ptr->id);
	glBindBuffer_(GL_ARRAY_BUFFER_ARB, ptr->id);
	glBufferData_(GL_ARRAY_BUFFER_ARB, sizeof(Point3)*count, (void*)data, GL_STATIC_DRAW_ARB);
	return ptr;
}
void vboDrawSprites3(Vbo * ptr, Image * img, float size) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, img->id);
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