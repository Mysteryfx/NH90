#pragma once
#include "ElementElectriqueDeBase.h"
#include "BoutonElectrique.h"
#include "VoyantTroisEtat.h"

namespace cockpit {
	namespace NH_90 {
		class SystemeElectrique;
		class Transforedresseur :
			public ElementElectriqueDeBase
		{
		public:
			Transforedresseur();
			Transforedresseur(std::string id, double voltSortie, etat_des_elements_electrique etat, SystemeElectrique* systeme_maitre, element::BoutonElectrique bouton, element::VoyantTroisEtat voyant);
			~Transforedresseur();

			/* double getEntree();
			* Donne le voltage en entrée.
			*/
			double getEntree();
			/* double getSortie();
			* Donne le voltage en sortie.
			*/
			double getSortie();
			/* void setEntree(double voltEntree);
			* Modifie le voltage d'entrée.
			*/
			void setEntree(double voltEntree);
			/* void setSortie(double voltSortie);
			* Modifie le voltage de sortie.
			*/
			void setSortie(double voltSortie);

			/* void userChangeEtat(float value) :
			* Si le changement d'état vient de l'utilisateur, il faut animé l'action approppriée dans le cockpit.
			*/
			virtual void userChangeEtat(float value);
			/* void update();
			* Mets à jours les informations de l'équipement.
			*/
			virtual void update();
			/* void teste_voyant();
			* Affiche l'état de test du voyant.
			*/
			void teste_voyant() { voyant.test_voyant(); }

		private:
			double voltEntree;							//Voltage en entrée.
			double voltSortie;							//Voltage en sortie.
			std::string id;								//Identifiant du transforedresseur.

			SystemeElectrique* systeme_maitre;			//Système électrique.
			element::BoutonElectrique bouton;			//Bouton lié au transforedresseur dans le cockpit.
			element::VoyantTroisEtat voyant;			//Voyant lié au transforedresseur dans le cockpit.
		};
	}
}