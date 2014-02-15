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

/* Some Cheetah's tuning values */

#ifndef CONFIG_H_
#define CONFIG_H_

/* Limit to virtual vertex buffer, after this limit engine flushes verticles */
#define VERTEX_BUFFER_LIMIT 4096 /* 4096 verticles (~256 Kb) */

/******************************DRAW USING QUADS********************************/
/* #undef GL_QUADS */
#ifdef GL_QUADS
#define VERTICLES_PER_SPRITE 4 * 2
#else
#define VERTICLES_PER_SPRITE 6 * 2
#endif
#endif /* CONFIG_H_ */

/******************************USE COLOR ARRAYS********************************/
#define COLOR_ARRAYS