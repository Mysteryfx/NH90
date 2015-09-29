#pragma once
#include "MFD.h"

namespace cockpit{
	namespace ecran {
		typedef enum arg_poss_bat {
			bat_stand_by = -100,
			stand_by_dbat = -75,
			stand_by_used_bat = -50,
			stand_by_full = -25,
			bat_off = 0,
			on_normal = 20,
			on_stand_by = 40,
			on_data = 60,
			on_dcepu = 80,
			bonus = 100
		};
		class MFDBatterie :
			public MFD
		{
		public:
			MFDBatterie();
			~MFDBatterie();
			MFDBatterie(std::string name, int argument, cockpit::NH_90::SystemeElectrique* electrique_systeme);
			MFDBatterie(std::string name, int argument, cockpit::NH_90::SystemeElectrique* electrique_systeme, MFD* secours, int argumen_systeme_secour);

			void batterieChange(float value);
			bool batterieIsActivate(float value);

		};
	}
}