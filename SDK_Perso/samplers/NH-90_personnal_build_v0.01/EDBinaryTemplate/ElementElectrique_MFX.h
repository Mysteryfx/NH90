#pragma once
#include "ElementDeBase_MFX.h"

namespace cockpit {
	namespace NH_90 { class SystemeElectrique; }
	namespace element {
		class ElementElectrique_MFX :
			public ElementDeBase_MFX
		{
		public:
			/* ElementElectrique_MFX(type_element_mfx type_element_electrique, SystemeElectrique* systeme_electrique, etat_des_elements_de_base_mfx etat_element_electrique = etat_des_elements_de_base_mfx::etat_inactif_mfx)
			* Pour créer un ElementElectrique_MFX, il faut :
			*			- SystemeElectrique* systeme_electrique, le système électrique;
			*			- type_element_mfx type_element_electrique, le type de l'élément électrique (par défaut : electrique_mfx);
			*			- etat_des_elements_de_base_mfx etat_element_electrique, état de l'élément électrique (par défaut : etat_inactif_mfx).
			*/
			ElementElectrique_MFX(NH_90::SystemeElectrique *systeme_electrique, type_element_mfx type_element_electrique = type_element_mfx::electrique_mfx, etat_des_elements_de_base_mfx etat_element_electrique = etat_des_elements_de_base_mfx::etat_inactif_mfx);
			ElementElectrique_MFX(){}
			~ElementElectrique_MFX();

			/* bool isOnOff() :
			* Retourne true si l'élément est actif.
			*/
			virtual bool isOnOff();
			/* void ElementElectriqueDeBase::setOnOff(bool status) :
			* Si l'état n'est pas etat_panne :
			*		- Affecte la valeur etat_actif si status true,
			*		- Affecte la valeur etat_stand_by si status false.
			*/
			virtual void setOnOff(bool status);
			/* void switchOnOff() :
			* Active ou désactive l'élément en fonction de son état actuelle (voir isOnOff() - voir setOnOff(bool status)).
			*/
			virtual void switchOnOff();

		protected:
			NH_90::SystemeElectrique *systeme_electrique;			//Tout élément électrique peut-être activé que si le système lui-même est actif (exception faites des batteries).
		};
	}
}