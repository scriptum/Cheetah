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

SDL_Event event;

unsigned int getEventType() {
	//В общем, тут идет дохрена ненужных нам событий (штук 300), которые могут тормозить игру, их нужно пропускать.
	while(SDL_PollEvent(&event)) {
		//printf("%d\n", event.type);
		switch(event.type) {
			case SDL_QUIT: return 1;
			case SDL_KEYDOWN: return 2;
			case SDL_KEYUP: return 3;
			case SDL_MOUSEBUTTONDOWN: return 4;
			case SDL_MOUSEBUTTONUP: return 5;
			case SDL_VIDEORESIZE: return 6;
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
	return event.button.x;
}

int getEventMouseY() {
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
	return x;
}

int getMouseY() {
	int y;
	SDL_GetMouseState(NULL, &y);
	return y;
}