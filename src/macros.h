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

/* dynamic vertex array */
#define VERTEX_QUERY(size) do {\
	if((size) > verAlloc) {\
		renew(vertexCoord, float, verAlloc * 2);\
		renew(texCoord, float, verAlloc * 2);\
		verAlloc *= 2;\
	}\
} while(0)

#define VERTEX_COORD(x,y,w,h) do {\
	vertexCoord[0] = x;\
	vertexCoord[1] = y;\
	vertexCoord[2] = x;\
	vertexCoord[3] = y + h;\
	vertexCoord[4] = x + w;\
	vertexCoord[5] = y + h;\
	vertexCoord[6] = x + w;\
	vertexCoord[7] = y;\
} while(0)

#define VERTEX_ROT_X(x,y) cosf(a)*(x-ox)-sinf(a)*(y-oy)
#define VERTEX_ROT_Y(x,y) sinf(a)*(x-ox)+cosf(a)*(y-oy)

#define VERTEX_COORD_TRANS(x,y,w,h,a,ox,oy) do {\
	vertexCoord[0] = x + VERTEX_ROT_X(0,0);\
	vertexCoord[1] = y + VERTEX_ROT_Y(0,0);\
	vertexCoord[2] = x + VERTEX_ROT_X(0,h);\
	vertexCoord[3] = y + VERTEX_ROT_Y(0,h);\
	vertexCoord[4] = x + VERTEX_ROT_X(w,h);\
	vertexCoord[5] = y + VERTEX_ROT_Y(w,h);\
	vertexCoord[6] = x + VERTEX_ROT_X(w,0);\
	vertexCoord[7] = y + VERTEX_ROT_Y(w,0);\
} while(0)

#define TEXTURE_COORD(qx,qy,qw,qh,w,h) do {\
	texCoord[0] = qx/w;\
	texCoord[1] = qy/h;\
	texCoord[2] = texCoord[0];\
	texCoord[3] = texCoord[1] + qh/h;\
	texCoord[4] = texCoord[0] + qw/w;\
	texCoord[5] = texCoord[3];\
	texCoord[6] = texCoord[4];\
	texCoord[7] = texCoord[1];\
} while(0)

#define DRAWQ do {\
	glVertexPointer(2, GL_FLOAT, 0, vertexCoord);\
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordQuad);\
	glDrawArrays(GL_QUADS, 0, 4);\
} while(0)

#define DRAWQT do {\
	glVertexPointer(2, GL_FLOAT, 0, vertexCoord);\
	glTexCoordPointer(2, GL_FLOAT, 0, texCoord);\
	glDrawArrays(GL_QUADS, 0, 4);\
} while(0)

/******************************VERTEX ACCUMULATOR******************************/
int vertex_accum_shift;
#define ACCUM_START(x) do {\
	vertex_accum_shift = 0;\
	VERTEX_QUERY(x);\
} while(0)

#define ACCUM_VERTEX(x,y,w,h) do {\
	vertexCoord[vertex_accum_shift + 0] = x;\
	vertexCoord[vertex_accum_shift + 1] = y;\
	vertexCoord[vertex_accum_shift + 2] = x;\
	vertexCoord[vertex_accum_shift + 3] = y + h;\
	vertexCoord[vertex_accum_shift + 4] = x + w;\
	vertexCoord[vertex_accum_shift + 5] = y + h;\
	vertexCoord[vertex_accum_shift + 6] = x + w;\
	vertexCoord[vertex_accum_shift + 7] = y;\
} while(0)

#define ACCUM_TEXTURE(qx,qy,qw,qh,w,h) do {\
	texCoord[vertex_accum_shift + 0] = qx/w;\
	texCoord[vertex_accum_shift + 1] = qy/h;\
	texCoord[vertex_accum_shift + 2] = texCoord[vertex_accum_shift + 0];\
	texCoord[vertex_accum_shift + 3] = texCoord[vertex_accum_shift + 1] + qh/h;\
	texCoord[vertex_accum_shift + 4] = texCoord[vertex_accum_shift + 0] + qw/w;\
	texCoord[vertex_accum_shift + 5] = texCoord[vertex_accum_shift + 3];\
	texCoord[vertex_accum_shift + 6] = texCoord[vertex_accum_shift + 4];\
	texCoord[vertex_accum_shift + 7] = texCoord[vertex_accum_shift + 1];\
} while(0)

#define ACCUM_TEXTURE_ARRAY(x) do {\
	memcpy(texCoord + vertex_accum_shift, x, 32);\
} while(0)

#define ACCUM_ADD() do {\
	vertex_accum_shift += 8;\
} while(0)

#define ACCUM_DRAW() do {\
	glVertexPointer(2, GL_FLOAT, 0, vertexCoord);\
	glTexCoordPointer(2, GL_FLOAT, 0, texCoord);\
	glDrawArrays(GL_QUADS, 0, vertex_accum_shift << 1);\
	vertex_accum_shift = 0;\
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
