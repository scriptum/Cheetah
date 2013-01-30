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

#include "config.h"

extern int vertexCounter;
extern float *texCoord;
extern float *vertexCoord;

/**********************************VERTEX OPS**********************************/

/**
 * Rotation of verticles. We are hoping that compiler will optimize sin(0) and
 * cos(0) and applies sincosf instead of four sin/cos calls. I checked assembly
 * for that.
 * */
#define VERTEX_ROT_X(x,y,a,ox,oy) cosf(a)*((x)-(ox))-sinf(a)*((y)-(oy))
#define VERTEX_ROT_Y(x,y,a,ox,oy) sinf(a)*((x)-(ox))+cosf(a)*((y)-(oy))

/******************************VERTEX ACCUMULATOR******************************/

#ifdef GL_QUADS
/******************************DRAW USING QUADS********************************/
#define VERTICLES_PER_SPRITE 4 * 2

/**
 * Flushing buffer accumulator. It doesn't send glFlush() or glFinish, it just
 * calls glDrawArrays to draw quads from buffer. Some functions as color, move,
 * bindShader need to flush buffer to avoid visual appearance corruption.
 * */
#define FLUSH_BUFFER() do {                                                    \
	if(vertexCounter) {                                                          \
		glDrawArrays(GL_QUADS, 0, vertexCounter / 2);                              \
		vertexCounter = 0;                                                         \
	}                                                                            \
} while(0)

/**
 * Just checking if buffer grows over his size and flushing it.
 * */
#define FLUSH_BUFFER_IF_OVERFLOW() do {                                        \
	if(vertexCounter >= VERTEX_BUFFER_LIMIT * VERTICLES_PER_SPRITE)              \
		FLUSH_BUFFER();                                                            \
} while(0)

/**
 * Default texture coordinates.
 * */
static const float DEFAULT_QUAD_TEX[] = {0,0,0,1,1,1,1,0};

/**
 * Genegal vertex operations: push vertex coordinates to buffer.
 * */
#define PUSH_QUAD_VERTEX_OPS(vx, vy, vw, vh, a, ox, oy) do {                   \
	vertexCoord[vertexCounter + 0] = (vx) + VERTEX_ROT_X(0,  0,  a, ox, oy);     \
	vertexCoord[vertexCounter + 1] = (vy) + VERTEX_ROT_Y(0,  0,  a, ox, oy);     \
	vertexCoord[vertexCounter + 2] = (vx) + VERTEX_ROT_X(0,  vh, a, ox, oy);     \
	vertexCoord[vertexCounter + 3] = (vy) + VERTEX_ROT_Y(0,  vh, a, ox, oy);     \
	vertexCoord[vertexCounter + 4] = (vx) + VERTEX_ROT_X(vw, vh, a, ox, oy);     \
	vertexCoord[vertexCounter + 5] = (vy) + VERTEX_ROT_Y(vw, vh, a, ox, oy);     \
	vertexCoord[vertexCounter + 6] = (vx) + VERTEX_ROT_X(vw, 0,  a, ox, oy);     \
	vertexCoord[vertexCounter + 7] = (vy) + VERTEX_ROT_Y(vw, 0,  a, ox, oy);     \
} while(0)

/**
 * Pushing full quad with all transformaions: size, rotation (a - angle), 
 * texture offsets and origin position (ox, oy).
 * */
#define PUSH_QUADT(vx, vy, vw, vh, a, ox, oy, tx, ty, tw, th, w, h) do {       \
	FLUSH_BUFFER_IF_OVERFLOW();                                                  \
	PUSH_QUAD_VERTEX_OPS(vx, vy, vw, vh, a, ox, oy);                             \
	texCoord[vertexCounter + 2] =                                                \
	texCoord[vertexCounter + 0] = (tx) / (w);                                    \
	texCoord[vertexCounter + 7] =                                                \
	texCoord[vertexCounter + 1] = (ty) / (h);                                    \
	texCoord[vertexCounter + 5] =                                                \
	texCoord[vertexCounter + 3] =                                                \
	texCoord[vertexCounter + 1] + (th) / (h);                                    \
	texCoord[vertexCounter + 6] =                                                \
	texCoord[vertexCounter + 4] =                                                \
	texCoord[vertexCounter + 0] + (tw) / (w);                                    \
	vertexCounter += VERTICLES_PER_SPRITE;                                       \
} while(0)

#else
/****************************DRAW USING TRIANGLES******************************/
#define VERTICLES_PER_SPRITE 6 * 2

#define FLUSH_BUFFER() do {                                                    \
	if(vertexCounter) {                                                          \
		glDrawArrays(GL_TRIANGLES, 0, vertexCounter / 2);                          \
		vertexCounter = 0;                                                         \
	}                                                                            \
} while(0)

static const float DEFAULT_QUAD_TEX[] = {0,0,0,1,1,1,1,1,1,0,0,0};

#define PUSH_QUAD_VERTEX_OPS(vx, vy, vw, vh, a, ox, oy) do {                   \
	vertexCoord[vertexCounter + 10] =                                            \
	vertexCoord[vertexCounter + 0]  = (vx) + VERTEX_ROT_X(0,  0,  a, ox, oy);    \
	vertexCoord[vertexCounter + 11] =                                            \
	vertexCoord[vertexCounter + 1]  = (vy) + VERTEX_ROT_Y(0,  0,  a, ox, oy);    \
	vertexCoord[vertexCounter + 2]  = (vx) + VERTEX_ROT_X(0,  vh, a, ox, oy);    \
	vertexCoord[vertexCounter + 3]  = (vy) + VERTEX_ROT_Y(0,  vh, a, ox, oy);    \
	vertexCoord[vertexCounter + 6]  =                                            \
	vertexCoord[vertexCounter + 4]  = (vx) + VERTEX_ROT_X(vw, vh, a, ox, oy);    \
	vertexCoord[vertexCounter + 7]  =                                            \
	vertexCoord[vertexCounter + 5]  = (vy) + VERTEX_ROT_Y(vw, vh, a, ox, oy);    \
	vertexCoord[vertexCounter + 8]  = (vx) + VERTEX_ROT_X(vw, 0,  a, ox, oy);    \
	vertexCoord[vertexCounter + 9]  = (vy) + VERTEX_ROT_Y(vw, 0,  a, ox, oy);    \
} while(0)

#define PUSH_QUADT(vx, vy, vw, vh, a, ox, oy, tx, ty, tw, th, w, h) do {       \
	FLUSH_BUFFER_IF_OVERFLOW();                                                  \
	PUSH_QUAD_VERTEX_OPS(vx, vy, vw, vh, a, ox, oy);                             \
	texCoord[vertexCounter + 10] =                                               \
	texCoord[vertexCounter + 2] =                                                \
	texCoord[vertexCounter + 0] = (tx) / (w);                                    \
	texCoord[vertexCounter + 11] =                                               \
	texCoord[vertexCounter + 9] =                                                \
	texCoord[vertexCounter + 1] = (ty) / (h);                                    \
	texCoord[vertexCounter + 7] =                                                \
	texCoord[vertexCounter + 5] =                                                \
	texCoord[vertexCounter + 3] =                                                \
	texCoord[vertexCounter + 1] + (th) / (h);                                    \
	texCoord[vertexCounter + 8] =                                                \
	texCoord[vertexCounter + 6] =                                                \
	texCoord[vertexCounter + 4] =                                                \
	texCoord[vertexCounter + 0] + (tw) / (w);                                    \
	vertexCounter += VERTICLES_PER_SPRITE;                                       \
} while(0)

#endif /* #ifdef GL_QUADS */

#define PUSH_QUAD_TEXTURE(vx, vy, vw, vh, a, ox, oy, texture) do {             \
	FLUSH_BUFFER_IF_OVERFLOW();                                                  \
	PUSH_QUAD_VERTEX_OPS(vx, vy, vw, vh, a, ox, oy);                             \
	memcpy(texCoord+vertexCounter, texture, sizeof(float)*VERTICLES_PER_SPRITE); \
	vertexCounter += VERTICLES_PER_SPRITE;                                       \
} while(0)

#define PUSH_QUAD(vx, vy, vw, vh, a, ox, oy) do {                              \
	PUSH_QUAD_TEXTURE(vx, vy, vw, vh, a, ox, oy, DEFAULT_QUAD_TEX);              \
} while(0)

#define TEXTURE_BIND(tex) do {                                                 \
	if(prevImageId != (tex)) {                                                   \
		FLUSH_BUFFER();                                                            \
		glBindTexture(GL_TEXTURE_2D, tex);                                         \
		prevImageId = tex;                                                         \
	}                                                                            \
} while(0)

#endif //__VERTEX_H__