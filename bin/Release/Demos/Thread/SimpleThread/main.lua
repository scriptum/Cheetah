require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
--enable windows resizing and disable autoscale to avoid font corruption
C.init('Test', '1024x700')
local ffi = require 'ffi'
ffi.cdef [[

	typedef struct lua_State lua_State;
	typedef double lua_Number;
	typedef ptrdiff_t lua_Integer;
	
	lua_State* luaL_newstate(void);
	void luaL_openlibs(lua_State *L);
	void lua_close (lua_State *L);
	void lua_call(lua_State *L, int nargs, int nresults);
	void lua_checkstack (lua_State *L, int sz);
	void lua_settop (lua_State *L, int index);
	int lua_type (lua_State *L, int index);
	
	void  lua_pushnil (lua_State *L);
	void  lua_pushnumber (lua_State *L, lua_Number n);
	void  lua_pushinteger (lua_State *L, lua_Integer n);
	void  lua_pushlstring (lua_State *L, const char *s, size_t l);
	void  lua_pushstring (lua_State *L, const char *s);
	void  lua_pushboolean (lua_State *L, int b);
	void  lua_pushlightuserdata (lua_State *L, void *p);
	
	void lua_gettable (lua_State *L, int idx);
	void lua_getfield (lua_State *L, int idx, const char *k);
	void lua_rawget (lua_State *L, int idx);
	void lua_rawgeti (lua_State *L, int idx, int n);
	lua_Integer lua_tointeger (lua_State *L, int index);
	const char *lua_tolstring (lua_State *L, int index, size_t *len);
]]
-- local L = ffi.C.luaL_newstate()
	-- if L == nil then
		-- error("Could not allocate new state",2)
	-- end
	-- t.state = L
	-- 
	-- ffi.C.luaL_openlibs(L)
	-- ffi.C.lua_settop(L,0)
cheetah.createThread("worker.lua")
cheetah.mainLoop()
