#pragma once

#include "FMElectricSystems/Items/ElectricDrive.h"

namespace EagleFM {

class DynamicBody;
class GearPost;

namespace Elec {

class ElecMachine_wShunt;

typedef std::function<void(bool)>			ElectricDriveHandler;

class ElectricDownLock
{
public:

	enum enumUpLock_Failures
	{
		FAULT_L_NO = 0,
		FAULT_L_JAM,
		FAULT_L_NLOCK
	};

	ElectricDownLock(Elec::ItemBase& pwr, ElectricDriveHandler hndl) : pwr_(pwr), mech_lock_(false), hndl_(hndl), failure_(FAULT_L_NO)
	{

	}

	virtual bool locked() const {

		if(failure_ == FAULT_L_NLOCK) return false;
		if(failure_ == FAULT_L_JAM) return true;

		return !pwr_.isPowered() && mech_lock_;;
	}

	void force(bool lock)
	{
		mech_lock_ = lock;	
		hndl_(lock);
	}

	void setLock(bool lock)
	{
		if(mech_lock_ != lock)
		{
			mech_lock_ = lock;
			mech_lock_ = locked();
			hndl_(mech_lock_);
		}
	}

	void set_failure(unsigned int failure)
	{
		failure_ = failure;
	}

	void repair()
	{
		failure_ = FAULT_L_NO;
	}

	bool isDamaged() const { return failure_ != FAULT_L_NO; }

protected:
	Elec::ItemBase& pwr_;

	bool mech_lock_;

	ElectricDriveHandler hndl_;

	unsigned int failure_;
};

class ElectricUpLock : public ElectricDownLock
{
public:

	ElectricUpLock(Elec::ItemBase& pwr, ElectricDriveHandler hndl, double& mech_unlock) : ElectricDownLock(pwr, hndl), mech_unlock_(mech_unlock)
	{

	}

	virtual bool locked() const override {
		if(failure_ == FAULT_L_NLOCK) return false;
		if(failure_ == FAULT_L_JAM) return true;

		return !(mech_unlock_ || pwr_.isPowered()) && mech_lock_;
	}

protected:
	const double& mech_unlock_;

};


class FMBASE_API ElectricGearDrive : public ElectricDrive
{	
public:

	enum enum_ElectricGearDrive_Faults
	{
		FAULT_E_G_DRIVE_UPL_J = ElectricDrive::FAULT_E_DRIVE_MAX, /* jammed */
		FAULT_E_G_DRIVE_UPL_N									 /* not locking */
	};

	typedef std::unique_ptr<ElectricDownLock>	ElectricDownLockPtr;
	typedef std::unique_ptr<ElectricUpLock>		ElectricUpLockPtr;

	ElectricGearDrive(Elec::ElecMachine_wShunt* motor,
		GearPost* gear_post,
		ElectricDownLock* down_lock,
		ElectricUpLock* up_lock,
		ElectricDriveInit& init);

	void force( bool up_down /* true - UP; false DOWN */);

	virtual void simulate(DynamicBody* dbody, double dt) override;
	virtual bool locked() const override;
	virtual void set_failure(unsigned int failure) override;
	virtual void repair() override;

	virtual double get_ext_moment();

	void set_post(GearPost* post);

protected:
	GearPost* gear_post_;

	ElectricDownLockPtr down_lock_ptr_;
	ElectricUpLockPtr up_lock_ptr_;
};

} }
