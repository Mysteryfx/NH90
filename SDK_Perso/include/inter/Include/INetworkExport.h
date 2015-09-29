#pragma once

#include "Inter.h"
#include "WorldPlugin.h"

class INTER_API INetworkExport : public WorldPlugin
{
public:
	INetworkExport(const ed::string& name) : WorldPlugin(name) {}
	virtual ~INetworkExport() = 0 {}
	
	virtual void Start() = 0 {}
	virtual void Stop() = 0 {}
	virtual void BeforeNextFrame() = 0 {}
	virtual void AfterNextFrame() = 0 {}
};

extern INTER_API INetworkExport *globalNetworkExport;
