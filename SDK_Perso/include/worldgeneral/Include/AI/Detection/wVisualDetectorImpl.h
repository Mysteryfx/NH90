#pragma once

#include "AI\Detection\wAngularVolume.h"
#include "AI\Detection\AtmosphereTransparency.h"

#include "wCommonDefinitions.h"
#include "wSensorInfo.h"

struct wTargetDetectionInfo;
class wTargetDetectionStatus;

class wVisualDetectorImpl
{
public:
	enum Test
	{
		TEST_VOID = 0,
		TEST_TERRAIN_LOS = 1 << 0,
		TEST_EARTH_CURVATURE_LOS = 1 << 1,
		TEST_OBJECTS_LOS = 1 << 2,
		TEST_TREES_LOS = 1 << 3,

		TEST_OVERCAST = 1 << 4,
		TEST_FOG = 1 << 5,
		TEST_PRECIPITATION = 1 << 6,
		TEST_SUN_LIGHT_SOURCE = 1 << 7,
		TEST_OTHER_LIGHT_SOURCES = 1 << 8,
		TEST_BACKGROUND = 1 << 9,
		TEST_ALL = 0xFFFFFFFF
	};
	enum FogTestResult { NO, BOTH_ABOVE, BOTH_BELOW, BETWEEN };
	enum WeaponType {	WEAPON_TYPE_MG, WEAPON_TYPE_SMALL_CALIBER_CANNON, WEAPON_TYPE_MEDIUM_CALIBER_CANNON, WEAPON_TYPE_LARGE_CALIBER_CANNON, 
						WEAPON_TYPE_ROCKET, WEAPON_TYPE_HEAVY_ROCKET, WEAPON_TYPE_MISSILE, WEAPON_TYPE_HEAVY_MISSILE, WEAPON_TYPE_WEAPON_TYPE_MAX};
	enum SpectrumBand { SPECTRUM_BAND_VISUAL, SPECTRUM_BAND_IR };
private:
	//typedef AtmosphereTransparency wLOSTransparencyFactor;
	class wLOSTransparencyFactor
	{
	public:		
		wLOSTransparencyFactor();
		void	load_from_state(Lua::Config & config);
		float	get(	woPointer target_ptr, woPointer self_ptr, SpectrumBand spectrumBand, unsigned int flags,
						FogTestResult & fog_test_reslt, FogTestResult & overcast_test_reslt) const;
		float	get_max_visibility(float h1, float h2, bool precipitations_test, SpectrumBand spectrumBand) const;
	public:
		float param[2];
	};
	class wMotionFactor
	{
	public:
		wMotionFactor();
		void	load_from_state(Lua::Config & config);
		float	get(woPointer target_ptr, const cVector & target_relative_speed_on_LOS, float distance_to_target, float tan_target_angular_size) const; //air-background target
		float	get(woPointer target_ptr, const cVector & target_relative_speed_on_LOS, float distance_to_target, float tan_target_angular_size, 
					const cPoint & ground_point_relative_speed_on_LOS, float distance_to_ground_point) const; //surface-background target
	private:
		float	angular_speed_to_angular_size_ratio_max;
		float	detection_distance_factor_max;
	};
	class wTargetTailFactor
	{
	public:
		wTargetTailFactor();
		void	load_from_state(Lua::Config & config);
		float	get(woPointer target_ptr) const;
	private:
		float	inverse_tail_detection_distance_coeff;
		float	smoke_tail_detection_distance_coeff;
		float	aircraft_min_height;
		float	detection_distance_coeff_in_zero_height;
		float	maximal_detection_distance_coeff_velocity;
		float	maximal_detection_distance_coeff;
	};
	class wTargetShootFactor
	{
	public:
		wTargetShootFactor();
		void	load_from_state(Lua::Config & config);
		float	get_detection_distance(woPointer target_ptr, woPointer self_ptr, wModelTime dtime, const wsType & weaponType, void * weaponData, WeaponType & wType) const;
	private:
		ed::vector<float> weapon_fire_detection_distance;
	};
	class wTargetLightFactor
	{
	public:
		wTargetLightFactor();
		void	load_from_state(Lua::Config & config);
		float	get_detection_distance(woPointer target_ptr, woPointer self_ptr, wModelTime dtime) const;
	private:
		float light_source_detection_distance;		
	};
	struct wDetectionDistance
	{
		float fromGround;
		float fromAir;
	};
	typedef ed::vector<std::pair<ed::string, wDetectionDistance> > wDetectionDistanceByClass;
public:
	enum wVisualDetectionMethod { VISUAL_DETECTION_BY_REFLECTED_LIGHT, VISUAL_DETECTION_BY_TARGET_LIGHT};

public:
	wVisualDetectorImpl() { clear(); }
	void					clear();
	void					load_from_state(Lua::Config & config);
	float					get_shooter_clear_detection_dist(const wTargetDetectionInfo & target_det_info, SpectrumBand spectrumBand, wModelTime dtime, const wsType & weaponType, void * weaponData, WeaponType & wType) const;
	float					get_clear_detection_dist(const wTargetDetectionInfo & target_det_info, SpectrumBand spectrumBand, wVisualDetectionMethod method, wModelTime dtime = 0.0) const;
	float					get_contrast_factor(const wTargetDetectionInfo & target_det_info, float illumination, wVisualDetectionMethod method) const;
	float					get_sun_illumination() const;
	float					get_illumination(const wTargetDetectionInfo & target_det_info) const;	
	float					get_transparency_factor(const wTargetDetectionInfo & target_det_info, SpectrumBand spectrumBand, FogTestResult & fog_test_reslt, FogTestResult & overcast_test_reslt) const;
	const wAngularVolume &	get_vision_volume() const { return vision_volume; }
	float					get_max_visibility_dist(float h1, float h2, SpectrumBand spectrumBand) const;
protected:
	wAngularVolumePyramid	focused_vision_volume;
	wAngularVolumePyramid	vision_volume;
	wAngularVolumePyramid	spherical_scan_volume;
	unsigned int			flags;

	float					max_detection_distance;
	wModelTime				average_det_time_max_dist_0;
	wModelTime				average_det_time_max_dist_180;
	wLOSTransparencyFactor	LOS_transparency_factor;	

	float					target_min_tan_angular_size;
	wDetectionDistanceByClass detection_distance_by_class;

	float					darkness_threshold;
	float					background_factors[BACKGROUND_TYPE_MAX];
	float					recognition_distance_ratio_threshold;

	wMotionFactor			motion_factor;
	wTargetShootFactor		target_shoot_factor;
	wTargetLightFactor		target_light_factor;
private:
	mutable float			sun_illumination;
	mutable wModelTime		last_sun_illumination_update_time;

};
