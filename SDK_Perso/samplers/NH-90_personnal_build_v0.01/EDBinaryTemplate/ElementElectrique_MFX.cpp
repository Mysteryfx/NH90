#include "stdafx.h"
#include "ElementElectrique_MFX.h"
#include "SystemeElectrique.h"

using namespace cockpit::element;
using namespace cockpit::NH_90;

ElementElectrique_MFX::ElementElectrique_MFX(SystemeElectrique *systeme_electrique, type_element_mfx type_element_electrique, etat_des_elements_de_base_mfx etat_element_electrique) :
ElementDeBase_MFX(systeme_electrique, type_element_electrique, etat_element_electrique)
{
	//this->systeme_electrique = systeme_electrique;
}
ElementElectrique_MFX::~ElementElectrique_MFX()
{
	panne();
}

/* bool isOnOff() :
* Retourne true si l'élément est actif.
*/
bool ElementElectrique_MFX::isOnOff(){
	return (systeme_electrique->isOnOff() ? ElementDeBase_MFX::isOnOff() : false);
}
/* void ElementElectriqueDeBase::setOnOff(bool status) :
* Si l'état n'est pas etat_panne :
*		- Affecte la valeur etat_actif si status true,
*		- Affecte la valeur etat_inactif si status false.
*/
void ElementElectrique_MFX::setOnOff(bool status) {
	if (systeme_electrique->isOnOff()) {
		ElementDeBase_MFX::setOnOff(status);
	}
}
/* void switchOnOff() :
* Active ou désactive l'élément en fonction de son état actuelle (voir isOnOff() - voir setOnOff(bool status)).
*/
void ElementElectrique_MFX::switchOnOff(){
	if (systeme_electrique->isOnOff()) {
		ElementDeBase_MFX::switchOnOff();
	}
}