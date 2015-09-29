#ifndef ED_LUA_OBJECT
#define ED_LUA_OBJECT

#include "Lua/Config.h"

namespace Lua 
{

inline void pushID(lua_State * L, int id)
{
	lua_pushnumber(L, id);
}

inline void pushID(lua_State * L, const char * id)
{
	lua_pushstring(L, id);
}

inline void pushID(lua_State * L, void * id)
{
	lua_pushlightuserdata(L, id);
}

inline bool getID(lua_State * L, int index, int & id)
{
	if(lua_isnumber(L, index))
	{
		id = lua_tonumber(L, index);
		return true;
	}
	else
		return false;
}

inline bool getID(lua_State * L, int index, unsigned int & id)
{
	return getID(L, index, (int&)id);
}

inline bool getID(lua_State * L, int index, ed::string & id)
{
	if(lua_isnumber(L, index))
	{
		id = lua_tostring(L, index);
		return true;
	}
	else
		return false;
}

inline bool getID(lua_State * L, int index, void * & id)
{
	if(lua_isuserdata(L, index))
	{
		id = lua_touserdata(L, index);
		return true;
	}
	else
		return false;
}

template<typename T>
void createLuaObject(lua_State * L, const char * objectClass, T id)
{
	lua_newtable(L);
	int index = lua_gettop(L);
	lua_pushstring(L, "id_");
	pushID(L, id);
	lua_settable(L, index);	
	Lua::Config config(L);
	if(config.open(objectClass))
		lua_setmetatable(L, -3);
}

template<typename T>
bool getLuaObject(lua_State * L, int index, const char * className, T & id)
{
	if(!lua_istable(L, index))
		return false;
	lua_pushstring(L, "className_");
	lua_gettable(L, index);	
	ed::string actualClassName = lua_tostring(L, -1);
	lua_pop(L, 1);
	if(actualClassName != className)
		return false;	
	lua_pushstring(L, "id_");
	lua_gettable(L, index);
	bool result = getID(L, -1, id);
	lua_pop(L, 1);
	return result;
}

template<typename T>
bool getLuaObject(lua_State * L, int index, T & id)
{
	if(!lua_istable(L, index))
		return false;
	lua_pushstring(L, "id_");
	lua_gettable(L, index);
	bool result = getID(L, -1, id);
	lua_pop(L, 1);
	return result;
}

}

#endif // ED_LUA_OBJECT