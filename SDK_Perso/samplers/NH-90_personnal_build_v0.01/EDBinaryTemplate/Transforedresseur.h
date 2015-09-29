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
			* Donne le voltage en entr�e.
			*/
			double getEntree();
			/* double getSortie();
			* Donne le voltage en sortie.
			*/
			double getSortie();
			/* void setEntree(double voltEntree);
			* Modifie le voltage d'entr�e.
			*/
			void setEntree(double voltEntree);
			/* void setSortie(double voltSortie);
			* Modifie le voltage de sortie.
			*/
			void setSortie(double voltSortie);

			/* void userChangeEtat(float value) :
			* Si le changement d'�tat vient de l'utilisateur, il faut anim� l'action approppri�e dans le cockpit.
			*/
			virtual void userChangeEtat(float value);
			/* void update();
			* Mets � jours les informations de l'�quipement.
			*/
			virtual void update();
			/* void teste_voyant();
			* Affiche l'�tat de test du voyant.
			*/
			void teste_voyant() { voyant.test_voyant(); }

		private:
			double voltEntree;							//Voltage en entr�e.
			double voltSortie;							//Voltage en sortie.
			std::string id;								//Identifiant du transforedresseur.

			SystemeElectrique* systeme_maitre;			//Syst�me �lectrique.
			element::BoutonElectrique bouton;			//Bouton li� au transforedresseur dans le cockpit.
			element::VoyantTroisEtat voyant;			//Voyant li� au transforedresseur dans le cockpit.
		};
	}
}