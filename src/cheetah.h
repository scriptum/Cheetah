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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <search.h>


#ifdef __ANDROID_API__
#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <SDL.h>
#include <SDL_opengl.h>
#endif

#include "macros.h"

#ifndef __CHEETAH_H__
#define __CHEETAH_H__

typedef unsigned char bool;
unsigned char * loadfile(const char * filename, unsigned int * length);

extern SDL_Surface *screen;

void myError(const char *fmt, ...);


#if 0
typedef struct SDL_Rect {
	short x, y;
	unsigned short w, h;
} SDL_Rect;


struct {
	unsigned int flags;				/**< Read-only */
	void *format;		/**< Read-only */
	int w, h;				/**< Read-only */
	unsigned short pitch;				/**< Read-only */
	void *pixels;				/**< Read-write */
	int offset;				/**< Private */

	/** Hardware-specific surface info */
	struct private_hwdata *hwdata;

	/** clipping information */
	SDL_Rect clip_rect;			/**< Read-only */
	unsigned int unused1;				/**< for binary compatibility */

	/** Allow recursive locks */
	unsigned int locked;				/**< Private */

	/** info for fast blit mapping to other surfaces */
	struct SDL_BlitMap *map;		/**< Private */

	/** format version, bumped at every change to invalidate blit maps */
	unsigned int format_version;		/**< Private */

	/** Reference count -- used when freeing surface */
	int refcount;				/**< Read-mostly */
} screen;

typedef struct __dirstream {
} DIR;

#endif

struct {
	char GLSL, BE, FBO, VBO, MT, PS;
} supported;

/*=================================images=====================================*/
typedef struct Image {
	char *name;
	char *options;
	/* OpenGL texture id */
	unsigned int id;
	/* width and height of the original image */
	float w, h;
	int channels;
	int queued;
} Image;

typedef struct ImageData {
	int w, h, channels;
	char *data;
} ImageData;

/*==============================framebuffers==================================*/
typedef struct Framebuffer {
	unsigned int id;
	Image *image;
} Framebuffer;

/*=================================fonts======================================*/
typedef struct FontChar
{
	/* OpenGL list id */
	unsigned int vertex;
	/* Width of char */
	float w;
	float v[4], t[4];
} FontChar;

typedef struct Font {
	Image *image;
	float _scale, height, _interval, spacew;
	FontChar ***chars;
	int allocated;
	int mem;
	bool scalable, unicode;
} Font;


/*================================shaders=====================================*/
typedef struct Shader {
	unsigned int id;
} Shader;

/*================================tilemap=====================================*/
typedef struct _Tilemap {
	int w, h;            // size in tiles
	int tw, th;          // single tile size
	float index[256][4]; // texture coords index
	unsigned char **map; // tile indexes map
	int scalable;        // should tilemap be scaled to screen size or drawed per-pixel
	Image *img;
} Tilemap;

/*==================================VBO=======================================*/
typedef struct Point {
	float x, y;
} Point;

typedef struct Point3 {
	float x, y, z;
} Point3;

typedef struct Vbo {
	unsigned int id, count, tex;
	Point *data;
} Vbo;

enum {
	blend_alpha,
	blend_additive,
	blend_multiplicative,
	blend_screen,
	blend_detail,
	blend_substractive,
	blend_difference,
	blend_mask,
	blendAlpha = 0,
	blendAdditive,
	blendMultiplicative,
	blendScreen,
	blendDetail,
	blendSubstractive,
	blendDifference,
	blendMask,
};

enum {
	alignLeft = 1,
	alignCenter,
	alignRight,
	alignJustify,
	align_left = 1,
	align_center,
	align_right,
	align_justify
};

struct {
	double scaleX, scaleY, offsetX, offsetY;
	/*оригинальные ширина и высота, относительно которых считаются все координаты*/
	double origWidth, origHeight;
	double aspect;
	bool autoScale, autoScaleFont;
} screenScale;

typedef struct Resource {
	Image *image;
	unsigned char *data;
	char *name;
	char *options;
	int len;
} Resource;

/*queue*/
SDL_mutex *resQueueMutex;
#define QDATA Resource
	typedef struct node_t node_t, *node, *queue;
	struct node_t {QDATA val; node prev, next;};
#define QHEAD(q) q->prev
#define QTAIL(q) q->next
#define QEMPTY(q) !QHEAD(q)
inline queue newQueue();
inline void enqueue(queue q, QDATA n);
inline int dequeue(queue q, QDATA *val);
queue resLoaderQueue;

Resource *resShared;

int resLoaderThread(void *unused);
inline unsigned char * loadImageData(const char *name, int *width, int *height, int *channels);
inline unsigned int loadImageTex(const char *options, unsigned char *img, int width, int height, int channels);

inline void imageBind(Image * image);
void initRenderer();

SDL_Event event;
extern Uint32 rescaleTime;
extern Uint32 globalTime;
extern int resizeDelay;
#endif //__CHEETAH_H__