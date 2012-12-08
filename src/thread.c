#include "cheetah.h"



/**
 * @descr Gets the number of milliseconds past from the execution time. Equivalent to SDL_GetTicks();
 * @group graphics/timer
 * */
unsigned int getTicks() {
	return SDL_GetTicks();
}

/**
 * @descr Gets the time in seconds past from the execution time. This function returns the time in view of game speed!
 * @group graphics/timer
 * */
double getTime() {
	return globalTimed;
}

/**
 * @descr Gets the time in seconds past from the execution time. Returns "pure" time (no game speed).
 * @group graphics/timer
 * */
double getRealTime() {
	return SDL_GetTicks() / 1000.0;
}

/**
 * @descr Do nothing some time.
 * @group graphics/timer
 * @var delay in milliseconds (1/1000 s)
 * @see sleep
 * */
void delay(unsigned int ms) {
	return SDL_Delay(ms);
}

/**
 * @descr Do nothing some time.
 * @group graphics/timer
 * @var delay in seconds
 * @see delay
 * */
void sleep(unsigned int sec) {
	return SDL_Delay(sec*1000);
}