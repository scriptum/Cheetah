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

#ifndef __MACROS_H__
#define __MACROS_H__

#include <stdlib.h>
#include "cdebug.h"

/**********************************MEMOTY OPS**********************************/

#define new(var, type, size) do {                                              \
    if(NULL != var) {                                                          \
        dprintf_memerr("variable %s already contains data: %x."                \
                       " Delete it before allocating", #var, var);             \
        exit(1);                                                               \
    }                                                                          \
    var = (type*)malloc((size_t)sizeof(type) * (size_t)(size));                \
    /*initialize memory for small structures*/                                 \
    if((size_t)size == 1)                                                      \
        memset(var, 0, (size_t)sizeof(type));                                  \
    if(!var) {                                                                 \
        dprintf_memerr("cannot allocate %z bytes for %s",                      \
                       (size_t)sizeof(type) * (size_t)(size), #var);           \
        exit(1);                                                               \
    }                                                                          \
    dprintf_mem("Added: %s %d %s (%x) %z bytes\n",                             \
           __FILE__, __LINE__, #var, var, (size_t)sizeof(type)*(size_t)(size));\
} while(0)

#define new0(var, type, size) do {                                             \
    new(var, type, size);                                                      \
    memset(var, 0, sizeof(type) * (size_t)(size));                             \
} while(0)

#define renew(var, type, size) do {                                            \
    var = (type*)realloc(var, (size_t)sizeof(type)*(size_t)(size));            \
    if(!var) {                                                                 \
        dprintf_memerr("cannot re-allocate %z bytes for %s",                   \
                       (size_t)sizeof(type) * (size_t)(size), #var);           \
        exit(1);                                                               \
    }                                                                          \
    dprintf_mem("Reallocated: %s %d %s (%x) %z bytes\n",                       \
         __FILE__, __LINE__, #var, var, (size_t)sizeof(type) * (size_t)(size));\
} while(0)

#define delete(var) do {                                                       \
    if(var) {                                                                  \
        free(var);                                                             \
        dprintf_mem("Removed: %s %d %s (%x)\n", __FILE__, __LINE__, #var, var);\
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

#define CHECK_OPTION(options, o) bool o = 0; do {                              \
    typeof(options) _o = strstr(options, #o);                                  \
    size_t l = strlen(#o);                                                     \
    /* check bounds */                                                         \
    if(NULL != _o && (' '==*(_o+l)||0==*(_o+l)) && (' '==*(_o-1)||0==*(_o-1))){\
        o = 1;                                                                 \
    }                                                                          \
} while(0)

/********************************ERROR CHECKING********************************/

#define ERROR_IF_NULL(name) if(NULL == name) goto error

bool isInit();

#define _NEEDED_INIT_STR "call init function before!"

#define NEEDED_INIT do {                                                       \
    if(!isInit()) {                                                            \
        myError(_NEEDED_INIT_STR); return 0;                                   \
    }                                                                          \
} while(0)

#define NEEDED_INIT_VOID do {                                                  \
    if(!isInit()) {                                                            \
        myError(_NEEDED_INIT_STR); return;                                     \
    }                                                                          \
} while(0)

#endif /*__MACROS_H__*/

/********************************OPTIMIZATIONS*********************************/

#define likely(x)   __builtin_expect((x),1)
#define unlikely(x) __builtin_expect((x),0)
