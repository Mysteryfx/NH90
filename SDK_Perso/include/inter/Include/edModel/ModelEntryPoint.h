#ifndef ModelEntryPoint_H__
#define ModelEntryPoint_H__

#include <ed/string.h>

namespace model
{

// враппер для загрузки разных менеджеров модели
template <typename ModelManager> class ModelEntryPoint
{
	ed::string dllname, entrypoint;

	bool edmProviderLoaded;
	void* edmProvider_dlhandle;
	ModelManager* edmManager;
public:
	ModelEntryPoint(const char * dllname=0, const char * entrypoint=0);
	~ModelEntryPoint();
public:
	ModelManager* instance();
	void close();
};

}

#include "inl/ModelEntryPoint.inl"

#endif
