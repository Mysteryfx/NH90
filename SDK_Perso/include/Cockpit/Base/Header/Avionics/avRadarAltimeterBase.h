#ifndef _AVRADARALTIMETERBASE_H
#define _AVRADARALTIMETERBASE_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "Avionics/avDevice.h"
#include "Avionics/avBasicElectric.h"
#include "Avionics/avBreakable.h"

#include "FMElectricSystems/Items/Consumer.h"
#include "Utilities/ccUtilities.h"

#include "wFloat.h"
#include "viFlags.h"

#ifdef _ED_HIDDEN_DEBUG 
//#define _DEBUG_DRAW
#endif

#ifdef _DEBUG_DRAW
#include "Renderer/DbgGraph.h"
#endif

namespace cockpit
{

const viFlag viRALTflags = viMapObject | viMissionObject | viWorldLightObject |
		viWorldHeavyObject | viMirageObject | viMapMissionObject | viMapObjectInDistrict;

class COCKPITBASE_API avRadarAltimeterBase : public avDevice,
											public avBreakable,
											public avBasicElectric
{
public:
	enum RALTIMETER_MODES
	{
		RALT_VOID = -1,
		RALT_OFF,
		RALT_WARMUP,
		RALT_TEST,
		RALT_SEARCH,
		RALT_TRACK
	};

	enum BaseRadarAltimeterFailures
	{
		TOTAL_FAILURE = 0
	};

public:
	avRadarAltimeterBase();
	virtual ~avRadarAltimeterBase();

	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void post_initialize();
	virtual void SetCommand(int, float value = 0) {}
	virtual void serialize(Serializer &serializer) {}
	virtual void update();
	//end of interface

	//avBreakable interface
	virtual bool set_damage(unsigned Failure_ID, const bool can_be_rejected /* = false */);
	virtual void repair();
	//end of interface

	//avBasicElectic interface
	virtual bool getElecPower() const {return Elec_Consumer_.isPowered(); }
	//end of interface

#ifdef _DEBUG_DRAW
	void setdbgCurrentObjPos(const Position3 &PosIn) {dbgCurrentObjPos = PosIn;}
	void setdbgCurrentObjBox(const Box &BoxIn) {dbgCurrentObjVisuals->set_box(BoxIn);}
	void setdbgCurrentObjIsValid(bool FlagIn) {dbgCurrentObjIsValid = FlagIn;}
	bool getdbgCurrentObjIsValid() const {return dbgCurrentObjIsValid;}
	void setdbgTestRay(const cVector& valIn) {testRay = valIn;}
	void setdbgTestRayIsValid(bool valIn) {testRayIsValid = valIn;}

	void external_parse(Graphics::ModelParser& parser, const Position3& pos, Graphics::effectState* effects);
#endif

	RALTIMETER_MODES getMode() const {return mode;}
	void setMode(RALTIMETER_MODES modeIn);

	double getAltitude() const {return altitudeOutput.GetValue(wTime::GetModelTime());}

	float getApertureSize() const {return apertureSize;}
	void setApertureSize(float apertureSize_in) {apertureSize = apertureSize_in;}

	float getDistanceLimit() const {return DistanceLimit;}
	void setDistanceLimit(float limIn) {DistanceLimit = limIn;}

	float getDistanceLimitPreciseCalc() const {return DistanceLimitPreciseCalc;}
	void setDistanceLimitPreciseCalc(float limIn) {DistanceLimitPreciseCalc = limIn;}

	wModelTime getSearchTime() const {return searchTime;}
	void setSearchTime(float timeIn) {searchTime = timeIn;}

	void dropTrack();
	wModelTime getStartUpTime() const {return StartUpTime;}

protected:
	virtual void resetOutput(wFloat& paramIn, float valueIn, wModelTime currentTime, wModelTime dTime);
	virtual void updateOutput(wFloat& paramIn, float valueIn, wModelTime currentTime, wModelTime dTime);

	const char * l_metatable_name() const { return "radio_altimeter_meta";}
	void		 l_register_functions(lua_State *Lua_s,int index);

	static int l_get_mode(lua_State * Lua_s);
	static int l_get_altitude(lua_State * Lua_s);
	static int l_get_aperture_size(lua_State * Lua_s);
	static int l_get_distance_limit(lua_State * Lua_s);

	wFloat altitudeOutput;
	dVector device_pos; // смещение прибора относительно центра модели

	EagleFM::Elec::Consumer	Elec_Consumer_;

private:

	float	getDistanceRayToSurface(cVector& pos, cVector& dir);
	void	check_surface_intersection(cPosition& cself_pos, float& altitude);
	bool	check_antenna_limits(const cPosition& cself_pos);

	RALTIMETER_MODES mode;

	float apertureSize;
	float DistanceLimit;
	float DistanceLimitPreciseCalc;
	wModelTime searchTime;
	wModelTime toTrackTimer;
	wModelTime StartUpTime;
	bool isContact;

	struct AntennaLimits
	{
		AntennaLimits()
			: pitch_(TO_RAD(-80.0f),TO_RAD(80.0f)),
			roll_(TO_RAD(-80.0f),TO_RAD(80.0f))
		{}
		limits_f pitch_;
		limits_f roll_;
	} antenna_limit_;

#ifdef _DEBUG_DRAW
	dbg_cone*	dbgConeVisuals;
	dbg_box*	dbgCurrentObjVisuals;
	dbg_line*   dbgLineVisuals;
	Position3	dbgCurrentObjPos;
	bool		dbgCurrentObjIsValid;
	cVector     testRay;
	bool        testRayIsValid;
#endif
};

}

#endif // _avRadarAltimeterBase_h
