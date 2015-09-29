#ifndef EDTG_RENDER_SURFACE_H
#define EDTG_RENDER_SURFACE_H

#include "edtg.h"
#include "TerrainOptions.h"
#include <osg/Vec3f>
#include <osg/Matrixf>
#include <osg/Polytope>

#ifndef EDGE
#include "Renderer/Fog.h"
#else
#include "Graphics/Light.h"
#include "Graphics/Fog.h"
#endif

namespace edtg
{
// солнце, луна и ambient
struct GlobalLights
{
	osg::Vec3d sunDirectionWS;
	osg::Vec4f sunColor;
	osg::Vec3d moonDirectionWS;
	osg::Vec4f moonColor;
	osg::Vec4f ambientColor;
};

enum enSurfaceRenderType
{
	// соответствуют очередям в edge
#ifdef EDGE_SPLIT_CAMERA
	SURFACE_NEAR_SURFACE      = 0,
	SURFACE_FAR_SURFACE       = 1,
	SURFACE_NEAR_OPAQUE      = 2,
	SURFACE_FAR_OPAQUE       = 3,
	SURFACE_NEAR_TRANSPARENT = 4,
	SURFACE_FAR_TRANSPARENT  = 5,
#else
	SURFACE_SURFACE           = 6,
	SURFACE_FLATSHADOWS       = 7,
	SURFACE_OPAQUE           = 8,
	SURFACE_TRANSPARENT      = 9,
#endif

	SURFACE_LIGHTS           = 10,
	SURFACE_SHELF			 = 11,
	SURFACE_WAVES			 = 12,
	SURFACE_COMPUTE			 = 13
};

struct SurfaceRenderData
{
	typedef edtg::enSurfaceRenderType SurfaceRenderType;
	SurfaceRenderType renderType;

	SurfaceRenderData() {};
	SurfaceRenderData(SurfaceRenderType type):renderType(type) {};

	// for debug
	const char* getRenderTypeName()
	{
		switch(renderType)
		{
			case SURFACE_LIGHTS:           return "SURFACE_LIGHTS";
#ifdef EDGE_SPLIT_CAMERA
			case SURFACE_NEAR_SURFACE:     return "SURFACE_NEAR_SURFACE";
			case SURFACE_FAR_SURFACE:      return "SURFACE_FAR_SURFACE";
			case SURFACE_NEAR_OPAQUE:      return "SURFACE_NEAR_OPAQUE";
			case SURFACE_FAR_OPAQUE:       return "SURFACE_FAR_OPAQUE";
			case SURFACE_NEAR_TRANSPARENT: return "SURFACE_NEAR_TRANSPARENT";
			case SURFACE_FAR_TRANSPARENT:  return "SURFACE_FAR_TRANSPARENT";
#else
			case SURFACE_SURFACE:          return "SURFACE_SURFACE";
			case SURFACE_FLATSHADOWS:      return "SURFACE_FLATSHADOWS";
			case SURFACE_OPAQUE:           return "SURFACE_OPAQUE";
			case SURFACE_TRANSPARENT:      return "SURFACE_TRANSPARENT";
#endif
			case SURFACE_SHELF:			   return "SURFACE_SHELF";
			case SURFACE_WAVES:			   return "SURFACE_WAVES";
			case SURFACE_COMPUTE:		   return "SURFACE_COMPUTE";
		}

		return "Unknown";
	}
};

// Инициализация
EDTERRAINGRAPHICS_API void Init(
    const char* cfgfile,
    const char* rootdir,
    const char* shaderdir
	);
EDTERRAINGRAPHICS_API void Reset();
EDTERRAINGRAPHICS_API void Exit();

// Вызвать после открытия файла земли
EDTERRAINGRAPHICS_API void InitTerrain();

// Рендер итем для земли (реализация скрыта)
typedef struct handle_renderitem* handle_renderitem_t;
EDTERRAINGRAPHICS_API handle_renderitem_t CreateSurfaceRenderItem();
EDTERRAINGRAPHICS_API void DeleteSurfaceRenderItem(handle_renderitem_t);

// Обновить клипмапы
EDTERRAINGRAPHICS_API void UpdateClipmaps(
    const edtg::TerrainOptions& terrainoptions
	);

// Парсинг поверхности
EDTERRAINGRAPHICS_API bool ParseSurface(
    handle_renderitem_t renderitem,
    const osg::Vec3f& origin,
    const osg::Matrixf& view,
    const osg::Matrixf& proj,
	const edtg::GlobalLights& globallights, 
	const edcg::Fog& fog,
    const edtg::TerrainOptions& terrainoptions,
    void* rendercontext=0		// в edge передается edge::Context *ctx
	);

// RenderComputes
EDTERRAINGRAPHICS_API void RenderComputes( 
	handle_renderitem_t renderItem,
	const osg::Vec3f& origin, 
	const osg::Matrixf& view, 
	const osg::Matrixf& proj,
	void* renderContext
	);

// Рендер поверхности
EDTERRAINGRAPHICS_API void RenderSurface(
    handle_renderitem_t renderitem,
    const osg::Vec3f& origin,
    const osg::Matrixf& view,
    const osg::Matrixf& proj,
	const edtg::GlobalLights& globallights, 
	const edcg::Fog& fog,
    void* rendercontext=0,		// в edge передается edge::Context *ctx
    edtg::SurfaceRenderData* renderData = 0
	);

// ForceLoading
EDTERRAINGRAPHICS_API void ForceLoading(const osg::Vec3f& pos, float radius, const char* forceLoadingPipeline, const edtg::TerrainOptions& terrainOptions, bool skipBuildings = false);

/// Loads all bildings in scene.
EDTERRAINGRAPHICS_API void ForceLoadingBuildings();

// Выборка лайтов
#ifdef EDGE
EDTERRAINGRAPHICS_API bool FetchLights(
    handle_renderitem_t renderitem,
    const osg::Vec3f& origin,
    const osg::Matrixf& view,
    const osg::Matrixf& proj,
    const edtg::TerrainOptions& terrainoptions,
    edge::ILightVisitor* visitor,
    void* rendercontext=0		// в edge передается edge::Context *ctx
	);
#endif

// Рендер отладочной информации
EDTERRAINGRAPHICS_API void RenderDebugInfo(void* renderContext, const char* clipmapName);

}
#endif
