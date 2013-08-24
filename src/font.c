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
#include "test.h"

bool fontShaderFailed = FALSE;
Shader * df_shader = NULL;
const char * fontShaderNormalSource = "#version 120\n\
uniform sampler2D texture;\n\
uniform float sharpness;\n\
uniform float gamma;\n\
varying vec2 TexCoord;\n\
void main() {\n\
	float color = texture2D(texture, TexCoord).a;\n\
	gl_FragColor = vec4(gl_Color.rgb, gl_Color.a * smoothstep(gamma - sharpness, gamma + sharpness, color));\n\
}";

const char * fontShaderFastSource = "#version 120\n\
uniform sampler2D texture;\n\
uniform float sharpness;\n\
uniform float gamma;\n\
varying vec2 TexCoord;\n\
void main() {\n\
	float color = texture2D(texture, TexCoord).a;\n\
	gl_FragColor = vec4(gl_Color.rgb, gl_Color.a * clamp((color - 0.5) * sharpness + 0.5, 0., 1.));\n\
}";

bool shaderCheck(Shader * ptr);
Shader * initShader();
void newFragmentShader(Shader * ptr, const char * frag);

static inline unsigned fontHashFunc(unsigned key)
{
	return key ^ (key << 7) ^ (key >> 5);
}

static inline unsigned fontCmpFunc(unsigned a, unsigned b)
{
	return a == b;
}

HASH_TEMPLATE(FontHash, unsigned, FontChar*, fontHashFunc, fontCmpFunc)

typedef struct KerningPair {
	unsigned first;
	unsigned second;
} KerningPair;

static inline unsigned kerningHashFunc(KerningPair key)
{
	return key.first * key.second;
}

static inline unsigned kerningCmpFunc(KerningPair a, KerningPair b)
{
	return a.second == b.second && a.first == b.first;
}

HASH_TEMPLATE(KernHash, KerningPair, float, kerningHashFunc, kerningCmpFunc)

void imageBind(Image * image);
extern SDL_Surface *screen;

#define UNICODE_TO_INT(a, i)                                                   \
if((a[i] & 0b10000000) == 0) {                                                 \
    c  =  (unsigned)a[i++];                                                              \
}                                                                              \
else if (((unsigned)a[i]   & 0b11100000) == 0b11000000) {                                \
    c =  ((unsigned)a[i++] & 0b00011111) << 6;                                           \
    c |=  (unsigned)a[i++] & 0b00111111;                                                 \
}                                                                              \
else if (((unsigned)a[i]   & 0b11110000) == 0b11100000) {                                \
    c =  ((unsigned)a[i++] & 0b00001111) << 12;                                          \
    c |= ((unsigned)a[i++] & 0b00111111) << 6;                                           \
    c |=  (unsigned)a[i++] & 0b00111111;                                                 \
}                                                                              \
else if (((unsigned)a[i]   & 0b11111000) == 0b11110000) {                                \
    c =  ((unsigned)a[i++] & 0b00000111) << 18;                                          \
    c |= ((unsigned)a[i++] & 0b00111111) << 12;                                          \
    c |= ((unsigned)a[i++] & 0b00111111) << 6;                                           \
    c |=  (unsigned)a[i++] & 0b00111111;                                                 \
} else { /* Error! */                                                          \
    c = 0;                                                                     \
}

void fontEnableDistanceField(Font *f) {
	f->distanceField = TRUE;
}

void fontDisableDistanceField(Font *f) {
	f->distanceField = FALSE;
}

/* Calculate width of string. */
float fontWidth(Font *f, const char *str) {
	float	width = 0;
	float	maxwidth = 0;
	unsigned	c = 0;
	int	i = 0;
	FontChar *fch;
	unsigned prevChar = 0;
	while(str[i])
	{
		UNICODE_TO_INT(str, i)
		switch(c)
		{
			case ' ':
				width += f->_spacewidth;
				continue;
			case '\t':
				width += f->_spacewidth * 8;
				continue;
			case '\n':
				if(width > maxwidth)
					maxwidth = width;
				width = 0;
				continue;
		}
		fch = FontHash_get(f->hash, c);
		if(fch)
			width += fch->w;
		if(NULL != f->kerningHash && prevChar > 0)
		{
			KerningPair kp = {prevChar, c};
			float kerning = KernHash_get(f->kerningHash, kp);
			width += kerning;
		}
	}
	if(width > maxwidth)
		maxwidth = width;
	return maxwidth * f->_scale;
}

/* Calculate height of string. */
float fontHeight(Font *font) {
	return font->height * font->_scale;
}

#define FONT_CEIL(font, x) (font->scalable || font->distanceField) ? x : ceilf(x)

#define DRAW_CHAR do {                                                         \
    FLUSH_BUFFER_IF_OVERFLOW                                                   \
    texCoord[vertexCounter] = texCoord[vertexCounter+2] = ch->t[0];            \
    texCoord[vertexCounter+1] = texCoord[vertexCounter+7] = ch->t[1];          \
    texCoord[vertexCounter+3] = texCoord[vertexCounter+5] = ch->t[3];          \
    texCoord[vertexCounter+4] = texCoord[vertexCounter+6] = ch->t[2];          \
    width = FONT_CEIL(currentFont, x);                                         \
    vertexCoord[vertexCounter] = vertexCoord[vertexCounter+2] = ch->v[0]+width;\
    vertexCoord[vertexCounter+1] = vertexCoord[vertexCounter+7] = ch->v[1] + h;\
    vertexCoord[vertexCounter+3] = vertexCoord[vertexCounter+5] = ch->v[3] + h;\
    vertexCoord[vertexCounter+4] = vertexCoord[vertexCounter+6]=ch->v[2]+width;\
    vertexCounter += 8;                                                        \
    x += ch->w;                                                                \
}                                                                              \
while(0)

// #define KERNING_CONDITION (TRUE == currentFont->_kerning && NULL != currentFont->kerningHash && prevChar > 0 && fontPrevChar && fontPrevChar->kerning)
#define KERNING_CONDITION (NULL != currentFont->kerningHash && fontPrevChar && fontPrevChar->kerning)
void fontPrintf(Font *currentFont, const unsigned char *str, float x, float y, float maxw, int align) {
	FontChar *ch           = NULL;
	unsigned       i            = 0;
	unsigned       j;
	unsigned       last_space   = 0;
	unsigned       buf          = 0;
	int       spaces       = -1;
	unsigned  c            = 0;
	float     width        = 0.0f;
	float     h            = 0.0f;
	float     lastw        = 0.0f;
	float     justifyWidth = currentFont->_spacewidth;
	float     spacew       = currentFont->_spacewidth;
	float     height   = currentFont->height * currentFont->_interval;
	float     oldy         = y / currentFont->_scale;
	bool      end          = FALSE;
	bool      yOutScreen;
	FontHash *hash         = (FontHash*)currentFont->hash;
	unsigned  prevChar     = 0;
	FontChar *fontPrevChar = NULL;
	if(NULL == hash)
		return;
	// if(maxw > 0.0f)
	{
		maxw = maxw / currentFont->_scale;
		// if(maxw == 0.0f) maxw = 0.0001f;
	}
	FLUSH_BUFFER();
	glPushMatrix();
	if(TRUE == currentFont->distanceField)
	{
		if(NULL == df_shader && FALSE == fontShaderFailed)
		{
			if(FALSE == supported.GLSL)
				fontShaderFailed = TRUE;
			else
			{
				df_shader = initShader();
				newFragmentShader(df_shader, fontShaderFastSource);
				if(FALSE == shaderCheck(df_shader))
					fontShaderFailed = TRUE;
			}
		}
		if(TRUE == fontShaderFailed)
		{
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, currentFont->dfGamma);
		}
		else
		{
			glUseProgramObject_(df_shader->id);
			glUniform1f_(glGetUniformLocation_(df_shader->id, "gamma"), currentFont->dfGamma);
			glUniform1f_(glGetUniformLocation_(df_shader->id, "sharpness"), 4.18f * currentFont->dfSharpness * currentFont->_scale * screenScale.scaleY);
		}
		glScalef(currentFont->_scale, currentFont->_scale, 1);
		glTranslatef(x / currentFont->_scale, y / currentFont->_scale, 0);
	}
	else if(FALSE == currentFont->scalable)
	{
		glScalef(currentFont->_scale / screenScale.scaleX, currentFont->_scale / screenScale.scaleY, 1);
		glTranslatef(floorf(x * screenScale.scaleX), floorf(y * screenScale.scaleY), 0);
		maxw = maxw * screenScale.scaleX;
	}
	else
	{
		glTranslatef(x, y, 0);
		glScalef(currentFont->_scale, currentFont->_scale, 1);
	}

	x = h = y = 0.0;

	imageBind(currentFont->image);
	if(maxw > 0.0f)
	{
		while(TRUE)
		{
			j = i;
			UNICODE_TO_INT(str, i)
			switch(c)
			{
				case ' ':
					last_space = j;
					lastw = width;
					spaces++;
					width += spacew;
					break;
				case '\t':
					width += spacew * 8;
					last_space = j;
					lastw = width;
					break;
				case '\0':
					end = TRUE;
					break;
				default:
					ch = FontHash_get(hash, c);
					if(NULL == ch)
						continue;
					width += ch->w;
			}
			/* drop invisible lines - great performance improvement for long texts */
			yOutScreen = (y + oldy + height) * currentFont->_scale > 0.0f;
			/* drop kerning computation for invisible lines - speed +15% */
			if(yOutScreen)
				if(KERNING_CONDITION)
				{
					KerningPair kp = {prevChar, c};
					width += KernHash_get(currentFont->kerningHash, kp);
				}
			prevChar = c;
			fontPrevChar = ch;
			if(width > maxw || '\n' == c || TRUE == end)
			{
				prevChar = 0;
				fontPrevChar = NULL;
				if(0 == last_space || '\n' == c || TRUE == end)
				{
					last_space = j;
					lastw = width;
				}
				if(buf == last_space)
					last_space++;
				/* dropping invisible lines from top */
				if(yOutScreen)
				{
					switch(align)
					{
						case alignCenter:
							x = (maxw - lastw) * 0.5f;
							break;
						case alignRight:
							x = maxw - lastw;
							break;
						case alignJustify:
							if('\n' == c || TRUE == end)
								justifyWidth = spacew;
							else
								justifyWidth = (maxw + spacew * ((float)spaces) - lastw) / (float)(spaces);
							x = 0;
							break;
							default: x = 0;
					}
					float kerningAccumulator = 0.0;
					while(buf < last_space)
					{
						UNICODE_TO_INT(str, buf)
						if(KERNING_CONDITION)
						{
							KerningPair kp = {prevChar, c};
							float kerning = KernHash_get(currentFont->kerningHash, kp);
							x += kerning;
							if(align == alignJustify)
								kerningAccumulator -= kerning;
						}
						ch = NULL;
						switch(c)
						{
							case ' ':
								x += justifyWidth + kerningAccumulator;
								kerningAccumulator = 0.0;
								break;
							case '\t':
								x += spacew * 8;
								break;
							default:
								ch = FontHash_get(hash, c);
								if(NULL == ch)
									continue;
								DRAW_CHAR;
						}
						prevChar = c;
						fontPrevChar = ch;
					}
				}
				else
					buf = last_space;
				if(TRUE == end)
					break;
				y += height;
				/* dropping invisible lines from buttom */
				if((y + oldy) * currentFont->_scale > screen->h)
					break;
				h = FONT_CEIL(currentFont, y);
				switch(str[buf])
				{
					case ' ':
					case '\t':
					case '\n':
						i = buf = last_space + 1;
						break;
					default:
						i = buf = last_space;
				}
				last_space = 0;
				width = 0;
				spaces = -1;
				prevChar = 0;
				fontPrevChar = NULL;
			}

		}
	}
	else
	{
		while(str[i])
		{
			UNICODE_TO_INT(str, i)
			ch = NULL;
			switch(c)
			{
				case ' ':
					x += spacew;
					break;
				case '\n':
					x = 0;
					y += height;
					if((y + oldy) * currentFont->_scale > screen->h)
						break;
					h = FONT_CEIL(currentFont, y);
					break;
				case '\t':
					x += spacew * 8;
					break;
				default:
					ch = FontHash_get(hash, c);
					if(NULL == ch)
						continue;
					if(NULL != currentFont->kerningHash && prevChar > 0 && fontPrevChar && fontPrevChar->kerning)
					{
						KerningPair kp = {prevChar, c};
						float kerning = KernHash_get(currentFont->kerningHash, kp);
						x += kerning;
					}
					DRAW_CHAR;
			}
			prevChar = c;
			fontPrevChar = ch;
		}
	}
	FLUSH_BUFFER();
	glPopMatrix();
	if(TRUE == currentFont->distanceField)
	{
		if(TRUE == fontShaderFailed)
		{
			glDisable(GL_ALPHA_TEST);
		}
		else
		{
			glUseProgramObject_(0);
		}
	}
}

void fontScale(Font *font, float scale) {
	font->scalable = TRUE;
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
	unsigned ch = 0;
	FontChar *fch = NULL;
	if(sscanf(line, "%u %f %f %f %f %f %f %f %f", &ch, &x1, &y1, &x2, &y2, &cx1, &cy1, &w, &h) == -1)
		return;
	if(NULL == ptr->hash)
	{
		ptr->hash = (void *)FontHash_new();
		new0(fch, FontChar, 1);
		FontHash_set((FontHash*)ptr->hash, '\0', fch);
		fch = NULL;
		new0(fch, FontChar, 1);
		FontHash_set((FontHash*)ptr->hash, '\n', fch);
		fch = NULL;
		ptr->mem += (unsigned)sizeof(FontHash)
			+ (unsigned)sizeof(Font) + (unsigned)sizeof(FontChar) * 2
			+ FontHash_size((FontHash*)ptr->hash) * (unsigned)sizeof(FontHashNode);
	}
	new0(fch, FontChar, 1);
	x1 = x1 / (float)ptr->image->w;
	y1 = y1 / (float)ptr->image->h;
	cx2 = cx1 + x2;
	cy2 = cy1 + y2;
	x2 = x1 + x2 / (float)ptr->image->w;
	y2 = y1 + y2 / (float)ptr->image->h;
	float ver[] = {cx1, cy1, cx2, cy2};
	float tex[] = {x1,  y1,  x2,  y2};
	memcpy(fch->v, ver, sizeof(ver));
	memcpy(fch->t, tex, sizeof(tex));
	fch->w = w;
	FontHash_set((FontHash*)ptr->hash, ch, fch);
	ptr->mem += (unsigned)sizeof(FontChar);
	if(' ' == ch)
	{
		ptr->_spacewidth = w;
		fch = NULL;
		new0(fch, FontChar, 1);
		fch->w = w * 8;
		FontHash_set((FontHash*)ptr->hash, '\t', fch);
		ptr->mem += (unsigned)sizeof(FontChar);
	}
	ptr->height = h;
}

void fontSetKerning(Font *ptr, const char *line) {
	unsigned	first;
	unsigned	second;
	float		kerning;
	if(sscanf(line, "%u %u %f", &first, &second, &kerning) == -1)
		return;
	if(NULL == ptr->kerningHash)
	{
		ptr->kerningHash = (void *)KernHash_new_size(64);
		ptr->mem += (unsigned)sizeof(KernHash) + KernHash_size((KernHash*)ptr->kerningHash) * (unsigned)sizeof(KernHashNode);
	}
	KerningPair kp = {first, second};
	KernHash_set(ptr->kerningHash, kp, kerning);
	if(NULL == ptr->hash)
		return;
	FontChar *fch = FontHash_get(ptr->hash, first);
	/* mark that this char has kerning (much faster kerning access) */
	if(fch)
		fch->kerning = TRUE;
}

void deleteFont(Font * ptr) {
	if(ptr)
	{
		if(NULL != ptr->hash)
		{
			HASH_EACH(((FontHash*)ptr->hash), delete(hashnode->value);)
			FontHash_destroy((FontHash*)ptr->hash);
		}
		if(NULL != ptr->kerningHash)
		{
			KernHash_destroy((KernHash*)ptr->kerningHash);
		}
	}
	else
		myError("Trying to free a null-font. Maybe, you did it manually?");
}
