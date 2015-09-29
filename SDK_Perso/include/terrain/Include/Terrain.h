#pragma once

#ifdef TERRAIN_EXPORTS
#define TERRAIN_API __declspec(dllexport)
#else
#define TERRAIN_API __declspec(dllimport)
#endif
