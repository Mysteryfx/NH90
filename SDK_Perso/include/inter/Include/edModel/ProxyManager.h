#ifndef INTER_ED_MODEL_PROXY_MANAGER_H
#define INTER_ED_MODEL_PROXY_MANAGER_H

#include "ed/vector.h"

namespace model
{

template<typename Manager, typename Res>class ProxyManager : public T
{
private:
	ed::vector<Manager*> managers;
public:
	inline Res* open(const char* name);
};

}

#include <inl/ProxyManager.inl>

#endif 
