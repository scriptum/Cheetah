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

bool clearScreenFlag = 1;

void colorMask(bool r, bool g, bool b, bool a) {
	glColorMask(r,g,b,a);
}

/***********************************GLOBALS************************************/

GLuint quadlist, pointlist, null_texture, rect_texture, vboVer, vboTex;

GLuint prevImageId = 0;

bool antiAliasing = 1;

const float texCoordQuad[] = {0,0,0,1,1,1,1,0};
float *texCoord = NULL;
float *vertexCoord = NULL;

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
	if(angle) glRotated(angle, 0, 0, 1);
	if(width != 1.0 || height != 1.0) glScalef(width, height, 1);
	if(origin_x || origin_y) glTranslated(-origin_x/width, -origin_y/height, 0);
}

/**
 * @descr Enable or disable autoscale. Autoscale allows you to draw stuff in the fixed coordinates, and engine automatically translates all coordinates if window changes his size. Is you want to control screen size yourself, disable this.
 * @group graphics/drawing
 * @var enable or disable autoscale
 * */
void autoScale(bool autoScale) {
	screenScale.autoScale = autoScale;
}

void prepare() {
	Resource * r;
	unsigned int millis;
	globalTime = SDL_GetTicks();
	if(screenScale.autoScale)
	{
		glLoadIdentity();
		glTranslatef(screenScale.offsetX, screenScale.offsetY, 0);
		glScalef(screenScale.scaleX, screenScale.scaleY, 1);
	}
	if(resShared) {
		r = resShared;
		//~ printf("Get image %s %d\n", r->image->name, r->image->id);
		millis = globalTime;
		r->image->id = loadImageTex(r->image->options, r->data, r->image->w, r->image->h, r->image->channels);
		printf("Delayed resource loader: loaded %s with %d ms\n", r->image->name, SDL_GetTicks() - millis);
		delete(r->image->name);
		delete(r->image->options);
		resShared = 0;
	}
	if(rescaleTime && globalTime > rescaleTime)
	{
		SDL_SetVideoMode(screen->w, screen->h, 32, screen->flags);
		glViewport( 0, 0, screen->w, screen->h );
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glOrtho( 0, screen->w, screen->h, 0, -1, 1 );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		if(screenScale.autoScale)
		{
			glTranslatef(screenScale.offsetX, screenScale.offsetY, 0);
			glScalef(screenScale.scaleX, screenScale.scaleY, 1);
		}
		rescaleTime = 0;
		//~ glClear(GL_COLOR_BUFFER_BIT);
	}
	if(clearScreenFlag) glClear(GL_COLOR_BUFFER_BIT);
}

void end() {
	if(rescaleTime && globalTime > rescaleTime)
	{
		SDL_GL_SwapBuffers();
		SDL_SetVideoMode(screen->w, screen->h, 32, screen->flags);
		glViewport( 0, 0, screen->w, screen->h );
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glOrtho( 0, screen->w, screen->h, 0, -1, 1 );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		rescaleTime = 0;
		SDL_GL_SwapBuffers();
	}
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
void rectangle() {
	if(prevImageId) {
		glBindTexture(GL_TEXTURE_2D, 0);
		prevImageId = 0;
	}
	glCallList(quadlist);
}

#define RECTBORDER 4
#define RECTOFF1 0.5
#define RECTOFF2 0.5
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
void rectanglexy(float x, float y, float w, float h) {
	const float t[20] = {0,0, RECTOFF1,RECTOFF1, 1,0, RECTOFF2,RECTOFF1, 1,1, RECTOFF2,RECTOFF2, 0,1, RECTOFF1,RECTOFF2, 0,0, RECTOFF1,RECTOFF1};
	if(antiAliasing) {
		vertexCoord[0] = x - RECTBORDER;
		vertexCoord[1] = y - RECTBORDER;
		vertexCoord[2] = x;
		vertexCoord[3] = y;
		vertexCoord[4] = x + w + RECTBORDER;
		vertexCoord[5] = y - RECTBORDER;
		vertexCoord[6] = x + w;
		vertexCoord[7] = y;
		vertexCoord[8] = x + w + RECTBORDER;
		vertexCoord[9] = y + h + RECTBORDER;
		vertexCoord[10] = x + w;
		vertexCoord[11] = y + h;
		vertexCoord[12] = x - RECTBORDER;
		vertexCoord[13] = y + h + RECTBORDER;
		vertexCoord[14] = x ;
		vertexCoord[15] = y + h;
		vertexCoord[16] = x - RECTBORDER;
		vertexCoord[17] = y - RECTBORDER;
		vertexCoord[18] = x;
		vertexCoord[19] = y;
		glBindTexture(GL_TEXTURE_2D, rect_texture);
		glVertexPointer(2, GL_FLOAT, 0, vertexCoord);
		glTexCoordPointer(2, GL_FLOAT, 0, t);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 10);
	}
	if(prevImageId) {
		glBindTexture(GL_TEXTURE_2D, 0);
		prevImageId = 0;
	}
	VERTEX_COORD(x,y,w,h);
	DRAWQ;
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
	antiAliasing = smooth;
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
 * @var red (0 - 1) float
 * @var green (0 - 1) float
 * @var blue (0 - 1) float
 * @var alpha (0 - 1) float
 * @see clear
 * */
void clearColor(float r, float g, float b, float a) {
	glClearColor(r,g,b,a);
}

/**
 * @descr Toggle auto-clear screen. Disable auto-clear if you want to do it manually or you have fullscreen background.
 * @group graphics/drawing
 * @var enable or disable
 * @see clear
 * */
void clearScreen(bool enabled) {
	clearScreenFlag = enabled;
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
