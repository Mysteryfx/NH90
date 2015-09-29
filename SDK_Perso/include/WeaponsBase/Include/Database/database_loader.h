#ifndef __DATABASE_LOADER_H__
#define __DATABASE_LOADER_H__

#include "WeaponsBase.h"

#include "lua/lua_loader.h"
#include "Database/database.h"

class wDatabase;
class wDescriptorBase;

class WEAPONSBASE_API wDatabaseLoader : public Lua::Loader
{
public:
	wDatabaseLoader(wDatabase* db,lua_State * L = 0);
	virtual ~wDatabaseLoader();

	void load(const ed::string& file, const ed::string& table);
    void load( const ed::string& table);

	template <class T>
	const T* getDescriptor(const ed::string& name)
	{
		wDescriptorBase* desc = loadDescriptor_(name);
		static_cast<T*>(desc);
		T* res = dynamic_cast<T*>(desc);
		if (res == 0)
			throw wDatabaseException(FMT("Invalid descriptor type at " << formatPath_(name.c_str())));

		return res;
	}

	// Из текущей таблицы
	template <class T>
	const T* getDescriptor()
	{
		wDescriptorBase* desc = loadDescriptor_();
		
		static_cast<T*>(desc);
		T* res = dynamic_cast<T*>(desc);
		if (res == 0)
			throw wDatabaseException(FMT("Invalid descriptor type at " << getCurrentPath()));

		return res;
	}

private:
	void loadNamespace_();
	wDescriptorBase* loadDescriptor_(bool use_name = false);
	wDescriptorBase* loadDescriptor_(const ed::string& name, bool use_name = false);

private:
	wDatabase* db_;

	typedef ed::vector<wDescriptorBase*> LoadedDescsList_;
	LoadedDescsList_ loadedDescs_;
};


#endif 
