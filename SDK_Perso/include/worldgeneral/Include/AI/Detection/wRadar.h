#pragma once

#include "WorldGeneral.h"

#include "AI\Detection\wTechSensorInfo.h"

class WORLDGENERAL_API wRadarInfo :	public wTechSensorInfo
{
public:
	typedef enum wRadarType {RADAR_VOID = -1, RADAR_AS, RADAR_SS, RADAR_MULTIROLE};
	typedef enum wHemisphere {HEMISPHERE_UPPER, HEMISPHERE_LOWER, HEMISPHERE_MAX};
	typedef enum wTargetAspect {ASPECT_HEAD_ON, ASPECT_TAIL_ON, ASPECT_MAX};	
	struct ModelParams : public wDetectionModelParams
	{
		enum Test
		{
			TEST_VOID = -1,
			//A2A
			TEST_RADIAL_VELOCITY = 1 << 0,
			TEST_RELATIVE_RADIAL_VELOCITY = 1 << 1,
			TEST_BACKGROUND = 1 << 2,
			TEST_ASPECT = 1 << 3,
			TEST_TAIL_ON_ASPECT_TERRAIN_CLUTTER_MASKING = 1 << 4,
			//A2G			
			TEST_VEHICLES_DETECTION_LIMITATION = 1 << 5,
			TEST_RESOLUTION_WITH_TERRAIN_OBJECTS = 1 << 6
		};
		ModelParams();
		void			clear();
		void			load_from_state(Lua::Config & config);
		unsigned int	get_flags(wSensorInfo * sensor_info = NULL) const;		
		unsigned int	flags;
	};
	friend struct ModelParams;
	//
	wRadarInfo();
	virtual void load_from_state(Lua::Config & config);
	//
	virtual const wAngularVolume * get_volume(const wSensorState & sensor_state) const;
	virtual bool					provideDistance() const { return true; }
	float		get_max_detection_distance(woPointer target_ptr) const;
	virtual int get_detection_dist(	const wTargetDetectionInfo & target_det_info, wModelTime dtime, 
									wSensorMode sensor_mode, float & dist) const;
	virtual int check_target(	const wTargetDetectionInfo & target_det_info, wModelTime dtime,
								const wSensorState & sensor_state) const;
	virtual int search_target(	const wTargetDetectionInfo & target_det_info, bool target_is_visible, wModelTime dtime, 
								const wSensorState & sensor_state) const;
	float		get_max_detection_dist() const;
	//
	virtual wSensorType get_sensor_type() const {return SENSOR_RADAR;}
	wRadarType getRadarType() const { return radar_type; }
	//
	static wTechSensorInfo * create() { return new wRadarInfo(); }
	static ModelParams * model_params;
private:
	float get_airborne_target_detection_dist_(float RCS, wHemisphere target_hemisphere, wTargetAspect target_aspect, bool tracking, float * tracking_dist_max = NULL) const;
	float get_airborne_target_detection_dist_(float RCS, bool tracking, float * tracking_dist_max = NULL) const;
	enum SurfaceSearchMode { SURFACE_SEARCH_MODE_RBM, SURFACE_SEARCH_MODE_GMTI, SURFACE_SEARCH_MODE_HRM };
	float get_surface_target_detection_dist_(float RCS, SurfaceSearchMode surfaceSearchMode) const;
	wModelTime get_scan_period_(wSensorMode sensor_mode) const;
public:
	//features and capabilities
	wRadarType radar_type;
	float max_measuring_distance; //instrumental detection distance
	float scan_period;
	//air search
	int  TWS_max_targets;
	wAngularVolumePyramid centered_scan_volume;
	static wAngularVolumePyramid ray_volume;
	float detection_distance[HEMISPHERE_MAX][ASPECT_MAX]; //for 5m^2 cross-section
	float detection_distance_best;
	float radial_velocity_min;
	float relative_radial_velocity_min;
	float tail_on_aspect_map_obj_mask_factor;
	float lock_on_distance_coeff;
	float TWS_scan_period;
	//surface search
	float RBM_detection_distance;
	bool GMTI; //ground moving target indication
	float GMTI_detection_distance;
	bool HRM; //high-resolution mapping (Doppler beam narrowing or SAR)
	float HRM_detection_distance;
	bool vehicles_detection; //is resolution is high enough to detect vehicles
};