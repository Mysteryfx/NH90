#pragma once
#ifdef USE_TERRAIN4
#include "edterrain/ITerrainGraphicsConstants.h"
#include "edTerrain/TerrainContextCamera.h"
#include <memory>

class TerrainRenderer;

namespace Graphics {
	class Context;
};

namespace render {
	class FrameBuffer;
}


class ISurfacePass {
public:
	void parse(TerrainRenderer& terrainRender);
	void draw(TerrainRenderer& terrainRender);
		
	virtual void parseImp(TerrainRenderer& terrainRender) = 0;
	virtual void drawImp(TerrainRenderer& terrainRender) = 0;
	// debug
	virtual bool isEnabled() = 0;
};

inline void ISurfacePass::parse(TerrainRenderer& terrainRender)
{
	if (isEnabled()){
		parseImp(terrainRender);
	}
}

inline void ISurfacePass::draw(TerrainRenderer& terrainRender)
{
	if (isEnabled()){
		drawImp(terrainRender);
	}
}

class SurfacePassBase : public ISurfacePass {
public:
	SurfacePassBase(render::enShadingModelMask parseShdingModel, render::enShadingModel renderShadingModel);

	virtual void parseImp(TerrainRenderer& terrainRender) override;
	virtual void drawImp(TerrainRenderer& terrainRender) override;
	// debug
	virtual bool isEnabled() override;
protected:
	virtual void updateCamera(const Graphics::Context& context);

	render::Camera mCamera;
	render::SampleHandle mSampleHandle;
	render::enShadingModelMask mParseShadingModel;
	render::enShadingModel mRenderShadingModel;

	//DEBUG
	bool mDisable;
};

template <render::enShadingModelMask parseMode, render::enShadingModel renderMode>
class SurfacePassBaseT : public SurfacePassBase
{
public:
	SurfacePassBaseT() :SurfacePassBase(parseMode, renderMode){}
};

#endif