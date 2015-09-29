#pragma once

//Information about a contact with a target

#include "edTerrain/ITerrain.h"
#include "landscape3/lPathFile.h"
#include "edTerrain/PointOnSurfaceAdapter.h"

#include "WorldGeneral.h"

#include "lPointOnSurface.h"

#include "wTime.h"
#include "Utilities/Parameter.h"

#include "cLinear.h"

//Detection methods
enum wDetected
{
	wcDetectedNot			= 0,
	wcDetectedVisual		= 1 << 0,
	wcDetectedByOptic		= 1 << 1,
	wcDetectedByRadar		= 1 << 2,
	wcDetectedByIRST		= 1 << 3,
	wcDetectedByRWR			= 1 << 4,
	wcDetectedByOtherUnits	= 1 << 5, //AWACS, EWR
	wcDetectedBySomething	= wcDetectedVisual | wcDetectedByOptic | wcDetectedByRadar | wcDetectedByIRST | wcDetectedByRWR | wcDetectedByOtherUnits
};

//Detection result flags
enum wDetectionFlag
{
	wcDetFlagNull						= 0,
	wcDetFlagTypeIsRecognized			= 1 << 0, //Is target type known
	wcDetFlagDistanceIsKnown			= 1 << 1, //Is distance to the target known
	wcDetFlagTargetCanBeTrackedByRadar	= 1 << 2, //Can target be tracked by radar now
	wcDetFlagTargetLockedByRadar		= 1 << 3  //Is target tracking the detector by its radar now
};

class WORLDGENERAL_API wTargetDetectionStatus
{
public:
	wTargetDetectionStatus();
	wTargetDetectionStatus(const wTargetDetectionStatus & tds);
	wTargetDetectionStatus & operator=(const wTargetDetectionStatus & tds);
	void		clear();
	void		clear_visibility();
	void		update(	wModelTime current_time,
						unsigned char can_be_visible_by_in,
						unsigned char visible_by_in,
						unsigned char flags_in,
						const cPoint & pos,
						const cVector & vel);
#ifdef DEBUG_TEXT
	ed::string get_status_str() const;
#endif
	unsigned char can_be_visible_by; //Can be visible
	unsigned char visible_by;	//Visible now
	unsigned char detected_by;	//Was detected
	unsigned char flags; //Flags of wDetectionFlag
	
	wModelTime  last_seen_time;
	cPoint		last_known_pos;
	cPoint		last_known_vel;

	wModelTime	last_update_time;
#ifndef USE_TERRAIN4
	mutable landscape::lPointOnSurface surfInfo; //Cached point on the surface
#else
	mutable edterrain::PointOnSurfaceAdapter surfInfo;
#endif
};

wTargetDetectionStatus WORLDGENERAL_API operator & (const wTargetDetectionStatus & left, const wTargetDetectionStatus & right);

wTargetDetectionStatus WORLDGENERAL_API operator | (const wTargetDetectionStatus & left, const wTargetDetectionStatus & right);

#ifdef DEBUG_TEXT
ed::string WORLDGENERAL_API get_detection_sources_name(unsigned int detection_source_flag);
ed::string WORLDGENERAL_API get_detection_sources_str(unsigned int detection_source_flag);
#endif
