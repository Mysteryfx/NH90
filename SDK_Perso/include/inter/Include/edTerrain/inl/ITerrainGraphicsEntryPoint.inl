#include <dlfcn.h>		// для загрузки длл
#include "Lua/State.h"
#include "Lua/Config.h"
#include "../ITerrain.h"

typedef edterrain::ITerrainGraphics* (createITerrainGraphics_t)(edterrain::ITerrain* terrain, lua_State* options, double version);

namespace edterrain
{
	ed::string RequiredTerrainGraphicsDll(ITerrain* terrain);
}

inline edterrain::ITerrainGraphicsEntryPoint::ITerrainGraphicsEntryPoint()
{
	provider_dlhandle = 0;
	terrainGraphics = 0;
}
inline edterrain::ITerrainGraphicsEntryPoint::~ITerrainGraphicsEntryPoint()
{
	close();
}
inline void edterrain::ITerrainGraphicsEntryPoint::close()
{
	if( terrainGraphics)
		terrainGraphics->release();
	dlclose(provider_dlhandle);
	provider_dlhandle = 0;
	terrainGraphics = 0;
}
inline edterrain::ITerrainGraphics* edterrain::ITerrainGraphicsEntryPoint::createTerrainGraphics(ITerrain* terrain, lua_State* options)
{
	this->close();

	ed::string dll = edterrain::RequiredTerrainGraphicsDll(terrain);
	if( dll.empty())
		return nullptr;

	this->provider_dlhandle = dlopen(dll.c_str(), RTLD_NOW | RTLD_GLOBAL); //
	if(this->provider_dlhandle)
	{
		void* proc = dlsym(this->provider_dlhandle, "CreateITerrainGraphics");
		if(proc)
		{
			createITerrainGraphics_t* createITerrainGraphics = (createITerrainGraphics_t*)proc;
			this->terrainGraphics = createITerrainGraphics(terrain, options, ITerrainGraphicsVersion);
			if( this->terrainGraphics)
			{
				return this->terrainGraphics;
			}
		}
		this->close();
	}

	return nullptr;
}
inline edterrain::ITerrainGraphics* edterrain::ITerrainGraphicsEntryPoint::instance()
{
	return terrainGraphics;
}

inline ed::string edterrain::RequiredTerrainGraphicsDll(edterrain::ITerrain* terrain)
{
	Lua::Config setup(terrain->getTerrainCfg());

	float edterrainVersion = setup.get("edterrainVersion", 4.1f);
	if( edterrainVersion==3)
	{
		return "edterrainGraphics3.dll";
	}
	else if( edterrainVersion==4.1f)
	{
		return "edterrainGraphics41.dll";
	}
	else
	{
		ED_ERROR("unsupported edterrain version = %d", edterrainVersion);
	}
	return "";
}
