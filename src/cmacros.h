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

#ifndef MACROS_H_
#define MACROS_H_

#include <stdlib.h>
#include <string.h>
#include "cheetah.h"
#include "cdebug.h"

/********************************OPTIMIZATIONS*********************************/

#define likely(x)   __builtin_expect((x),1)
#define unlikely(x) __builtin_expect((x),0)

/**********************************MEMOTY OPS**********************************/

#define new(var, type, size) do {                                              \
    if(unlikely(NULL != (var))) {                                              \
        dprintf_memerr("variable %s already contains data: %x."                \
                       " Delete it before allocating", #var, var);             \
        exit(1);                                                               \
    }                                                                          \
    var = (type*)malloc((size_t)sizeof(type) * (size_t)(size));                \
    /* initialize memory for small structures */                               \
    if((size) == 1)                                                            \
        memset(var, 0, (size_t)sizeof(type));                                  \
    if(unlikely(NULL == var)) {                                                \
        dprintf_memerr("cannot allocate %zd bytes for %s",                     \
                       (size_t)sizeof(type) * (size_t)(size), #var);           \
        exit(1);                                                               \
    }                                                                          \
    dprintf_mem("Added: %s %d %s (%x) %zd bytes\n",                            \
           __FILE__, __LINE__, #var, var, (size_t)sizeof(type)*(size_t)(size));\
} while(0)

#define new0(var, type, size) do {                                             \
    new(var, type, size);                                                      \
    memset(var, 0, (size_t)sizeof(type) * (size_t)(size));                     \
} while(0)

#define new1(var, type) do {                                                   \
    new(var, type, 1);                                                         \
} while(0)

#define renew(var, type, size) do {                                            \
    var = (type*)realloc(var, (size_t)sizeof(type)*(size_t)(size));            \
    if(unlikely(NULL == var)) {                                                \
        dprintf_memerr("cannot re-allocate %zd bytes for %s",                  \
                       (size_t)sizeof(type) * (size_t)(size), #var);           \
        exit(1);                                                               \
    }                                                                          \
    dprintf_mem("Reallocated: %s %d %s (%x) %zd bytes\n",                      \
         __FILE__, __LINE__, #var, var, (size_t)sizeof(type) * (size_t)(size));\
} while(0)

#define delete(var) do {                                                       \
    if(likely(NULL != var)) {                                                  \
        dprintf_mem("Removed: %s %d %s (%x)\n", __FILE__, __LINE__, #var, var);\
        free(var);                                                             \
        var = NULL;                                                            \
    }                                                                          \
} while(0)

#define renewif(condition, var, type, size) do {                               \
    if(condition) {                                                            \
        renew(var, type, size);                                                \
    }                                                                          \
} while(0)

#define fill(var, character, type, size) do {                                  \
    memset(var, character, (size_t)sizeof(type) * (size_t)(size));             \
} while(0)

/*********************************TEXTURE OPS**********************************/

#define TEX_LINEAR do {                                                        \
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);              \
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);              \
} while(0)

#define TEX_NEAREST do {                                                       \
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);             \
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);             \
} while(0)

#define TEX_CLAMP do {                                                         \
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);                   \
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);                   \
} while(0)

#define TEX_REPEAT do {                                                        \
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                  \
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);                  \
} while(0)


/********************************OPTIONS CHECKER*******************************/

static inline bool check_option_helper(const char *options, const char *o)
{
	char *name = strstr(options, o);
	size_t l = strlen(o);
	/* check bounds */
	if(NULL != name &&
	  (' '== *(name + l) || '\0' == *(name + l)) &&
	  (' '== *(name - 1) || '\0' == *(name - 1)))
		return TRUE;
	return FALSE;
}

#define CHECK_OPTION(options, o) bool o = check_option_helper(options, #o)

/********************************ERROR CHECKING********************************/

#define ERROR_IF_NULL(name) if(NULL == name) goto error

bool isInit();

#define _NEEDED_INIT_STR "call init function before!"

#define NEEDED_INIT do {                                                       \
    if(unlikely(!isInit())) {                                                  \
        myError(_NEEDED_INIT_STR);                                             \
        return 0;                                                              \
    }                                                                          \
} while(0)

#define NEEDED_INIT_VOID do {                                                  \
    if(unlikely(!isInit())) {                                                  \
        myError(_NEEDED_INIT_STR);                                             \
        return;                                                                \
    }                                                                          \
} while(0)

#endif /*MACROS_H_*/
