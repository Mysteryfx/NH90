#pragma once

#include "WorldGeneral.h"

#include "AI\Detection\wVisualDetector.h"

#include <ed/unordered_map.h>

class wTechSensorInfo;

class WORLDGENERAL_API wSensorInfoStorage
{
public:
	typedef ed::unordered_map<ed::string, wTechSensorInfo*> SensorsInfo;
	wSensorInfoStorage();
	~wSensorInfoStorage();
	void init();
	void clear();
	wTechSensorInfo * get_sensor_info(const ed::string & name);

	static wSensorInfoStorage instance;
private:
	SensorsInfo sensors_map;
};