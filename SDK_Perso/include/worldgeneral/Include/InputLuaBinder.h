#ifndef __NEW_INPUT_LUA_BINDER_H__
#define __NEW_INPUT_LUA_BINDER_H__

#include "WorldGeneral.h"
#include <ed/string.h>

struct lua_State;
class InputLuaBinder
{
public:
	WORLDGENERAL_API static InputLuaBinder* get() { return instance_; }
	WORLDGENERAL_API static void create(lua_State* L);
	WORLDGENERAL_API static void destroy();

	WORLDGENERAL_API bool loadUnitLayer(const ed::string& name, const char* pluginPath = 0);
	WORLDGENERAL_API void createUnitLayer(const ed::string& name, const char* pluginPath = 0);
	WORLDGENERAL_API bool createUnitLayerAndSetOnTop(const ed::string& name, const char* pluginPath = 0); //without restoring layers on top of object layer
	WORLDGENERAL_API void removeUnitLayer(const ed::string& name); 

private:
	InputLuaBinder(lua_State* L, const ed::string& filename);

	static InputLuaBinder* instance_;
	lua_State* L_;
};

#endif