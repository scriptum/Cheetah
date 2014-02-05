/*******************************************************************************

Copyright (c) 2012-2014 Pavel Roschin (aka RPG) <rpg89@post.ru>

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
#include "cdebug.h"
#include "test.h"

SDL_Event event;

void recomputeScreenScale(float w, float h);
void setWindowSize(unsigned w, unsigned h);

unsigned int getEventType(void) {
	/* skip unneeded events */
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				dbg("Quit");
				return EVENT_QUIT;
			case SDL_KEYUP:
				dbgv("Key up");
				return EVENT_KEYUP;
			case SDL_KEYDOWN:
				dbgv("Key down");
				return EVENT_KEYDOWN;
			case SDL_MOUSEBUTTONUP:
				dbgv("Mouse up");
				return EVENT_MOUSEUP;
			case SDL_MOUSEBUTTONDOWN:
				dbgv("Mouse down");
				return EVENT_MOUSEDOWN;
			case SDL_VIDEORESIZE:
				dbgv("Resize");
				if(event.resize.w < 1)
					event.resize.w = 1;
				if(event.resize.h < 1)
					event.resize.h = 1;
				recomputeScreenScale((float)event.resize.w, (float)event.resize.h);
				globalTimers.rescaleTime = globalTimers.time + globalTimers.resizeDelay;
				setWindowSize((unsigned)event.resize.w, (unsigned)event.resize.h);
				return EVENT_RESIZE;
			/* TODO to do something here */
			case SDL_VIDEOEXPOSE:
				dbgv("Expose");
				return EVENT_EXPOSE;
			case SDL_ACTIVEEVENT:
				dbgv("Active");
				return EVENT_ACTIVE;
			case SDL_JOYAXISMOTION:
			case SDL_JOYBALLMOTION:
			case SDL_JOYHATMOTION:
			case SDL_JOYBUTTONDOWN:
			case SDL_JOYBUTTONUP:
				dbgv("Joystick");
				return EVENT_JOY;
			default:
				dbgv("Some event #%d", event.type);
		}
	}
	return 0;
}

unsigned int getEventKey(void) {
	return event.key.keysym.sym;
}

unsigned int getEventKeyUnicode(void) {
	return event.key.keysym.unicode;
}

int getEventMouseX(void) {
	if(!screenScale.autoScale)
		return event.button.x;
	return (int)(((float)event.button.x - screenScale.offsetX) / screenScale.scaleX);
}

int getEventMouseY(void) {
	if(!screenScale.autoScale)
		return event.button.y;
	return (int)(((float)event.button.y - screenScale.offsetY) / screenScale.scaleY);
}

unsigned int getEventMouseButton(void) {
	return event.button.button;
}

int getEventResizeW(void) {
	return event.resize.w;
}

int getEventResizeH(void) {
	return event.resize.h;
}

void setResizeDelay(unsigned delay) {
	globalTimers.resizeDelay = delay;
}

int getMouseX(void) {
	int x;
	SDL_GetMouseState(&x, NULL);
	if(!screenScale.autoScale)
		return x;
	return (int)(((float)x - screenScale.offsetX) / screenScale.scaleX);
}

int getMouseY(void) {
	int y;
	SDL_GetMouseState(NULL, &y);
	if(!screenScale.autoScale)
		return y;
	return (int)(((float)y - screenScale.offsetY) / screenScale.scaleY);
}

unsigned char *getKeyState(void) {
	return SDL_GetKeyState(NULL);
}

void gameSpeed(double speed) {
	globalTimers.gameSpeed = speed;
}
