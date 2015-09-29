#pragma once
#include "Sources.h"
#include "VoyantTroisEtat.h"
#include "BoutonElectrique.h"

namespace cockpit {
	namespace NH_90 {
		class Alternateur :
			public Sources
		{
		public:
			Alternateur();
			Alternateur(double nominalVolt);
			/* Alternateur(double nominalVolt, etat_des_elements_electrique etat, SystemeElectrique* systeme_maitre, element::BoutonDeuxPosition bouton, element::VoyantTroisEtat voyant);
			* Instanciation d'un alternateur avec :
			*		- nominalVolt - Voltage nominal de l'alternateur,
			*		- etat - l'état initiale de l'alternateur,
			*		- systeme_maitre - le système maître (le système électrique),
			*		- bouton - le bouton lié à l'alternateur dans le cockpit,
			*		- voyant - le voyant lié à l'alternateur dans le cockpit.
			*/
			Alternateur(double nominalVolt, etat_des_elements_electrique etat, SystemeElectrique* systeme_maitre, element::BoutonElectrique bouton, element::VoyantTroisEtat voyant);
			~Alternateur();

			/* int get_etat();
			* Donne l'état sous forme d'entier.
			*/
			int get_etat();
			/* void setEtat(etat_des_elements_electrique etat);
			* Modifie l'état de l'élement électrique.
			*/
			virtual void setEtat(etat_des_elements_electrique etat);
			/* void userChangeEtat(float value) :
			* Si le changement d'état vient de l'utilisateur, il faut animé l'action approppriée dans le cockpit.
			*/
			void userChangeEtat(float value);
			/* void update();
			* Mets à jours les informations de l'équipement.
			*/
			virtual void update();
			/* void afficheEtat();
			* Affiche l'état de l'alternateur.
			* Fonction utilisé pendant de la phase de débogage.
			*/
			void afficheEtat();
			/* void teste_voyant();
			* Affiche l'état de test du voyant.
			*/
			void teste_voyant() { voyant.test_voyant(); }

		private:
			cockpit::element::BoutonElectrique bouton;				//Bouton On/Off de l'alternateur.
			cockpit::element::VoyantTroisEtat voyant;				//Voyant d'état du bouton de l'alternateur.
		};
	}
}