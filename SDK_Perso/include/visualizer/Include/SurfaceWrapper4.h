#pragma once
#ifdef USE_TERRAIN4
#include "SurfacePass.h"

typedef SurfacePassBaseT<render::SHMM_FINALCOLOR,    render::SHM_FINALCOLOR>     MainSurfacePass;
typedef SurfacePassBaseT<render::SHMM_ENVIRONMENTMAP,render::SHM_ENVIRONMENTMAP> EnvironmentSurfacePass; 
typedef SurfacePassBaseT<render::SHMM_REFLECTIONMAP, render::SHM_REFLECTIONMAP>  ReflectionSurfacePass; 
typedef SurfacePassBaseT<render::SHMM_SHELFMAP,      render::SHM_SHELFMAP>       RefractionSurfacePass;
typedef SurfacePassBaseT<render::SHMM_FLATSHADOWS,   render::SHM_FLATSHADOWS>    FlatShadowsSurfacePass;
typedef SurfacePassBaseT<render::SHMM_TRANSPARENT,   render::SHM_TRANSPARENT>    TransparentSurfacePass;
typedef SurfacePassBaseT<render::SHMM_RADAR,         render::SHM_RADAR>          RadarSurfacePass;



//-------------------------------------------InstancerSurfacePass-----------------------------------------

class InstancerSurfacePass : public SurfacePassBaseT<render::SHMM_COMPUTE, render::SHM_COMPUTE> 
{
public:
	void drawImp(TerrainRenderer& terrainRender) override;
};

//-------------------------------------------LightMapSurfacePass------------------------------------------

class LightMapSurfacePass : public SurfacePassBaseT<render::SHMM_LIGHTMAP, render::SHM_LIGHTMAP>
{
public:
	LightMapSurfacePass(render::FrameBuffer* frameBuffer) : mLightMapFrameBuffer(frameBuffer) {};

	void updateCamera(const Graphics::Context& context) override;
	void drawImp(TerrainRenderer& terrainRenderer) override;
	void parseImp(TerrainRenderer& terrainRender) override;
private:
	render::FrameBuffer* mLightMapFrameBuffer;
	render::Camera mParentCamera;
};

//-------------------------------------------HeightMapSurfacePass-------------------------------------------

class HeightMapSurfacePass : public SurfacePassBaseT<render::SHMM_HEIGHTMAP, render::SHM_HEIGHTMAP> {
public:
	HeightMapSurfacePass(render::FrameBuffer* frameBuffer) : mHeightMapFrameBuffer(frameBuffer){}

	void updateCamera(const Graphics::Context& context) override;
	void drawImp(TerrainRenderer& terrainRenderer) override;
	void parseImp(TerrainRenderer& terrainRender) override;
private:
	render::FrameBuffer* mHeightMapFrameBuffer;
	render::Camera mParentCamera;
};

//-------------------------------------------MapSurfacePass--------------------------------------------------

class MapSurfacePass : public SurfacePassBase
{
public:	
	MapSurfacePass() : SurfacePassBase(render::SHMM_MAP, render::SHM_MAP), mMapMode(MapMode::NONE){}

	enum class MapMode{NONE, MAP = 6, MAPALT, MAPTEX };
	void setMode(const MapMode& mode); 	
private:
	render::enShadingModel renderShaingModel(const MapMode& mode) const;
	render::enShadingModelMask parseShadingModel(const MapMode& mode) const;
	MapMode mMapMode;
};

//-------------------------------------------OceanPass-----------------------------------------------------
namespace water { class Ocean; }
class OceanPass : public ISurfacePass
{
public:
	OceanPass(water::Ocean* ocean) :mOcean(ocean), mDisable(false){};

	void parseImp(TerrainRenderer& terrainRender) override;
	void drawImp(TerrainRenderer& terrainRender) override;
	bool isEnabled(){ return !mDisable; }
private:	
	water::Ocean* mOcean;
	bool mDisable;
};

//-------------------------------------------SurfaceWrapper------------------------------------------------

class SurfaceWrapper : public ISurfacePass
{
public:
	SurfaceWrapper() :mDisable(false){};
	void addPass(std::shared_ptr<ISurfacePass> surfacePass);

	void parseImp(TerrainRenderer& terrainRender) override;
	void drawImp(TerrainRenderer& terrainRender) override;
	bool isEnabled();
private:
	ed::vector<std::shared_ptr<ISurfacePass>> mSurfacePass;
	bool mDisable;
};
#endif
