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

#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdlib.h>
#include <stdio.h>

/*
 * Usage: compile engine with flag -DDEBUG_<class>[V[V]] or uncomment one of examples.
 * class - one of:
 * - MEMORY_ERRORS
 * - MEMORY
 * - FRAMEBUFFER
 * - SHADERS
 * - EVENTS
 * - GRAPHICS
 * Add V to get more messages. Add VV to get HUGE amount of messages 
 * (tells about anything happened in engine).
 * e.g.: -DDEBUG_THREADV (verbose output about thread)
 * */

/*******************************DEBUGGING OPTIONS******************************/

/* Debug memory operations */
// #define DEBUG_MEMORY_ERRORS
// #define DEBUG_MEMORY
// 
// #define DEBUG_FRAMEBUFFER
// #define DEBUG_SHADERS
// #define DEBUG_EVENTS
// #define DEBUG_GRAPHICS

// #define DEBUG_THREAD
// #define DEBUG_THREADV
// #define DEBUG_THREADVV

/**********************************DEBUG STUFF*********************************/

#define __DBG_PRINT(class, ...) printf(#class ": "); printf(__VA_ARGS__)

#ifdef DEBUG_MEMORY
	#define dprintf_mem(...) printf("Memory: ");printf(__VA_ARGS__)
#else
	#define dprintf_mem(...)
#endif

#ifdef DEBUG_MEMORY_ERRORS
	#define dprintf_memerr(...) myError(__VA_ARGS__)
#else
	#define dprintf_memerr(...)
#endif

#ifdef DEBUG_FRAMEBUFFER
	#define dprintf_fbo(...) printf("Framebuffer: ");printf(__VA_ARGS__)
#else
	#define dprintf_fbo(...)
#endif

#ifdef DEBUG_SHADERS
	#define dprintf_shader(...) printf("Shaders: ");printf(__VA_ARGS__)
#else
	#define dprintf_shader(...)
#endif

#ifdef DEBUG_EVENTS
	#define dprintf_event(...) printf("Event: ");printf(__VA_ARGS__)
#else
	#define dprintf_event(...)
#endif

#ifdef DEBUG_GRAPHICS
	#define dprintf_graphics(...) printf("Graphics: ");printf(__VA_ARGS__)
#else
	#define dprintf_graphics(...)
#endif

#ifdef DEBUG_THREADVV
	#define dprintf_thread(...) __DBG_PRINT(Thread, __VA_ARGS__)
	#define dprintf_threadv(...) __DBG_PRINT(ThreadV, __VA_ARGS__)
	#define dprintf_threadvv(...) __DBG_PRINT(ThreadVV, __VA_ARGS__)
#elif defined DEBUG_THREADV
	#define dprintf_thread(...) __DBG_PRINT(Thread, __VA_ARGS__)
	#define dprintf_threadv(...) __DBG_PRINT(ThreadV, __VA_ARGS__)
	#define dprintf_threadvv(...)
#elif defined DEBUG_THREAD
	#define dprintf_thread(...) __DBG_PRINT(Thread, __VA_ARGS__)
	#define dprintf_threadv(...)
	#define dprintf_threadvv(...)
#else
	#define dprintf_thread(...)
	#define dprintf_threadv(...)
	#define dprintf_threadvv(...)
#endif

/******************************************************************************/

#define DEBUG_MSG_FORMAT "%s (%d) - %s: "

#define LOCATION __FILE__, __LINE__, __FUNCTION__

#define vard(v) printf(DEBUG_MSG_FORMAT " %s = %d\n", LOCATION, #v, v);
#define vars(v) printf(DEBUG_MSG_FORMAT " %s = %s\n", LOCATION, #v, v);
#define varf(v) printf(DEBUG_MSG_FORMAT " %s = %f\n", LOCATION, #v, v);


#endif /* DEBUG_H_ */
