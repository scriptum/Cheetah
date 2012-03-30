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

#include "math.h"
#include "cheetah.h"
#include "render.h"

#define NO_VBO

Font * currentFont;

/**
 * @descr Create new font.
 * @group font
 * @var Image object
 * @return Font object
 * */
Font * newFont(Image * img) {
	Font *ptr;
	ptr = new(Font);
	ptr->image = img;
	ptr->scale = 1.0f;
	return ptr;
}

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
			width += f->chars[32].w * 8;
			continue;
		}
		else if(*str == '\n')
			width = 0;
		width += f->chars[(unsigned char)*str].w;
	} while(*++str);
	return width * f->scale;
}

float fontHeight(Font *font) {
	return font->height * font->scale;
}

#define ALIGN(width) if(align == 0) \
	glTranslatef(floor((maxw - width)*0.5), 0, 0);\
	else if(align == 1) glTranslatef(floor((maxw - width)), 0, 0);
#ifdef NO_VBO
#define DRAW_CHAR glCallList(ch->vertex);
#else
#define DRAW_CHAR \
glBindBufferARB(GL_ARRAY_BUFFER_ARB, ch->vertex); \
glVertexPointer(2, GL_FLOAT, 0, (char *) NULL); \
glBindBufferARB(GL_ARRAY_BUFFER_ARB, ch->tex); \
glTexCoordPointer(2, GL_FLOAT, 0, (char *) NULL); \
glDrawArrays(GL_QUADS, 0, 4); 
#endif

float Font_Width(Font *f, register const char *str)
{
	float width = 0;
	if(*str)
	do {
		if(*str == '\t')\
		{
			width += f->chars[32].w * 8;
			continue;
		}
		else if(*str == '\n')
			width = 0;
		width += f->chars[(unsigned char)*str].w;
	} while(*++str);
	return width * f->scale;
}

void fontPrint(register const char * str, int maxw, int align) {
	float w;
	unsigned char c;
	FontChar *ch;
	if(!currentFont) return myError("Call <yourfont>:select() first!");
	w = 0;
	ALIGN(Font_Width(currentFont, str))
	
	glPushMatrix();
	glScalef(currentFont->scale / screenScale.scaleX, currentFont->scale / screenScale.scaleY, 1);
	//~ glScalef(currentFont->scale, currentFont->scale, 1);
	
	glBindTexture(GL_TEXTURE_2D, currentFont->image->id);
	glEnable(GL_TEXTURE_2D);
	#ifndef NO_VBO
	glEnableClientState(GL_VERTEX_ARRAY); 
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
	#endif
	if(*str)
	do {
		switch(*str) {
			case '\n':
				glTranslatef(-w, currentFont->height, 0);
				w = 0;
				continue;
			case '\t':
				glTranslatef(currentFont->chars[32].w * 8, 0, 0);
				w += currentFont->chars[32].w * 8;
				continue;
    }
		ch = &currentFont->chars[(unsigned char)*str];
		DRAW_CHAR
		glTranslatef(ch->w, 0, 0);
		w += ch->w;
	} while(*++str);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	#ifndef NO_VBO
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	#endif
}

#define PRINT_LINES(A,B) \
while(str[i]) {\
    c = (unsigned char)str[i];\
    switch(c)\
    {\
        case '\t':\
            w += currentFont->chars[32].w * 8;\
            last_space = i;\
            break;\
        case ' ':\
            last_space = i;\
        default:\
            w += currentFont->chars[c].w;\
    }\
    if(w > maxw || c == '\n')\
    {\
        if(!last_space || c == '\n') last_space = i;\
        A\
        i = last_space;\
        buf = last_space + 1;\
        last_space = 0;\
        w = 0;\
    }\
    i++;\
}\
B

void fontPrintf(register const char * str, float maxw, int align) {
	FontChar *ch;
	int i = 0, last_space = 0, buf = 0;
	float w = 0;
	unsigned char c;
	if(!currentFont) myError("Call <yourfont>:select() first!");
	maxw = maxw / currentFont->scale * screenScale.scaleX;
	glPushMatrix();
	glScalef(currentFont->scale / screenScale.scaleX, currentFont->scale / screenScale.scaleY, 1);
	//~ glScalef(currentFont->scale, currentFont->scale, 1);

	glBindTexture(GL_TEXTURE_2D, currentFont->image->id);
	glEnable(GL_TEXTURE_2D);
	#ifndef NO_VBO
	glEnableClientState(GL_VERTEX_ARRAY); 
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	#endif
	PRINT_LINES(
		ALIGN(w)
		w = 0;
		while(buf < last_space) {
			c = (unsigned char)str[buf];
			if(c == '\t') {
					glTranslatef(currentFont->chars[32].w * 8, 0, 0);
					w += currentFont->chars[32].w * 8;
					continue;
			}
			ch = &currentFont->chars[c];
			DRAW_CHAR
			glTranslatef(ch->w, 0, 0);
			w += ch->w;
			buf++;
		}
		glTranslatef(-w, currentFont->height, 0);
	,
    ALIGN(w)
		while(buf < i) {
			c = (unsigned char)str[buf];
			if(c == '\t') {
				glTranslatef(currentFont->chars[32].w * 8, 0, 0);
				continue;
			}
			ch = &currentFont->chars[c];
			DRAW_CHAR
			glTranslatef(ch->w, 0, 0);
			buf++;
	})
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	#ifndef NO_VBO
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	#endif
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

void fontSelect(Font *font) {
	currentFont = font;
}

void fontScale(Font *font, float scale) {
	font->scale = scale;
}

void fontSetGlyph(Font *ptr, unsigned int ch, float x1, float y1, float x2, float y2, float cx1, float cy1, float w, float h) {
	float cx2, cy2;
	x1 = x1 / (float)ptr->image->w;
	y1 = y1 / (float)ptr->image->h;
	cx2 = cx1 + x2;
	cy2 = cy1 + y2;
	x2 = x1 + x2 / (float)ptr->image->w;
	y2 = y1 + y2 / (float)ptr->image->h;
	float vert[] = {cx1,cy1,cx1,cy2,cx2,cy2,cx2,cy1};
	float tex[] = {x1,y1,x1,y2,x2,y2,x2,y1};
	#ifdef NO_VBO
	ptr->chars[ch].vertex = glGenLists(1);
	glNewList(ptr->chars[ch].vertex, GL_COMPILE);
	glBegin(GL_QUADS);
	glTexCoord2f(x1, y1);         glVertex2i(cx1, cy1);
	glTexCoord2f(x1, y2);         glVertex2i(cx1, cy2);
	glTexCoord2f(x2, y2);         glVertex2i(cx2, cy2);
	glTexCoord2f(x2, y1);         glVertex2i(cx2, cy1);
	glEnd();
	glEndList();
	#else
	glGenBuffersARB(1, &ptr->chars[ch].vertex);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, ptr->chars[ch].vertex);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vert), vert, GL_STATIC_DRAW_ARB);
	glGenBuffersARB( 1, &ptr->chars[ch].tex );
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, ptr->chars[ch].tex);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(tex), tex, GL_STATIC_DRAW_ARB);
	#endif
	ptr->chars[ch].w = w;
	ptr->height = h;
}

void deleteFont(Font * ptr) {
	if(ptr) {
		glDeleteTextures(1, &ptr->image->id);
		free(ptr->image);
	}
	else myError("Trying to free a null-font. Maybe, you did it manually?");
}