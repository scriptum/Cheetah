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

void PUSH_QUAD(float vx, float vy, float vw, float vh, float a, float ox, float oy);
void PUSH_QUADT(float vx, float vy, float vw, float vh, float a, float ox, float oy, float tx, float ty, float tw, float th, float w, float h);
void PUSH_QUAD_VERTEX_OPS(float vx, float vy, float vw, float vh, float a, float ox, float oy);
void PUSH_QUAD_TEXTURE(float vx, float vy, float vw, float vh, float a, float ox, float oy, const float *texture);

#endif
