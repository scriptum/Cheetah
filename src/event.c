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

#include <stddef.h>
#include <math.h>
#include <SDL.h>

#include "cheetah.h"
#include "debug.h"
#include "test.h"

SDL_Event event;

void recomputeScreenScale(float w, float h);
void setWindowSize(unsigned w, unsigned h);

enum {
	EVENT_QUIT = 1,
	EVENT_KEYDOWN,
	EVENT_KEYUP,
	EVENT_MOUSEBUTTONDOWN,
	EVENT_MOUSEBUTTONUP,
	EVENT_RESIZE,
	EVENT_EXPOSE,
	EVENT_ACTIVE,
	EVENT_JOY
};

unsigned int getEventType() {
	/* skip unneeded events */
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				dprintf_event("Quit\n");
				return EVENT_QUIT;
			case SDL_KEYDOWN:
				dprintf_event("Key down\n");
				return EVENT_KEYDOWN;
			case SDL_KEYUP:
				dprintf_event("Key up\n");
				return EVENT_KEYUP;
			case SDL_MOUSEBUTTONDOWN:
				dprintf_event("Mouse down\n");
				return EVENT_MOUSEBUTTONDOWN;
			case SDL_MOUSEBUTTONUP:
				dprintf_event("Mouse up\n");
				return EVENT_MOUSEBUTTONUP;
			case SDL_VIDEORESIZE:
				dprintf_event("Resize\n");
				if(event.resize.w < 1) event.resize.w = 1;
				if(event.resize.h < 1) event.resize.h = 1;
				recomputeScreenScale(event.resize.w, event.resize.h);
				globalTimers.rescaleTime = globalTimers.time + globalTimers.resizeDelay;
				setWindowSize(event.resize.w, event.resize.h);
				return EVENT_RESIZE;
			/* TODO to do something here */
			case SDL_VIDEOEXPOSE:
				dprintf_event("Expose\n");
				return EVENT_EXPOSE;
			case SDL_ACTIVEEVENT:
				dprintf_event("Active\n");
				return EVENT_ACTIVE;
			case SDL_JOYAXISMOTION:
			case SDL_JOYBALLMOTION:
			case SDL_JOYHATMOTION:
			case SDL_JOYBUTTONDOWN:
			case SDL_JOYBUTTONUP:
				dprintf_event("Joystick\n");
				return EVENT_JOY;
			// default:
				// dprintf_event("Event: Smth %d\n", event.type);
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
	globalTimers.resizeDelay = delay;
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

unsigned char *getKeyState() {
	return SDL_GetKeyState(NULL);
}

void gameSpeed(double speed) {
	globalTimers.gameSpeed = speed;
}
