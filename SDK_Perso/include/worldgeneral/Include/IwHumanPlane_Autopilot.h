#pragma once

#include "WorldGeneral.h"

enum AutopilotCommands
{
	AP_CMD_Stab_Hbar = 0,
	AP_CMD_Stab_Angl,
	AP_CMD_Stab_Horiz,
	AP_CMD_Stab_Vel,
	AP_CMD_Incr_Vel,
	AP_CMD_Decr_Vel,
	AP_CMD_Route,
	AP_CMD_Arrival,
	AP_CMD_Landing,
	AP_CMD_Incr_H,
	AP_CMD_Decr_H = 10,
	AP_CMD_Stab_TangBank = 12,
	AP_CMD_Stab_HbarBank,
	AP_CMD_Stab_Horizon,
	AP_CMD_Stab_Hbar_,
	AP_CMD_Stab_Hrad,
	AP_CMD_Route_wout_stab_vel,
	AP_CMD_Arrival_wout_stab_vel,
	AP_CMD_Landing_wout_stab_vel,
	AP_CMD_Stab_HbarHeading = 20,
	AP_CMD_Stab_PathHold,
	AP_CMD_Stab_Off = 22
};

// Режимы автопилота
enum AutopilotModes
{
	AP_MODE_None		= 0,
	AP_MODE_Hbar1		= 2,
	AP_MODE_HbarBank1	= 3,
	AP_MODE_Horizon1	= 4,
	AP_MODE_Route		= 6,
	AP_MODE_Arrival		= 7,
	AP_MODE_Landing		= 8,
	AP_MODE_HeadingPitch= 10,
	AP_MODE_AutoRefuel	= 11,
	AP_MODE_PitchBank	= 21,
	AP_MODE_HbarBank2	= 22,
	AP_MODE_Horizon2	= 23,
	AP_MODE_Hbar2		= 24,
	AP_MODE_Hrad		= 25,
	AP_MODE_HbarHeading	= 26,
	AP_MODE_PathHold	= 27
};


class WORLDGENERAL_API IwHumanPlane_Autopilot
{
public:
	virtual ~IwHumanPlane_Autopilot() = 0;
	virtual void  set_mode(unsigned mode) = 0; 
	virtual int   get_mode() = 0;
	virtual void  set_command(int Command) = 0;
	virtual bool  get_disengaged_by_stick() = 0;


	virtual float get_pitch_required() const = 0;
	virtual float get_roll_required() const  = 0;
	virtual float get_heading_required() const = 0;

	virtual void set_pitch_required(float p)  = 0;
	virtual void set_roll_required(float r)    = 0;
	virtual void set_heading_required(float h)  = 0;
};
