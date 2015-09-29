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
			*		- etat - l'�tat initiale de l'alternateur,
			*		- systeme_maitre - le syst�me ma�tre (le syst�me �lectrique),
			*		- bouton - le bouton li� � l'alternateur dans le cockpit,
			*		- voyant - le voyant li� � l'alternateur dans le cockpit.
			*/
			Alternateur(double nominalVolt, etat_des_elements_electrique etat, SystemeElectrique* systeme_maitre, element::BoutonElectrique bouton, element::VoyantTroisEtat voyant);
			~Alternateur();

			/* int get_etat();
			* Donne l'�tat sous forme d'entier.
			*/
			int get_etat();
			/* void setEtat(etat_des_elements_electrique etat);
			* Modifie l'�tat de l'�lement �lectrique.
			*/
			virtual void setEtat(etat_des_elements_electrique etat);
			/* void userChangeEtat(float value) :
			* Si le changement d'�tat vient de l'utilisateur, il faut anim� l'action approppri�e dans le cockpit.
			*/
			void userChangeEtat(float value);
			/* void update();
			* Mets � jours les informations de l'�quipement.
			*/
			virtual void update();
			/* void afficheEtat();
			* Affiche l'�tat de l'alternateur.
			* Fonction utilis� pendant de la phase de d�bogage.
			*/
			void afficheEtat();
			/* void teste_voyant();
			* Affiche l'�tat de test du voyant.
			*/
			void teste_voyant() { voyant.test_voyant(); }

		private:
			cockpit::element::BoutonElectrique bouton;				//Bouton On/Off de l'alternateur.
			cockpit::element::VoyantTroisEtat voyant;				//Voyant d'�tat du bouton de l'alternateur.
		};
	}
}