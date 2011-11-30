#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <SDL.h>
#include <SDL_opengl.h>
#ifndef __CHEETAH_H__
#define __CHEETAH_H__

typedef unsigned char bool;
int barfunc(int foo);
//~ typedef struct color {
	//~ GLubyte r, g, b, a;
//~ } color;
//~ 
//~ typedef struct entity {
	//~ float x, y;                //(8b) x and y position of entity
	//~ color col;                 //(4b) col - color(rgba)
	//~ void (*draw)(entity *);    //(4b) draw function
	//~ void *ptr;                 //(4b) void pointer depends on entity type
	//~ struct entity *prev;       //(4b) previous entity (needed for tree)
	//~ struct entity *next;       //(4b) next entity (needed for processEntities)
	//~ struct entity *child;      //(4b) children entity (needed for processEntities and tree)
	//~ char visible;              //(1b) to draw or not to draw
//~ } entity;
//~ 
//~ entity * screen;
//~ 
//~ entity * newEntity(entity*);
//~ 
//~ int init(const char*, int, int, char, char, char, char);
//~ 
//~ void loop();

//~ void error(char *fmt, ...)
//~ {
    //~ va_list args;
    //~ va_start(args, fmt);
    //~ fprintf(stderr, "error: ");
    //~ vfprintf(stderr, fmt, args);
    //~ fprintf(stderr, "\n");
    //~ va_end(args);
    //~ exit(1);
//~ }
#endif //__CHEETAH_H__