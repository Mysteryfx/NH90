#pragma once
#include "Avionics\avDevice.h"
#include "Element.h"

namespace cockpit {
	namespace NH_90 {
		typedef enum switch_mode {
			data = -1,
			normal = 0,
			stand_by = 1
		};
		class ElecSys :
			public avDevice
		{
		public:
			ElecSys();
			~ElecSys();

			virtual void initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name);
			virtual void SetCommand(int command, float value = 0);
			virtual void update();

			switch_mode getSwitchMode();
			bool batterieUsed(Element* batterie);
			bool isSwitched(Element* multiprise);
			bool isDcepuAlimented(Element* multiprise);
			bool isDcAlimented(Element* multiprise);
			bool isAcAlimented(Element* multiprise);
			bool isTruAlimented(Element* transforedresseur);
			bool isTruDcepuAlimented(Element* transforedresseur);
		private:
				Element alt1, alt2, alt3, acepu,
					bat1, bat2, dcepu,
					dbat1, dbat2,
					acb1, acb2, acb3, acb4,
					tru1, tru2,
					dcb1, dcb2,
					dcsb1, dcsb2,
					egen, emerb1, emerb2,
					essb1, essb2,
					sbmb1, sbmb2;

				int argument_switch;
				int ancienne_valeur_switch;
				switch_mode mode_du_switch;
				bool first_activate_update;

				void switch_mode_change(float value);
				void element_change(Element* element, float value);
				void alimentation_externer_acepu(float value);
				void alimentation_externer_dcepu(float value);
				void emergency(float value);

				void init_multiprise();
				void init_cockpit_mfd();
				void init_cockpit_switch();
		};
	}
}