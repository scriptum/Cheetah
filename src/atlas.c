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

#include <math.h>
#include <string.h>

#include "cheetah.h"
#include "render.h"
#include "cvertex.h"
#include "test.h"

void imageBind(Image * image);

CHEETAH_EXPORT void atlasDrawt(Atlas *p, float x, float y, float w, float h, float a, float ox, float oy)
{
	float scalex = w/p->w;
	float scaley = h/p->h;
	imageBind(p->image);
	if(.0f == a)
		PUSH_QUAD_TEXTURE(x, y, p->aw * scalex, p->ah * scaley, 0.0, ox - p->x * scalex, oy - p->y * scaley, p->tex);
	else
		PUSH_QUAD_TEXTURE(x, y, p->aw * scalex, p->ah * scaley, a, ox - p->x * scalex, oy - p->y * scaley, p->tex);
}

CHEETAH_EXPORT void atlasDrawxy(Atlas *p, float x, float y, float w, float h)
{
	float scalex = w / p->w;
	float scaley = h / p->h;
	imageBind(p->image);
	PUSH_QUAD_TEXTURE(x + p->x * scalex, y + p->y * scaley, p->aw * scalex, p->ah * scaley, 0.0, 0.0, 0.0, p->tex);
}
