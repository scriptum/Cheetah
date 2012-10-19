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

#include <math.h>
#include "cheetah.h"
#include "SOIL/SOIL.h"
#include "render.h"

#define ATLAS_DRAW imageBind(p->image);\
	DRAW_QUAD(vertexCoord, p->tex)

void atlasDrawxy(Atlas *p, float x, float y, float w, float h) {
	float scalex = w/p->w;
	float scaley = h/p->h;
	VERTEX_COORD(x + p->x * scalex, y + p->y * scaley, p->aw * scalex, p->ah * scaley);
	ATLAS_DRAW;
}

void atlasDrawt(Atlas *p, float x, float y, float w, float h, float a, float ox, float oy) {
	float scalex = w/p->w;
	float scaley = h/p->h;
	VERTEX_COORD_TRANS(x + p->x * scalex, y + p->y * scaley, p->aw * scalex, p->ah * scaley, a, ox, oy);
	ATLAS_DRAW;
}