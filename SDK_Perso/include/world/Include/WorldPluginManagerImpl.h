#pragma once

#include "WorldPluginManager.h"

#include <ed/map.h>

class WorldPlugin;

class IdComparer
{
public:
	inline bool operator ()(const ed::string& op1, const ed::string& op2) const
	{
		return (stricmp(op1.c_str(), op2.c_str()) < 0);
	}
};

// Аппаратно-независимая реализация менеджера ресурсов
class WorldPluginManagerImpl : public WorldPluginManager
{
protected:
	typedef std::multimap<ed::string, WorldPlugin *, IdComparer> pluginList;
	// инициализирован
	bool initialized;
	// список плагинов
	pluginList plugins;
public:
	WorldPluginManagerImpl();
	~WorldPluginManagerImpl();

	// инициализация/деинициализация
	virtual bool Initialize();
	virtual bool IsInitialized();
	virtual void Uninitialize();

	virtual void LoadUserPlugins(Lua::Config & config, std::function<void (const ed::string& pluginPath)> initPlugin);

	// регистрация плагинов (автоматическая)
	virtual void RegisterPlugin(WorldPlugin *plugin);
	virtual void UnregisterPlugin(WorldPlugin *plugin);
	// получение зарегистрированных плагинов
	virtual WorldPlugin *GetPlugin(const ed::string& name);
	virtual bool		 GetPluginIsAvailable(const ed::string& name);

};
