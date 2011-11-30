#include "cheetah.h"

SDL_Surface *SDLScreen;

int init(const char * appName, int width, int height, char bpp, char fullscreen, char resizable, char vsync)
{
	Uint32 flags;
	flags = SDL_OPENGL;
	
	if (fullscreen) {
		flags |= SDL_FULLSCREEN;
	} else if (resizable) {
		flags |= SDL_RESIZABLE;
	}
	if (SDLScreen == NULL) 
	{
		if ( SDL_Init ( SDL_INIT_VIDEO | SDL_INIT_AUDIO| SDL_INIT_TIMER ) != 0 )
		{
			fprintf(stderr, "Couldn't initialize SDL: %s", SDL_GetError ());
			return 0;
		}
		atexit(SDL_Quit);
	}
	/* set window caption */
	SDL_WM_SetCaption (appName, appName);
	/* enable double buffering */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, vsync);
	SDLScreen = SDL_SetVideoMode(width, height, bpp, flags);
	if (SDLScreen == NULL)
	{
		fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s",
								width, height, bpp, SDL_GetError());
		return 0;
	}
	/* set the OpenGL state */
	/* set background color */
	glClearColor( 0, 0, 0, 1);
	/* set line antialiasing */
	glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
	/* enable blending */
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	/* enable backface culling */
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDisable(GL_DEPTH_TEST);

	glViewport( 0, 0, width, height );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, width, height, 0, -1, 1 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	//~ if(!quadlist)
	//~ {
		//~ quadlist = glGenLists(1);
		//~ glNewList(quadlist, GL_COMPILE);
		//~ glBegin(GL_QUADS);
		//~ glTexCoord2f(0, 0); glVertex2f(0, 0);
		//~ glTexCoord2f(0, 1); glVertex2f(0, 1);
		//~ glTexCoord2f(1, 1); glVertex2f(1, 1);
		//~ glTexCoord2f(1, 0); glVertex2f(1, 0);
		//~ glEnd();
		//~ glEndList();
	//~ }
	return 1;
}