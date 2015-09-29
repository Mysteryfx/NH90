#pragma once
#include "MFXDevice.h"
#include "Element.h"
#include "Batterie.h"
#include "Alternateur.h"
#include "SourcesExternes.h"
#include "Transforedresseur.h"

#include "BoutonDeuxPositionAVerrouDeSecurite.h"
#include "BoutonTroisPosition.h"

#include "ccMainPanel.h"

namespace cockpit {
	namespace NH_90 {
		typedef enum switch_mode {
			data = -1,
			normal = 0,
			stand_by = 1
		};
		class SystemeElectrique :
			public MFXDevice
		{
		public:
			SystemeElectrique();
			~SystemeElectrique();

			virtual void initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name);
			virtual void SetCommand(int command, float value = 0);
			virtual void update();

			//virtual bool checkCallbackKeyboard(int command, float& value) { return true; }
			//virtual bool checkCallbackClickable(int command, float& value) { return true; }
			virtual bool checkAnimationConditions(int arg_num, int command, float& value){ if (command == 3039) { return (get_context()->panel->GetArgument(arg_num) < 1.0f); } else { return true; } };

			switch_mode getSwitchMode();
			bool batterieUsed(Element* batterie);
			bool isSwitched(Element* multiprise);
			bool isDcepuAlimented(Element* multiprise);
			bool isDcAlimented(Element* multiprise);
			bool isAcAlimented(Element* multiprise);
			bool isTruAlimented(Element* transforedresseur);
			bool isTruDcepuAlimented(Element* transforedresseur);
			bool isOnOff();
			bool isDCBAlimented();
			bool isACBAlimented(std::string id);
			bool externalAlimUsed(type_alim_externe type);

			double getUpdateTime();
			void teste_voyant();
			bool getMoteur() { return b_moteur; }

			/* void elementAChangerEtat(element::type_element_mfx type)
			* Un élément informe le système qu'il à changer d'état.
			* On retrouve les éléments à vérifié par le type de celui-ci.
			*/
			virtual void elementAChangerEtat(element::type_element_mfx type) {}

		private:
			float test_voyant_time;
			Batterie batterie1, batterie2;
			Alternateur alternateur1, alternateur2, alternateur3;
			SourcesExternes acepu, dcepu;
			Transforedresseur transforedresseur1, transforedresseur2;

			Element alt1, alt2, alt3, acepu_mfd,
				bat1, bat2, dcepu_mfd,
				dbat1, dbat2,
				acb1, acb2, acb3, acb4,
				tru1, tru2,
				dcb1, dcb2,
				dcsb1, dcsb2,
				egen, emerb1, emerb2,
				essb1, essb2,
				sbmb1, sbmb2;

			cockpit::element::BoutonDeuxPositionAVerrouDeSecurite bouton_emergency;
			cockpit::element::BoutonTroisPosition bouton_switch_mode, bouton_test_voyant;

			int argument_switch;
			int ancienne_valeur_switch;
			switch_mode mode_du_switch;
			bool first_activate_update, b_test_voyant, b_moteur, is_mco_cut;

			void switch_mode_change(int value);
			void test_voyant_change(int value);
			void element_change(Element* element, float value);
			void alimentation_externer_acepu(float value);
			void alimentation_externer_dcepu(float value);
			void emergency(float value);

			void init_multiprise();
			void init_cockpit_mfd();
			void init_cockpit_switch();
			void init_sources();

			void update_lights();
		};
	}
}