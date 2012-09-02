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
#include <locale.h>
#include <string.h>
#include "cheetah.h"
#include "render.h"

//~ #define NO_VBO

//~ Font * currentFont;

//~ /**
 //~ * @descr Create new font.
 //~ * @group font
 //~ * @var Image object
 //~ * @return Font object
 //~ * */
//~ Font * newFont(Image * img) {
	//~ Font *ptr;
	//~ new(ptr, Font, 1);
	//~ ptr->image = img;
	//~ ptr->scale = 1.0f;
	//~ return ptr;
//~ }

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
		if(*str == '\t')\
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

#ifdef NO_VBO
#define DRAW_CHAR \
	do {\
		glCallList(ch->vertex);\
		glTranslatef(ch->w, 0, 0);\
		w += ch->w;\
	}\
	while(0)
#else 
#define DRAW_CHAR \
	do {\
		VERTEX_QUERY(memstep + 8);\
		texCoord[memstep] = texCoord[memstep+2] = ch->t[0];\
		texCoord[memstep+1] = texCoord[memstep+7] = ch->t[1];\
		texCoord[memstep+3] = texCoord[memstep+5] = ch->t[3];\
		texCoord[memstep+4] = texCoord[memstep+6] = ch->t[2];\
		w = ceil(x);\
		vertexCoord[memstep] = vertexCoord[memstep+2] = ch->v[0] + w;\
		vertexCoord[memstep+1] = vertexCoord[memstep+7] = ch->v[1] + h;\
		vertexCoord[memstep+3] = vertexCoord[memstep+5] = ch->v[3] + h;\
		vertexCoord[memstep+4] = vertexCoord[memstep+6] = ch->v[2] + w;\
		memstep += 8;\
		x += ch->w;\
	}\
	while(0)
#endif

//~ #define UNICODE_TO_INT(a,i,increment) \
//~ high = low = 0;\
//~ if((a[i] & 0b10000000) == 0) {\
	//~ low = a[i];\
	//~ increment = 1;\
//~ }\
//~ else if((a[i] & 0b11100000) == 0b11000000) {\
	//~ low = a[i+1] & 0b00111111;\
	//~ low |= (a[i] & 0b00000011) << 6;\
	//~ high = (a[i] & 0b00011111) >> 2;\
	//~ increment = 2;\
//~ }\
//~ else if((a[i] & 0b11110000) == 0b11100000) {\
	//~ low = a[i+2] & 0b00111111;\
	//~ low |= (a[i+1] & 0b00000011) << 6;\
	//~ high = (a[i+1] & 0b00111111) >> 2;\
	//~ high |= (a[i] & 0b00001111) << 4;\
	//~ increment = 3;\
//~ }\
//~ else if((a[i] & 0b11111000) == 0b11110000) {\
	//~ low = a[i+3] & 0b00111111;\
	//~ low |= (a[i+2] & 0b00000011) << 6;\
	//~ high = (a[i+2] & 0b00111111) >> 2;\
	//~ high |= (a[i+1] & 0b00111111) << 4;\
	//~ high |= (a[i] & 0b00000111) << 10;\
	//~ increment = 4;\
//~ }

#define UNICODE_TO_INT(a,i,increment) \
high = low = 0;\
if((a[i] & 0x80) == 0) {\
	low = a[i];\
	increment = 1;\
}\
else if((a[i] & 0xe0) == 0xc0) {\
	low = a[i+1] & 0x3f;\
	low |= (a[i] & 0x03) << 6;\
	high = (a[i] & 0x1f) >> 2;\
	increment = 2;\
}\
else if((a[i] & 0xf0) == 0xe0) {\
	low = a[i+2] & 0x3f;\
	low |= (a[i+1] & 0x03) << 6;\
	high = (a[i+1] & 0x3f) >> 2;\
	high |= (a[i] & 0x0f) << 4;\
	increment = 3;\
}\
else if((a[i] & 0xf8) == 0xf0) {\
	low = a[i+3] & 0x3f;\
	low |= (a[i+2] & 0x03) << 6;\
	high = (a[i+2] & 0x3f) >> 2;\
	high |= (a[i+1] & 0x3f) << 4;\
	high |= (a[i] & 0x07) << 10;\
	increment = 4;\
}

void fontPrintf(Font *currentFont, register const unsigned char * str, float x, float y, float maxw, int align) {
	FontChar *ch;
	int i = 0, last_space = 0, buf = 0, spaces = -1, high = 0, low = 0, increment, incrementBuf, c;
	float w = 0, lastw = 0, h;
	float justifyWidth = 0;
	#ifdef NO_VBO
		float justifyFrac = 0, justifyAdd = 0, justifyAddw;
	#else
		int memstep = 0;
	#endif
	float spacew = currentFont->spacew;
	float fontHeight = currentFont->height * currentFont->_interval;
	bool end = 0;
	bool justify = align == alignJustify;
	if(maxw > .0) {
		maxw = maxw / currentFont->_scale * screenScale.scaleX;
		if(maxw == .0) maxw = 0.0001;
	}
	glPushMatrix();
	if(!currentFont->scalable)
	{
		glScalef(currentFont->_scale / screenScale.scaleX, currentFont->_scale / screenScale.scaleY, 1);
		glTranslatef(floor(x * screenScale.scaleX), floor(y * screenScale.scaleY), 0);
	}
	else
	{
		glScalef(currentFont->_scale, currentFont->_scale, 1);
		glTranslatef(x, y, 0);
	}
	
	h = y = 0;

	imageBind(currentFont->image);
	if(maxw > .0)
	{
		while(1) {
			UNICODE_TO_INT(str,i, increment)
			c = low | (high << 8);
			if(currentFont->allocated < high)
			{
				i += increment;
				continue;
			}
			switch(c)
			{
					case '\t':
							w += spacew * 8;
							last_space = i;
							lastw = w;
							break;
					case '\0':
							end = 1;
							break;
					case ' ':
							last_space = i;
							lastw = w;
							spaces++;
					default:
							w += currentFont->chars[high][low]->w;
			}
			if(w > maxw || c == '\n' || end)
			{
				if(!last_space || c == '\n'|| end) {
					last_space = i;
					lastw = w;
				}
				switch(align) {
					case alignCenter:
						#ifdef NO_VBO
							w = floor((maxw - lastw)*0.5);
							glTranslatef(w, 0, 0);
						#else
							x = (maxw - lastw)*0.5;
						#endif
						break;
					case alignRight:
						#ifdef NO_VBO
							w = floor((maxw - lastw));
							glTranslatef(w, 0, 0);
						#else
							x = maxw - lastw;
						#endif
						break;
					case alignJustify:
						if(c == '\n'|| end)
							justifyWidth = spacew;
						else
						#ifdef NO_VBO
							justifyFrac = modff((maxw + spacew*(spaces) - lastw)/(float)(spaces), &justifyWidth);
						w = 0;
						#else
							justifyWidth = (maxw + spacew*(spaces) - lastw)/(float)(spaces);
						x = 0;
						#endif
						break;
					#ifdef NO_VBO
						default: w = 0;
					#else 
						default: x = 0;
					#endif
				}
				if(buf == last_space) last_space++;
				while(buf < last_space) {
					UNICODE_TO_INT(str,buf,incrementBuf)
					c = low | (high << 8);
					buf += incrementBuf;
					if(currentFont->allocated < high)
						continue;
					if(c == '\t')
					{
						#ifdef NO_VBO
							glTranslatef(spacew * 8, 0, 0);
							w += spacew * 8;
						#else
							x += spacew * 8;
						#endif
					}
					else if(c == ' ')
					{
						if(justify)
						{
							#ifdef NO_VBO
								justifyAdd = modff(justifyFrac + justifyAdd, &justifyAddw);
								glTranslatef(justifyWidth + justifyAddw, 0, 0);
								w += justifyWidth + justifyAddw;
							#else 
								x += justifyWidth;
							#endif
						}
						else
						{
							#ifdef NO_VBO
								glTranslatef(spacew, 0, 0);
								w += spacew;
							#else 
								x += spacew;
							#endif
						}
					}
					else
					{
						ch = currentFont->chars[high][low];
						DRAW_CHAR;
					}
				}
				if(end) break;
				#ifdef NO_VBO
					glTranslatef(-w, h, 0);
				#else 
					x = 0;
				#endif
				y += fontHeight;
				h = ceil(y);
				increment = 0;
				if(str[buf] == ' ' || str[buf] == '\t' || str[buf] == '\n')
				{
					i = buf = last_space+1;
				}
				else
				{
					i = buf = last_space;
				}
				last_space = 0;
				w = 0;
				spaces = -1;
				#ifdef NO_VBO
					justifyAdd = 0;
				#endif
			}
			i += increment;
		}
	}
	else
			while(str[i]) {
				UNICODE_TO_INT(str,i, increment)
				c = low | (high << 8);
				switch(c) {
					case '\n':
						#ifdef NO_VBO
							glTranslatef(-w, h, 0);
							w = 0;
						#else 
							x = 0;
						#endif
						y += fontHeight;
						h = ceil(y);
						goto end_loop;
					case '\t':
						#ifdef NO_VBO
							glTranslatef(spacew * 8, 0, 0);
							w += spacew * 8;
						#else 
							x += spacew * 8;
						#endif
						goto end_loop;
				}
				
				if(currentFont->allocated < high)
					goto end_loop;
				ch = currentFont->chars[high][low];
				DRAW_CHAR;
				end_loop:
				i += increment;
			}
	#ifndef NO_VBO
		glVertexPointer(2, GL_FLOAT, 0, vertexCoord);
		glTexCoordPointer(2, GL_FLOAT, 0, texCoord);
		glDrawArrays(GL_QUADS, 0, (memstep>>1));
	#endif
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
	static float cx2=0, cy2=0, x1=0, y1=0, x2=0, y2=0, cx1=0, cy1=0, w=0, h=0;
	static unsigned int ch=0, ch2;
	static unsigned char c[5];
	int i = 0, high=0, low=0, increment = 0;
	FontChar * fch;
	if(sscanf(line, "%d %f %f %f %f %f %f %f %f", &ch, &x1, &y1, &x2, &y2, &cx1, &cy1, &w, &h) == -1)
		return;
	ch2 = ch;
	while(ch) {
		if(ch & 0xff000000)
		{
			c[i] = (ch & 0xff000000) >> 24;
			i++;
		}
		ch <<= 8;
	}
	c[i] = 0;
	UNICODE_TO_INT(c,0,increment)
	else {
		myError("character %d is out of range (high: %x, low: %x)", ch2, high, low);
		return;
	}
	if(!ptr->chars) {
		ptr->allocated = 1;
		new(ptr->chars, FontChar**, ptr->allocated);
		fill(ptr->chars, 0, FontChar**, ptr->allocated);
		ptr->mem += ptr->allocated * sizeof(FontChar**);
	}
	const int incr = 1;
	if(high + 1> ptr->allocated) {
		renew(ptr->chars, FontChar**, high + incr);
		fill(ptr->chars + ptr->allocated, 0, FontChar**, high + incr - ptr->allocated);
		ptr->mem += (high + incr - ptr->allocated) * sizeof(FontChar**);
		ptr->allocated = incr + high;
	}
	if(!ptr->chars[high]) {
		new(ptr->chars[high], FontChar*, 256);
		fill(ptr->chars[high], 0, FontChar*, 256);
		ptr->mem += 256 * sizeof(FontChar*);
	}
	if(!ptr->chars[high][low]) {
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
	#ifdef NO_VBO
	fch->vertex = glGenLists(1);
	glNewList(fch->vertex, GL_COMPILE);
	glBegin(GL_QUADS);
	glTexCoord2f(x1, y1);         glVertex2i(cx1, cy1);
	glTexCoord2f(x1, y2);         glVertex2i(cx1, cy2);
	glTexCoord2f(x2, y2);         glVertex2i(cx2, cy2);
	glTexCoord2f(x2, y1);         glVertex2i(cx2, cy1);
	glEnd();
	glEndList();
	#else
	//~ float vert[] = {cx1,cy1,cx1,cy2,cx2,cy2,cx2,cy1};
	//~ float tex[] = {x1,y1,x1,y2,x2,y2,x2,y1};
	float vert[] = {cx1,cy1,cx2,cy2};
	float tex[] = {x1,y1,x2,y2};
	memcpy(fch->v, vert, sizeof(vert));
	memcpy(fch->t, tex, sizeof(tex));
	#endif
	fch->w = w;
	if(low == 32 && high==0) {
		ptr->spacew = w;
		if(!ptr->chars[0][0]) new(ptr->chars[0][0], FontChar, 1);
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
	if(ptr) {
		//~ glDeleteTextures(1, &ptr->image->id);
		//~ delete(ptr->image);
		if(ptr->chars)
		{
			for(i = 0; i < ptr->allocated; i++)
			{
				if(ptr->chars[i])
				{
					for(j = 0; j < 255; j++)
					{
						if(ptr->chars[i][j]) delete(ptr->chars[i][j]);
					}
					delete(ptr->chars[i]);
				}
			}
			delete(ptr->chars);
		}
	}
	else myError("Trying to free a null-font. Maybe, you did it manually?");
}