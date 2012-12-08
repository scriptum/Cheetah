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


//~ #define MEMORY_TEST

#ifndef __MACROS_H__
#define __MACROS_H__

#include "config.h"


/**********************************MEMOTY OPS**********************************/
#ifdef MEMORY_TEST
	#define new(var, type, size) do {\
		if(var){\
			myError("variable %s already contains data: %x. Delete it before allocating", #var, var);\
			exit(1);\
		}\
		var = (type*)malloc(sizeof(type)*(size));\
		/*initialize memory for small structures*/\
		if(size == 1) memset(var, 0, sizeof(type));\
		if(!var) {\
			myError("cannot allocate %d bytes for %s", sizeof(type)*(size), #var);\
			exit(1);\
		}\
		printf("Added: %s %d %s (%x) %d bytes\n", __FILE__, __LINE__, #var, var, sizeof(type)*(size)); \
	} while(0)

	#define renew(var, type, size) do {\
		var = (type*)realloc(var, sizeof(type)*(size));\
		if(!var) {\
			myError("cannot re-allocate %d bytes for %s", sizeof(type)*(size), #var);\
			exit(1);\
		}\
		printf("Reallocated: %s %d %s (%x) %d bytes\n", __FILE__, __LINE__, #var, var, sizeof(type)*(size)); \
	} while(0)

	#define delete(var) do {\
		if(var) {\
			free(var);\
			printf("Removed: %s %d %s (%x)\n", __FILE__, __LINE__, #var, var); \
			var = NULL;\
		}\
	} while(0)
#else
	#define new(var, type, size) do {\
		if(var){\
			myError("variable %s already contains data: %x. Delete it before allocating", #var, var);\
			exit(1);\
		}\
		var = (type*)malloc(sizeof(type)*(size));\
		/*initialize memory for small structures*/\
		if(size == 1) memset(var, 0, sizeof(type));\
		if(!var) {\
			myError("cannot allocate %d bytes for %s", sizeof(type)*(size), #var);\
			exit(1);\
		}\
	} while(0)

	#define renew(var, type, size) do {\
		var = (type*)realloc(var, sizeof(type)*(size));\
		if(!var) {\
			myError("cannot re-allocate %d bytes for %s (%x)", sizeof(type)*(size), #var, var);\
			exit(1);\
		}\
	} while(0)

	#define delete(data) do {\
		if(data) {\
			free(data);\
			data = NULL;\
		}\
	} while(0)
#endif

#define renewif(condition, var, type, size) do {\
	if(condition) {\
		renew(var, type, size);\
	}\
} while(0)

#define fill(var, character, type, size) do {\
	memset(var, character, sizeof(type) * (size));\
} while(0)

/*********************************TEXTURE OPS**********************************/

#define TEX_LINEAR do {\
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);\
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);\
} while(0)

#define TEX_NEAREST do {\
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);\
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);\
} while(0)

#define TEX_CLAMP do {\
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);\
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);\
} while(0)

#define TEX_REPEAT do {\
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);\
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);\
} while(0)

/**********************************VERTEX OPS**********************************/

/* GLES compatibility */
#ifndef GL_QUADS
#define GL_QUADS GL_TRIANGLES
#endif

/* dynamic vertex array */
#define VERTEX_QUERY(size) do {\
	if((size) > verAlloc) {\
		renew(vertexCoord, float, verAlloc * 2);\
		renew(texCoord, float, verAlloc * 2);\
		verAlloc *= 2;\
	}\
} while(0)



#define VERTEX_ROT_X(x,y,a,ox,oy) cosf(a)*(x-ox)-sinf(a)*(y-oy)
#define VERTEX_ROT_Y(x,y,a,ox,oy) sinf(a)*(x-ox)+cosf(a)*(y-oy)

/******************************VERTEX ACCUMULATOR******************************/

#if 1

#define VERTICLES_PER_SPRITE 4 * 2

#define FLUSH_BUFFER() do {\
	if(vertexCounter) {\
		glDrawArrays(GL_QUADS, 0, vertexCounter / 2);\
		vertexCounter = 0;\
	}\
} while(0)

static const float DEFAULT_QUAD_TEX[] = {0,0,0,1,1,1,1,0};

#define PUSH_QUAD_VERTEX_OPS(vx, vy, vw, vh, a, ox, oy) do {\
	vertexCoord[vertexCounter + 0] = vx + VERTEX_ROT_X(0,  0,  a, ox, oy);\
	vertexCoord[vertexCounter + 1] = vy + VERTEX_ROT_Y(0,  0,  a, ox, oy);\
	vertexCoord[vertexCounter + 2] = vx + VERTEX_ROT_X(0,  vh, a, ox, oy);\
	vertexCoord[vertexCounter + 3] = vy + VERTEX_ROT_Y(0,  vh, a, ox, oy);\
	vertexCoord[vertexCounter + 4] = vx + VERTEX_ROT_X(vw, vh, a, ox, oy);\
	vertexCoord[vertexCounter + 5] = vy + VERTEX_ROT_Y(vw, vh, a, ox, oy);\
	vertexCoord[vertexCounter + 6] = vx + VERTEX_ROT_X(vw, 0,  a, ox, oy);\
	vertexCoord[vertexCounter + 7] = vy + VERTEX_ROT_Y(vw, 0,  a, ox, oy);\
} while(0)


#define PUSH_QUADT(vx, vy, vw, vh, a, ox, oy, tx, ty, tw, th, w, h) do {\
	if(vertexCounter >= VERTEX_BUFFER_LIMIT * VERTICLES_PER_SPRITE) { FLUSH_BUFFER(); }\
	PUSH_QUAD_VERTEX_OPS(vx, vy, vw, vh, a, ox, oy); \
	texCoord[vertexCounter + 2] = texCoord[vertexCounter + 0] = tx / w;\
	texCoord[vertexCounter + 7] = texCoord[vertexCounter + 1] = ty / h;\
	texCoord[vertexCounter + 5] = texCoord[vertexCounter + 3] = texCoord[vertexCounter + 1] + th / h;\
	texCoord[vertexCounter + 6] = texCoord[vertexCounter + 4] = texCoord[vertexCounter + 0] + tw / w;\
	vertexCounter += VERTICLES_PER_SPRITE;\
} while(0)

#else

#define VERTICLES_PER_SPRITE 6 * 2

#define FLUSH_BUFFER() do {\
	if(vertexCounter) {\
		glDrawArrays(GL_TRIANGLES, 0, vertexCounter / 2);\
		vertexCounter = 0;\
	}\
} while(0)

static const float DEFAULT_QUAD_TEX[] = {0,0,0,1,1,1,1,1,1,0,0,0};

#define PUSH_QUAD_VERTEX_OPS(vx, vy, vw, vh, a, ox, oy) do {\
	vertexCoord[vertexCounter + 10] = vertexCoord[vertexCounter + 0] = vx + VERTEX_ROT_X(0,  0,  a, ox, oy);\
	vertexCoord[vertexCounter + 11] = vertexCoord[vertexCounter + 1] = vy + VERTEX_ROT_Y(0,  0,  a, ox, oy);\
	vertexCoord[vertexCounter + 2] = vx + VERTEX_ROT_X(0,  vh, a, ox, oy);\
	vertexCoord[vertexCounter + 3] = vy + VERTEX_ROT_Y(0,  vh, a, ox, oy);\
	vertexCoord[vertexCounter + 6] = vertexCoord[vertexCounter + 4] = vx + VERTEX_ROT_X(vw, vh, a, ox, oy);\
	vertexCoord[vertexCounter + 7] = vertexCoord[vertexCounter + 5] = vy + VERTEX_ROT_Y(vw, vh, a, ox, oy);\
	vertexCoord[vertexCounter + 8] = vx + VERTEX_ROT_X(vw, 0,  a, ox, oy);\
	vertexCoord[vertexCounter + 9] = vy + VERTEX_ROT_Y(vw, 0,  a, ox, oy);\
} while(0)

#define PUSH_QUADT(vx, vy, vw, vh, a, ox, oy, tx, ty, tw, th, w, h) do {\
	if(vertexCounter >= VERTEX_BUFFER_LIMIT * VERTICLES_PER_SPRITE) { FLUSH_BUFFER(); }\
	PUSH_QUAD_VERTEX_OPS(vx, vy, vw, vh, a, ox, oy); \
	texCoord[vertexCounter + 10] = texCoord[vertexCounter + 2] = texCoord[vertexCounter + 0] = tx / w;\
	texCoord[vertexCounter + 11] = texCoord[vertexCounter + 9] = texCoord[vertexCounter + 1] = ty / h;\
	texCoord[vertexCounter + 7] = texCoord[vertexCounter + 5] = texCoord[vertexCounter + 3] = texCoord[vertexCounter + 1] + th / h;\
	texCoord[vertexCounter + 8] = texCoord[vertexCounter + 6] = texCoord[vertexCounter + 4] = texCoord[vertexCounter + 0] + tw / w;\
	vertexCounter += VERTICLES_PER_SPRITE;\
} while(0)

#endif

#define PUSH_QUAD_TEXTURE(vx, vy, vw, vh, a, ox, oy, texture) do {\
	if(vertexCounter >= VERTEX_BUFFER_LIMIT * VERTICLES_PER_SPRITE) { FLUSH_BUFFER(); }\
	PUSH_QUAD_VERTEX_OPS(vx, vy, vw, vh, a, ox, oy); \
	memcpy(texCoord + vertexCounter, texture, sizeof(float) * VERTICLES_PER_SPRITE); \
	vertexCounter += VERTICLES_PER_SPRITE;\
} while(0)

#define PUSH_QUAD(vx, vy, vw, vh, a, ox, oy) do {\
	PUSH_QUAD_TEXTURE(vx, vy, vw, vh, a, ox, oy, DEFAULT_QUAD_TEX);\
} while(0)

#define TEXTURE_BIND(tex) do {\
	if(prevImageId != tex) {\
		FLUSH_BUFFER();\
		glBindTexture(GL_TEXTURE_2D, tex);\
		prevImageId = tex;\
	}\
} while(0)

/**********************************DEBUG STUFF*********************************/

#define DEBUG_MSG_FORMAT "%s (%d) - %s: "

#define LOCATION __FILE__, __LINE__, __FUNCTION__

#define MYERROR(fmt, ...) fprintf(stderr, DEBUG_MSG_FORMAT fmt "\n", LOCATION, ##__VA_ARGS__)
#define MYASSERT(condition, fmt, ...) do {\
	if(!(condition)) { \
		fprintf(stderr, DEBUG_MSG_FORMAT fmt "\n", LOCATION, ##__VA_ARGS__);\
		return;\
	}\
} while(0)
#define vard(v) printf(DEBUG_MSG_FORMAT " %s = %d\n", LOCATION, #v, v);
#define vars(v) printf(DEBUG_MSG_FORMAT " %s = %s\n", LOCATION, #v, v);
#define varf(v) printf(DEBUG_MSG_FORMAT " %s = %f\n", LOCATION, #v, v);

/**********************************INIT ASSERT*********************************/
#define NEDED_INIT do {\
	if(!screen)\
	{\
		MYERROR("call init function before!");\
		return;\
	}\
} while(0)



#endif //__MACROS_H__
