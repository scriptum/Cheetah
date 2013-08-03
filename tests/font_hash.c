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
#include <time.h>
// #include <SDL.h>

// #define CHASH_DEBUG

#include "../src/cheetah.h"
#include "../src/macros.h"
// #include "render.h"
// #include "vertex.h"
#include "../src/chash.h"
// #include "test.h"

/* test wrapper */
#define FLUSH_BUFFER()
#define vertexCounter 0
float texCoord[8];
float vertexCoord[8];
unsigned charsCount = 0;


static inline unsigned fontHashFunc(unsigned key)
{
	return key ^ (key << 7) ^ (key >> 3);
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

#ifndef TEST_NOUNICODE

#define UNICODE_TO_INT(a, i)                                                   \
if((a[i] & 0b10000000) == 0) {                                                 \
    c  =  a[i++];                                                              \
}                                                                              \
else if ((a[i]   & 0b11100000) == 0b11000000) {                                \
    c =  (a[i++] & 0b00011111) << 6;                                           \
    c |=  a[i++] & 0b00111111;                                                 \
}                                                                              \
else if ((a[i]   & 0b11110000) == 0b11100000) {                                \
    c =  (a[i++] & 0b00001111) << 12;                                          \
    c |= (a[i++] & 0b00111111) << 6;                                           \
    c |=  a[i++] & 0b00111111;                                                 \
}                                                                              \
else if ((a[i]   & 0b11111000) == 0b11110000) {                                \
    c =  (a[i++] & 0b00000111) << 18;                                          \
    c |= (a[i++] & 0b00111111) << 12;                                          \
    c |= (a[i++] & 0b00111111) << 6;                                           \
    c |=  a[i++] & 0b00111111;                                                 \
} else {                                                                       \
    c = 0;                                                                     \
}

#else

#define UNICODE_TO_INT(a, i) c=a[i]; i++;

#endif

#define FONT_CEIL(font, x) font->scalable ? x : ceilf(x)

#ifndef TEST_NODRAW

#define DRAW_CHAR do {charsCount++;                                                         \
    texCoord[vertexCounter] = texCoord[vertexCounter+2] = ch->t[0];            \
    texCoord[vertexCounter+1] = texCoord[vertexCounter+7] = ch->t[1];          \
    texCoord[vertexCounter+3] = texCoord[vertexCounter+5] = ch->t[3];          \
    texCoord[vertexCounter+4] = texCoord[vertexCounter+6] = ch->t[2];          \
    width = FONT_CEIL(currentFont, x);                                         \
    vertexCoord[vertexCounter] = vertexCoord[vertexCounter+2] = ch->v[0]+width;\
    vertexCoord[vertexCounter+1] = vertexCoord[vertexCounter+7] = ch->v[1] + h;\
    vertexCoord[vertexCounter+3] = vertexCoord[vertexCounter+5] = ch->v[3] + h;\
    vertexCoord[vertexCounter+4] = vertexCoord[vertexCounter+6]=ch->v[2]+width;\
    x += ch->w;                                                                \
}                                                                              \
while(0)

#else
/* "fake" drawing without writing to memory */

#define DRAW_CHAR do {charsCount++;                                                         \
    width = FONT_CEIL(currentFont, x);                                         \
    x += ch->w;                                                                \
}                                                                              \
while(0)

#endif

// #define KERNING_CONDITION if(TRUE == currentFont->_kerning && NULL != currentFont->kerningHash && prevChar > 0 && fontPrevChar && fontPrevChar->kerning)
#define KERNING_CONDITION if(TRUE == currentFont->_kerning)

void fontPrintf(Font *currentFont, const unsigned char *str, float x, float y, float maxw, int align) {
	FontChar *ch           = NULL;
	int       i            = 0;
	int       j;
	int       last_space   = 0;
	int       buf          = 0;
	int       spaces       = -1;
	unsigned  c            = 0;
	float     width        = 0.0f;
	float     h            = 0.0f;
	float     lastw        = 0.0f;
	float     justifyWidth = currentFont->_spacewidth;
	float     spacew       = currentFont->_spacewidth;
	float     fontHeight   = currentFont->height * currentFont->_interval;
	float     oldy         = y;
	bool      end          = FALSE;
	bool      yOutScreen;
	FontHash *hash         = (FontHash*)currentFont->hash;
	unsigned  prevChar     = 0;
	FontChar *fontPrevChar = NULL;
	if(NULL == hash)
		return;
	// if(maxw > 0.0f)
	// {
		// maxw = maxw / currentFont->_scale * screenScale.scaleX;
		// if(maxw == 0.0f) maxw = 0.0001f;
	// }
	FLUSH_BUFFER();


	x = h = y = 0.0;

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
			yOutScreen = (y + oldy + fontHeight) * currentFont->_scale >= 0.0f;
			/* drop kerning computation for invisible lines - speed +15% */
			if(yOutScreen)
				KERNING_CONDITION
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
								justifyWidth = (maxw + spacew * (spaces) - lastw) / (float)(spaces);
							x = 0;
							break;
							default: x = 0;
					}
					float kerningAccumulator = 0.0;
					while(buf < last_space)
					{
						UNICODE_TO_INT(str, buf)
						KERNING_CONDITION
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
				y += fontHeight;

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
					y += fontHeight;
					if((y + oldy) * currentFont->_scale > 1080)
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
		ptr->mem += sizeof(FontHash) + sizeof(Font) + sizeof(FontChar) * 2 + FontHash_size((FontHash*)ptr->hash) * sizeof(FontHashNode);
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
	ptr->mem += sizeof(FontChar);
	if(' ' == ch)
	{
		ptr->_spacewidth = w;
		fch = NULL;
		new0(fch, FontChar, 1);
		fch->w = w * 8;
		FontHash_set((FontHash*)ptr->hash, '\t', fch);
		ptr->mem += sizeof(FontChar);
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
		ptr->mem += sizeof(KernHash) + KernHash_size((KernHash*)ptr->kerningHash) * sizeof(KernHashNode);
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
}

int main(int argc, char **argv)
{
	Font *font;
	Image *img;
	bool kerning = FALSE;
	char line[128];
	unsigned char *text = NULL;
	size_t size = 0;
	clock_t begin, end;
	double time_spent;
	float width;
	if(argc != 3)
	{
		printf("Usage: %s font_file text_to_render\n", argv[0]);
		exit(1);
	}
	FILE *fontFile = fopen(argv[1], "r");
	if(NULL == fontFile)
	{
		puts("Cannot open font file!");
		exit(1);
	}
	FILE *file = fopen(argv[2], "rb");
	if(NULL == file)
	{
		puts("Cannot open text file!");
		exit(1);
	}
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);
	new(text, unsigned char, size);
	if (size != fread(text, sizeof(unsigned char), size, file)) {
		puts("Cannot read text file!");
		exit(1);
	}
	new(img, Image, 1);
	img->h = img->w = 512;
	new(font, Font, 1);
	font->image = img;
	font->_scale = 1.0f;
	font->_interval = 1.0f;
	font->_kerning = FALSE;
	while(fgets(line, sizeof(line), fontFile))
	{
		if(line[0] >= '0' && line[0] <= '9')
			fontSetGlyph(font, line);
	}
	begin = clock();
	width = 1000;
	fontPrintf(font, text, 0, 0, width, alignJustify);
	fontPrintf(font, text, 0, 0, width, alignCenter);
	fontPrintf(font, text, 0, 0, width, alignRight);
	fontPrintf(font, text, 0, 0, width, alignLeft);
	width = 100;
	fontPrintf(font, text, 0, 0, width, alignJustify);
	fontPrintf(font, text, 0, 0, width, alignCenter);
	fontPrintf(font, text, 0, 0, width, alignRight);
	fontPrintf(font, text, 0, 0, width, alignLeft);
	width = 0;
	fontPrintf(font, text, 0, 0, width, alignJustify);
	fontPrintf(font, text, 0, 0, width, alignCenter);
	fontPrintf(font, text, 0, 0, width, alignRight);
	fontPrintf(font, text, 0, 0, width, alignLeft);
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Time: %.4f\n", time_spent);
}
