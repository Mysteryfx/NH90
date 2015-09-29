#pragma once
#include "../../inter.h"
#include "osg/Matrixd"

namespace edterrain
{

// Подготовка камеры для light map pass
void INTER_API makeLightMapCamera(
    const osg::Matrixd& view, const osg::Matrixd& proj,
    osg::Matrixd& out_view, osg::Matrixd& out_proj
	);

}