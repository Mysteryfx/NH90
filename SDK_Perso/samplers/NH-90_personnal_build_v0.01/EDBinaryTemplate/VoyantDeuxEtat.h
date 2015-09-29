#pragma once
#include "Voyant.h"

namespace cockpit {
	namespace NH_90 {
		enum etat_des_elements_electrique;
	}
	namespace element {
		typedef enum enum_voyant_batterie {
			test_voyant_batterie			= 100,
			on_voyant_batterie				= 0,
			off_panne_voyant_batterie		= 100,
			pas_de_courant_voyant_batterie	= 0
		};
		typedef enum type_voyant_deux_etat{
			batterie,
			emergency_bus_bar
		};
		class VoyantDeuxEtat :
			public Voyant
		{
		public:
			VoyantDeuxEtat();
			~VoyantDeuxEtat();
			VoyantDeuxEtat(int i_argument, type_voyant_deux_etat type, cockpit::NH_90::SystemeElectrique* systeme_electrique);

			/* void afficheEtat();
			* Affiche toutes les informations du verrou.
			*/
			void afficheEtat();

		protected:
			/* void mise_a_jour_voyant(int etat);
			* Mets à jour les informations du voyant.
			*/
			virtual void mise_a_jour_voyant(int etat);

		private:
			type_voyant_deux_etat type;		//Type du voyant (Batterie, emergency, etc...)
		};
	}
}