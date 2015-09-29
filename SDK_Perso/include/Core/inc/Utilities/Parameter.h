#ifndef __Parameter__
#define __Parameter__

#include "ed_lua.h"
#include "math.h"
#include <assert.h>

class Parameter
{
private:
	enum Type { TYPE_VOID = -1, TYPE_BOOL, TYPE_INT, TYPE_DOUBLE, TYPE_STRING, TYPE_PTR };
public:	
	Parameter() : type(TYPE_VOID) {;}
	Parameter(const Parameter & parameter)
	{
		(*this) = parameter;
	}
	Parameter(bool b_)			{ *this = b_;}
	Parameter(int i_)			{ *this = i_; }
	Parameter(unsigned int i_)	{ *this = (int)i_; }
	Parameter(double d_)		{ *this = d_; }
	Parameter(float f_)			{ *this = f_; }
	Parameter(const char * s_)	{ *this = s_; }
	Parameter(void * p_)		{ *this = p_; }
	template<typename T>
	Parameter(T t)				{ *this = (int)t; }
	template<typename T>
	Parameter(T * t)			{ *this = (void*)t; }

	inline Parameter & operator = (const Parameter & parameter)
	{
		type = parameter.type;
		u = parameter.u;
		return *this;
	}

	inline void clear()			{ type = TYPE_VOID; }
	
	inline void operator = (bool b_)			{ u.b = b_,			type = TYPE_BOOL; }
	inline void operator = (int i_)				{ u.i = i_,			type = TYPE_INT; }
	inline void operator = (unsigned int i_)	{ u.i = (int)i_,	type = TYPE_INT; }
	inline void operator = (double d_)			{ u.d = d_,			type = TYPE_DOUBLE; }
	inline void operator = (float f_)			{ u.d = double(f_), type = TYPE_DOUBLE; }
	inline void operator = (const char * s_)	{ u.s = s_,			type = TYPE_STRING; }
	inline void operator = (void * p_)			{ u.p = p_,			type = TYPE_PTR; }
	template<typename T>
	inline void operator = (T t)				{ u.i = t,			type = TYPE_INT; }
	template<typename T>
	inline void operator = (T * t)				{ u.p = t,			type = TYPE_PTR; }
	
	template<typename T>
	inline operator T () const		{ assert(type == TYPE_INT);				return (T)u.i; }
	template<typename T>
	inline operator T * () const	{ assert(type == TYPE_PTR);				return (T*)u.p; }
	inline operator bool() const	{ return type != TYPE_VOID && (type != TYPE_BOOL || u.b) ; } //like lua
	inline operator int () const	{ assert(type == TYPE_INT);				return u.i;  }
	inline operator unsigned int () const	{ assert(type == TYPE_INT);		return (unsigned int)u.i;  }
	inline operator double () const { assert(type == TYPE_DOUBLE || type == TYPE_INT);	return type == TYPE_DOUBLE ? u.d : double(u.i); }
	inline operator float () const	{ assert(type == TYPE_DOUBLE || type == TYPE_INT);	return type == TYPE_DOUBLE ? float(u.d) : float(u.i); }
	inline operator const char * () const { assert(type == TYPE_STRING);	return u.s; }
	inline operator void * () const { assert(type == TYPE_PTR);				return u.p; }

	inline bool operator == (const Parameter & parameter) const
	{
		if(type == TYPE_INT)
			return u.i == parameter.u.i;
		else if(type == TYPE_DOUBLE)
			return u.d == parameter.u.d;
		else if(type == TYPE_BOOL)
			return u.b == parameter.u.b;
		else if(type == TYPE_STRING)
			return u.s == parameter.u.s;
		else if(type == TYPE_PTR)
			return u.p == parameter.u.p;
		else
			return false;
	}

	inline bool operator != (const Parameter & parameter) const
	{
		return *this == parameter;
	}
	
	inline bool fromState(lua_State * L)
	{
		if(lua_isuserdata(L, -1))
			*this = lua_touserdata(L, -1);
		else if(lua_isboolean(L, -1))
			*this = (bool)lua_toboolean(L, -1);
		else if(lua_isnumber(L, -1))
		{
			double num = lua_tonumber(L, -1);
			if(fabs(num - floor(num + 0.5f)) < 1.0E-8)
				*this = (unsigned int)fabs(num + 0.5f);
			else
				*this = num;	
		}
		else if(lua_isstring(L, -1))
			*this = lua_tostring(L, -1);
		else if(lua_istable(L, -1))
		{
			int top = lua_gettop(L);
			lua_pushstring(L,"id_");
			lua_gettable(L, top);
			if(lua_isnumber(L, -1))
				*this = int(lua_tonumber(L, -1));	
			else if(lua_isstring(L, -1))
				*this = lua_tostring(L, -1);
			lua_pop(L, 1);
		}
		else
			return false;
		return true;
	}
	inline void toState(lua_State * L) const
	{
		if(type == TYPE_INT)
			lua_pushnumber(L, u.i);
		else if(type == TYPE_DOUBLE)
			lua_pushnumber(L, u.d);
		else if(type == TYPE_BOOL)
			lua_pushboolean(L, u.b);
		else if(type == TYPE_STRING)
			lua_pushstring(L, u.s);
		else if(type == TYPE_PTR)
			lua_pushlightuserdata(L, u.p);		
	}
private: //ибонех
	union
	{
		bool b;
		int i;
		double d;
		const char * s;
		void * p;
	} u;
	Type type;
};

#endif __Parameter__