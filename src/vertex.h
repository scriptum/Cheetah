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

#ifndef __VERTEX_H__
#define __VERTEX_H__
#include <math.h>
#include "config.h"

extern int vertexCounter;
extern float *texCoord;
extern float *vertexCoord;

/**********************************VERTEX OPS**********************************/


/******************************VERTEX ACCUMULATOR******************************/

/******************************DRAW USING QUADS********************************/
#define VERTICLES_PER_SPRITE 4 * 2

/**
 * Flushing buffer accumulator. It doesn't send glFlush() or glFinish, it just
 * calls glDrawArrays to draw quads from buffer. Some functions as color, move,
 * bindShader need to flush buffer to avoid visual appearance corruption.
 * */
#define FLUSH_BUFFER() do {                                                    \
    if(vertexCounter) {                                                        \
        glDrawArrays(GL_QUADS, 0, vertexCounter / 2);                          \
        vertexCounter = 0;                                                     \
    }                                                                          \
} while(0)

#define TEXTURE_BIND(tex) do {                                                 \
    if(prevImageId != (tex)) {                                                 \
        FLUSH_BUFFER();                                                        \
        glBindTexture(GL_TEXTURE_2D, tex);                                     \
        prevImageId = tex;                                                     \
    }                                                                          \
} while(0)

//~ void PUSH_QUAD(float vx, float vy, float vw, float vh, float a, float ox, float oy);
//~ void PUSH_QUADT(float vx, float vy, float vw, float vh, float a, float ox, float oy, float tx, float ty, float tw, float th, float w, float h);
//~ void PUSH_QUAD_VERTEX_OPS(float vx, float vy, float vw, float vh, float a, float ox, float oy);
//~ void PUSH_QUAD_TEXTURE(float vx, float vy, float vw, float vh, float a, float ox, float oy, const float *texture);

/**
 * Just checking if buffer grows over his size and flushing it.
 * */
#define FLUSH_BUFFER_IF_OVERFLOW                                               \
    if(vertexCounter >= VERTEX_BUFFER_LIMIT * VERTICLES_PER_SPRITE)            \
		FLUSH_BUFFER();

/**
 * Rotation of verticles. We are hoping that compiler will optimize sin(0) and
 * cos(0) and applies sincosf instead of four sin/cos calls. I checked assembly
 * for that.
 * */
//~ static inline float VERTEX_ROT_X(float x,float y,float a,float ox,float oy)
//~ {
	//~ return cosf(a)*((x)-(ox))-sinf(a)*((y)-(oy));
//~ }
//~ static inline float VERTEX_ROT_Y(float x,float y,float a,float ox,float oy)
//~ {
	//~ return sinf(a)*((x)-(ox))+cosf(a)*((y)-(oy));
//~ }

#define VERTEX_ROT_X(x,y,a,ox,oy) cosf(a) * ((x) - (ox)) - sinf(a) * ((y) - (oy))
#define VERTEX_ROT_Y(x,y,a,ox,oy) sinf(a) * ((x) - (ox)) + cosf(a) * ((y) - (oy))



/******************************VERTEX ACCUMULATOR******************************/

/******************************DRAW USING QUADS********************************/
#define VERTICLES_PER_SPRITE 4 * 2

/**
 * Default texture coordinates.
 * */
static const float DEFAULT_QUAD_TEX[] = {0,0,0,1,1,1,1,0};

/**
 * Genegal vertex operations: push vertex coordinates to buffer.
 * */
static inline void PUSH_QUAD_VERTEX_OPS(float vx, float vy, float vw, float vh, float a, float ox, float oy) {
	vertexCoord[vertexCounter + 0] = vx + VERTEX_ROT_X(0,  0,  a, ox, oy);
	vertexCoord[vertexCounter + 1] = vy + VERTEX_ROT_Y(0,  0,  a, ox, oy);
	vertexCoord[vertexCounter + 2] = vx + VERTEX_ROT_X(0,  vh, a, ox, oy);
	vertexCoord[vertexCounter + 3] = vy + VERTEX_ROT_Y(0,  vh, a, ox, oy);
	vertexCoord[vertexCounter + 4] = vx + VERTEX_ROT_X(vw, vh, a, ox, oy);
	vertexCoord[vertexCounter + 5] = vy + VERTEX_ROT_Y(vw, vh, a, ox, oy);
	vertexCoord[vertexCounter + 6] = vx + VERTEX_ROT_X(vw, 0,  a, ox, oy);
	vertexCoord[vertexCounter + 7] = vy + VERTEX_ROT_Y(vw, 0,  a, ox, oy);
}

/**
 * Pushing full quad with all transformaions: size, rotation (a - angle),
 * texture offsets and origin position (ox, oy).
 * */
static inline void PUSH_QUADT(float vx, float vy, float vw, float vh, float a, float ox, float oy, float tx, float ty, float tw, float th, float w, float h) {
	FLUSH_BUFFER_IF_OVERFLOW
	PUSH_QUAD_VERTEX_OPS(vx, vy, vw, vh, a, ox, oy);
	texCoord[vertexCounter + 2] =
	texCoord[vertexCounter + 0] = tx / w;
	texCoord[vertexCounter + 7] =
	texCoord[vertexCounter + 1] = ty / h;
	texCoord[vertexCounter + 5] =
	texCoord[vertexCounter + 3] =
	texCoord[vertexCounter + 1] + th / h;
	texCoord[vertexCounter + 6] =
	texCoord[vertexCounter + 4] =
	texCoord[vertexCounter + 0] + tw / w;
	vertexCounter += VERTICLES_PER_SPRITE;
}
static inline void PUSH_QUAD_TEXTURE(float vx, float vy, float vw, float vh, float a, float ox, float oy, const float *texture) {
	FLUSH_BUFFER_IF_OVERFLOW
	PUSH_QUAD_VERTEX_OPS(vx, vy, vw, vh, a, ox, oy);
	memcpy(texCoord+vertexCounter, texture, sizeof(float) * VERTICLES_PER_SPRITE);
	vertexCounter += VERTICLES_PER_SPRITE;
}

static inline void PUSH_QUAD(float vx, float vy, float vw, float vh, float a, float ox, float oy) {
	PUSH_QUAD_TEXTURE(vx, vy, vw, vh, a, ox, oy, DEFAULT_QUAD_TEX);
}


#endif
