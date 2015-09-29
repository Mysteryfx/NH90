#ifndef WORLD_GENERAL_COPY_AND_DO_H
#define WORLD_GENERAL_COPY_AND_DO_H

#include <ed/string.h>

#include "ed_lua.h"

#include "WorldGeneral.h"

WORLDGENERAL_API bool         copy_and_do_lua_file(lua_State * L,const ed::string & file_name,const char * env_name = 0,bool from_user_dir = false);

//add compiled lua chunk on top of the stack (for keep away problems with handling environment inside of C code)

enum copy_and_compile_to_lua_chunk_result
{
    COPY_AND_COMPILE_FAILED,
    COPY_AND_COMPILE_DEFAULT_USED,
    COPY_AND_COMPILE_MISSION_CHUNK_USED,
};

WORLDGENERAL_API int         copy_and_compile_to_lua_chunk(lua_State * L,const ed::string & file_name,bool from_user_dir = false);

WORLDGENERAL_API ed::string fix_chunk_name(const ed::string & file_name);

#endif
