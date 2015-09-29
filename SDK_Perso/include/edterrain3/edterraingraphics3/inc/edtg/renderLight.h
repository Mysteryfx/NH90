#ifndef EDTG_RENDER_LIGHT_H
#define EDTG_RENDER_LIGHT_H

#include "edtg.h"
#include "TerrainOptions.h"
#include <osg/Vec3f>
#include <osg/Matrixf>
#include <osg/Polytope>
#include "edtg/LightRenderData.h"

namespace edtg
{

typedef struct handle_renderitem* handle_renderitem_t;
EDTERRAINGRAPHICS_API handle_renderitem_t CreateLightRenderItem();
EDTERRAINGRAPHICS_API void DeleteLightRenderItem(edtg::handle_renderitem_t lightRenderItem);

EDTERRAINGRAPHICS_API void ParseLight(
    handle_renderitem_t renderItem,
    ed::vector<LightRenderData> &lights,
    const osg::Vec3f& origin,
    const osg::Matrixf& view,
    const osg::Matrixf& proj,
    const edtg::TerrainOptions& terrainOptions);

EDTERRAINGRAPHICS_API void RenderLight(
    handle_renderitem_t renderItem,
    const osg::Vec3f& origin,
    const osg::Matrixf& view,
    const osg::Matrixf& proj);

// Подготовка view матрицы для лайтов
EDTERRAINGRAPHICS_API void PrepareLightViewMatrix(
    osg::Matrixf& out_view,
    osg::Matrixf& out_proj,
    const osg::Vec3f& origin,
    const osg::Matrixf& view,
    const osg::Matrixf& proj
    );

// создание ttf матрицы
EDTERRAINGRAPHICS_API void GetTTFMatrix(
    osg::Matrixf& out_ttf,
    const osg::Matrixf& view,
    const osg::Matrixf& projection);

};

#endif