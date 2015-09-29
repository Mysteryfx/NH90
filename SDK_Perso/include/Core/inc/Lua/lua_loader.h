#ifndef __LUA_LOADER_H__
#define __LUA_LOADER_H__

#include "_config.h"

#include <ed/string.h>
#include <ed/vector.h>

#include "exception.h"

#include "Math/Vector.h"
#include "Math/Rotation3.h"

struct lua_State;

namespace Lua
{

SIMPLE_SUBEXCEPTION(LoaderException, Exception);

// Не очень, но неохота инклудить

class ED_CORE_EXTERN Loader
{
private:
	Loader(const Loader& ldr);
	Loader& operator = (const Loader&);
    void  requireState(const char * dbg_name);
public:
	Loader();
	Loader(lua_State * Sin,const char * root_dir_ = NULL);
	virtual ~Loader();

	void load(const char* filename, const char* tablename);
    void load(const char* tablename);

	// Получить state
	lua_State* getLuaState() { return S_; }

	// Директорию со скриптом
	const ed::string& getRootDir() const { return rootDir_; }

	// Есть ли в текущей таблице заданное поле
	template <class T> bool			hasEntry(const T key);

	// Get
	template <class T> void*		getPointer(const T key);
	template <class T> int			getInt(const T key);
	template <class T> double		getDouble(const T key);
	template <class T> bool			getBoolean(const T key);
	template <class T> ed::string	getString(const T key);
	template <class T> Math::Vec3d	getVector(const T key);
	template <class T> Math::Rot3d  getQuaternion(const T key);
	// Aliases
	template <class T> void			get(const T key, int& val) { val = getInt(key); }
	template <class T> void			get(const T key, double& val) { val = getDouble(key); }
    template <class T> void         get(const T key, float& val) { val = (float)getDouble(key); }
	template <class T> void			get(const T key, bool& val) { val = getBoolean(key); }
	template <class T> void			get(const T key, ed::string& val) { val = getString(key); }
	template <class T> void			get(const T key, Math::Vec3d& val) { val = getVector(key); }

	// Set
	void							createGlobalTable(const ed::string& name);
	template <class T> void			setInt(const T, int val);	
	template <class T> void			setDouble(const T, double val);
	template <class T> void			setString(const T, const ed::string& val);
	template <class T> void			setBoolean(const T, bool val);
	template <class T> void			setVector(const T, const Math::Vec3d& val);
	template <class T> void			setQuaternion(const T, const Math::Rot3d& val);
	template <class T> void			setPointer(const T, void * ptr);	

	// Aliases 
	template <class T> void			set(const T key, int val) { setInt(key, val); }
	template <class T> void			set(const T key, double val) { setDouble(key, val); }
	template <class T> void			set(const T key, const ed::string& val) { setString(key, val); }
	template <class T> void			set(const T key, bool val) { setBoolean(key, val); }
	template <class T> void			set(const T key, const Math::Vec3d& val) { setVector(key, val); }

	// Обработка вложенных таблиц
	template <class T> void beginSubtable(const T key);
	template <class T> void createSubtable(const T key);
	void endSubtable();

	// Пройти все подтаблицы (ключи должны быть строковые)
	void beginSubtables();
	bool nextSubtable(ed::string& key);

	// Пройти все подтаблицы с ключами 1..n
	void beginArray();
	template <class T> void beginArray(const T key);
	bool nextArray();

	void beginMetatable();
	void newMetatable();
	void endMetatable();

	ed::string getCurrentPath(int skipno = 0) const;

protected:
	template <class T>
	void pushKey_(const T key);
	template <class T> void addPathToken_(const T t);

	// Перед вызовом на вершине стека - таблица
	// Поместить соответствующую запись из нее на вершину стека, заодно проверить тип
	template <class T> void selectValue_(const T key, int type);
	template <class T> ed::string formatPath_(const T key);

	ed::string extractPath_(const ed::string& str);
   
    ed::string rootDir_;
private:
	lua_State* S_;
	bool		ownS_;
	ed::vector<ed::string> currentPath_;		// текущий путь (по таблицам) для сообщений об ошибках
};

}

#endif 