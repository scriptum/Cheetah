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

#include <math.h>
#include <string.h>

#include "cheetah.h"
#include "render.h"
#include "cvertex.h"
#include "test.h"

#include "SDL_endian.h"


extern void resLoaderMainThread(void);
void getWindowSize(int *w, int *h);

unsigned prevColor = 0xffffffff;

CHEETAH_EXPORT void colorMask(bool r, bool g, bool b, bool a)
{
	FLUSH_BUFFER();
	glColorMask(r,g,b,a);
}

/***********************************GLOBALS************************************/

GLuint quadlist, pointlist, null_texture, rect_texture, vboVer, vboTex;

GLuint prevImageId = 0;

bool antiAliasing = 1;
int vertexCounter;
float *texCoord = NULL;
float *vertexCoord = NULL;

#ifdef COLOR_ARRAYS
unsigned char *colorArray;
unsigned colorArrayBuf[4] = {0xffffffff,0xffffffff,0xffffffff,0xffffffff};
#endif

static void flushBuffer(void)
{
	FLUSH_BUFFER();
}

CHEETAH_EXPORT void enableDepthTest(void)
{
	flushBuffer();
	glEnable(GL_DEPTH_TEST);
}

CHEETAH_EXPORT void disableDepthTest(void)
{
	flushBuffer();
	glDisable(GL_DEPTH_TEST);
}

CHEETAH_EXPORT void enableStencilTest(void)
{
	flushBuffer();
	glEnable(GL_STENCIL_TEST);
}

CHEETAH_EXPORT void disableStencilTest(void)
{
	flushBuffer();
	glDisable(GL_STENCIL_TEST);
}

CHEETAH_EXPORT void enableScissorTest(void)
{
	flushBuffer();
	glEnable(GL_SCISSOR_TEST);
}

CHEETAH_EXPORT void disableScissorTest(void)
{
	flushBuffer();
	glDisable(GL_SCISSOR_TEST);
}

CHEETAH_EXPORT void enableAlphaTest(void)
{
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5f);
}

CHEETAH_EXPORT void disableAlphaTest(void)
{
	glDisable(GL_ALPHA_TEST);
}

CHEETAH_EXPORT void setScissor(int x, int y, int w, int h)
{
	int ww, wh;
	flushBuffer();
	getWindowSize(&ww, &wh);
	glScissor(x, wh - y - h, w, h);
}

CHEETAH_EXPORT void flush(void)
{
	flushBuffer();
	glFinish();
}

CHEETAH_EXPORT void move(double translateX, double translateY)
{
	FLUSH_BUFFER();
	glTranslated(translateX, translateY, 0);
}

CHEETAH_EXPORT void scale(double scaleX, double scaleY)
{
	FLUSH_BUFFER();
	glScaled(scaleX, scaleY, 1);
}

CHEETAH_EXPORT void rotate(double angle)
{
	FLUSH_BUFFER();
	glRotated(angle, 0, 0, 1);
}

CHEETAH_EXPORT void translateObject(double x, double y, double angle, double width, double height, double origin_x, double origin_y)
{
	flushBuffer();
	if(x || y) glTranslated(x, y, 0);
	if(angle) glRotated(angle, 0, 0, 1);
	if(width != 1.0 || height != 1.0) glScaled(width, height, 1);
	if(origin_x || origin_y) glTranslated(-origin_x/width, -origin_y/height, 0);
}

CHEETAH_EXPORT void blend(bool blendEnabled)
{
	FLUSH_BUFFER();
	if(blendEnabled) glEnable(GL_BLEND);
	else glDisable(GL_BLEND);
}

CHEETAH_EXPORT void enableBlend(void)
{
	FLUSH_BUFFER();
	glEnable(GL_BLEND);
}

CHEETAH_EXPORT void disableBlend(void)
{
	FLUSH_BUFFER();
	glDisable(GL_BLEND);
}

CHEETAH_EXPORT void push(void)
{
	FLUSH_BUFFER();
	glPushMatrix();
	if (glGetError() == GL_STACK_OVERFLOW)
		myError("No more free slots to save the view.");
}

CHEETAH_EXPORT void pop(void)
{
	FLUSH_BUFFER();
	glPopMatrix();
	if (glGetError() == GL_STACK_UNDERFLOW)
		myError("No saved view was found.");
}

CHEETAH_EXPORT void reset(void)
{
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

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    #define COLOR_UINT                                                         \
    unsigned low = (unsigned)g << 8 | ((unsigned)r & 0xff);                    \
    unsigned high = (unsigned)a << 8 | (unsigned)b;                            \
    unsigned c = (low & 0xffff) | (high << 16);
#else
    #define COLOR_UINT                                                         \
    unsigned low = (unsigned)b << 8 | ((unsigned)a & 0xff);                    \
    unsigned high = (unsigned)r << 8 | (unsigned)g;                            \
    unsigned c = (low & 0xffff) | (high << 16);
#endif

#ifdef COLOR_ARRAYS
    #define COLOR_BODY                                                         \
    COLOR_UINT                                                                 \
    colorArrayBuf[0] = colorArrayBuf[1] = colorArrayBuf[2] = colorArrayBuf[3] = c;
#else
    #define COLOR_BODY                                                         \
    COLOR_UINT                                                                 \
    if(unlikely(c != prevColor))                                               \
    {                                                                          \
        FLUSH_BUFFER();                                                        \
        glColor4ubv((const GLubyte*)c);                                        \
        prevColor = c;                                                         \
    }
#endif

CHEETAH_EXPORT void color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	COLOR_BODY
}

CHEETAH_EXPORT void colorf(float r, float g, float b, float a)
{
	COLOR_BODY
}

CHEETAH_EXPORT void colord(double r, double g, double b, double a)
{
	COLOR_BODY
}

CHEETAH_EXPORT void colorC(Color C)
{
	unsigned char r = C.r;
	unsigned char g = C.g;
	unsigned char b = C.b;
	unsigned char a = C.a;
	COLOR_BODY
}

CHEETAH_EXPORT void clearColor(float r, float g, float b, float a)
{
	glClearColor(r,g,b,a);
}

CHEETAH_EXPORT void setClearColor(float r, float g, float b, float a)
{
	glClearColor(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

CHEETAH_EXPORT void blendMode(int mode)
{
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

CHEETAH_EXPORT void blendEquation(unsigned mode)
{
	FLUSH_BUFFER();
	glBlendEquation_(mode);
}

CHEETAH_EXPORT void blendFunc(unsigned sourcefactor, unsigned destinationfactor)
{
	FLUSH_BUFFER();
	glBlendFunc(sourcefactor, destinationfactor);
}

CHEETAH_EXPORT void clear(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
}

/**
 * @descr Clear screen and depth buffer. Usually used in lQuery.addhook(cheetah.clearColorDepth). Slow.
 * @group graphics/drawing
 * @see clear clearColorStencil clearDepth clearStencil
 * */
CHEETAH_EXPORT void clearColorDepth(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * @descr Clear screen and stencil buffer. Usually used in lQuery.addhook(cheetah.clearColorStencil). Slow.
 * @group graphics/drawing
 * @see clear clearColorDepth clearDepth clearStencil
 * */
CHEETAH_EXPORT void clearColorStencil(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

/**
 * @descr Clear depth buffer. Usually used in lQuery.addhook(cheetah.clearDepth). Slow.
 * @group graphics/drawing
 * @see clear clearColorDepth clearColorStencil clearStencil
 * */
CHEETAH_EXPORT void clearDepth(void)
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

/**
 * @descr Clear stencil buffer. Usually used in lQuery.addhook(cheetah.clearStencil). Slow.
 * @group graphics/drawing
 * @see clear clearColorDepth clearColorStencil clearDepth
 * */
CHEETAH_EXPORT void clearStencil(void)
{
	glClear(GL_STENCIL_BUFFER_BIT);
}

CHEETAH_EXPORT void stencilFunc(unsigned func, int ref, unsigned mask)
{
	glStencilFunc(func, ref, mask);
}

CHEETAH_EXPORT void stencilOp(unsigned fail, unsigned zfail, unsigned zpass)
{
	// glStencilOp(fail, zfail, zpass);
}

CHEETAH_EXPORT void drawToStencil(void)
{
	glStencilFunc (GL_ALWAYS, 0x0, 0x1);
	// glStencilOp (GL_REPLACE, GL_REPLACE, GL_REPLACE);
}

CHEETAH_EXPORT void drawUsingStencil(void)
{
	glStencilFunc (GL_EQUAL, 0x1, 0x1);
	// glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
}
