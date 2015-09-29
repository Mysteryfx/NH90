#ifndef __AI_ClassFactory__
#define __AI_ClassFactory__

#include "WorldGeneral.h"
#include <hash_map>

namespace AI
{

template<typename Key, class T, class KeyComp = hash_compare<Key, _STD less<Key> > >
class WORLDGENERAL_API ClassFactory
{
private:
	ClassFactory()
	{
	}
public:
	typedef T(*Creator)();
	void addCreator(const Key & key, Creator pCreator)
	{
		assert(creators.find(key) == creators.end());
		creators[key] = pCreator;
		assert(creators.find(key) != creators.end());
	}
	T  create(const Key & key) const
	{
		Creators::const_iterator itC = creators.find(key);
		if(itC != creators.end())
		{
			return (*itC->second)();
		}
		else
		{
			return NULL;
		}
	}
	static ClassFactory<Key, T, KeyComp> * instance()
	{
		if(instance_ == NULL)
		{
			instance_ = new ClassFactory<Key, T, KeyComp>();
		}
		return instance_;
	}
private:
	typedef stdext::hash_map<Key, Creator, KeyComp> Creators;
	Creators creators;
	static ClassFactory<Key, T, KeyComp> * instance_;
};

template<typename Key, class T, class KeyComp>
ClassFactory<Key, T, KeyComp> * ClassFactory<Key, T, KeyComp>::instance_ = NULL;

}

#endif __AI_ClassFactory__