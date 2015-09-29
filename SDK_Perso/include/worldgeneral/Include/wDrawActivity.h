#pragma  once

#include "wActivity.h"
#include "DebugGeometry/GlobalPrimitiveCache.h"
#include "DebugGeometry/PrimitiveContainer.h"


template <class T>
class wDrawActivity : public wActivity<T>
{
public:
	wDrawActivity(T* theHost, wModelTime timeStep0, woHandler function_, const ed::string &name);
	virtual ~wDrawActivity();
	virtual void NextEvent();

private:
	GlobalPrimitiveCache::Cache::size_type _cacheIndex;
};

template <class T>
wDrawActivity<T>::wDrawActivity(T* theHost, wModelTime timeStep0, woHandler function_, const ed::string &name)
:wActivity(theHost, timeStep0, function_, name)
{
	_cacheIndex = GlobalPrimitiveCache::instance()->addCache();
}

template <class T>
wDrawActivity<T>::~wDrawActivity()
{
	GlobalPrimitiveCache::instance()->removeCache(_cacheIndex);
}

template <class T>
void wDrawActivity<T>::NextEvent()
{
	PrimitiveContainer *currentCache = GlobalPrimitiveCache::instance()->setCurrent(_cacheIndex);
	assert(currentCache);
	currentCache->reset();
	wActivity::NextEvent();
}
