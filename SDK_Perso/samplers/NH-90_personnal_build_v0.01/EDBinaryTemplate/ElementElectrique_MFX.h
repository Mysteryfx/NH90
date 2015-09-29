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
			* Pour cr�er un ElementElectrique_MFX, il faut :
			*			- SystemeElectrique* systeme_electrique, le syst�me �lectrique;
			*			- type_element_mfx type_element_electrique, le type de l'�l�ment �lectrique (par d�faut : electrique_mfx);
			*			- etat_des_elements_de_base_mfx etat_element_electrique, �tat de l'�l�ment �lectrique (par d�faut : etat_inactif_mfx).
			*/
			ElementElectrique_MFX(NH_90::SystemeElectrique *systeme_electrique, type_element_mfx type_element_electrique = type_element_mfx::electrique_mfx, etat_des_elements_de_base_mfx etat_element_electrique = etat_des_elements_de_base_mfx::etat_inactif_mfx);
			ElementElectrique_MFX(){}
			~ElementElectrique_MFX();

			/* bool isOnOff() :
			* Retourne true si l'�l�ment est actif.
			*/
			virtual bool isOnOff();
			/* void ElementElectriqueDeBase::setOnOff(bool status) :
			* Si l'�tat n'est pas etat_panne :
			*		- Affecte la valeur etat_actif si status true,
			*		- Affecte la valeur etat_stand_by si status false.
			*/
			virtual void setOnOff(bool status);
			/* void switchOnOff() :
			* Active ou d�sactive l'�l�ment en fonction de son �tat actuelle (voir isOnOff() - voir setOnOff(bool status)).
			*/
			virtual void switchOnOff();

		protected:
			NH_90::SystemeElectrique *systeme_electrique;			//Tout �l�ment �lectrique peut-�tre activ� que si le syst�me lui-m�me est actif (exception faites des batteries).
		};
	}
}