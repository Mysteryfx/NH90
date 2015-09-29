#pragma once

#include "CockpitBase.h"
#include "FMElectricSystems/Items/Consumer.h"
#include "Lua/Config.h"

namespace cockpit
{

typedef float (*RPM_CallBack)(float power);

class GyroPowerSource
{
public:

	virtual ~GyroPowerSource(){}

	virtual void initialize(float spin_up_T, float slow_down_T, float nominal_rpm)
	{
		spin_up_T_ = spin_up_T;
		slow_down_T_ = slow_down_T;
	}

	virtual void l_read(Lua::Config& config){}

	virtual bool is_powered() const = 0;
	virtual void set_broken(bool failure)  = 0;
	virtual void simulate(double dt){};

	void set_nominal(float power) { nominal_power_ = power; }

	float get_nominal() const { return nominal_power_; }
	float get_RPM() const {return current_rpm_; }
	void  set_RPM(float valIn) {current_rpm_ = valIn;}

	bool is_spin_up()  const { return is_spin_up_; }

	void set_rpm_callback(RPM_CallBack rpm_callback)
	{
		rpm_callback_ = rpm_callback;
	}

protected:
	GyroPowerSource(RPM_CallBack rpm_callback = NULL) : nominal_power_(0.0f),
		current_spin_(0.0f), spin_up_T_(0.0f), slow_down_T_(0.0f),
		rpm_callback_(rpm_callback), current_rpm_(0.0f), is_spin_up_(false)
	{
	}

	float nominal_power_;

	float current_spin_;
	float spin_up_T_;
	float slow_down_T_;

	bool  is_spin_up_;

	float current_rpm_;

	RPM_CallBack	rpm_callback_;
};

typedef std::unique_ptr<GyroPowerSource> GyroPowerSrcPtr;

class COCKPITBASE_API GyroElectricSource : public GyroPowerSource
{

public:
	GyroElectricSource(double nominalVoltage) : ElecConDevice(nominalVoltage) {}
	virtual ~GyroElectricSource(){}

	virtual void initialize(float spin_up_T, float slow_down_T, float nominal_rpm)
	{
		spin_up_T_ = spin_up_T;
		slow_down_T_ = slow_down_T;
		nominal_rpm_ = nominal_rpm;
	}

	virtual void l_read(Lua::Config& config) override
	{
		if(!config.open_raw("GyroElectConsumerParams"))
			return;

		config.pop();
		EagleFM::Elec::ConsumerParams params;
		params.loadFromState(config, "GyroElectConsumerParams");
		ElecConDevice.setParams(params);
	}

	virtual bool is_powered() const
	{
		return ElecConDevice.isPowered();
	}

	EagleFM::Elec::ItemBase& get_power_connector()
	{
		return ElecConDevice;
	}

	virtual void set_broken(bool broken)
	{
		ElecConDevice.setFailure(broken);
	}

	virtual void simulate(double dt)
	{
		if (!is_powered())
		{
			// slowing up
			if (current_rpm_ > 0.0)
			{
				current_rpm_ -= dt / slow_down_T_ * current_rpm_;
				current_rpm_ = __max(current_rpm_, 0.0);
			}
			is_spin_up_ = false;
		}
		else
		{
			// spinning up
			if (current_rpm_ < nominal_rpm_)
			{
				current_rpm_ += dt / spin_up_T_ * (nominal_rpm_ - current_rpm_);
				current_rpm_ = __min(current_rpm_, nominal_rpm_);
			}
			is_spin_up_ = true;
		}
	}

protected:
	EagleFM::Elec::Consumer	ElecConDevice;
	float nominal_rpm_;
};

class COCKPITBASE_API GyroAirSource : public GyroPowerSource
{
public:
	GyroAirSource() : set_pressure_(0.0f), broken_(false)
// #ifdef _ED_HIDDEN_DEBUG 
// 		,force_stop_(false)
// #endif
	{ nominal_power_ = 3.75f; }

	virtual ~GyroAirSource(){}

	virtual bool is_powered() const
	{
// #ifdef _ED_HIDDEN_DEBUG 
// 			return !force_stop_ || set_pressure_ > nominal_power_;
// #else
		return set_pressure_ >= nominal_power_;
/*#endif*/
	}

	void set_power(float pressure)
	{
// #ifdef _ED_HIDDEN_DEBUG 
// 		if(force_stop_)
// 			pressure = 0.0f;
// #endif
		current_spin_ = pressure > set_pressure_ ? spin_up_T_: slow_down_T_;
		is_spin_up_ = (pressure >= set_pressure_ && pressure > 0.1);
		set_pressure_ = pressure;
	}

	virtual void set_broken(bool broken)
	{
		broken_ = broken;
	}

	void hot_start(double pressure)
	{
		set_power(pressure);

		assert(rpm_callback_);
		current_rpm_ = rpm_callback_(pressure);
	}

	float get_pressure() const { return set_pressure_; }

	virtual void simulate(double dt)
	{
		assert(rpm_callback_);
		float nominal_rpm = rpm_callback_(set_pressure_); //nominal RPM at this pressure

// #ifdef _ED_HIDDEN_DEBUG 
// 			if (force_stop_)
// 				current_rpm_ += (0.0f - current_rpm_) * dt/current_spin_;
// 			else
// 				current_rpm_ += (nominal_rpm - current_rpm_) * dt/current_spin_;
// #else
		float last_rpm = current_rpm_;
		current_rpm_ += (nominal_rpm - current_rpm_) * dt/current_spin_;

/*#endif*/
		current_rpm_ = __max(0.0,current_rpm_);
	}

#ifdef _ED_HIDDEN_DEBUG 
	bool force_stop_;
#endif
private:
	float set_pressure_; //in in. Hg.
	bool broken_;
};

}
