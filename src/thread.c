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

#include <string.h>
#include <stddef.h>
#include <SDL.h>
#include <lualib.h>
#include <lauxlib.h>

#include "cheetah.h"
#include "cmacros.h"
#include "test.h"
#include "clist.h"
#include "chashfunc.h"
#include "chashtable.h"

/* Get the number of milliseconds past from the execution time. Equivalent to SDL_GetTicks(); */
CHEETAH_EXPORT unsigned int getTicks(void)
{
	return SDL_GetTicks();
}

/* Get the time in seconds past from the execution time. This time may change its speed! */
CHEETAH_EXPORT double getGameTime(void)
{
	return globalTimers.gameTimed;
}

/* Get the time in seconds past from the execution time. Returns system's (without game speed). */
CHEETAH_EXPORT double getTime(void)
{
	return globalTimers.timed;
}

/* Do nothing some time. */
CHEETAH_EXPORT void delay(unsigned int ms)
{
	SDL_Delay(ms);
}

/* Do nothing some time. */
CHEETAH_EXPORT void sleep(double sec)
{
	SDL_Delay((unsigned)(sec * 1000.));
}

/*================================threads=====================================*/
typedef struct Thread
{
	const char *file;
	lua_State  *L;
} Thread;

//
// typedef struct threadMessage {
// const char *message;
// lua_State  *L;
// } threadMessage;

HASH_TEMPLATE(threadHash, const char *, listDouble *, hash_string, cmp_string)

threadHash *threadArray = NULL;
SDL_mutex *threadMutex = NULL;

int Lua_Thread_create(void *data)
{
	// Thread *t = (Thread*)data;
	lua_State *L;
	if(unlikely(NULL == data))
	{
		return -1;
	}
	L = luaL_newstate();
	luaL_openlibs(L);
	int n = lua_gettop(L);
	if(unlikely(luaL_loadfile(L, (const char *)data) != 0))
	{
		dbg("cannot load lua file: %s", (const char *)data);
		return lua_error(L);
	}
	dbg("created thread: %s (%p)", (const char *)data, L);
	lua_call(L, 0, LUA_MULTRET);
	lua_pop(L, lua_gettop(L) - n);
	lua_close(L);
	dbg("closed thread: %s (%p)", (const char *)data, L);
	return 0;
}

CHEETAH_EXPORT bool newThread(const char *file)
{
	// Thread t;
	// t.file = file;
	SDL_Thread *t;
	if(unlikely(NULL == file))
	{
		return FALSE;
	}
	if(unlikely(NULL == threadMutex))
	{
		threadMutex = SDL_CreateMutex();
		if(unlikely(NULL == threadMutex))
		{
			dbg("failed to create new thread mutex");
			return FALSE;
		}
		else
		{
			dbg("created new thread mutex");
		}
	}
	t = SDL_CreateThread(Lua_Thread_create, NULL, (void *)file);
	if(unlikely(NULL == t))
	{
		dbg("failed created new thread!");
		return FALSE;
	}
	return TRUE;
}

CHEETAH_EXPORT void threadMutexLock(void)
{
	dbgvv("lock mutex");
	SDL_mutexP(threadMutex);
}

CHEETAH_EXPORT void threadMutexUnlock(void)
{
	dbgvv("unlock mutex");
	SDL_mutexV(threadMutex);
}

// void threadSend(listDouble *messageItem) {
// threadMutexLock();
// listPush(threadMessages, messageItem);
// threadMutexUnlock();
// }

CHEETAH_EXPORT void threadSendStr(const char *message, const char *queue)
{
	listDouble *messageItem = NULL;
	listDouble *threadMessages = NULL;
	if(unlikely(NULL == message || NULL == queue))
	{
		return;
	}
	threadMutexLock();
	if(unlikely(NULL == threadArray))
	{
		threadArray = threadHash_new();
		dbg("created new thread hashtable");
	}
	threadMessages = threadHash_get(threadArray, queue);
	if(unlikely(NULL == threadMessages))
	{
		new1(threadMessages, listDouble);
		threadHash_set(threadArray, queue, threadMessages);
		dbg("created new thread queue: %s", queue);
	}
	new1(messageItem, listDouble);
	messageItem->data = (void *)(message);
	listPush(threadMessages, messageItem);
	dbgv("sent message to queue: %s", queue);
	dbgvv("message: %s", message);
	threadMutexUnlock();
}

// const char *threadRecv(void) {
// const char *message = NULL;
// listDouble *messageItem;
// if(NULL == threadMessages)
// {
// return NULL;
// }
// threadMutexLock();
// messageItem = listPop(threadMessages);
// threadMutexUnlock();
// if(messageItem)
// {
// message = (const char*)messageItem->data;
// }
// return message;
// }

CHEETAH_EXPORT const char *threadRecvStr(const char *queue)
{
	const char *message = NULL;
	listDouble *messageItem;
	listDouble *threadMessages = NULL;
	if(unlikely(NULL == queue))
	{
		dbg("empty queue, assign \"main\"");
		queue = "main";
	}
	dbgvv("waiting for queue: %s", queue);
	threadMutexLock();
	if(unlikely(NULL == threadArray))
	{
		dbg("hashtable isn't initialized yet, exitting (race condition)");
		threadMutexUnlock();
		return NULL;
	}
	threadMessages = threadHash_get(threadArray, queue);
	if(unlikely(NULL == threadMessages))
	{
		dbg("message queue '%s' not found in hashtable, exitting", queue);
		threadMutexUnlock();
		return NULL;
	}
	messageItem = listPop(threadMessages);
	if(unlikely(NULL != messageItem))
	{
		message = (const char *)messageItem->data;
		delete(messageItem);
		dbgvv("try to recieve message from queue: %s", queue);
		if(message)
		{
			dbgv("recieved message from queue: %s", queue);
			dbgvv("message contents: %s", message);
		}
	}
	else
	{
		dbgv("message queue '%s' empty, exitting", queue);
	}
	threadMutexUnlock();
	return message;
}
