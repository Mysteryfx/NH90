#ifndef GUN_WEAPON_COMMON_H
#define GUN_WEAPON_COMMON_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

/************************************************************************/
/*
	This class should be inherited by the aircraft weapon system
	for supporting the extended gun heating model

	call GunWeaponCommon::initialize() function from the derived's initializator
	and on aircraft REARM

	call GunWeaponCommon::update() to send to heater current ATMO temperature and elec heater heat
																		*/
/************************************************************************/

#include "CockpitBase.h"
#include "ed/map.h"

#include "FMElectricSystems/Items/Consumer.h"

class wAircraftGunMount;

namespace cockpit
{
	class GunHeatProxy3;

	class COCKPITBASE_API GunWeaponCommon
	{
	public:
		GunWeaponCommon(const ed::string& name,
			char pilon_count = 0 /*0 - no station guns availability (no gun pods at all)*/);

		void initialize();
		void release();

		~GunWeaponCommon();

		float gun_temp(char gun_num, char station =-1); /*returns gun temperature*/
	protected:

		void update_heat_exchange();

		void set_electric_heater(EagleFM::Elec::ItemBase* heater,char gun_num, char station =-1);

		//////////////////////////////////////////////////////////////////////////
		struct GunMountData
		{
			GunMountData() : heat_proxy_(nullptr),electric_heater_(nullptr) {}
			~GunMountData();

			GunHeatProxy3* heat_proxy_;
			EagleFM::Elec::ItemBase* electric_heater_;
		};

		short get_gun_id(char station, char gun_num) const
		{
			return (station << 8) | gun_num;
		}

		wAircraftGunMount* get_gun_mount(char station, char gun_num);

		ed::map<short,GunMountData*> gun_mounts_;

		ed::string name_;
		char pilon_count_;

#ifdef _ED_HIDDEN_DEBUG 
//#define LOG_GUN_HEATING
#endif

		#ifdef LOG_GUN_HEATING
		    bool first_run_;
			void log_gun_heat();
		#endif
	};
}

#endif
