#pragma once
#include "Voyant.h"

namespace cockpit {
	namespace NH_90 {
		class SystemeElectrique;
	}
	namespace element {
		typedef enum enum_voyant_alternateur {
			test_voyant_alternateur			= -100,
			off_voyant_alternateur			= 100,
			panne_voyant_alternateur		= 100,
			stand_by_voyant_alternateur		= 50,
			on_voyant_alternateur			= 0
		};
		typedef enum enum_voyant_alim_externe {
			test_voyant_alim_externe		= -100,
			off_voyant_alim_externe			= 0,
			panne_voyant_alim_externe		= 0,
			fleche_voyant_alim_externe		= 30,
			bouton_voyant_alim_externe		= 60,
			stand_by_voyant_alim_externe	= 100
		};
		typedef enum enum_voyant_transforedresseur{
			test_voyant_transforedresseur = -100,
			off_voyant_transforedresseur = 100,
			panne_voyant_transforedresseur = 100,
			stand_by_voyant_transforedresseur = 50,
			on_voyant_transforedresseur = 0
		};
		typedef enum type_voyant_trois_etat {
			alimentation_externe,
			alternateur,
			transforedresseur
		};
		class VoyantTroisEtat :
			public Voyant
		{
		public:
			VoyantTroisEtat();
			~VoyantTroisEtat();
			VoyantTroisEtat(int i_argument, type_voyant_trois_etat type, cockpit::NH_90::SystemeElectrique* systeme_electrique);

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
			type_voyant_trois_etat type;		//Type du voyant (Batterie, emergency, etc...)

			/* void majAlimExterne(int etat);
			* Mets à jours les informations du voyant en ce basant sur les états d'une alimentation externe.
			*/
			void majAlimExterne(int etat);
			/* void majAlternateur(int etat);
			* Mets à jours les informations du voyant en ce basant sur les états d'un alternateur.
			*/
			void majAlternateur(int etat);
			/* void majTransforedresseur(int etat);
			* Mets à jours les informtaions du voyant en ce basant sur les états d'un transforedresseur.
			*/
			void majTransforedresseur(int etat);
		};
	}
}