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

#include <math.h>
#include <string.h>

#include "cheetah.h"
#include "render.h"
#include "cvertex.h"
#include "test.h"

extern void resLoaderMainThread();
int getWindowHeight();

unsigned prevColor = 0xffffffff;

void colorMask(bool r, bool g, bool b, bool a) {
	glColorMask(r,g,b,a);
}

/***********************************GLOBALS************************************/

GLuint quadlist, pointlist, null_texture, rect_texture, vboVer, vboTex;

GLuint prevImageId = 0;

bool antiAliasing = 1;
int vertexCounter;
float *texCoord = NULL;
float *vertexCoord = NULL;

static void flushBuffer()
{
	FLUSH_BUFFER();
}

void enableDepthTest() {
	flushBuffer();
	glEnable(GL_DEPTH_TEST);
}

void disableDepthTest() {
	flushBuffer();
	glDisable(GL_DEPTH_TEST);
}

void enableStencilTest() {
	flushBuffer();
	glEnable(GL_STENCIL_TEST);
}

void disableStencilTest() {
	flushBuffer();
	glDisable(GL_STENCIL_TEST);
}

void enableScissorTest() {
	flushBuffer();
	glEnable(GL_SCISSOR_TEST);
}

void disableScissorTest() {
	flushBuffer();
	glDisable(GL_SCISSOR_TEST);
}

void enableAlphaTest() {
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5f);
}

void disableAlphaTest() {
	glDisable(GL_ALPHA_TEST);
}

void setScissor(int x, int y, int w, int h) {
	flushBuffer();
	glScissor(x, getWindowHeight() - y - h, w, h);
}

void flush() {
	flushBuffer();
	glFinish();
}

void move(double translateX, double translateY) {
	FLUSH_BUFFER();
	glTranslated(translateX, translateY, 0);
}

void scale(double scaleX, double scaleY) {
	FLUSH_BUFFER();
	glScaled(scaleX, scaleY, 1);
}

void rotate(double angle) {
	FLUSH_BUFFER();
	glRotated(angle, 0, 0, 1);
}

void translateObject(double x, double y, double angle, double width, double height, double origin_x, double origin_y) {
	flushBuffer();
	if(x || y) glTranslated(x, y, 0);
	if(angle) glRotated(angle, 0, 0, 1);
	if(width != 1.0 || height != 1.0) glScaled(width, height, 1);
	if(origin_x || origin_y) glTranslated(-origin_x/width, -origin_y/height, 0);
}

void blend(bool blendEnabled) {
	FLUSH_BUFFER();
	if(blendEnabled) glEnable(GL_BLEND);
	else glDisable(GL_BLEND);
}

void enableBlend() {
	FLUSH_BUFFER();
	glEnable(GL_BLEND);
}

void disableBlend() {
	FLUSH_BUFFER();
	glDisable(GL_BLEND);
}

void push() {
	FLUSH_BUFFER();
	glPushMatrix();
	if (glGetError() == GL_STACK_OVERFLOW)
		myError("No more free slots to save the view.");
}

void pop() {
	FLUSH_BUFFER();
	glPopMatrix();
	if (glGetError() == GL_STACK_UNDERFLOW)
		myError("No saved view was found.");
}

void reset() {
	FLUSH_BUFFER();
	glLoadIdentity();
}

//~ #define RECTBORDER 4
//~ #define RECTOFF1 0.5
//~ #define RECTOFF2 0.5
//~
//~ void rectanglexy(float x, float y, float w, float h) {
	//~ const float t[20] = {0,0, RECTOFF1,RECTOFF1, 1,0, RECTOFF2,RECTOFF1, 1,1, RECTOFF2,RECTOFF2, 0,1, RECTOFF1,RECTOFF2, 0,0, RECTOFF1,RECTOFF1};
	//~ if(antiAliasing) {
		//~ FLUSH_BUFFER();
		//~ vertexCoord[0] = x - RECTBORDER;
		//~ vertexCoord[1] = y - RECTBORDER;
		//~ vertexCoord[2] = x;
		//~ vertexCoord[3] = y;
		//~ vertexCoord[4] = x + w + RECTBORDER;
		//~ vertexCoord[5] = y - RECTBORDER;
		//~ vertexCoord[6] = x + w;
		//~ vertexCoord[7] = y;
		//~ vertexCoord[8] = x + w + RECTBORDER;
		//~ vertexCoord[9] = y + h + RECTBORDER;
		//~ vertexCoord[10] = x + w;
		//~ vertexCoord[11] = y + h;
		//~ vertexCoord[12] = x - RECTBORDER;
		//~ vertexCoord[13] = y + h + RECTBORDER;
		//~ vertexCoord[14] = x ;
		//~ vertexCoord[15] = y + h;
		//~ vertexCoord[16] = x - RECTBORDER;
		//~ vertexCoord[17] = y - RECTBORDER;
		//~ vertexCoord[18] = x;
		//~ vertexCoord[19] = y;
		//~ memcpy(texCoord, t, sizeof(float) * 20);
		//~ glBindTexture(GL_TEXTURE_2D, rect_texture);
		//~ glDrawArrays(GL_TRIANGLE_STRIP, 0, 10);
	//~ }
	//~ FLUSH_BUFFER();
	//~ if(prevImageId) {
		//~ glBindTexture(GL_TEXTURE_2D, 0);
		//~ prevImageId = 0;
	//~ }
	//~ PUSH_QUAD(x,y,w,h,0,0,0);
//~ }

//~ void circlexy(float x, float y, float rad, float segments, bool filled) {
	//~ int i;
	//~ const float DBLPI = 3.1415926 * 2;
	//~ GLfloat angle;
	//~ glBegin(filled ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
	//~ int max = segments;
	//~ for (i = 0; i <= max; i++)
	//~ {
		//~ angle = DBLPI / segments * (float)i;
		//~ glVertex2d(sinf(angle) * rad + x, cosf(angle) * rad + y);
	//~ }
	//~ glEnd();
//~ }

void color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	union {
		unsigned uColor;
		struct {
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} ubColor;
	} uniColor;
	uniColor.ubColor.r = r;
	uniColor.ubColor.g = g;
	uniColor.ubColor.b = b;
	uniColor.ubColor.a = a;
	if(uniColor.uColor != prevColor)
	{
		FLUSH_BUFFER();
		glColor4ub(r, g, b, a);
		prevColor = uniColor.uColor;
	}
}

void colorf(float r, float g, float b, float a) {
	unsigned c = 
		((unsigned)(r * 255.f)) << 24 | 
		((unsigned)(g * 255.f)) << 16 | 
		((unsigned)(b * 255.f)) << 8 | 
		((unsigned)(a * 255.f));
	if(c != prevColor)
	{
		FLUSH_BUFFER();
		glColor4f(r, g, b, a);
		prevColor = c;
	}
}

void clearColor(float r, float g, float b, float a) {
	glClearColor(r,g,b,a);
}

void blendMode(int mode) {
	FLUSH_BUFFER();
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

void blendEquation(unsigned mode) {
	FLUSH_BUFFER();
	glBlendEquation_(mode);
}

void blendFunc(unsigned sourcefactor, unsigned destinationfactor) {
	FLUSH_BUFFER();
	glBlendFunc(sourcefactor, destinationfactor);
}

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

void stencilFunc(unsigned func, int ref, unsigned mask) {
	glStencilFunc(func, ref, mask);
}

void stencilOp(unsigned fail, unsigned zfail, unsigned zpass) {
	glStencilOp(fail, zfail, zpass);
}

void drawToStencil() {
	glStencilFunc (GL_ALWAYS, 0x0, 0x1);
	glStencilOp (GL_REPLACE, GL_REPLACE, GL_REPLACE);
}

void drawUsingStencil() {
	glStencilFunc (GL_EQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
}
