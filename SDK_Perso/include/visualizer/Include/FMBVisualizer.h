#ifndef FmbVisualizer_H
#define FmbVisualizer_H

#include "Visualizer.h"
#include "landscape4/fmb/FrameModelBufferDispatcher.h"
#include "smCamera_Implement.h"
#include "visearch.h"

class SceneManager_Implement;

class VISUALIZER_API FmbVisualizer
{
public:
	FmbVisualizer();
	~FmbVisualizer();

	void Init();
	void Render(SceneManager_Implement* pSceneManager);
protected:
	fmb::FrameModelBufferDispatcher fmbDispatcher;

	int DrawObjects(viFlag mask, ClipVolume *volume, viSearchHandler func, fmb::IFrameModelBuffer* fmb);
	
};

#endif // FmbVisualizer_H
