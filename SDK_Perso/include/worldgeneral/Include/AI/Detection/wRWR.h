#pragma once

#include "AI\Detection\wTechSensorInfo.h"

class wRWRInfo : public wTechSensorInfo
{
public:
	struct ModelParams : public wDetectionModelParams
	{
		ModelParams();
		unsigned int	get_flags(wSensorInfo * sensor_info = NULL) const;		
	};
	friend struct ModelParams;
	wRWRInfo();
	virtual void load_from_state(Lua::Config & config);
	//
	virtual const wAngularVolume * get_scan_volume(const wSensorState & sensor_state) const { return NULL; }
	virtual const wAngularVolume * get_volume(const wSensorState & sensor_state) const { return NULL; }
	virtual bool				   provideDistance() const { return false; }
	virtual bool is_in_scan_volume(	woPointer target_ptr, woPointer self_ptr, 
									const wSensorState & sensor_state) const { return true; }
	virtual int check_target(	const wTargetDetectionInfo & target_det_info, wModelTime dtime, 
								const wSensorState & sensor_state) const;
	virtual int search_target(	const wTargetDetectionInfo & target_det_info, bool target_is_visible, wModelTime dtime, 
								const wSensorState & sensor_state) const;
	virtual wSensorType get_sensor_type() const {return SENSOR_RWR;}
	static wTechSensorInfo * create() { return new wRWRInfo(); }
	virtual bool is_in_scan_volume(	woPointer target_ptr, const cVector & dir_to_target_local, woPointer self_ptr, 
									const wSensorState & sensor_state) const { return true; }
	bool can_detect_lock_on() const { return lock_on_detection; }
	static ModelParams * model_params;
protected:
	virtual int get_detection_dist(	const wTargetDetectionInfo & target_det_info, wModelTime dtime,
									wSensorMode sensor_mode, float & dist) const;
private:
	float detection_dist_to_radar_detection_dist_max_ratio;
	bool  lock_on_detection;

};