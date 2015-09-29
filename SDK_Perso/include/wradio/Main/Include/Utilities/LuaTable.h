#ifndef __LuaTable__
#define __LuaTable__

#include "wRadio.h"

#include <ed/string.h>
#include <ed/vector.h>
#include <ed/map.h>

struct lua_State;

namespace Mail
{
	class Stream;
}

#ifdef OUT_OF_DCS
class LuaTableElement
#else
class WRADIO_API LuaTableElement
#endif
{
public:
	struct Object
	{
		ed::string className;
		int id;
		ed::string strid;
	};
private:
	enum Type
	{
		TYPE_NIL,
		TYPE_BOOL,
		TYPE_INT,
		TYPE_DOUBLE,
		TYPE_STRING,
		TYPE_OBJECT,
		TYPE_VECTOR,
		TYPE_MAP
	};
	union {
		bool	b;
		int		i;
		double	d;
		ed::vector<LuaTableElement>* pV;
		ed::map<LuaTableElement, LuaTableElement>* pM;
	} u;
	Type		type;
	ed::string	str;
	Object		object;
public:
	LuaTableElement();
	LuaTableElement(const LuaTableElement & element);
	~LuaTableElement();
	LuaTableElement & operator = (const LuaTableElement & element);
	bool operator <(const LuaTableElement & element) const;
	bool operator ==(const LuaTableElement & element) const;
	bool operator !=(const LuaTableElement & element) const;

	operator bool() const;
	operator int() const;
	operator double() const;
	operator ed::string() const;
	
	LuaTableElement & operator = (bool b);
	LuaTableElement & operator = (int i);
	LuaTableElement & operator = (double d);
	LuaTableElement & operator = (const ed::string & str_);
	LuaTableElement & operator = (const char * c);
	LuaTableElement & operator = (const Object & luaObject);

	const LuaTableElement & operator () (int i) const;
	const LuaTableElement & operator () (double d) const ;
	const LuaTableElement & operator () (const ed::string & str_) const;
	const LuaTableElement & operator () (const char * str_) const;
	const LuaTableElement & operator () (const LuaTableElement & key);

	LuaTableElement & operator [] (int i);
	LuaTableElement & operator [] (double d);
	LuaTableElement & operator [] (const ed::string & str_);
	LuaTableElement & operator [] (const char * str_);
	LuaTableElement & operator [] (const LuaTableElement & key);

	bool empty() const;

	void fromState(lua_State * L);
	void toState(lua_State * L) const;
	ed::string print() const;
private:
	void fromState_(lua_State * L, int index);
	void setNewType_(Type type_);
	ed::string print_(int level) const;
	friend WRADIO_API Mail::Stream& operator >> (Mail::Stream& s, LuaTableElement & luaTableElement);
};

WRADIO_API Mail::Stream& operator >> (Mail::Stream& s, LuaTableElement & luaTableElement);

typedef LuaTableElement LuaTable;

#endif