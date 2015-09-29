#ifndef __wDetector_h__
#define __wDetector_h__

//Detector detects target by sensors
//Detector doesn't store detected targets

#include "WorldGeneral.h"

#include <ed/unordered_map.h>

#include "AI\Detection\wAngularVolume.h"

#include "wMovingObject.h"

#include "AI\Detection\wSensorInfo.h"

namespace Lua
{
	class Config;
}


struct WORLDGENERAL_API wSensorCarrier
{
	typedef ed::vector<wSensorInfo*> wSensorsInfo;	
	wSensorCarrier();
	virtual void	load_from_state(Lua::Config & config);
	inline bool		is_sensor_type_present(wSensorInfo::wSensorType sensor_type) const { return !(sensors[sensor_type].empty()); }
	bool			visualDetection; //Ability of visual detection
	bool			laserRanger;
	bool			canDetectLockOnByRWR;
	//
	ed::string		name;
	unsigned int	sensorCount;
	wSensorsInfo	sensors[wSensorInfo::SENSOR_TYPE_MAX];
private:
	bool			findLaserRanger_() const;
	bool			findRWRLockOnDetector_() const;
};

struct WORLDGENERAL_API wDetectorInfo : public wSensorCarrier
{	
	wDetectorInfo();
	void					load_from_state(Lua::Config & config);	
	//
	unsigned int			crew;
	wAngularVolumeMinElev	canopy_geometry;
};

class WORLDGENERAL_API wDetectorInfoStorage
{
public:
	typedef ed::unordered_map<ed::string, wDetectorInfo> wDetectorsInfoMap;
	typedef ed::unordered_map<ed::string, wSensorCarrier> PodsInfoMap;
	void				init();
	void				clear();
	wDetectorInfo *		get_detector_info(woPointer obj_ptr) const;
	wSensorCarrier *	get_pod_info(const wsType & obj_type) const;
	static wDetectorInfoStorage instance;
private:
	mutable wDetectorsInfoMap	detectors_info;
	mutable PodsInfoMap			pods_info; 
};

class wTargetDetectionStatus;

struct wTargetDetectionInfo;

typedef ed::vector<const wSensorCarrier*> wSensorCarriers;

class WORLDGENERAL_API wDetector
{
public:
	wDetector();
	virtual ~wDetector();
	//Initialization
	virtual void				init(woPointer self_ptr_, const wDetectorInfo * detector_info_);

	//Adds sensor pods
	void						addSensorPod(const wSensorCarrier * sensorPod);

	//Removes sensor pods
	void						removeSensorPod(const wSensorCarrier * sensorPod);
	
	//Check is the target is detected
	virtual unsigned int		checkTarget(woPointer target_ptr, wTargetDetectionStatus & target_det_status, bool search);

	//Checks if the given target is radiated by the detector and returns radar mode
	virtual RadarMode			isTargetRadiated(woPointer target_ptr) const;

	//Checks if the shooter is detected, called on shoot event
	virtual unsigned int		checkShooter(woPointer& target_ptr, wTargetDetectionStatus & target_det_status, const wsType & weaponType, void * weaponData, ObjectID targetID);

	//Checks if the target with active radar is detected, called on radiation event
	virtual unsigned int		checkRadar(woPointer& target_ptr, wTargetDetectionStatus & target_det_status, RadarMode radarMode, float radarDetectionDistance);

	//Checks if the missile is detected
	virtual bool				detectMissile(woPointer pMissile, bool missileIsRadiating);
	
	bool						setSensorMode(wSensorInfo::wSensorType sensor_type, wSensorInfo::wSensorMode mode);
	inline void					setTargetOfInterest(woPointer target_of_interest_) { target_of_interest = target_of_interest_; }
	wSensorInfo::wSensorMode	getSensorMode(wSensorInfo::wSensorType sensor_type) const;	
	inline woPointer			getTargetOfInterest() const { return target_of_interest; }
	inline bool					hasSensors(wSensorInfo::wSensorType sensor_type) const { return !detector_info->sensors[sensor_type].empty(); }
	//
	inline const wDetectorInfo* getDetectorInfo() const { return detector_info; }
protected:
	unsigned int				getDetectionFlags() const;
	//Returns nearest detected target to the given target
	virtual bool				getNearestDetectedTarget(const cPoint & point, cPoint & targetPos) const;

	//Build target detection structure to use it into the sensors
	void						buildTargetDetectionInfo(woPointer target_ptr, const wTargetDetectionStatus & target_det_status, wTargetDetectionInfo & target_detection_info, unsigned int flags) const;
protected:
	woPointer					self_ptr; //Self object
	woPointer					target_of_interest; //Target of interest
	const wDetectorInfo *		detector_info; //Descriptor
	wSensorInfo::wSensorMode	visual_detector_mode;
	wSensorInfo::wSensorMode	sensor_type_mode[wSensorInfo::SENSOR_TYPE_MAX];
	wSensorCarriers				sensorCarriers;
	//
	unsigned int				target_det_info_building_flags; //Flags used to build wTargetDetectionInfo for the detector
	typedef std::map<ObjectID, wModelTime> RadarEvents;
	RadarEvents					radarEvents; //Objects detected by RWR on event (player-controlled aircraft)
};

#endif //__wDetector_h__
