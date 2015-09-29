#pragma once

#include <ed/string.h>

#include "cLinear.h"
#include "landscape3/lPathFile.h"
#include "edTerrain/PointOnSurfaceAdapter.h"

#include "WorldGeneral.h"

#include "wTime.h"
#include "Randomizer\RandomObject.h"
#include "wMovingObject.h"

#include "AI\Detection\wCommonDefinitions.h"

namespace Lua
{
	class Config;
}

namespace landscape
{
	class lPointOnSurface;
}

class wAngularVolume;
struct wTargetDetectionInfo;

class WORLDGENERAL_API wSensorInfo
{
public:
	//Sensor types
	typedef enum wSensorType
	{
		SENSOR_VOID = -1,
		SENSOR_OPTICAL,
		SENSOR_RADAR,
		SENSOR_IRST,
		SENSOR_RWR,
		SENSOR_TYPE_MAX
	};
	//Sensor modes
	typedef enum wSensorMode
	{
		SENSOR_MODE__VOID = -1,
		SENSOR_MODE_IS_OFF, //The sensor is not active
		SENSOR_MODE_FREE_SCAN, //The sensor is scanning
		SENSOR_MODE_CENTERED_SCAN, //The sensor is scanning around the target
		SENSOR_MODE_TRACKING //The sensor is focused on the target
	};
	//Contact result flags
	typedef enum ContactResult
	{
		RESULT_NOT_VISIBLE			= 0,
		RESULT_VISIBLE				= 1 << 0, //The target is visible now
		RESULT_CAN_BE_VISIBLE		= 1 << 1, //The target can be visible in better position of the detector
		RESULT_TRACKING				= 1 << 2, //The target is tracking the detector by radar or the detector can track the target by radar
		RESULT_DISTANCE_IS_KNOWN	= 1 << 3, //The distance to the target is known
		RESULT_TYPE_IS_RECOGNIZED	= 1 << 4 //The type of the target is known
	};
	//The state of the the sensor instance
	struct wSensorState
	{
		wSensorState();
		wSensorState(const wAngularVolume * canopy_geometry_, wSensorInfo::wSensorMode sensor_mode_, woPointer	target_of_interest_, unsigned int crew_);
		//
		const wAngularVolume*		canopy_geometry;
		wSensorInfo::wSensorMode	sensor_mode;
		woPointer					target_of_interest;
		unsigned int				crew;
	};
	wSensorInfo();
	~wSensorInfo();

	//Loading
	virtual void load_from_state(Lua::Config & config) {;}

	//Returns scan volume of the sensor for the given mode
	virtual const wAngularVolume * get_scan_volume(const wSensorState & sensor_state) const = 0;

	//Returns instant FOV
	virtual const wAngularVolume * get_volume(const wSensorState & sensor_state) const = 0;

	//Check if the sensor provides distance to the target
	virtual bool				   provideDistance() const = 0;

	//Is the target in the scan volume
	virtual bool is_in_scan_volume(	woPointer target_ptr, woPointer self_ptr,
									const wSensorState & sensor_state) const;

	//Is the position of the target in the scan volume
	virtual bool is_in_scan_volume(	woPointer target_ptr, const cVector & dir_to_target_local, const cVector & dir_to_target,
									woPointer self_ptr, const wSensorState & sensor_state) const;

	//Poisson event flow
	static bool check_detected_by_time(wModelTime dtime, wModelTime average_time_to_detection);

public:

	//Return target detection distance and detection flags
	virtual int get_detection_dist(	const wTargetDetectionInfo & target_det_info, wModelTime dtime,
									wSensorMode sensor_mode, float & dist) const = 0;

	//Checks if the target is visible (scan volume + LOS + distance) and returns target detection result as. No time is required for detection
	virtual int check_target(	const wTargetDetectionInfo & target_det_info, wModelTime dtime,
								const wSensorState & sensor_state) const = 0;

	//Even if the target is visible it may require time to detect it. The function checks if the target is detected during the given time
	virtual int search_target(	const wTargetDetectionInfo & target_det_info, bool target_is_visible, wModelTime dtime,
								const wSensorState & sensor_state) const = 0;

	//The function checks if the shooter is detected during the given time
	virtual int check_shooter(	const wTargetDetectionInfo & target_det_info, wModelTime dtime,
								const wSensorState & sensor_state, const wsType & weaponType, void * weaponData, ObjectID shooterID) const { return wSensorInfo::RESULT_NOT_VISIBLE;}

	//Returns sensor type
	virtual wSensorType get_sensor_type() const = 0;

	ed::string name;
	//
	static const char * sensor_type_names[SENSOR_TYPE_MAX];
	static RandomObject random_evenly;
protected:
	bool multiple_targets_tracking;
	static bool is_in_angular_volume(const cVector & dir_to_target_abs, const wAngularVolume & volume, const cPosition & volume_pos);
};

struct wTargetDetectionInfo
{
	wTargetDetectionInfo();
#ifdef _ED_HIDDEN_DEBUG 
	ed::string selfObjType;
	ed::string targetObjType;
#endif
	woPointer	target_ptr;
	woPointer	self_ptr;
#ifndef USE_TERRAIN4
	landscape::lPointOnSurface * pSelfSurfInfo;
#else
	edterrain::PointOnSurfaceAdapter * pSelfSurfInfo;
#endif
	cVector		dir_to_target;
	float		distance_to_target;
	float		distance_to_target_hor;
	float		target_azimuth;
	float		target_elevation;
	bool		air_background;
	float		target_horizon_elevation;
	cVector		dir_to_target_local;
	cVector		target_relative_speed;
	cPosition	LOS_position;
	cVector		target_relative_speed_on_LOS;
	cPoint		ground_point;
	cVector		ground_point_relative_speed_on_LOS;
	bool		LOS;
	bool		LOS_with_objects;
	float		target_size;
	float		tan_target_angular_size;
	BACKGROUND_TYPE background_type;
	bool		nearest_detected_target_flag;
	cPoint		nearest_detected_target_pos;
	RadarMode	targetRadarMode;
	float		targetRadarDetectionDistance;
};

bool isFlag(Lua::Config & config, const char * name);

class wDetectionModelParams
{
public:
	enum Test
	{
		FLAG_VOID = 0,
		FLAG_TERRAIN_LOS = 1 << 1,
		FLAG_EARTH_CURVATURE_LOS = 1 << 2,
		FLAG_OBJECTS_LOS = 1 << 3,
		FLAG_TREES_LOS = 1 << 4,
		FLAG_SUN_LIGHT_SOURCE = 1 << 5,
		FLAG_OTHER_LIGHT_SOURCES = 1 << 6,
		FLAG_BACKGROUND = 1 << 7,
		FLAG_DEMASKING_BY_NEAR_DETECTED_TARGET = 1 << 8,
		FLAG_RADAR = 1 << 9,
		FLAG_ALL = 0xFFFFFFFF
	};
	virtual void clear() {}
	virtual void load_from_state(Lua::Config & config) {;}
	//
	virtual unsigned int get_flags(wSensorInfo * sensor_info = NULL) const = 0;
};

template<class DetectionModelParamsType>
struct FlagPair
{
	typename DetectionModelParamsType::Test	innerFlag;
	typename wDetectionModelParams::Test	outerFlag;
};

template<class DetectionModelParamsType, size_t size>
unsigned int exportFlags(unsigned int innerFlags, const FlagPair<DetectionModelParamsType> (&flagPair)[size])
{
	unsigned int outerFlags = wDetectionModelParams::FLAG_VOID;
	for(unsigned int i = 0; i < size; i++)
		if(innerFlags & flagPair[i].innerFlag)
			outerFlags |= flagPair[i].outerFlag;
	return outerFlags;
}

class WORLDGENERAL_API wDetectionModelParamsLoader
{
public:
	typedef ed::vector<wSensorInfo*> wSensorInfoVector;
	typedef std::pair<wSensorInfo*,  ed::vector<wSensorInfoVector> > wAllSensorsInfo;
	wDetectionModelParamsLoader();
	void load(const ed::string & file_name);
	void clear();
	unsigned int get_flags(const wAllSensorsInfo & all_sensors_info) const;
	//
	wDetectionModelParams * visual_detection_model_params;
	wDetectionModelParams * sensor_detection_model_params[wSensorInfo::SENSOR_TYPE_MAX];
	//
	static wDetectionModelParamsLoader * instance;
};
