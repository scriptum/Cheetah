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

#include "cheetah.h"
#include "math.h"

SDL_Event event;

unsigned int getEventType() {
	Resource * r;
	if(resShared) {
		r = resShared;
		//~ printf("Get image %s %d\n", r->image->name, r->image->id);
		r->image->id = loadImageTex(r->image->options, r->data, r->image->w, r->image->h, r->image->channels);
		printf("Get image %s  %d\n", r->image->name, r->image->id);
		delete(r->image->name);
		delete(r->image->options);
		resShared = 0;
	}
	//В общем, тут идет дохрена ненужных нам событий (в основном mousemove), которые могут тормозить игру, их нужно пропускать.
	while(SDL_PollEvent(&event)) {
		//printf("%d\n", event.type);
		switch(event.type) {
			case SDL_QUIT: return 1;
			case SDL_KEYDOWN: return 2;
			case SDL_KEYUP: return 3;
			case SDL_MOUSEBUTTONDOWN: return 4;
			case SDL_MOUSEBUTTONUP: return 5;
			case SDL_VIDEORESIZE: 
				screenScale.aspect = (float)event.resize.w/(float)event.resize.h;
				if(screenScale.aspect > (float)4/3)
				{
					screenScale.scaleX = screenScale.scaleY = event.resize.h/screenScale.origHeight;
					screenScale.offsetX = floor((event.resize.w - screenScale.origWidth * screenScale.scaleX)*0.5);
					screenScale.offsetY = 0;
				}
				else
				{
					screenScale.scaleX = screenScale.scaleY = event.resize.w/screenScale.origWidth;
					screenScale.offsetY = floor((event.resize.h - screenScale.origHeight * screenScale.scaleY)*0.5);
					screenScale.offsetX = 0;
				}
				SDL_SetVideoMode(event.resize.w, event.resize.h, 32, screen->flags);
				glViewport( 0, 0, event.resize.w, event.resize.h );
				glMatrixMode( GL_PROJECTION );
				glLoadIdentity();
				glOrtho( 0, event.resize.w, event.resize.h, 0, -1000, 1000 );
				glMatrixMode( GL_MODELVIEW );
				glLoadIdentity();
				return 6;
			case SDL_VIDEOEXPOSE: return 7;
			case SDL_ACTIVEEVENT: return 8;
			case SDL_JOYAXISMOTION:
			case SDL_JOYBALLMOTION:
			case SDL_JOYHATMOTION:
			case SDL_JOYBUTTONDOWN:
			case SDL_JOYBUTTONUP: return 9;
			case SDL_USEREVENT: {
				//~ SDL_mutexP(resQueueMutex);
				
				//~ return 0;
			}
		}
	}
	return 0;
}

unsigned int getEventKey() {
	return event.key.keysym.sym;
}

unsigned int getEventKeyUnicode() {
	return event.key.keysym.unicode;
}

int getEventMouseX() {
	if(screenScale.autoScale)
		return (int)(((float)event.button.x - screenScale.offsetX) / screenScale.scaleX);
	return event.button.x;
}

int getEventMouseY() {
	if(screenScale.autoScale)
		return (int)(((float)event.button.y - screenScale.offsetY) / screenScale.scaleY);
	return event.button.y;
}

unsigned int getEventMouseButton() {
	return event.button.button;
}

unsigned int getEventResizeW() {
	return event.resize.w;
}

unsigned int getEventResizeH() {
	return event.resize.h;
}

int getMouseX() {
	int x;
	SDL_GetMouseState(&x, NULL);
	if(screenScale.autoScale)
		return (int)(((float)x - screenScale.offsetX) / screenScale.scaleX);
	return x;
}

int getMouseY() {
	int y;
	SDL_GetMouseState(NULL, &y);
	if(screenScale.autoScale)
		return (int)(((float)y - screenScale.offsetY) / screenScale.scaleY);
	return y;
}