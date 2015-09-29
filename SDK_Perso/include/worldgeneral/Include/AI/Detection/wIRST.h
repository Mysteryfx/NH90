#pragma once

#include "AI\Detection\wTechSensorInfo.h"

class wIRSTInfo : public wTechSensorInfo
{
public:
	typedef enum wEngineStatus {ENGINE_MODE_FORSAGE, ENGINE_MODE_MAXIMAL, ENGINE_MODE_MINIMAL, ENGINE_MODE_MAX};
	//
	wIRSTInfo();
	virtual void load_from_state(Lua::Config & config);
	//
	virtual const wAngularVolume * get_volume(wSensorMode sensor_mode = SENSOR_MODE_FREE_SCAN) const;
	virtual bool				   provideDistance() const { return laserRanger; }
	virtual int get_detection_dist(	const wTargetDetectionInfo & target_det_info, wModelTime dtime,
									wSensorMode sensor_mode, float & dist) const;
	virtual int check_target(	const wTargetDetectionInfo & target_det_info, wModelTime dtime, 
								const wSensorState & sensor_state) const;
	virtual int search_target(	const wTargetDetectionInfo & target_det_info, bool target_is_visible, wModelTime dtime,
								const wSensorState & sensor_state) const;
	//
	virtual wSensorType get_sensor_type() const {return SENSOR_IRST;}
	static wTechSensorInfo * create() { return new wIRSTInfo(); }
private:
	virtual float get_detection_dist_(	const wsType & target_type, float cos_aspect, const wTargetDetectionInfo & target_det_info,
										float * engines_power, bool * engines_afterburner, unsigned int engines_qty) const;
public:
	float detection_distance_for_tail_on_Su_27[ENGINE_MODE_MAX];
	float head_on_distance_coeff;
	wModelTime scan_period;
	float background_factor;
	bool	laserRanger;
};