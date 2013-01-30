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
#include "chash.h"

inline unsigned fontHashFunc(unsigned key)
{
	return key ^ (key << 7) ^ (key >> 5);
}

inline unsigned fontCmpFunc(unsigned a, unsigned b)
{
	return a == b;
}

HASH_TEMPLATE(FontHash, unsigned, FontChar*, fontHashFunc, fontCmpFunc)

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
    if(vertexCounter >= VERTEX_BUFFER_LIMIT * 8) { FLUSH_BUFFER(); }           \
    texCoord[vertexCounter] = texCoord[vertexCounter+2] = ch->t[0];            \
    texCoord[vertexCounter+1] = texCoord[vertexCounter+7] = ch->t[1];          \
    texCoord[vertexCounter+3] = texCoord[vertexCounter+5] = ch->t[3];          \
    texCoord[vertexCounter+4] = texCoord[vertexCounter+6] = ch->t[2];          \
    w = ceil(x);                                                               \
    vertexCoord[vertexCounter] = vertexCoord[vertexCounter+2] = ch->v[0] + w;  \
    vertexCoord[vertexCounter+1] = vertexCoord[vertexCounter+7] = ch->v[1] + h;\
    vertexCoord[vertexCounter+3] = vertexCoord[vertexCounter+5] = ch->v[3] + h;\
    vertexCoord[vertexCounter+4] = vertexCoord[vertexCounter+6] = ch->v[2] + w;\
    vertexCounter += 8;                                                        \
    x += ch->w;                                                                \
}                                                                              \
while(0)

#define UNICODE_TO_INT(a, i, increment)                                        \
if((a[i] & 0b10000000) == 0) {                                                 \
    c = a[i];  increment=1;                                                    \
}                                                                              \
else if((a[i] & 0b11100000) == 0b11000000) {                                   \
    c = (a[i] & 0b00011111) << 6;                                              \
    c |= a[i+1] & 0b00111111;                                                  \
    increment=2;                                                               \
}                                                                              \
else if((a[i] & 0b11110000) == 0b11100000) {                                   \
    c = (a[i] & 0b00001111) << 12;                                             \
    c |= (a[i+1] & 0b00111111) << 6;                                           \
    c |= a[i+2] & 0b00111111;                                                  \
    increment=3;                                                               \
}                                                                              \
else if((a[i] & 0b11111000) == 0b11110000) {                                   \
    c = (a[i] & 0b00000111) << 18;                                             \
    c |= (a[i+1] & 0b00111111) << 12;                                          \
    c |= (a[i+2] & 0b00111111) << 6;                                           \
    c |= a[i+3] & 0b00111111;                                                  \
    increment=4;                                                               \
}

void fontPrintf(Font *currentFont, const unsigned char * str, float x, float y, float maxw, int align) {
	FontChar *ch;
	int       i            = 0;
	int       last_space   = 0;
	int       buf          = 0;
	int       spaces       = -1;
	int       increment    = 0;
	int       incrementBuf = 0;
	unsigned  c            = 0;
	float     w            = 0.0;
	float     h            = 0.0;
	float     lastw        = 0.0;
	float     justifyWidth = 0.0;
	float     spacew       = currentFont->spacew;
	float     fontHeight   = currentFont->height * currentFont->_interval;
	float     oldy         = y;
	bool      end          = FALSE;
	bool      justify      = (align == alignJustify);
	FontHash *hash         = (FontHash*)currentFont->hash;
	if(NULL == hash)
		return;
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
	if(maxw > 0.0)
	{
		while(TRUE)
		{
			UNICODE_TO_INT(str, i, increment)
			switch(c)
			{
				case ' ':
					last_space = i;
					lastw = w;
					spaces++;
					w += spacew;
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
					ch = FontHash_get(hash, c);
					if(NULL == ch)
					{
						i += increment;
						continue;
					}
					w += ch->w;
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
				if(buf == last_space)
					last_space++;
				/* dropping invisible lines from top */
				if((y + oldy + fontHeight) * currentFont->_scale > 0)
				{
					while(buf < last_space)
					{
						UNICODE_TO_INT(str, buf, incrementBuf)
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
							ch = FontHash_get(hash, c);
							if(NULL == ch)
								continue;
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
				/* dropping invisible lines from buttom */
				if((y + oldy) * currentFont->_scale > screen->h)
					break;
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
			ch = FontHash_get(hash, c);
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
			if(NULL == ch)
				goto end_loop;
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
	unsigned c = 0;
	FontChar * fch = NULL;
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
		myError("character %d is out of range", ch2);
		return;
	}
	if(NULL == ptr->hash)
	{
		ptr->hash = (void *)FontHash_new();
		new0(fch, FontChar, 1);
		FontHash_set((FontHash*)ptr->hash, '\0', fch);
		fch = NULL;
		new0(fch, FontChar, 1);
		FontHash_set((FontHash*)ptr->hash, '\n', fch);
		fch = NULL;
		ptr->mem += sizeof(FontHash) + sizeof(Font) + sizeof(FontChar) * 2 + FontHash_size((FontHash*)ptr->hash) * sizeof(FontHashNode);
	}
	new0(fch, FontChar, 1);
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
	FontHash_set((FontHash*)ptr->hash, c, fch);
	ptr->mem += sizeof(FontChar);
	if(' ' == c)
	{
		ptr->spacew = w;
		fch = NULL;
		new0(fch, FontChar, 1);
		fch->w = w * 8;
		FontHash_set((FontHash*)ptr->hash, '\t', fch);
		ptr->mem += sizeof(FontChar);
	}
	ptr->height = h;
}

void deleteFont(Font * ptr) {
	if(ptr)
	{
		if(NULL != ptr->hash)
		{
			HASH_EACH(((FontHash*)ptr->hash), delete(hashnode->value);)
			FontHash_destroy((FontHash*)ptr->hash);
		}
	}
	else
		myError("Trying to free a null-font. Maybe, you did it manually?");
}