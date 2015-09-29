#pragma once

#include "FMElectricSystems/Items/Relay.h"

namespace EagleFM
{

class DynamicBody;

namespace Elec
{

class ElecMachine_wShunt;

class FMBASE_API ElectricDrive
{
public:
	enum enum_ElectricDrive_Faults
	{
		FAULT_E_DRIVE_MOTOR = 0,
		FAULT_E_DRIVE_MAX
	};

	struct ElectricDriveInit
	{
		ElectricDriveInit(double red_k, double own_moi, double friction_moment,
			double min_a, double max_a, bool has_worm_gear) :
			RED_K_(red_k), OWN_MOI_(own_moi), FRICTION_MOMENT_(friction_moment),
				MIN_ANGLE_(min_a), MAX_ANGLE_(max_a), HAS_WORM_GEAR_(has_worm_gear)
		{

		}
		const double RED_K_;	       // reductor
		const double OWN_MOI_;	       // own MOI
		const double FRICTION_MOMENT_; // friction_moment
		const double MIN_ANGLE_;       // in rad
		const double MAX_ANGLE_;       // in rad
		const bool HAS_WORM_GEAR_;     // has_worm_gear (червячная передача, т.е. необратимая - моменты внешних сил не могут прокрутить привод)
	};

	virtual ~ElectricDrive(){}

	virtual void	simulate(DynamicBody* dbody, double dt);
	virtual bool	locked() const = 0;
	virtual double	get_ext_moment() {return 0.0;}

	virtual void	set_failure(unsigned int failure);
	virtual void	repair();

	virtual Elec::ItemBase& operator >>(Elec::ItemBase& next_item);

	bool	active() const { return fabs(reductor_omega_) > 0.0001; }

	double	get_rel_pos() const { return drive_rel_pos_; }
	double	get_pos() const { return drive_pos_; }

	double	get_output_omega() const { return reductor_omega_; }
	double	get_motor_omega() const { return motorOmega_; }

	Elec::ElecMachine_wShunt* get_motor() const { return motor_; }

protected:

	ElectricDrive(Elec::ElecMachine_wShunt* motor, ElectricDriveInit& init);

	void onNegative(const lwl::CSignalInfo* pInfo);
	void onPositive(const lwl::CSignalInfo* pInfo);

	Elec::ElecMachine_wShunt* motor_;

	//power
	Elec::Relay Up_Relay_;
	Elec::Relay Down_Relay_;

	//control
	Elec::Wire wire_up_ctl_;
	Elec::Wire wire_down_ctl_;

	double motorOmega_, reductor_omega_;
	double drive_pos_; /*angle in rad*/
	double drive_rel_pos_;
	
	const ElectricDriveInit params_;


};

} }
