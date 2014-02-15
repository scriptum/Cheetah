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

CHEETAH_EXPORT unsigned int getEventType(void)
{
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
		case SDL_JOYAXISMOTION:
		case SDL_JOYBALLMOTION:
		case SDL_JOYHATMOTION:
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
			dbgv("Joystick");
			return EVENT_JOY;
		case SDL_WINDOWEVENT:
			switch(event.window.event)
			{
				int w, h;
			case SDL_WINDOWEVENT_RESIZED:
				dbgv("Resize");
				w = event.window.data1;
				h = event.window.data2;
				if(w < 1)
				{
					w = 1;
				}
				if(h < 1)
				{
					h = 1;
				}
				recomputeScreenScale((float)w, (float)h);
				globalTimers.rescaleTime = globalTimers.time + globalTimers.resizeDelay;
				setWindowSize(w, h);
				return EVENT_RESIZED;
			case SDL_WINDOWEVENT_EXPOSED:
				dbgv("Window expose");
				return EVENT_EXPOSED;
			case SDL_WINDOWEVENT_SHOWN:
				dbgv("Window shown");
				return EVENT_SHOWN;
			case SDL_WINDOWEVENT_HIDDEN:
				dbgv("Window hidden");
				return EVENT_HIDDEN;
			case SDL_WINDOWEVENT_MOVED:
				dbgv("Window moved");
				return EVENT_MOVED;
			case SDL_WINDOWEVENT_MINIMIZED:
				dbgv("Window minimized");
				return EVENT_MINIMIZED;
			case SDL_WINDOWEVENT_MAXIMIZED:
				dbgv("Window maximized");
				return EVENT_MAXIMIZED;
			case SDL_WINDOWEVENT_RESTORED:
				dbgv("Window restored");
				return EVENT_RESTORED;
			case SDL_WINDOWEVENT_ENTER:
				dbgv("Window enter");
				return EVENT_ENTER;
			case SDL_WINDOWEVENT_LEAVE:
				dbgv("Window leave");
				return EVENT_LEAVE;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				dbgv("Window focus gained");
				return EVENT_FOCUS_GAINED;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				dbgv("Window focus lost");
				return EVENT_FOCUS_LOST;
			case SDL_WINDOWEVENT_CLOSE:
				dbgv("Window closed");
				return EVENT_CLOSE;
			}
			break;
		default:
			dbgv("Some event #%d", event.type);
		}
	}
	return 0;
}

CHEETAH_EXPORT unsigned int getEventKey(void)
{
	return event.key.keysym.scancode;
}

/*
CHEETAH_EXPORT unsigned int getEventKeyUnicode(void)
{
	return event.key.keysym.unicode;
}
*/
CHEETAH_EXPORT int getEventMouseX(void)
{
	if(!screenScale.autoScale)
	{
		return event.button.x;
	}
	return (int)(((float)event.button.x - screenScale.offsetX) / screenScale.scaleX);
}

CHEETAH_EXPORT int getEventMouseY(void)
{
	if(!screenScale.autoScale)
	{
		return event.button.y;
	}
	return (int)(((float)event.button.y - screenScale.offsetY) / screenScale.scaleY);
}

CHEETAH_EXPORT unsigned int getEventMouseButton(void)
{
	return event.button.button;
}

CHEETAH_EXPORT int getEventResizeW(void)
{
	return event.window.data1;
}

CHEETAH_EXPORT int getEventResizeH(void)
{
	return event.window.data2;
}

CHEETAH_EXPORT void setResizeDelay(unsigned delay)
{
	globalTimers.resizeDelay = delay;
}

CHEETAH_EXPORT int getMouseX(void)
{
	int x;
	SDL_GetMouseState(&x, NULL);
	if(!screenScale.autoScale)
	{
		return x;
	}
	return (int)(((float)x - screenScale.offsetX) / screenScale.scaleX);
}

CHEETAH_EXPORT int getMouseY(void)
{
	int y;
	SDL_GetMouseState(NULL, &y);
	if(!screenScale.autoScale)
	{
		return y;
	}
	return (int)(((float)y - screenScale.offsetY) / screenScale.scaleY);
}

CHEETAH_EXPORT const unsigned char *getKeyState(void)
{
	return SDL_GetKeyboardState(NULL);
}

CHEETAH_EXPORT void gameSpeed(double speed)
{
	globalTimers.gameSpeed = speed;
}
