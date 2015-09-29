#pragma once

#include "World.h"
#include <ed/string.h>

// Интерфейс для плагинов

class WORLD_API WorldPlugin
{
protected:
	ed::string name;
public:
	WorldPlugin(const ed::string& name);
	virtual ~WorldPlugin();

	const ed::string& GetName();
    virtual bool  Initialize();
    virtual void  Uninitialize();
	virtual bool  Initialized();
	virtual bool  KillMe() { return false; } ; // automatic kill after Uninitialize
	virtual void* GetInterface(const char * name_of_interface = nullptr) = 0;
	virtual void* GetLibraryHandle(const char * name_of_interface = nullptr)
	{
		return nullptr;
	}
	virtual const char * GetPath() const { return nullptr; };
};





