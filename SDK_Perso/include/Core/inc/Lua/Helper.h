#ifndef core_lua_Helper_h__
#define core_lua_Helper_h__

#include "ed_lua.h"


/*
 * Загрузка файла в стейт с дополнительной инициализацией стейта посредством функции из передаванной dll.
 */
ED_CORE_EXTERN
bool luaLoadFileWithDynamicLib(const char* path, const char* libName, const char* function, lua_State* state);

/*
 * Дамп таблицы.
 */
ED_CORE_EXTERN
ed::string luaDumpTable(lua_State* state, const char* tableName);



#endif // core_lua_Helper_h__
