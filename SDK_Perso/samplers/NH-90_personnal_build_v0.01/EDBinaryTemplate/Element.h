#pragma once
#include <string>

namespace cockpit {
	namespace NH_90 {
		class SystemeElectrique;
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
		typedef enum arg_poss_alt {
			stand_by__unused_alt3 = -100,
			stand_by_alt3 = -75,
			stand_by_short = -50,
			alt_off = 0,
			off_short = 50,
			on_short = 75,
			alt_on = 100
		};
		typedef enum arg_poss_gen {
			gen_off = 0,
			gen_on = 100
		};
		typedef enum arg_poss_external_source {
			panne = -100,
			ext_src_stand_by = -50,
			ext_src_off = 0,
			ext_src_on = 100
		};
		typedef enum arg_poss_dbat {
			dbat_standby = -100,
			HEAT = -50,
			dbat_off = 0,
			dbat_on = 100
		};
		typedef enum arg_poss_tru {
			tru_standby = -100,
			standby_dcepu = -60,
			off_DCEPU = -30,
			tru_off = 0,
			tru_on = 100
		};
		typedef enum type_elem {
			alt,
			bat,
			acb,
			egen,
			emerb,
			essb,
			dbat,
			sbmb,
			tru,
			dcb,
			dcsb
		};
		class Element
		{
		public:
			Element();
			Element(std::string name, int argument, type_elem type, SystemeElectrique* electrique_systeme);
			Element(std::string name, int argument, type_elem type, SystemeElectrique* electrique_systeme, Element* secours, int argumen_systeme_secour);
			~Element();

			void setSecour(int argument_secour, Element* secour);

			void change(float value);

			float getValue();
			bool comparaison(float value);
			std::string getName();
			int getArgument();
			type_elem getType();
			void update();

		protected:
			void batterieChange(float value);
			bool batterieIsActivate(float value);
			void updateBat();

			void dc_multipprise_change(float value);
			float getDCVal();
			bool isDcAlimented();

			void alternateurChange(float value);
			float getAcVal();
			bool isAcAlimented();

			void transforedresseurChange(float value);
			float getTruVal();
			bool isTruAlimented();

			void dc_secourable(bool value);
			bool isDcSecourable();

			void updateMfd();
		private:
			int argument;
			std::string name;
			type_elem type;
			int value;

			Element* secours;
			int argumen_systeme_secour;


			SystemeElectrique* electrique_systeme;
		};
	}
}