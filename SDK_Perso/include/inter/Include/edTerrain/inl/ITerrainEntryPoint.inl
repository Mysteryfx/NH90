#include <dlfcn.h>		// для загрузки длл
#include "Lua/State.h"
#include "Lua/Config.h"
#include "Lua/Helper.h"
#include "ed_vfs.h"
#include "ed_log.h"
#include "Util/Strings.h"

typedef edterrain::ITerrain* (createITerrain_t)(double version);

inline edterrain::ITerrainEntryPoint::ITerrainEntryPoint()
{
	provider_dlhandle = 0;
	terrain = 0;
}
inline edterrain::ITerrainEntryPoint::~ITerrainEntryPoint()
{
	close();
}
inline void edterrain::ITerrainEntryPoint::close()
{
	if( terrain)
		terrain->release();
	dlclose(provider_dlhandle);
	provider_dlhandle = 0;
	terrain = 0;
}
inline edterrain::ITerrain* edterrain::ITerrainEntryPoint::openTerrain(const char* filename, lua_State* options)
{
	this->close();

	ed::string dll = ITerrain::RequiredTerrainDll(filename);
	if( dll.empty())
		return nullptr;

	this->provider_dlhandle = dlopen(dll.c_str(), RTLD_NOW | RTLD_GLOBAL); //
	if(this->provider_dlhandle)
	{
		void* proc = dlsym(this->provider_dlhandle, "CreateITerrain");
		if(proc)
		{
			createITerrain_t* createITerrain = (createITerrain_t*)proc;
			this->terrain = createITerrain(ITerrainVersion);
			if( this->terrain->openTerrain(filename, options))
			{
				return this->terrain;
			}
		}
		this->close();
	}

	return nullptr;
}
inline edterrain::ITerrain* edterrain::ITerrainEntryPoint::instance()
{
	return terrain;
}

// RequiredTerrainDll
inline ed::string edterrain::ITerrain::RequiredTerrainDll(const char* filename)
{
	const char* LOCAL_APPDATA = VFS_get_appdatadir();

	ed::string dirname = ed::extractFolder(filename);
	Lua::State L;
	luaL_openlibs(L);
	Lua::Config setup(L);
	setup.create("options");
	setup.set("season", "");
	setup.set("quality", "");
	setup.set("lang", "");
	setup.set("dir", dirname.c_str());
	setup.set("userdir", (std::string(LOCAL_APPDATA)+"/edterrain").c_str());
	setup.pop();

	ed::string libpath = ed::string(filename) + ".dll";
	if (!luaLoadFileWithDynamicLib(filename, libpath.c_str(), "loadTerrainConfig", L))
	{
		ED_ERROR("Can\'t load terrain cfg \"%s\": %s", filename, lua_tostring(L, -1));
		return "";
	}
	float edterrainVersion = setup.get("edterrainVersion", 4.1f);
	if( edterrainVersion==3)
	{
		return "edterrain.dll";
	}
	else if( edterrainVersion==4.1f)
	{
		return "edterrain4.dll";
	}
	else
	{
		ED_ERROR("unsupported edterrain version = %d", edterrainVersion);
	}
	return "";
}
