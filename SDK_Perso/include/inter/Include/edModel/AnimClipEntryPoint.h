#ifndef AnimClipEntryPoint_H__
#define AnimClipEntryPoint_H__

#include "IAnimClipManager.h"

namespace model
{

// враппер для загрузки edAnimClip.dll и доступа к IAnimClipManager
class AnimClipEntryPoint
{
	bool animProviderLoaded;
	void* animProvider_dlhandle;
	model::IAnimClipManager* animClipManager;
public:
	AnimClipEntryPoint();
	~AnimClipEntryPoint();
public:
	model::IAnimClipManager* instance();
	void close();
};

}

#include "inl/AnimClipEntryPoint.inl"

#endif
