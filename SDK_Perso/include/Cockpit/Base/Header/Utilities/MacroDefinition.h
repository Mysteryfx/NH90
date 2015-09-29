#pragma once

namespace cockpit
{

template <typename L, typename T> 
int register_lambda(T n){
	L lambda_f;
	return lambda_f(n);
}

template <typename L, typename T1, typename T2> 
void register_lambda_2( T1 type_1,  T2 type_2){
	L lambda_f;
	return lambda_f(type_1,type_2);
}

#define REG_EXPORT_FUNCTION(Lua_s,CtlName,FunctionName) {\
	lua_pushstring(Lua_s, CtlName);\
	lua_pushcfunction(Lua_s, l_##FunctionName);\
	lua_settable(Lua_s, index);}

#define REG_CTL_LAMBDA_2(CtlName,Function,Type1,Type2) {\
	auto f = Function;\
	lua_pushstring(L, CtlName);\
	lua_pushlightuserdata(L, (void(*)())register_lambda_2<decltype(f),Type1,Type2>);\
	lua_settable(L, index);}

#define REGISTER_CONTROLLER(ClassName,CtlName,FunctionName)\
	lua_pushstring(L, CtlName);\
	lua_pushlightuserdata(L, &ClassName##::##FunctionName);\
	lua_settable(L, index);

#define REG_LUA_EXPORT_FUNC(Lua_s,ClassName,CtlName,FunctionName){\
	auto f = [](lua_State* Lua_s) ->int{\
	ClassName##*  device = ClassName##::l_get_device_from_meta<##ClassName##>(Lua_s);\
	lua_pushnumber(Lua_s,(device) ? device->##FunctionName##(): 0);return 1;};\
	lua_pushstring(Lua_s,CtlName);\
	lua_pushcfunction(Lua_s, (int(*)(lua_State*))register_lambda<decltype(f)>);\
	lua_settable(Lua_s, index); }

#define REG_CTL_MAIN_P_L(CtlName,Function)\
	REG_CTL_LAMBDA_2(CtlName,Function,ccPanelGauge*,ccMainPanel*)
}

