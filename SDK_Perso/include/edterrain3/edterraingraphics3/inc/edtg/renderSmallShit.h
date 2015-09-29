#ifndef EDTG_RENDER_SMALL_SHIT_H
#define EDTG_RENDER_SMALL_SHIT_H

#include "edtg.h"
#include "TerrainOptions.h"
#include <osg/Vec3f>
#include <osg/Matrixf>
#include <osg/Polytope>

namespace edtg
{

typedef struct handle_renderitem* handle_renderitem_t;
EDTERRAINGRAPHICS_API handle_renderitem_t CreateSmallShitRenderItem();
EDTERRAINGRAPHICS_API void DeleteSmallShitRenderItem(edtg::handle_renderitem_t smallShitRenderItem);

EDTERRAINGRAPHICS_API void InitSmallShit(
    handle_renderitem_t renderItem,
    edtg::TerrainOptions& terrainOptions);

EDTERRAINGRAPHICS_API void ParseSmallShit(
    handle_renderitem_t renderItem,
    const osg::Vec3f& origin,
    const osg::Vec3f& cameraPosition,
    const edtg::TerrainOptions& terrainOptions,
    void* context);

EDTERRAINGRAPHICS_API void RenderSmallShit(
    handle_renderitem_t renderItem,
    handle_renderitem_t surfaceRenderItem);
};

#endif