#include <dlfcn.h>		// для загрузки длл

typedef model::IAnimClipManager* (getAnimClipManager_t)(double version);

inline model::AnimClipEntryPoint::AnimClipEntryPoint()
{
	animProviderLoaded = false;
	animProvider_dlhandle = 0;
	animClipManager = 0;
}
inline model::AnimClipEntryPoint::~AnimClipEntryPoint()
{
	close();
}
inline void model::AnimClipEntryPoint::close()
{
	dlclose(animProvider_dlhandle);
	animProviderLoaded = false;
	animProvider_dlhandle = 0;
	animClipManager = 0;
}
inline model::IAnimClipManager* model::AnimClipEntryPoint::instance()
{
	if(!this->animProviderLoaded)
	{
		this->animProviderLoaded = true;
		this->animProvider_dlhandle = dlopen("edAnimClip.dll", RTLD_NOW | RTLD_GLOBAL); //
		if(this->animProvider_dlhandle)
		{
			void* proc = dlsym(this->animProvider_dlhandle, "getAnimClipManager");
			if(proc)
			{
				getAnimClipManager_t* getAnimClipManager = (getAnimClipManager_t*)proc;
				this->animClipManager = getAnimClipManager(IAnimClipManagerVersion);
			}
		}
	}
	return this->animClipManager;
}

