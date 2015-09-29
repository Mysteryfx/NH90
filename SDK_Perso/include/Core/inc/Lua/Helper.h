#ifndef core_lua_Helper_h__
#define core_lua_Helper_h__

#include "ed_lua.h"


/*
 * �������� ����� � ����� � �������������� �������������� ������ ����������� ������� �� ������������ dll.
 */
ED_CORE_EXTERN
bool luaLoadFileWithDynamicLib(const char* path, const char* libName, const char* function, lua_State* state);

/*
 * ���� �������.
 */
ED_CORE_EXTERN
ed::string luaDumpTable(lua_State* state, const char* tableName);



#endif // core_lua_Helper_h__
