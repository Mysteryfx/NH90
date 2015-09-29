#pragma once

#include <array>

#include "CockpitBase.h"
#include "Avionics/avMechanics.h"
#include "Avionics/avDevice.h"

namespace cockpit
{

class avFMProxyBase;

class COCKPITBASE_API MechCanopyCrank : public avDevice_BasicTimer
{
public:
	typedef std::function<void(void)> OnCrankUpdate; 

	MechCanopyCrank(avDevice* p, wModelTime _dtime, OnCrankUpdate upd_hndl);
	virtual ~MechCanopyCrank(){};

	void l_read(Lua::Config& config, const char* name);

	void setFM_proxy(avFMProxyBase* FM_proxy_);
	void NextEvent();
	
	void	force(bool fully_open);

	double	get_crank_value() const /*value for model animation*/ {return canopy_crank_.get_value_out();}

	float	get_turns() const { return turns_count_; }
	float	cur_value() const { return value_; }

	bool	canopyDropped();

	bool	check_animation(float& anim_value); 

private:
	void moveCanopy();
	void setCanopyMoveDirection(char direction);
	void setMovementDegradationValues(double speedMin, double speedMax, double coeff1, double coeff2);

	double	getCanopyCrankDegradeK() const;
	bool	can_move(char new_direction) const;

	std::array<double, 2> degradeSpeeds;
	std::array<double, 2> degradeCoefficients;

	int external_model_canopy_arg; 
	int external_model_canopy_trucks_arg;

	float	value_;
	float	prev_value_;
	float	turns_count_; /*total turns count */
	double	last_check_time_;

	avFMProxyBase* FM_proxy;
	OnCrankUpdate on_upd_hndl_;

	gauge_linear canopy_crank_;

	double in_direction_move_time_;
	char canopy_move_direction_;
	float prev_crank_value_;
	double maxRate;
};

}