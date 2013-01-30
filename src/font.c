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

//TODO: rich text

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL.h>

#include "cheetah.h"
#include "macros.h"
#include "render.h"
#include "vertex.h"

void imageBind(Image * image);
extern SDL_Surface *screen;

/**
 * @descr Calculate width of string.
 * @group font
 * @var Font object
 * @var string 
 * @return length of string in pixels
 * */
float fontWidth(Font *f, register const char *str) {
	float width = 0;
	if(*str) do {
		if(*str == '\t') {
			//~ width += f->chars[32]->w * 8;
			continue;
		}
		else if(*str == '\n')
			width = 0;
		//~ width += f->chars[(unsigned char)*str]->w;
	} while(*++str);
	return width * f->_scale;
}

/**
 * @descr Calculate height of string.
 * @group font
 * @var Font object
 * @return font height (pixels)
 * */
float fontHeight(Font *font) {
	return font->height * font->_scale;
}

float Font_Width(Font *f, register const char *str)
{
	float width = 0;
	if(*str)
	do {
		if(*str == '\t')
		{
			//~ width += f->chars[32]->w * 8;
			continue;
		}
		else if(*str == '\n')
			width = 0;
		//~ width += f->chars[(unsigned char)*str]->w;
	} while(*++str);
	return width * f->_scale;
}


#define DRAW_CHAR do {                                                         \
	if(vertexCounter >= VERTEX_BUFFER_LIMIT * 8) { FLUSH_BUFFER(); }             \
	texCoord[vertexCounter] = texCoord[vertexCounter+2] = ch->t[0];              \
	texCoord[vertexCounter+1] = texCoord[vertexCounter+7] = ch->t[1];            \
	texCoord[vertexCounter+3] = texCoord[vertexCounter+5] = ch->t[3];            \
	texCoord[vertexCounter+4] = texCoord[vertexCounter+6] = ch->t[2];            \
	w = ceilf(x);                                                                 \
	vertexCoord[vertexCounter] = vertexCoord[vertexCounter+2] = ch->v[0] + w;    \
	vertexCoord[vertexCounter+1] = vertexCoord[vertexCounter+7] = ch->v[1] + h;  \
	vertexCoord[vertexCounter+3] = vertexCoord[vertexCounter+5] = ch->v[3] + h;  \
	vertexCoord[vertexCounter+4] = vertexCoord[vertexCounter+6] = ch->v[2] + w;  \
	vertexCounter += 8;                                                          \
	x += ch->w;                                                                  \
}                                                                              \
while(0)

#define UNICODE_TO_INT(a, i, increment)                                        \
high = low = 0;                                                                \
if((a[i] & 0x80) == 0) {                                                       \
	low = a[i];                                                                  \
	increment=1;                                                                 \
}                                                                              \
else if((a[i] & 0xe0) == 0xc0) {                                               \
	low = a[i+1] & 0x3f;                                                         \
	low |= (a[i] & 0x03) << 6;                                                   \
	high = (a[i] & 0x1f) >> 2;                                                   \
	increment=2;                                                                 \
}                                                                              \
else if((a[i] & 0xf0) == 0xe0) {                                               \
	low = a[i+2] & 0x3f;                                                         \
	low |= (a[i+1] & 0x03) << 6;                                                 \
	high = (a[i+1] & 0x3f) >> 2;                                                 \
	high |= (a[i] & 0x0f) << 4;                                                  \
	increment=3;                                                                 \
}                                                                              \
else if((a[i] & 0xf8) == 0xf0) {                                               \
	low = a[i+3] & 0x3f;                                                         \
	low |= (a[i+2] & 0x03) << 6;                                                 \
	high = (a[i+2] & 0x3f) >> 2;                                                 \
	high |= (a[i+1] & 0x3f) << 4;                                                \
	high |= (a[i] & 0x07) << 10;                                                 \
	increment=4;                                                                 \
}

void fontPrintf(Font *currentFont, const unsigned char * str, float x, float y, float maxw, int align) {
	FontChar *ch;
	int       i            = 0;
	int       last_space   = 0;
	int       buf          = 0;
	int       spaces       = -1;
	int       high         = 0;
	int       low          = 0;
	int       increment    = 0;
	int       incrementBuf = 0;
	int       c;
	float     w            = 0.0;
	float     h            = 0.0;
	float     lastw        = 0.0;
	float     justifyWidth = 0.0;
	float     spacew       = currentFont->spacew;
	float     fontHeight   = currentFont->height * currentFont->_interval;
	float     oldy         = y;
	bool      end          = FALSE;
	bool      justify      = (align == alignJustify);
	if(maxw > 0.0)
	{
		maxw = maxw / currentFont->_scale * screenScale.scaleX;
		if(maxw == 0.0) maxw = 0.0001;
	}
	FLUSH_BUFFER();
	glPushMatrix();
	if(FALSE == currentFont->scalable)
	{
		glScalef(currentFont->_scale / screenScale.scaleX, currentFont->_scale / screenScale.scaleY, 1);
		glTranslatef(floor(x * screenScale.scaleX), floor(y * screenScale.scaleY), 0);
	}
	else
	{
		glScalef(currentFont->_scale, currentFont->_scale, 1);
		glTranslatef(x, y, 0);
	}
	
	x = h = y = 0.0;

	imageBind(currentFont->image);
	#define CHAR_NOT_EXISTS (high > currentFont->allocated || NULL == currentFont->chars[high] || NULL == currentFont->chars[high][low])
	if(maxw > 0.0)
	{
		while(TRUE)
		{
			UNICODE_TO_INT(str, i, increment)
			c = low | (high << 8);
			
			switch(c)
			{
				case ' ':
						last_space = i;
						lastw = w;
						spaces++;
						w += currentFont->chars[0][' ']->w;
						break;
				case '\t':
						w += spacew * 8;
						last_space = i;
						lastw = w;
						break;
				case '\0':
						end = TRUE;
						break;
				default:
						if(CHAR_NOT_EXISTS)
						{
							i += increment;
							continue;
						}
						w += currentFont->chars[high][low]->w;
			}
			if(w > maxw || '\n' == c || TRUE == end)
			{
				if(0 == last_space || '\n' == c || TRUE == end)
				{
					last_space = i;
					lastw = w;
				}
				switch(align)
				{
					case alignCenter:
						x = (maxw - lastw) * 0.5;
						break;
					case alignRight:
						x = maxw - lastw;
						break;
					case alignJustify:
						if('\n' == c || TRUE == end)
							justifyWidth = spacew;
						else
							justifyWidth = (maxw + spacew * (spaces) - lastw) / (float)(spaces);
						x = 0;
						break;
						default: x = 0;
				}
				if(buf == last_space) last_space++;
				if((y + oldy + fontHeight) * currentFont->_scale > 0)
				{
					while(buf < last_space)
					{
						UNICODE_TO_INT(str,buf,incrementBuf)
						c = low | (high << 8);
						buf += incrementBuf;
						
						if('\t' == c)
								x += spacew * 8;
						else if(' ' == c)
						{
							if(TRUE == justify)
								x += justifyWidth;
							else
								x += spacew;
						}
						else
						{
							if(CHAR_NOT_EXISTS)
								continue;
							ch = currentFont->chars[high][low];
							DRAW_CHAR;
						}
					}
				}
				else
					buf = last_space;
				if(TRUE == end)
					break;
				x = 0;
				y += fontHeight;
				/* fast dropping invisible lines */
				if((y + oldy) * currentFont->_scale > screen->h) break;
				h = ceil(y);
				increment = 0;
				if(' ' == str[buf] || '\t' == str[buf] || '\n' == str[buf])
					i = buf = last_space + 1;
				else
					i = buf = last_space;
				last_space = 0;
				w = 0;
				spaces = -1;
			}
			i += increment;
		}
	}
	else
		while(str[i]) {
			UNICODE_TO_INT(str, i, increment)
			c = low | (high << 8);
			switch(c) {
				case '\n':
					x = 0;
					y += fontHeight;
					if((y + oldy) * currentFont->_scale > screen->h) break;
					h = ceil(y);
					goto end_loop;
				case '\t':
					x += spacew * 8;
					goto end_loop;
			}
			if(CHAR_NOT_EXISTS)
				goto end_loop;
			ch = currentFont->chars[high][low];
			DRAW_CHAR;
			end_loop:
			i += increment;
		}
	FLUSH_BUFFER();
	glPopMatrix();
}

//~ static int Font_stringToLines() {
	//~ register char * str;
	//~ float maxw;
	//~ int j = 1;
	//~ int i = 0, last_space = 0, buf = 0;
	//~ float w = 0;
	//~ unsigned char c;
    //~ if(!currentFont) return luaL_error(L, "Call <yourfont>:select() first!");
    //~ str = (char *)luaL_checkstring(L, 1);
    //~ maxw = (float)luaL_checknumber(L, 2) / currentFont->scale;
    //~ lua_newtable(L);
    //~ 
//~ 
    //~ PRINT_LINES(lua_pushlstring(L, str + buf, last_space - buf);
            //~ lua_rawseti(L, -2, j++);,
            //~ lua_pushstring(L, str + buf);
            //~ lua_rawseti(L, -2, j);)
//~ 
    //~ return 1;
//~ }

//~ void fontSelect(Font *font) {
	//~ currentFont = font;
//~ }

void fontScale(Font *font, float scale) {
	font->_scale = scale;
}

void fontInterval(Font *font, float interval) {
	font->_interval = interval;
}

float fontGetScale(Font *font) {
	return font->_scale;
}

float fontGetInterval(Font *font) {
	return font->_interval;
}

void fontSetGlyph(Font *ptr, const char *line) {
	float cx2 = 0.0;
	float cy2 = 0.0;
	float x1  = 0.0;
	float y1  = 0.0;
	float x2  = 0.0;
	float y2  = 0.0;
	float cx1 = 0.0;
	float cy1 = 0.0;
	float w   = 0.0;
	float h   = 0.0;
	unsigned ch = 0, ch2;
	unsigned char char_string_buffer[9];
	int i    = 0;
	int high = 0;
	int low  = 0;
	FontChar * fch;
	if(sscanf(line, "%d %f %f %f %f %f %f %f %f", &ch, &x1, &y1, &x2, &y2, &cx1, &cy1, &w, &h) == -1)
		return;
	ch2 = ch;
	while(ch) {
		if(ch & 0xff000000)
		{
			char_string_buffer[i] = (ch & 0xff000000) >> 24;
			i++;
		}
		ch <<= 8;
	}
	char_string_buffer[i] = 0;
	UNICODE_TO_INT(char_string_buffer, 0, i)
	else
	{
		myError("character %d is out of range (high: %x, low: %x)", ch2, high, low);
		return;
	}
	if(!ptr->chars)
	{
		ptr->allocated = 1;
		new(ptr->chars, FontChar**, ptr->allocated);
		fill(ptr->chars, 0, FontChar**, ptr->allocated);
		ptr->mem += ptr->allocated * sizeof(FontChar**);
	}
	const int incr = 1;
	if(high + 1> ptr->allocated)
	{
		renew(ptr->chars, FontChar**, high + incr);
		fill(ptr->chars + ptr->allocated, 0, FontChar**, high + incr - ptr->allocated);
		ptr->mem += (high + incr - ptr->allocated) * sizeof(FontChar**);
		ptr->allocated = incr + high;
	}
	if(!ptr->chars[high])
	{
		new(ptr->chars[high], FontChar*, 256);
		fill(ptr->chars[high], 0, FontChar*, 256);
		ptr->mem += 256 * sizeof(FontChar*);
	}
	if(!ptr->chars[high][low])
	{
		new(ptr->chars[high][low], FontChar, 1);
		ptr->mem += sizeof(FontChar);
	}
	else return;
	fch = ptr->chars[high][low];
	
	x1 = x1 / (float)ptr->image->w;
	y1 = y1 / (float)ptr->image->h;
	cx2 = cx1 + x2;
	cy2 = cy1 + y2;
	x2 = x1 + x2 / (float)ptr->image->w;
	y2 = y1 + y2 / (float)ptr->image->h;
	float vert[] = {cx1,cy1,cx2,cy2};
	float tex[] = {x1,y1,x2,y2};
	memcpy(fch->v, vert, sizeof(vert));
	memcpy(fch->t, tex, sizeof(tex));
	fch->w = w;
	if(' ' == low && '\0' == high)
	{
		ptr->spacew = w;
		if(!ptr->chars[0]['\0']) new(ptr->chars[0]['\0'], FontChar, 1);
		if(!ptr->chars[0]['\n']) new(ptr->chars[0]['\n'], FontChar, 1);
		if(!ptr->chars[0]['\r']) new(ptr->chars[0]['\r'], FontChar, 1);
		if(!ptr->chars[0]['\t']) new(ptr->chars[0]['\t'], FontChar, 1);
		ptr->mem += sizeof(FontChar) * 4;
		ptr->chars[0]['\t']->w = w * 8;
	}
	ptr->height = h;
}

void deleteFont(Font * ptr) {
	int i, j;
	if(ptr)
	{
		if(ptr->chars)
		{
			for(i = 0; i < ptr->allocated; i++)
			{
				if(ptr->chars[i])
				{
					for(j = 0; j < 255; j++)
					{
						if(ptr->chars[i][j])
							delete(ptr->chars[i][j]);
					}
					delete(ptr->chars[i]);
				}
			}
			delete(ptr->chars);
		}
	}
	else
		myError("Trying to free a null-font. Maybe, you did it manually?");
}