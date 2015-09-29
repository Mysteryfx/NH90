#pragma once

#include "Avionics/Sensors/avBasicSensor.h"

#ifdef _ED_HIDDEN_DEBUG 
namespace Graphics
{
	class RenderObject;
}
#endif


namespace cockpit {

enum IRSNSR_STATUS
{
	IRSNSR_UNCAGED,
	IRSNSR_CAGED,
	IRSNSR_SLEW,
	IRSNSR_TRACK,
};

enum IRSNSR_COMMAND
{
	IRSNSR_TO_UNCAGE,
	IRSNSR_TO_CAGE,
	IRSNSR_TO_SLEW_VERT,
	IRSNSR_TO_SLEW_HOR,
	IRSNSR_TO_SLEW_STOP_ALL,
	IRSNSR_TO_SLEW_STOP_VERT,
	IRSNSR_TO_SLEW_STOP_HOR,
	IRSNSR_TO_TRACK,
};

class COCKPITBASE_API avIRSensor : public avBasicSensor
{
public:
	avIRSensor();
	virtual ~avIRSensor();

	virtual void initialize() {}
	void update(double delta);
	virtual void clear();
	virtual void update_angles(double delta);

	virtual void setElecPower(bool flag_in);
	// not avDevice
	virtual bool setCommand(unsigned Command, double Value = 0.0);

	unsigned getStatusID() const {return StatusID;}

	virtual float getLockonDistance(const woPointer objPtr) const = 0;
	float getMaxRangeWoutAB() const {return MaxRangeWoutAB;}
	float getMaxRangeWithAB() const {return MaxRangeWithAB;}

	const avTrackData& getTarget() {return Target;}
	void checkInSearch(bool FlagIn);

	const float &getHalfApertureSize()const{return HalfApertureSize;};

	void set_default_azimuth(double a);
	void set_default_elevation(double e);

	double get_default_azimuth();
	double get_default_elevation();

	virtual void slaveToDirection(const dVector& DirectionIn) {};


	void setStatusID(unsigned StatusID_In) {StatusID = StatusID_In;}


protected:
	void search();
	virtual void consentToTrack();
	virtual void dropTrack();

	virtual bool check_gimbal_limits() {return true;}
	bool gimbal_limits(const double k = 1.05) const;

	virtual void startSlew();
	void slewHorizontal(double Value);
	void slewVertical(double Value);
    void slewStop();
	void slewStopHorizontal();
	void slewStopVertical();
	virtual void onAxesStop();

	virtual void cage();
	void uncage();

	bool getInSearch() const {return InSearch;}

	double getMaxSlewVelocity() const {return MaxSlewVelocity;}
	void setMaxSlewVelocity(double ValIn) {MaxSlewVelocity = ValIn;}

	double getMaxVelocity() const {return MaxVelocity;}
	void setMaxVelocity(double ValIn) {MaxVelocity = ValIn;}

	virtual void setTarget(const woPointer Item);
	virtual void resetAxes();
	virtual void resetVerticalAxis();
	virtual void resetHorizontalAxis();

	bool		initialized;

	float		MaxSearchRange;
	float		HalfApertureSize;
	float		MaxRangeWoutAB;
	float		MaxRangeWithAB;

	unsigned	StatusID;
	bool		InSearch;
	bool		OnSun;

	double		MaxSlewVelocity;
	double		MaxVelocity;
    double		AzSlewVal, ElSlewVal;
	double		AzDriftVal, ElDriftVal;

	avTrackData Target;
	ed::vector< std::pair<woPointer, avIRSensor*> > SearchList;

#ifdef _ED_HIDDEN_DEBUG 
	Graphics::RenderObject * dbg_robj;
public:
	void parse(Graphics::RenderParser & parser,const Position3 & pos);
#endif

};

}
