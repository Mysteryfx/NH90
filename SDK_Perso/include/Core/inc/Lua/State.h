/**
 * Lua Wrapper library
 * (C)opyright 2004 by Dmitry S. Baikov
 *
 * Eagle Dynamics has non-exclusive rights to use, sell and sublicense this code.
 *
 */
#ifndef _Lua_State_h_
#define _Lua_State_h_


#include "./_config.h"


#include "ed_lua.h"
#include <cassert>
#include <ed/string.h>
#include <string.h>


namespace Lua {


class Nil {};


class StateRef {
protected:
	lua_State* lua;
public:
	StateRef(lua_State* s) : lua(s) {}

	operator lua_State*() const { return lua; }

	// push values to the stack

	StateRef& push(bool b)
	{
		lua_pushboolean(lua, b);
		return *this;
	}

	StateRef& push(int i)
	{
		lua_pushnumber(lua, i);
		return *this;
	}

	StateRef& push(float n)
	{
		lua_pushnumber(lua, n);
		return *this;
	}

	StateRef& push(double n)
	{
		lua_pushnumber(lua, n);
		return *this;
	}

	StateRef& push(const char* str)
	{
		lua_pushstring(lua, str);
		return *this;
	}

	StateRef& push(const char* str, size_t len)
	{
		lua_pushlstring(lua, str, len);
		return *this;
	}

	StateRef& push(const ed::string& str)
	{
		lua_pushlstring(lua, str.data(), str.size());
		return *this;
	}

	StateRef& push(Nil)
	{
		lua_pushnil(lua);
		return *this;
	}

	StateRef& push(lua_CFunction f, int upvalues=0)
	{
		lua_pushcclosure(lua, f, upvalues);
		return *this;
	}

	StateRef& push(void* light)
	{
		lua_pushlightuserdata(lua, light);
		return *this;
	}

	StateRef& push(void* full, size_t len)
	{
		void* data = lua_newuserdata(lua, len);
		memcpy(data, full, len);
		return *this;
	}
	
	StateRef& pushValue(int i)
	{
		lua_pushvalue(lua, i);
		return *this;
	}

	StateRef& pop(int i)
	{
		lua_pop(lua, i);
		return *this;
	}

	StateRef& pushFormat(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		lua_pushvfstring(lua, fmt, args);
		va_end(args);
		return *this;
	}

	StateRef& pushFormat(const char* fmt, va_list args)
	{
		lua_pushvfstring(lua, fmt, args);
		return *this;
	}

	// get values from the stack

	StateRef& get(bool& val, int i=-1)
	{
		assert( lua_isboolean(lua, i) );
		val = lua_toboolean(lua, i)!=0;
		return *this;
	}
	
	StateRef& get(short& val, int i=-1)
	{
		assert( lua_isnumber(lua, i) );
		lua_Number num = lua_tonumber(lua, i);
		val = short(num);
		assert( val == num ); // ? should we check this ?
		return *this;
	}

	StateRef& get(int& val, int i=-1)
	{
		assert( lua_isnumber(lua, i) );
		lua_Number num = lua_tonumber(lua, i);
		val = int(num);
		assert( val == num ); // ? should we check this ?
		return *this;
	}

	StateRef& get(float& val, int i=-1)
	{
		assert( lua_isnumber(lua, i) );
		val = float(lua_tonumber(lua, i));
		return *this;
	}

	StateRef& get(double& val, int i=-1)
	{
		assert( lua_isnumber(lua, i) );
		val = double(lua_tonumber(lua, i));
		return *this;
	}

	template<class T>
	StateRef& get(T* &val, int idx=-1)
	{
		assert( lua_isuserdata(lua, idx) );
		val = static_cast<T*>( lua_touserdata(lua, idx) );
		return *this;
	}

	StateRef& get(ed::string& val, int i=-1)
	{
		assert( lua_isstring(lua, i) );
		val = lua_tostring(lua, i);
		return *this;
	}

	// stack manipulation

	StateRef& remove(int i)
	{
		lua_remove(lua, i);
		return *this;
	}

	StateRef& setTop(int top)
	{
		lua_settop(lua, top);
		return *this;
	}

	// table manipulation

	StateRef& newTable()
	{
		lua_newtable(lua);
		return *this;
	}

	StateRef& rawSet(int i)
	{
		lua_rawset(lua, i);
		return *this;
	}

	StateRef& rawGet(int i)
	{
		lua_rawget(lua, i);
		return *this;
	}

	StateRef& pcall(int nargs, int nres, int errfunc=0)
	{
		lua_pcall(lua, nargs, nres, errfunc);
		return *this;
	}

}; // class StateRef


class State : public StateRef {
public:
	State(const char *name="?") : StateRef(ED_lua_open(name)) {}
	~State() { ED_lua_close(lua); }

	void reset()
	{
		lua_close(lua);
		lua=lua_open();
	}
	
private:
	State(const State&);
	void operator = (const State&);
};


} // namespace Lua



#endif /* _Lua_State_h_ */
