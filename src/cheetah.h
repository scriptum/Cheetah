#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <SDL.h>
#include <SDL_opengl.h>
//~ #include "physfs.h"

#ifndef __CHEETAH_H__
#define __CHEETAH_H__

typedef unsigned char bool;
unsigned char * loadfile(const char * filename, unsigned int * length);
SDL_Surface *screen;

#define new(type) (type *)malloc(sizeof(type))

void myError(const char *fmt, ...);

#if 0
typedef struct SDL_Rect {
	short x, y;
	unsigned short w, h;
} SDL_Rect;

typedef struct __dirstream {
} DIR;

typedef struct dirent {
	unsigned int ino;
	unsigned int off;
	unsigned short int reclen;
	unsigned char type;
	char name[256];
} dirent;

//~ typedef struct lstat {
	//~ unsigned long int     st_dev;     /* ID of device containing file */
	//~ unsigned long int     st_ino;     /* inode number */
	//~ int    st_mode;    /* protection */
	   //~ st_nlink;   /* number of hard links */
	 //~ uid_t     st_uid;     /* user ID of owner */
	 //~ gid_t     st_gid;     /* group ID of owner */
	 //~ dev_t     st_rdev;    /* device ID (if special file) */
	 //~ off_t     st_size;    /* total size, in bytes */
	 //~ blksize_t st_blksize; /* blocksize for file system I/O */
	 //~ blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
	 //~ time_t    st_atime;   /* time of last access */
	 //~ time_t    st_mtime;   /* time of last modification */
	 //~ time_t    st_ctime;   /* time of last status change */
//~ } lstat;
#endif

struct {
	char GLSL, BE, FBO, VBO, MT, PS;
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

/*==================================VBO=======================================*/
typedef struct Point {
	float x, y;
} Point;

typedef struct Point3 {
	float x, y, z;
} Point3;

typedef struct Vbo {
	unsigned int id, count, tex;
	Point * data;
} Vbo;

enum {
	blend_alpha,
	blend_additive,
	blend_multiplicative,
	blend_screen,
	blend_detail,
	blend_substractive,
	blendAlpha = 0,
	blendAdditive,
	blendMultiplicative,
	blendScreen,
	blendDetail,
	blendSubstractive,
};

#endif //__CHEETAH_H__