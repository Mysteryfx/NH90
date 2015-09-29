#ifndef __AMMUNITION_MANAGER_IMPL_H__
#define __AMMUNITION_MANAGER_IMPL_H__

#include <ed/map.h>
#include <ed/vector.h>

#include "weapon_exception.h"
#include "descriptor_base.h"

#include "WeaponsBase.h"

SIMPLE_SUBEXCEPTION(wDatabaseException, wWeaponException);

class wDescriptorBase;
struct lua_State;
class wDatabase
{
public:
	WEAPONSBASE_API static wDatabase* instance();

public:
	WEAPONSBASE_API wDatabase();
	WEAPONSBASE_API ~wDatabase();

	WEAPONSBASE_API void init(lua_State * L = 0);
	WEAPONSBASE_API void reload();
	WEAPONSBASE_API void addDescriptor(wDescriptorBase* desc);

	template <class T>
	T* getDescriptor(const ed::string& name)
	{
		wDescriptorBase* d = findDescriptorByName_(name);
		static_cast<T*>(d);
		T* res = dynamic_cast<T*>(d);
		if (res == 0)
			throw wDatabaseException("Descriptor " + name + " has invalid type (" + typeid(T).name() + ")");

		return res;
	}

    ed::vector<wDescriptorBase*> getDescriptors();

protected:
	WEAPONSBASE_API virtual wDescriptorBase* findDescriptorByName_(const ed::string& name);

	WEAPONSBASE_API void initTable_(lua_State * L = 0);
	WEAPONSBASE_API void freeTable_();

private:
	// Full name (with namespace) as key
	typedef ed::map<ed::string, wDescriptorBase*> DescMap_;

	DescMap_ descMap_;

	ed::vector<wDescriptorBase*> anonymousDescs_;
};

#endif
