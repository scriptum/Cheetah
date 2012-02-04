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