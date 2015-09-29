#ifndef ED_LUA_CONFIG_H
#define ED_LUA_CONFIG_H

#include "_config.h"
#include "ed_lua.h"
#include <assert.h>
#include <ed/string.h>

namespace osg {
	class Vec2f;
	class Vec2d;
	class Vec3f;
	class Vec3d;
	class Vec4d;
	class Vec4f;
	class Vec4ub;
}

namespace Lua {

class StackCheck {
public:
#ifndef _ED_HIDDEN_DEBUG 
		StackCheck(lua_State* s, int res=0) {}
#else
		StackCheck(lua_State* s, int res=0)	: L(s), top(lua_gettop(s) + res) {}
		~StackCheck() { assert( lua_gettop(L) == top ); }
private:
	lua_State* L;
	int top;
#endif
};

/*
	Usage:

	// reading
	Lua::Config conf = globalConfig;
	if( conf.open("my.config.table") ) {
		conf.get("int1", &i);
		conf.get("str2", &str);
		if( conf.open("sub_table") ) {
			conf.get("bool3", &b);
			conf.pop(); // pop "sub_table"
		}
		conf.get("sub_table.bool4", &b);
		conf.pop(); // pop "my.config.table"
	}

	// writing
	if( overwrite )
		conf.unset("my.conf.table");
	conf.create("my.conf.table");
		conf.set("bool1", b);
		conf.set("int2", i);
		conf.set("sub_table.bool3", b);
	conf.pop();

	// iteration
	conf.iterate_begin();
	while (conf.iterate_next()) {
		float v = 0;
		conf.get_key(v);
		break;
	}
	conf.iterate_end();
*/
class Config {
	lua_State* L;
	int top;
public:
		Config(lua_State* lua, int i=LUA_GLOBALSINDEX) : L(lua), top(lua_gettop(lua)) { lua_pushvalue(L, i); }
		// RAI - Resource Acquision is Initialization
		// NEVER EVER move destructor body into any kind of finish() function
		// Use scopes, Luke!!! {Lua::Config cfg(L); .... } <- here goes the destructor
		~Config() { lua_settop(L, top); }

	operator lua_State*() const { return L; }

// LOADING:
    /**
     * Loads file into current state.
     */
    ED_CORE_EXTERN bool load(const char *fname);

    /**
     * Loads file into current state and returns error string.
     */
	ED_CORE_EXTERN bool load2(const char *fname, ed::string & errorStr);

    /**
     * Loads string into current state.
     */
    ED_CORE_EXTERN bool load_buffer(const char *buf, size_t len, const char* name);

    /**
     * Loads file into temporary table.
     * If succeeds, leaves the table on the top of the stack.
     * Call pop() after using the table.
     */
    ED_CORE_EXTERN bool tmpload(const char *fname, bool see_globals=false);

    /**
     * Loads buffer into temporary table.
     * If succeeds, leaves the table on the top of the stack.
     * Call pop() after using the table.
     */
	ED_CORE_EXTERN bool tmpload_buf(const char *buf,size_t buff_size, bool see_globals=false);

// READING:
	// push table from path to stack
	// if result is false, NOTHING is pushed to the stack (not nil).
	ED_CORE_EXTERN bool open(const char* path);
    	ED_CORE_EXTERN bool open(int i);

	// open table without parsing
	ED_CORE_EXTERN bool open_raw(const char* name);


	// helpers
	template<typename T>
	bool get(const char* path, T* v) { get(path); return pop(v); }

	template<typename T>
	bool get(const char* path, T* v, const T& d) { get(path); if(pop(v)) return true; else {*v = d; return false;} }

	template<typename T>
	bool get(int key, T* v) { get(key); return pop(v); }

	template<typename T>
	bool get(int key, T* v, const T& d) { get(key); return pop(v) ? true : (*v = d), false; }

	// immediate mode
	template<typename T>
	T get(const char* path, const T& v) { T r(v); get(path); pop(&r); return r; }

	template<typename T>
	T get(int key, const T& v) { T r(v); get(key); pop(&r); return r; }


	// push value onto stack
	ED_CORE_EXTERN void get(const char* path);
	ED_CORE_EXTERN void get(int key);

	int  get_with_type(int key)
	{
		get(key);
		int t = lua_type(L,-1);
		if (t == LUA_TNONE || t == LUA_TNIL)
		{
			pop();
			return LUA_TNIL;
		}
		return t;
	}
	int  get_with_type(const char* path)
	{
		get(path);
		int t = lua_type(L,-1);
		if (t == LUA_TNONE || t == LUA_TNIL)
		{
			pop();
			return LUA_TNIL;
		}
		return t;
	}

	ED_CORE_EXTERN bool pop(bool*);
	ED_CORE_EXTERN bool pop(unsigned char*); // one char!
	ED_CORE_EXTERN bool pop(int*);
	ED_CORE_EXTERN bool pop(float*);
	ED_CORE_EXTERN bool pop(double*);
	ED_CORE_EXTERN bool pop(ed::string*);
    ED_CORE_EXTERN bool pop(unsigned short*);
    ED_CORE_EXTERN bool pop(unsigned int*);
    ED_CORE_EXTERN bool pop(unsigned long*);
	ED_CORE_EXTERN bool pop(osg::Vec2f*);
	ED_CORE_EXTERN bool pop(osg::Vec3d*);
	ED_CORE_EXTERN bool pop(osg::Vec4f*);
	ED_CORE_EXTERN bool pop(osg::Vec4d*);
	ED_CORE_EXTERN bool pop(osg::Vec4ub*);
	ED_CORE_EXTERN bool pop(void**);

	template<typename T>
	inline bool pop(T * t)	{ int temp; return pop(&temp) ? (*t) = T(temp), true : false; }


// WRITING:
	ED_CORE_EXTERN void unset(const char* path);

	// push table from path (and create if needed)
	ED_CORE_EXTERN void create(const char* path);
	ED_CORE_EXTERN void create(int key);

	ED_CORE_EXTERN void create_raw(const char* name);

	// helpers
	template<typename T>
	void set(const char* path, T val) { push(val); set(path); }

	template<typename T>
	void set(int key, T val) { push(val); set(key); }

	// set from the top of stack
	ED_CORE_EXTERN void set(const char* path);
	ED_CORE_EXTERN void set(int key);

	void push(bool v) { lua_pushboolean(L, v); }
	void push(int v) { lua_pushnumber(L, v); }
    void push(unsigned int v) { lua_pushnumber(L, v); }
	void push(double v) { lua_pushnumber(L, v); }
    void push(float v) { lua_pushnumber(L, v); }
	void push(const char* v) { lua_pushstring(L, v); }
	void push(const ed::string& v) { lua_pushlstring(L, v.c_str(), v.length()); }
	void push(void * p) { lua_pushlightuserdata(L, p); }
	void push(lua_CFunction f) { lua_pushcfunction(L, f); }

// CALLING:
	bool call(const char* path, int nargs=0, int nresults=0)
	{
		return call_func(path, NULL, nargs, nresults);
	}
	bool call2(const char* path, ed::string & errorMsg, int nargs=0, int nresults=0)
	{
		return call_func(path, &errorMsg, nargs, nresults);
	}

    struct Nil {};
    template<class T> struct Arg { enum { Len = 1 }; };

	void push(const Nil&) {}

	template<class A1, class A2, class A3, class A4>
	bool call(int nresults, const char* path, const A1& a1, const A2& a2, const A3& a3, const A4& a4)
	{
		get(path);
		if (lua_isfunction(L, -1)) {
			push(a1);
			push(a2);
			push(a3);
			push(a4);
			if (lua_pcall(L, Arg<A1>::Len+Arg<A2>::Len+Arg<A3>::Len+Arg<A4>::Len, nresults, 0)) {
                call_log(path,lua_tostring(L, -1));
				lua_pop(L, 1);
				return false;
			}
			return true;
		}
		lua_pop(L, 1);
		return false;
	}

	template<class A1>
	bool call(int nresults, const char* path, const A1& a1)
	{ return call(nresults, path, a1, Nil(), Nil(), Nil()); }

	template<class A1, class A2>
	bool call(int nresults, const char* path, const A1& a1, const A2& a2)
	{ return call(nresults, path, a1, a2, Nil(), Nil()); }

	template<class A1, class A2, class A3>
	bool call(int nresults, const char* path, const A1& a1, const A2& a2, const A3& a3)
	{ return call(nresults, path, a1, a2, a3, Nil()); }

// EVERYTHING:
	void pop(int n=1) { lua_pop(L, n); }

    ED_CORE_EXTERN bool dostring(const char* str);
	ED_CORE_EXTERN bool dostring2(const char* str, ed::string & errorMsg);
	ED_CORE_EXTERN bool dofile(const char* file);

// ITERATION
	void iterate_begin() { lua_pushnil(L); lua_pushnil(L); }

	template<class T>
	void iterate_begin(const T& v) { push(v); lua_pushnil(L); }

	bool iterate_next()
	{
		lua_pop(L, 1);
		if (lua_next(L, -2))
			return true;
		lua_pushnil(L);
		lua_pushnil(L);
		return false;
	}
    // direct using of pop is impossible in current iteration routine
    ED_CORE_EXTERN bool iterate_pop(bool*);
    ED_CORE_EXTERN bool iterate_pop(unsigned char*); // one char!
    ED_CORE_EXTERN bool iterate_pop(int*);
    ED_CORE_EXTERN bool iterate_pop(float*);
    ED_CORE_EXTERN bool iterate_pop(double*);
    ED_CORE_EXTERN bool iterate_pop(ed::string*);
	ED_CORE_EXTERN bool iterate_pop(void**);

	void iterate_end() { lua_pop(L, 2); }

	// get current iteration key
	template<class T>
    bool get_key(T* v) { lua_pushvalue(L, -2); return pop(v); }

	static ED_CORE_EXTERN int errorFunc(lua_State* L);

private: // NOCOPY
	Config(const Config&);
	void operator = (const Config&);
    void* operator new(size_t sz); // Forbidden. This is a stack-only object.
    void* operator new[](size_t sz); // Forbidden. This is a stack-only object.

	ED_CORE_EXTERN bool call_func(const char* path, ed::string * errorStr, int nargs=0, int nresults=0);
    ED_CORE_EXTERN void call_log(const char * path,const char * err) const;
};

template<> struct Config::Arg<Config::Nil> { enum { Len = 0 }; };

}

#endif /* ED_LUA_CONFIG_H */
