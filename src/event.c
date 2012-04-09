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

Uint32 rescaleTime = 0;
Uint32 globalTime = 0;
int resizeDelay = 100;

unsigned int getEventType() {
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
				rescaleTime = globalTime + resizeDelay;
				screen->w = event.resize.w;
				screen->h = event.resize.h;
				//~ glClear(GL_COLOR_BUFFER_BIT);
				//~ SDL_UpdateRect(screen, 0,0,0,0);
				//~ SDL_GL_SwapBuffers();
				return 6;
			case SDL_VIDEOEXPOSE: return 7;
			case SDL_ACTIVEEVENT: return 8;
			case SDL_JOYAXISMOTION:
			case SDL_JOYBALLMOTION:
			case SDL_JOYHATMOTION:
			case SDL_JOYBUTTONDOWN:
			case SDL_JOYBUTTONUP: return 9;
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

void setResizeDelay(int delay) {
	resizeDelay = delay;
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