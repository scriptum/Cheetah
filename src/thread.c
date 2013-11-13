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
#include <lualib.h>
#include <lauxlib.h>

#include "cheetah.h"
#include "cmacros.h"
#include "test.h"
#include "clist.h"
#include "chashfunc.h"
#include "chashtable.h"

/* Get the number of milliseconds past from the execution time. Equivalent to SDL_GetTicks(); */
unsigned int getTicks(void) {
	return SDL_GetTicks();
}

/* Get the time in seconds past from the execution time. This time may change its speed! */
double getGameTime(void) {
	return globalTimers.gameTimed;
}

/* Get the time in seconds past from the execution time. Returns system's (without game speed). */
double getTime(void) {
	return globalTimers.timed;
}

/* Do nothing some time. */
void delay(unsigned int ms) {
	SDL_Delay(ms);
}

/* Do nothing some time. */
void sleep(double sec) {
	SDL_Delay((unsigned)(sec * 1000.));
}

/*================================threads=====================================*/
typedef struct Thread {
	const char *file;
	lua_State  *L;
} Thread;

static inline bool threadCmpFunc(const char *a, const char *b)
{
	return strcmp(a, b) == 0;
}
// 
// typedef struct threadMessage {
	// const char *message;
	// lua_State  *L; 
// } threadMessage;

HASH_TEMPLATE(threadHash, const char *, listDouble *, jenkins_one_at_a_time_hash, threadCmpFunc)

threadHash *threadArray = NULL;
SDL_mutex *threadMutex = NULL;

int Lua_Thread_create(void *data) 
{
	// Thread *t = (Thread*)data;
	lua_State *L;
	if(unlikely(NULL == data))
		return -1;
	L = luaL_newstate();
	luaL_openlibs(L);
	int n = lua_gettop(L);
	if(unlikely(luaL_loadfile(L, (const char*)data) != 0))
	{
		dprintf_thread("cannot load lua file: %s\n", (const char*)data);
		return lua_error(L);
	}
	dprintf_thread("created thread: %s (%p)\n", (const char*)data, L);
	lua_call(L, 0, LUA_MULTRET);
	lua_pop(L, lua_gettop(L) - n);
	lua_close(L);
	dprintf_thread("closed thread: %s (%p)\n", (const char*)data, L);
	return 0;
}

bool newThread(const char *file) {
	// Thread t;
	// t.file = file;
	SDL_Thread *t;
	if(unlikely(NULL == file))
		return FALSE;
	if(unlikely(NULL == threadMutex))
	{
		threadMutex = SDL_CreateMutex();
		if(unlikely(NULL == threadMutex))
		{
			dprintf_thread("failed to create new thread mutex\n");
			return FALSE;
		}
		else
		{
			dprintf_thread("created new thread mutex\n");
		}
	}
	t = SDL_CreateThread(Lua_Thread_create, (void*)file);
	if(unlikely(NULL == t))
	{
		dprintf_thread("failed created new thread!\n");
		return FALSE;
	}
	return TRUE;
}

void threadMutexLock(void) {
	dprintf_threadvv("lock mutex\n");
	SDL_mutexP(threadMutex);
}

void threadMutexUnlock(void) {
	dprintf_threadvv("unlock mutex\n");
	SDL_mutexV(threadMutex);
}

// void threadSend(listDouble *messageItem) {
	// threadMutexLock();
	// listPush(threadMessages, messageItem);
	// threadMutexUnlock();
// }

void threadSendStr(const char *message, const char *queue) {
	listDouble *messageItem = NULL;
	listDouble *threadMessages = NULL;
	if(unlikely(NULL == message || NULL == queue))
		return;
	threadMutexLock();
	if(unlikely(NULL == threadArray))
	{
		threadArray = threadHash_new();
		dprintf_thread("created new thread hashtable\n");
	}
	threadMessages = threadHash_get(threadArray, queue);
	if(unlikely(NULL == threadMessages))
	{
		new1(threadMessages, listDouble);
		threadHash_set(threadArray, queue, threadMessages);
		dprintf_thread("created new thread queue: %s\n", queue);
	}
	new1(messageItem, listDouble);
	messageItem->data = (void *)(message);
	listPush(threadMessages, messageItem);
	dprintf_threadv("sent message to queue: %s\n", queue);
	dprintf_threadvv("message: %s\n", message);
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

const char *threadRecvStr(const char *queue) {
	const char *message = NULL;
	listDouble *messageItem;
	listDouble *threadMessages = NULL;
	if(unlikely(NULL == queue))
	{
		dprintf_thread("empty queue, assign \"main\"\n");
		queue = "main";
	}
	dprintf_threadvv("waiting for queue: %s\n", queue);
	threadMutexLock();
	if(unlikely(NULL == threadArray))
	{
		dprintf_thread("hashtable isn't initialized yet, exitting (race condition)\n");
		threadMutexUnlock();
		return NULL;
	}
	threadMessages = threadHash_get(threadArray, queue);
	if(unlikely(NULL == threadMessages))
	{
		dprintf_thread("message queue '%s' not found in hashtable, exitting\n", queue);
		threadMutexUnlock();
		return NULL;
	}
	messageItem = listPop(threadMessages);
	if(unlikely(NULL != messageItem))
	{
		message = (const char*)messageItem->data;
		delete(messageItem);
		dprintf_threadvv("try to recieve message from queue: %s\n", queue);
		if(message)
		{
			dprintf_threadv("recieved message from queue: %s\n", queue);
			dprintf_threadvv("message contents: %s\n", message);
		}
	}
	else
	{
		dprintf_threadv("message queue '%s' empty, exitting\n", queue);
	}
	threadMutexUnlock();
	return message;
}
