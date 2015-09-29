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
			* Donne l'�tat de l'�l�ment �lectrique.
			*/
			virtual etat_des_elements_electrique getEtat();
			/* void setEtat(etat_des_elements_electrique etat);
			* Modifie l'�tat de l'�lement �lectrique.
			*/
			virtual void setEtat(etat_des_elements_electrique etat);

			/* bool ElementElectriqueDeBase::isOnOff() :
			* Retourne true si l'�tat est etat_actif.
			*/
			virtual bool isOnOff();
			/* bool ElementElectriqueDeBase::isAlive() :
			* Retourne true si l'�tat est etat_actif ou etat_stand_by.
			*/
			virtual bool isAlive();
			/* void ElementElectriqueDeBase::setOnOff(bool status) :
			* Si l'�tat n'est pas etat_panne :
			*		- Affecte la valeur etat_actif si status true,
			*		- Affecte la valeur etat_stand_by si status false.
			*/
			virtual void setOnOff(bool status);
			/* void ElementElectriqueDeBase::switchOnOff() :
			* Active ou mets en stand_by un �l�ment �lectrique en fonction de son �tat actuelle (voir isOnOff() - voir setOnOff(bool status)).
			*/
			virtual void switchOnOff();
			/* void userChangeEtat(float value) :
			* Si le changement d'�tat vient de l'utilisateur, il faut anim� l'action approppri�e dans le cockpit.
			*/
			virtual void userChangeEtat(float value) = 0;
			/* void update();
			* Mets � jours les informations de l'�quipement.
			*/
			virtual void update() = 0;

		protected:
			etat_des_elements_electrique etat;	//Etat de l'�l�ment �lectrique.
		};
	}
}

