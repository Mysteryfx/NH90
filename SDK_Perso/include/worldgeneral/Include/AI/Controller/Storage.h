#ifndef __AI_Storage__
#define __AI_Storage__

#include "WorldGeneral.h"
#include <hash_map>

namespace AI
{

template<typename Key, class Value, class KeyComp = hash_compare<Key, _STD less<Key> > >
class WORLDGENERAL_API Storage
{
private:
	Storage()
	{
	}
public:
	void add(const Key & key, const Value & value)
	{
		assert(hashMap.find(key) == hashMap.end());
		hashMap[key] = value;
		assert(hashMap.find(key) != hashMap.end());
	}
	Value get(const Key & key) const
	{
		HashMap::const_iterator itC = hashMap.find(key);
		if(itC != hashMap.end())
		{
			return itC->second;
		}
		else
		{
			return NULL;
		}
	}
	static Storage<Key, Value, KeyComp> * instance()
	{
		if(instance_ == NULL)
		{
			instance_ = new Storage<Key, Value, KeyComp>();
		}
		return instance_;
	}
private:
	typedef stdext::hash_map<Key, Value, KeyComp> HashMap;
	HashMap hashMap;
	static Storage<Key, Value, KeyComp> * instance_;
};

template<typename Key, class Value, class KeyComp>
Storage<Key, Value, KeyComp> * Storage<Key, Value, KeyComp>::instance_ = NULL;

}

#endif __AI_Storage__