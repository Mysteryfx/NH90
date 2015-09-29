#ifndef __TERRAIN_CFG_H_
#define __TERRAIN_CFG_H_

/*/
#include "lTerraDispatch.h"
#include "Resourcer\ResourceManager.h"
#include "smSceneManager.h"
#include "viObjectManager.h"
//#include "lLandRoadManager.h"
// Чтение файла terrain.cfg.lua
// И загрузка земли, накладных, дорог, сцены
bool EDTERRAIN_API LoadTerrainCfg(
	const char* directoryOrFile, 
	const char* season, 
	const char* quality, 
	const char* lang,
	lTerraDispatch* pTerraDispatch, 
	landscape::lSurfaceDataManager* pSurfaceDataManager,
	ResourceManager* resourcer, 
	viObjectManager* pObjectManager, 
	smSceneManager* visualizer
	);
/*/

#endif
