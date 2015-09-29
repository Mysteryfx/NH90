#pragma once
#include "Debuggueur.h"

namespace cockpit{
	namespace NH_90 {
		typedef enum etat_des_elements_electrique
		{
			etat_panne		= -1,
			etat_inactif	= 0,
			etat_actif		= 1,
			etat_stand_by	= 2
		};

		class ElementElectriqueDeBase
		{
		public:
			ElementElectriqueDeBase();
			ElementElectriqueDeBase(etat_des_elements_electrique etat);
			~ElementElectriqueDeBase();

			/* etat_des_elements_electrique getEtat();
			* Donne l'état de l'élément électrique.
			*/
			virtual etat_des_elements_electrique getEtat();
			/* void setEtat(etat_des_elements_electrique etat);
			* Modifie l'état de l'élement électrique.
			*/
			virtual void setEtat(etat_des_elements_electrique etat);

			/* bool ElementElectriqueDeBase::isOnOff() :
			* Retourne true si l'état est etat_actif.
			*/
			virtual bool isOnOff();
			/* bool ElementElectriqueDeBase::isAlive() :
			* Retourne true si l'état est etat_actif ou etat_stand_by.
			*/
			virtual bool isAlive();
			/* void ElementElectriqueDeBase::setOnOff(bool status) :
			* Si l'état n'est pas etat_panne :
			*		- Affecte la valeur etat_actif si status true,
			*		- Affecte la valeur etat_stand_by si status false.
			*/
			virtual void setOnOff(bool status);
			/* void ElementElectriqueDeBase::switchOnOff() :
			* Active ou mets en stand_by un élément électrique en fonction de son état actuelle (voir isOnOff() - voir setOnOff(bool status)).
			*/
			virtual void switchOnOff();
			/* void userChangeEtat(float value) :
			* Si le changement d'état vient de l'utilisateur, il faut animé l'action approppriée dans le cockpit.
			*/
			virtual void userChangeEtat(float value) = 0;
			/* void update();
			* Mets à jours les informations de l'équipement.
			*/
			virtual void update() = 0;

		protected:
			etat_des_elements_electrique etat;	//Etat de l'élément électrique.
		};
	}
}

