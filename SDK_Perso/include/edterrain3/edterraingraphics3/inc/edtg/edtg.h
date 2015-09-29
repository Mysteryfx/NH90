#ifndef EDTG_H
#define EDTG_H

#include <stdint.h>
#include <osg/BoundingBox>
#include <osg/Vec3s>
#include <osg/Vec3f>
#include <osg/Vec3d>
#include <osg/Vec4f>
#include <osg/Vec4d>
#include <osg/Matrixf>
#include <osg/Matrix3>

#ifdef EDGE
#include "graphics/options.h"
#endif

#include "edterraingraphics3_config.h"

// сохраняет для RenderItem имя материала и геометрии
// Позволяет дампить рендер итему идущие на рендер
#ifdef _ED_HIDDEN_DEBUG 
#define DUMPRENDER
#else
#ifdef EDGE
// EDGE
#define DUMPRENDER
#else
// LOCKON
#define DUMPRENDER
#endif
#endif

#endif
