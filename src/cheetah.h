#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <SDL.h>
//~ #include "physfs.h"

#ifndef __CHEETAH_H__
#define __CHEETAH_H__

typedef unsigned char bool;
unsigned char * loadfile(const char * filename, unsigned int * length);
SDL_Surface *screen;

#define new(type) (type *)malloc(sizeof(type))

void myError(char *fmt, ...);

#if 0
typedef struct SDL_Rect {
	short x, y;
	unsigned short w, h;
} SDL_Rect;
#endif

struct {
	char GLSL, BE, FBO, MT;
} supported;

/*=================================images=====================================*/
typedef struct Image {
	/* OpenGL texture id */
	unsigned int id;
	/* width and height of the original image */
	unsigned int w, h;
} Image;

/*==============================framebuffers==================================*/
typedef struct Framebuffer {
	unsigned int id;
	Image * image;
} Framebuffer;

/*=================================fonts======================================*/
typedef struct FontChar
{
	/* OpenGL list id */
	unsigned int vertex;
	/* Width of char */
	float w;
} FontChar;

typedef struct Font {
	Image * image;
	float scale, height;
	FontChar chars[256];
} Font;


/*================================shaders=====================================*/
typedef struct Shader {
	unsigned int id;
} Shader;





#endif //__CHEETAH_H__