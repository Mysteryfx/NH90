#ifndef __CORE__LUA_CHECKED_LOADER_H__
#define __CORE__LUA_CHECKED_LOADER_H__

#include <exception.h>
#include "_config.h"

struct lua_State;

namespace Lua
{

SIMPLE_EXCEPTION(CheckedLoaderException);
SIMPLE_EXCEPTION(CheckedLoaderExceptionFileMissed);
// Загружает lua-файлы с проверкой контрольной суммы (для игры по сети)
class ED_CORE_EXTERN CheckedLoader
{
public:
    typedef int Checksum;

public:
    static CheckedLoader* instance();

    // Добавить проверяемый файл. При следующей загрузке файла он будет проверен 
    // на соответсвие заданной контрольной сумме.
    //   filename - имя файла (относительно LockOnExe
    //   checksum - контрольная сумма
    void addCheckedFile(const char* filename, Checksum checksum);
    // Загрузить файл filename в состояние state. Если файл есть в списке проверяемых - 
    // проверить контрольную сумму на соответствие.
    void loadFile(lua_State* state, const char* filename,const char * env_name = 0);
    // Тоже что и loadFile, только не бросает исключение, а возвращает true/false
    // TODO: сообщение об ошибке теряется!!
    bool tryLoadFile(lua_State* state, const char* filename,const char * env_name = 0);

private:
    CheckedLoader();
    ~CheckedLoader();
};

}

#endif