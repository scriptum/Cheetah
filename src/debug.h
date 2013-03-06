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

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdlib.h>

/*******************************DEBUGGING OPTIONS******************************/

/* Debug memory operations */
#define DEBUG_MEMORY_ERRORS 0
#define DEBUG_MEMORY 0

#define DEBUG_FRAMEBUFFER 0
#define DEBUG_SHADERS 0
#define DEBUG_EVENTS 0

/**********************************DEBUG STUFF*********************************/

#if DEBUG_MEMORY
	#define dprintf_mem(...) printf(__VA_ARGS__)
#else
	#define dprintf_mem(...)
#endif

#if DEBUG_MEMORY_ERRORS
	#define dprintf_memerr(...) myError(__VA_ARGS__)
#else
	#define dprintf_memerr(...)
#endif

#if DEBUG_FRAMEBUFFER
	#define dprintf_fbo(...) myError(__VA_ARGS__)
#else
	#define dprintf_fbo(...)
#endif

#if DEBUG_SHADERS
	#define dprintf_shader(...) myError(__VA_ARGS__)
#else
	#define dprintf_shader(...)
#endif

#if DEBUG_EVENTS
	#define dprintf_event(...) myError(__VA_ARGS__)
#else
	#define dprintf_event(...)
#endif

/******************************************************************************/

#define DEBUG_MSG_FORMAT "%s (%d) - %s: "

#define LOCATION __FILE__, __LINE__, __FUNCTION__

#define vard(v) printf(DEBUG_MSG_FORMAT " %s = %d\n", LOCATION, #v, v);
#define vars(v) printf(DEBUG_MSG_FORMAT " %s = %s\n", LOCATION, #v, v);
#define varf(v) printf(DEBUG_MSG_FORMAT " %s = %f\n", LOCATION, #v, v);


#endif /*__DEBUG_H__*/