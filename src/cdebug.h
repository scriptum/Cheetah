/*******************************************************************************

Copyright (c) 2012-2014 Pavel Roschin (aka RPG) <rpg89@post.ru>

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

/*******************************DEBUGGING OPTIONS******************************/

// #define DEBUG
// #define DEBUGV
// #define DEBUGVV

/**********************************DEBUG STUFF*********************************/

#define DEBUG_MSG_FORMAT "%s:%d(%s): "

#define LOCATION __FILE__, __LINE__, __FUNCTION__

#define DBG_PRINT(format, args...) printf(DEBUG_MSG_FORMAT format "\n", LOCATION, ## args)

#ifdef DEBUGVV
	#define dbg(format, args...) DBG_PRINT(format, ## args)
	#define dbgv(format, args...) DBG_PRINT(format, ## args)
	#define dbgvv(format, args...) DBG_PRINT(format, ## args)
#elif defined DEBUGV
	#define dbg(format, args...) DBG_PRINT(format, ## args)
	#define dbgv(format, args...) DBG_PRINT(format, ## args)
	#define dbgvv(...)
#elif defined DEBUG
	#define dbg(format, args...) DBG_PRINT(format, ## args)
	#define dbgv(...)
	#define dbgvv(...)
#else
	#define dbg(...)
	#define dbgv(...)
	#define dbgvv(...)
#endif


#define vard(v) printf(DEBUG_MSG_FORMAT " %s = %d\n", LOCATION, #v, v);
#define vars(v) printf(DEBUG_MSG_FORMAT " %s = %s\n", LOCATION, #v, v);
#define varf(v) printf(DEBUG_MSG_FORMAT " %s = %f\n", LOCATION, #v, v);


#endif /* DEBUG_H_ */
