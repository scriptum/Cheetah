#include "cheetah.h"

inline queue newQueue()
{
	node q = malloc(sizeof(node_t));
	q->next = q->prev = 0;
	return q;
}

inline void enqueue(queue q, QDATA n)
{
	//~ printf("%s\n", n.image->name);
	//~ SDL_mutexP(resQueueMutex);
	node nd = malloc(sizeof(node_t));
	nd->val = n;
	if (!QHEAD(q)) QHEAD(q) = nd;
	nd->prev = QTAIL(q);
	if (nd->prev) nd->prev->next = nd;
	QTAIL(q) = nd;
	nd->next = 0;
	//~ printf("%d\n", QEMPTY(resLoaderQueue));
	//~ SDL_mutexV(resQueueMutex);
}
 
inline int dequeue(queue q, QDATA *val)
{
	//~ SDL_mutexP(resQueueMutex);
	node tmp = QHEAD(q);
	if (!tmp) return 0;
	*val = tmp->val;
	//~ printf("%s\n", tmp->val.image->name);
	QHEAD(q) = tmp->next;
	if (QTAIL(q) == tmp) QTAIL(q) = 0;
	free(tmp);
	//~ SDL_mutexV(resQueueMutex);
	return 1;
}

int resLoaderThread(void *unused)
{
	Resource r;
	SDL_Event e;
	unsigned char *img;
	unsigned char *myBuf;
	bool empty;
	while(1){
		//~ SDL_mutexP(resQueueMutex);
		//~ empty = QEMPTY(resLoaderQueue)
		//~ SDL_mutexV(resQueueMutex);
		SDL_Delay(10);
		//~ if(QEMPTY(resLoaderQueue)) printf(": %d\n", QEMPTY(resLoaderQueue));
		if(!resShared&&!QEMPTY(resLoaderQueue))
		{
			//~ printf("Queue: %d\n", QEMPTY(resLoaderQueue));
			//~ SDL_mutexP(resQueueMutex);
			dequeue(resLoaderQueue, &r);
			img = loadImageData(r.image->name, &r.image->w, &r.image->h, &r.image->channels);
			r.data = img;
			//~ printf("%s\n", r.image->name);
			resShared = &r;
			//~ SDL_Delay(5);
			//~ new(e, SDL_Event, 1);
			//~ e.type = SDL_USEREVENT;
			//~ e.user.code = 0;
			//~ e.user.data1 = (void*)&r;
			//~ SDL_PushEvent(&e);
		}
	}
}


//~ #include "Main.h"
//~ 
//~ #include "Macros.h"
//~ #include "FileIO.h"
//~ #include "Font.h"
//~ #include "Graphics/Graphics.h"
//~ #include "Mouse.h"
//~ #include "Keyboard.h"
//~ #include "Sound.h"
//~ #include "Thread.h"
//~ 
//~ #define checkthread(L) \
	//~ (Lua_Thread *)luaL_checkudata(L, 1, "scrupp.thread")
//~ 
//~ lua_State *messages;
//~ SDL_mutex *mut;
//~ 
//~ int Lua_Thread_create(void *data) {
	//~ Lua_Thread *t = (Lua_Thread *)data;
	//~ int n = lua_gettop(t->L);
	//~ if (PHYSFS_exists(t->file)) {
		//~ if (FS_loadFile(t->L, t->file) == FILEIO_ERROR) {
			//~ return lua_error(t->L);
		//~ }
	//~ } else if (luaL_loadfile(t->L, t->file) != 0) {
		//~ return lua_error(t->L);
	//~ }
	//~ //printf("%d\n",lua_gettop(t->L));
	//~ lua_call(t->L, 0, LUA_MULTRET);
	//~ lua_pop(t->L, lua_gettop(t->L) - n);
	//~ return 0;
//~ }
//~ 
//~ static int Lua_Thread_new(lua_State *L) {
    //~ Lua_Thread *t;
    //~ const char* file = luaL_checkstring(L, 1);
    //~ lua_State *L1 = luaL_newstate();
    //~ luaL_openlibs(L1);
    //~ lua_newtable(L1);
	//~ luaopen_main(L1, NULL);
	//~ luaopen_fileio(L1, NULL);
	//~ luaopen_font(L1, NULL);
	//~ luaopen_graphics(L1, NULL);
	//~ luaopen_sound(L1, NULL);
	//~ luaopen_mouse(L1, NULL);
	//~ luaopen_keyboard(L1, NULL);
	//~ luaopen_thread(L1, NULL);
	//~ //luaopen_movie(L, NULL);
	//~ lua_setglobal(L1, NAMESPACE);
    //~ //lua_pop(L, 1);
    //~ t = lua_newuserdata(L, sizeof(Lua_Thread));
    //~ t->L = L1;
    //~ t->file = file;
    //~ t->thread = SDL_CreateThread(Lua_Thread_create, t);
    //~ luaL_getmetatable(L, "scrupp.thread");
    //~ lua_setmetatable(L, -2);
    //~ return 1;
//~ }
//~ 
//~ static int Lua_Thread_delay(lua_State *L) {
    //~ SDL_Delay(luaL_checkint(L, 1));
    //~ return 0;
//~ }
//~ 
//~ void Lua_Thread_type(lua_State *L, lua_State *L2, int index)
//~ {
  //~ int len;
  //~ const char * data;
    //~ switch(lua_type(L2, index)){
        //~ case LUA_TNUMBER:
            //~ lua_pushnumber(L, lua_tonumber(L2, index));
            //~ break;
        //~ case LUA_TBOOLEAN:
            //~ lua_pushboolean(L, lua_toboolean(L2, index));
            //~ break;
        //~ case LUA_TSTRING:
	    //~ data = luaL_checklstring(L2, index, &len);
            //~ lua_pushlstring(L, data, len);
            //~ break;
        //~ default:
            //~ lua_pushnil(L);
    //~ }
//~ }
//~ 
//~ static int Lua_Thread_send(lua_State *L) {
    //~ SDL_mutexP(mut);
    //~ lua_getglobal(messages, "send");
    //~ Lua_Thread_type(messages, L, 1);
    //~ lua_pushstring(messages, luaL_checkstring(L, 2));
    //~ lua_call(messages, 2, 0);
    //~ SDL_mutexV(mut);
	//~ return 0;
//~ }
//~ 
//~ static int Lua_Thread_recv(lua_State *L) {
    //~ SDL_mutexP(mut);
    //~ lua_getglobal(messages, "recv");
    //~ lua_pushstring(messages, luaL_checkstring(L, 1));
    //~ lua_call(messages, 1, 1);
    //~ Lua_Thread_type(L, messages, -1);
    //~ SDL_mutexV(mut);
	//~ return 1;
//~ }
//~ 
//~ static int thread_gc(lua_State *L) {
	//~ return 0;
//~ }
//~ 
//~ static int thread_tostring(lua_State *L) {
	//~ lua_pushliteral(L, "Thread");
	//~ return 1;
//~ }
//~ 
//~ static const struct luaL_Reg threadlib [] = {
	//~ {"newThread",				Lua_Thread_new},
	//~ {"delay",				    Lua_Thread_delay},
	//~ {"send",				    Lua_Thread_send},
	//~ {"recv",				    Lua_Thread_recv},
	//~ {NULL, NULL}
//~ };
//~ 
//~ static const struct luaL_Reg threadlib_m [] = {
	//~ {"__gc",				thread_gc},
	//~ {"kill",				thread_gc},
	//~ {"__tostring",			thread_tostring},
	//~ {NULL, NULL}
//~ };
//~ 
//~ void thread_init()
//~ {
	//~ int res;
    //~ messages = luaL_newstate();
    //~ luaL_openlibs(messages);
    //~ res = luaL_dostring (messages, "messages = {} \
//~ \
//~ function send(mess, key) \
	//~ if not messages[key] then messages[key] = {} end \
	//~ table.insert(messages[key], mess) \
//~ end \
//~ \
//~ function recv(key) \
	//~ local buf \
	//~ if messages[key] then \
		//~ buf = messages[key][1] \
		//~ if buf then \
			//~ table.remove(messages[key], 1) \
			//~ return buf \
		//~ end \
	//~ end \
	//~ return nil \
//~ end");
    //~ mut=SDL_CreateMutex();
//~ }
//~ 
//~ int luaopen_thread(lua_State *L, const char *parent) {
//~ 
	//~ luaL_newmetatable(L, "scrupp.thread");
	//~ /* metatable.__index = metatable */
	//~ lua_pushvalue(L, -1);	/* duplicates the metatable */
	//~ lua_setfield(L, -2, "__index");
	//~ luaL_register(L, NULL, threadlib_m);
	//~ lua_pop(L, 1);	/* pop the metatable */
	//~ luaL_register(L, parent, threadlib);
	//~ return 1;
//~ }
