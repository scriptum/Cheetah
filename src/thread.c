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

#include <string.h>
#include <stddef.h>
#include <SDL.h>
#include <lua.h>

#include "cheetah.h"
#include "cmacros.h"
#include "test.h"

/* Get the number of milliseconds past from the execution time. Equivalent to SDL_GetTicks(); */
unsigned int getTicks() {
	return SDL_GetTicks();
}

/* Get the time in seconds past from the execution time. This time may change its speed! */
double getGameTime() {
	return globalTimers.gameTimed;
}

/* Get the time in seconds past from the execution time. Returns system's (without game speed). */
double getTime() {
	return globalTimers.timed;
}

/* Do nothing some time. */
void delay(unsigned int ms) {
	SDL_Delay(ms);
}

/* Do nothing some time. */
void sleep(unsigned int sec) {
	SDL_Delay(sec * 1000);
}

/*================================threads=====================================*/
typedef struct Thread {
	const char *file;
	lua_State  *L;
} Thread;

int Lua_Thread_create(void *data) 
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	int n = lua_gettop(L);
	if (luaL_loadfile(L, (const char*)data) != 0) {
		return lua_error(L);
	}
	lua_call(L, 0, LUA_MULTRET);
	lua_pop(L, lua_gettop(L) - n);
	return 0;
}

void createThread(const char *file) {
	SDL_CreateThread(Lua_Thread_create, (void*)file);
}
