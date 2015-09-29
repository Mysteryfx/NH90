#pragma once

#include "wVisualDetectorImpl.h"
#include "wSensorInfo.h"

namespace Lua
{
	class Config;
}

class wVisualDetector : public wSensorInfo,
						public wDetectionModelParams,
						public wVisualDetectorImpl
{
private:
	enum wVisualDetectedBy { NOT_DETECTED, DETECTED_BY_REFLECTED_LIGHT, DETECTED_BY_TARGET_LIGHT, DETECTED_BY_SHOOTS };
public:
	wVisualDetector();
//-=wSensorInfo=-
public:
	virtual const wAngularVolume * get_scan_volume(const wSensorState & sensor_state) const;
	virtual const wAngularVolume * get_volume(const wSensorState & sensor_state) const;
	virtual bool				   provideDistance() const { return true; }
public:
	//
	virtual int get_detection_dist(	const wTargetDetectionInfo & target_det_info, wModelTime dtime, 
									wSensorMode sensor_mode, float & dist) const;
	//
	virtual int check_target(	const wTargetDetectionInfo & target_det_info, wModelTime dtime,
								const wSensorState & sensor_state) const;
	virtual int search_target(	const wTargetDetectionInfo & target_det_info, bool target_is_visible, wModelTime dtime, 
								const wSensorState & sensor_state) const;
	virtual int check_shooter(	const wTargetDetectionInfo & target_det_info, wModelTime dtime,
								const wSensorState & sensor_state, const wsType & weaponType, void * weaponData, ObjectID targetID) const;
	//
	virtual wSensorType get_sensor_type() const { return SENSOR_VOID; }
protected:
	int get_detection_dist_(const wTargetDetectionInfo & target_det_info, wModelTime dtime, wSensorMode sensor_mode, float & dist) const;
	int get_shooter_detection_dist_(	const wTargetDetectionInfo & target_det_info, wModelTime dtime, wSensorMode sensor_mode, 
										float & dist, wVisualDetectorImpl::WeaponType & wType,
										const wsType & weaponType, void * weaponData) const;
//-=wSensorInfo::wDetectionModelParams=-
public:
	virtual void clear() { wVisualDetectorImpl::clear(); }
	virtual void load_from_state(Lua::Config & config) { wVisualDetectorImpl::load_from_state(config); }
	virtual unsigned int get_flags(wSensorInfo * sensor_info = NULL) const;
public:
	static wVisualDetector * instance;
};
