#pragma once
#include <memory>
#include "Visualizer.h"
#include "edTerrain/ITerrainGraphicsConstants.h"
#include "TerrainParseContext.h"
#include "edterrain/ITerrainGraphics.h"
#include "edterrain/TerrainContextCamera.h"
#include "Lua/State.h"

namespace render
{
	class IRenderContext;
	class IParseContext;
	class IRenderPass;
	class Camera;
	class StructuredBuffer;
	class IDump;
}

namespace edterrain {
	class ITerrainGraphics;
}

class TerrainRenderContext;

class VISUALIZER_API TerrainRenderer {
public:
	TerrainRenderer();
	~TerrainRenderer();

	bool initialize();
	void release();

	bool forceLoading(const osg::Vec3d& pt, double radius);

	void parse(/*const render::IParseContext& context*/);
	void render(const render::SampleHandle& sampleHandle, render::enShadingModel shadingModel);
	void render(const render::SampleHandle& sampleHandle, render::enShadingModel shadingModel, const render::Camera& camera);
	void renderInstanced(const render::SampleHandle& sampleHandle);
	void renderInstanced(const render::SampleHandle& sampleHandle, const render::Camera& camera);
	void updateSharedParams();
	void updateLightMapCamera(const render::Camera& lightMapCamera);
	void updateHeightMapCamera(const render::Camera& hegihtMapCamera);
	void setStructuredBuffer(render::SharedStructuredBufferHandle handle, render::StructuredBuffer &buf);
	osg::BoundingBoxd getBoundingBox(const render::SampleHandle& sample);
	render::IDump* getDump() const;
	void startDumpOffshore() const;
	void setCivilLights(double civilLight);

	//
	render::TerrainParseContext* getCurrentParseContext(){ return mCurrentParseContext.get(); }
private:
	// 
	void updateRenderContext(const render::SampleHandle& sampleHandle, render::enShadingModel shadingModel);
	TerrainRenderContext* getRenderContext();
	
	edterrain::ITerrainGraphicsEntryPoint mTerrainGraphicsEntryPoint;
	edterrain::ITerrainGraphics* mTerrainGraphics;
	std::unique_ptr<render::TerrainParseContext> mCurrentParseContext;
	std::unique_ptr<render::IRenderPass> mPassInstancer;
	std::unique_ptr<TerrainRenderContext> mRenderContext;
	std::unique_ptr<render::IDump> mDump;
	//
	Lua::State mTerrainOptionsLua;

};

// helper functions
namespace Graphics { class Context; }
render::Camera makeTerrainCameraFromContext(const Graphics::Context& context);