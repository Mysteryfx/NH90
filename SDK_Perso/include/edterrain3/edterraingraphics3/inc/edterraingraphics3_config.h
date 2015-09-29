#ifndef _EDTERRAINGRAPHICS33_CONFIG_H__
#define _EDTERRAINGRAPHICS33_CONFIG_H__

#ifdef EDGE

// EDGE
#include "core/io/mmfStream.h"
#include "osg/BoundingBox"
#include "osg/Vec3f"
#include "osg/Matrixd"
#include "renderer/IRenderAPI.h"
#include "rwrappers/renderUserAPI.h"

#include "edterrain/landscape3/lGeometry.h"

#include "Core/Lua/Config.h"
#include "Core/Lua/State.h"

#ifdef EDTERRAINGRAPHICS_EXPORTS
#define EDTERRAINGRAPHICS_API __declspec(dllexport)
#else
#define EDTERRAINGRAPHICS_API __declspec(dllimport)
#endif

#else
// DCS
#ifdef EDTERRAINGRAPHICS_EXPORTS
#define EDTERRAINGRAPHICS_API __declspec(dllexport)
#else
#define EDTERRAINGRAPHICS_API __declspec(dllimport)
#endif

#include "Lua/State.h"
#include "Lua/Config.h"

#include "ed_log.h"

#endif


#endif
