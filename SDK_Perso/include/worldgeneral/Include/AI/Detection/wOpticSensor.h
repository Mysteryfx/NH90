#pragma once

#include "AI\Detection\wTechSensorInfo.h"

namespace Lua
{
	class Config;
}

class wOpticSensorInfo : public wTechSensorInfo
{
private:
	enum wOpticDetectedBy { NOT_DETECTED, DETECTED_BY_REFLECTED_LIGHT, DETECTED_BY_TARGET_LIGHT, DETECTED_BY_SHOOTS };
public:
	typedef enum wOpticSensorType {OPTIC_SENSOR_VOID = -1, OPTIC_SENSOR_TV, OPTIC_SENSOR_LLTV, OPTIC_SENSOR_IR};
	typedef ed::vector<float> wOpticMagnifications;
	struct ModelParams : public wDetectionModelParams
	{
		ModelParams();
		void clear();
		void load_from_state(Lua::Config & config);
		unsigned int get_flags(wSensorInfo * sensor_info = NULL) const;
		//
		wModelTime	scan_time_for_double_scan_to_view_angular_square_ratio;
		float		recognition_distance_ratio_threshold;
		//
		bool IR_view_trough_fog_and_overcast;
		bool light_detection_magnification_factor;
	};
	//
	wOpticSensorInfo();
	virtual void load_from_state(Lua::Config & config);
	//
	virtual const wAngularVolume * get_volume(const wSensorState & sensor_state) const;
	virtual bool				   provideDistance() const { return laserRanger; }
public:
	//
	virtual int get_detection_dist(	const wTargetDetectionInfo & target_det_info, wModelTime dtime, 
									wSensorMode sensor_mode, float & dist) const;
	//
	virtual int check_target(	const wTargetDetectionInfo & target_det_info, wModelTime dtime,
								const wSensorState & sensor_state) const;
	virtual int search_target(	const wTargetDetectionInfo & target_det_info, bool target_is_visible, wModelTime dtime, 
								const wSensorState & sensor_state) const;
	//
	virtual wSensorType get_sensor_type() const {return SENSOR_OPTICAL;}
	//
	float get_target_contrast(const wTargetDetectionInfo & target_det_info) const;
	static wTechSensorInfo * create() { return new wOpticSensorInfo(); }
	//
	static ModelParams * model_params;
private:
	int get_detection_dist_(	const wTargetDetectionInfo & target_det_info, wModelTime dtime, wSensorMode sensor_mode, 
								wOpticDetectedBy & optic_detected_by, float & detection_dist_without_magnification, 
								float & dist) const;
	int check_target_(	const wTargetDetectionInfo & target_det_info, wModelTime dtime,
						const wSensorState & sensor_state, wOpticDetectedBy & optic_detected_by,
						float & detection_dist_without_magnification) const;
public:
	wOpticSensorType optic_sensor_type;
	wAngularVolumePyramid view_volume_max;	
	float	lightness_limit; //for OPTIC_SENSOR_LLTV only!
	float	resolution; //resolution relative to human eye resolution (greater - better)
	wOpticMagnifications magnifications;
	bool	laserRanger;
	//
	float scan_angular_square_to_view_angular_square_max_ratio;
};