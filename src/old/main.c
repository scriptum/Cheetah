#include "cheetah.h"
//~ void* my_callback_param;
//~ int FPS = 0;
//~ Uint32 my_callbackfunc ( Uint32 interval, void *param )
//~ {
	//~ printf ( "FPS: %d\n", FPS );
	//~ FPS = 0;
	//~ return interval;
//~ }
//~ 
//~ char done;
//~ 
//~ void main()
//~ {
	//~ init("Test", 800, 600, 24, 0, 0, 0);
	//~ done = 0;
	//~ screen = (entity *)malloc(sizeof(entity));
	//~ screen->visible = 1;
	//~ screen->next = screen->child = 0;
	//~ screen->prev = screen;
	//~ entity * a = newEntity(screen);
	//~ a->x = 10;
	//~ a->y = 10;
	//~ entity * b = newEntity(screen);
	//~ b->x = 20;
	//~ b->y = 10;
	//~ int i;
	//~ for(i = 0; i < 10000000; i++) newEntity(screen);
	//~ SDL_TimerID my_timer_id = SDL_AddTimer ( 1000, my_callbackfunc, my_callback_param );
	//~ //while(!done)
	//~ {
		//~ //glClear(GL_COLOR_BUFFER_BIT);
		//~ processEntities(screen);
		//~ SDL_GL_SwapBuffers();
		//~ FPS++;
	//~ }
//~ }
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include <stdio.h>

int barfunc(int foo)
{
    /* a dummy function to test with FFI */ 
    return foo + 1;
}

int main(void)
{
    int status, result;
    lua_State *L;
    L = luaL_newstate();

    luaL_openlibs(L);

    /* Load the file containing the script we are going to run */
    status = luaL_loadfile(L, "main.lua");
    if (status) {
        fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
        exit(1);
    }

    /* Ask Lua to run our little script */
    result = lua_pcall(L, 0, LUA_MULTRET, 0);
    if (result) {
        fprintf(stderr, "Failed to run script: %s\n", lua_tostring(L, -1));
        exit(1);
    }

    lua_close(L);   /* Cya, Lua */
barfunc(1);
    return 0;
}