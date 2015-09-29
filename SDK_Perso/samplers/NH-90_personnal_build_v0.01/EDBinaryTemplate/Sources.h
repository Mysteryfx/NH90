#pragma once
#include "ElementElectriqueDeBase.h"

namespace cockpit {
	namespace NH_90 {
		class SystemeElectrique;
		class Sources :
			public ElementElectriqueDeBase
		{
		public:
			Sources(double nominalVolt);
			Sources(double nominalVolt, etat_des_elements_electrique etat);
			Sources(double nominalVolt, etat_des_elements_electrique etat, SystemeElectrique* systeme_maitre);
			~Sources();

			/* void setVoltCourant(double volt);
			* Modifie le voltage courant (sortie).
			*/
			virtual void setVoltCourant(double volt);
			/* double getNominalVolt();
			* Donne le voltage nominal (Volt normal).
			*/
			virtual double getNominalVolt();
			/* double getVoltCourant();
			* Donne le voltage courant (sortie).
			*/
			virtual double getVoltCourant();
			/* void userChangeEtat(float value) :
			* Si le changement d'état vient de l'utilisateur, il faut animé l'action approppriée dans le cockpit.
			*/
			virtual void userChangeEtat(float value) = 0;
			/* void update();
			* Mets à jours les informations de l'équipement.
			*/
			virtual void update() = 0;

		protected:
			double nominalVolt;						//Voltage nominal.
			double voltCourant;						//Voltage courant (sortie).

			SystemeElectrique* systeme_maitre;		//Systeme électrique qui permettra de savoir si le matériel est utilisé.

		};
	}
}