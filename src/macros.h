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

#endif //__MACROS_H__