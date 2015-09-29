#ifndef __LUA_SERIALIZER_H__
#define __LUA_SERIALIZER_H__

#include "_config.h"

#include "ed_vfs.h"
#include "exception.h"

struct lua_State;

namespace Lua
{

SIMPLE_SUBEXCEPTION(SerializerException, Exception);


class Serializer
{
public:
	ED_CORE_EXTERN Serializer(const char* filename);
	ED_CORE_EXTERN ~Serializer();

	ED_CORE_EXTERN void serialize(lua_State* state, const ed::string& table);

private:
	void write_(const ed::string& str);

	friend int g_writeCallback(lua_State* s);
private:
    VFS_File* out_;
};

}
#endif
