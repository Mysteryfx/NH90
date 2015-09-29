#pragma once

#include "World.h"

class WorldPlugin;

#include <ed/string.h>
#include <functional>

namespace Lua
{
	class Config;
}

// Интерфейс менеджера плагинов мира
class WORLD_API WorldPluginManager
{
public:
	virtual ~WorldPluginManager() {};

	// инициализация/деинициализация
	virtual bool Initialize() = 0;
	virtual bool IsInitialized() = 0;
	virtual void Uninitialize() = 0;

	virtual void LoadUserPlugins(Lua::Config & config, std::function<void (const ed::string& pluginPath)> initPlugin) = 0;

	// получение зарегистрированных плагинов
	virtual WorldPlugin *GetPlugin(const ed::string& name) = 0;
	virtual bool		 GetPluginIsAvailable(const ed::string& name) = 0;
};

extern WORLD_API WorldPluginManager *worldPluginManager;
