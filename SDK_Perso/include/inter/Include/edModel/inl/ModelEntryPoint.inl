#include "edModel/IGraphModelManager.h"
#include "edModel/IModelManager.h"

#include <dlfcn.h>		// для загрузки длл

template <typename ModelManager> struct ModelManagerTraits{};

template <> struct ModelManagerTraits<model::IGraphModelManager>{
	static const char *getDllName(){return "edModelGraph.dll";}
	static const char *getManagerName(){return "getGraphModelManager";}
	typedef model::IGraphModelManager*(getModelManager_t)(void);
};

#ifdef EDGE
template <> struct ModelManagerTraits<model::IModelManager>{
	static const char *getDllName(){return "edModel.dll";}
	static const char *getManagerName(){return "getModelManager";}
	typedef model::IModelManager*(getModelManager_t)(void);
};
#else
template <> struct ModelManagerTraits<model::IModelManager>{
	static const char *getDllName(){return "NGModel.dll";}
	static const char *getManagerName(){return "getModelManager";}
	typedef model::IModelManager*(getModelManager_t)(void);
};

#endif

template <typename ModelManager> model::ModelEntryPoint<ModelManager>::ModelEntryPoint(const char * dllname, const char * entrypoint)
{
	edmProviderLoaded = false;
	edmProvider_dlhandle = 0;
	edmManager = 0;

	this->dllname = ModelManagerTraits<ModelManager>::getDllName();
	if( dllname)
	{
		this->dllname = dllname;
	}
	this->entrypoint = ModelManagerTraits<ModelManager>::getManagerName();
	if( entrypoint)
	{
		this->entrypoint = entrypoint;
	}
}

template <typename ModelManager> model::ModelEntryPoint<ModelManager>::~ModelEntryPoint()
{
	close();
}

template <typename ModelManager> void model::ModelEntryPoint<ModelManager>::close()
{
	if( edmProvider_dlhandle)
		dlclose(edmProvider_dlhandle);
	edmProviderLoaded = false;
	edmProvider_dlhandle = 0;
	edmManager = 0;
}

template <typename ModelManager> ModelManager* model::ModelEntryPoint<ModelManager>::instance()
{
	if(!this->edmProviderLoaded)
	{
		this->edmProviderLoaded = true;
		this->edmProvider_dlhandle = dlopen(this->dllname.c_str(), RTLD_NOW | RTLD_GLOBAL); //
		if(this->edmProvider_dlhandle)
		{
			void* proc = dlsym(this->edmProvider_dlhandle, this->entrypoint.c_str());
			if(proc)
			{
				typename ModelManagerTraits<ModelManager>::getModelManager_t* getModelManager = 
					(typename ModelManagerTraits<ModelManager>::getModelManager_t*)proc;
				this->edmManager = getModelManager();
			}
		}
	}
	return this->edmManager;
}

