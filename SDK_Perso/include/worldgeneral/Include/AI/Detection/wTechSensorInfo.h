#pragma once

#include "AI\Detection\wSensorInfo.h"
#include "AI\Detection\wAngularVolume.h"

namespace Lua
{
	class Config;
}

class wTechSensorInfo;
typedef wTechSensorInfo* (*wTechSensorInfoCreator)();

class wTechSensorInfo : public wSensorInfo
{
public:
	wTechSensorInfo();
	~wTechSensorInfo();
	virtual void load_from_state(Lua::Config & config);
	//
	virtual const wAngularVolume *	get_scan_volume(const wSensorState & sensor_state) const;
	virtual const wAngularVolume *	get_volume(const wSensorState & sensor_state) const;
	//
	static wTechSensorInfo* create_from_state(Lua::Config & config);
	static wTechSensorInfoCreator creators[SENSOR_TYPE_MAX];
protected:
	wAngularVolumePyramidRot scan_volume;
};
