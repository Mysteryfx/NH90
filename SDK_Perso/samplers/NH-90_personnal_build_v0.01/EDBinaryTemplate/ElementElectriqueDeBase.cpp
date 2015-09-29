#include "stdafx.h"
#include "ElementElectriqueDeBase.h"

using namespace cockpit::NH_90;

ElementElectriqueDeBase::ElementElectriqueDeBase()
{
	etat = etat_stand_by;
}

ElementElectriqueDeBase::ElementElectriqueDeBase(etat_des_elements_electrique etat) : ElementElectriqueDeBase(){
	setEtat(etat);
}

ElementElectriqueDeBase::~ElementElectriqueDeBase()
{
	etat = etat_inactif;
}

/* etat_des_elements_electrique getEtat();
* Donne l'�tat de l'�l�ment �lectrique.
*/
void ElementElectriqueDeBase::setEtat(etat_des_elements_electrique etat){
	this->etat = etat;
}
/* void setEtat(etat_des_elements_electrique etat);
* Modifie l'�tat de l'�lement �lectrique.
*/
etat_des_elements_electrique ElementElectriqueDeBase::getEtat() {
	return etat;
}
/* bool ElementElectriqueDeBase::isOnOff() :
* Retourne true si l'�tat est etat_actif.
*/
bool ElementElectriqueDeBase::isOnOff() {
	return (getEtat() == etat_actif);
}
/* void ElementElectriqueDeBase::setOnOff(bool status) :
* Si l'�tat n'est pas etat_panne :
*		- Affecte la valeur etat_actif si status true,
*		- Affecte la valeur etat_stand_by si status false.
*/
void ElementElectriqueDeBase::setOnOff(bool status){
	if (isAlive()){
		if (status) {
			setEtat(etat_actif);
		}
		else {
			setEtat(etat_stand_by);
		}
	}
}
/* void ElementElectriqueDeBase::switchOnOff() :
* Active ou mets en stand_by un �l�ment �lectrique en fonction de son �tat actuelle (voir isOnOff() - voir setOnOff(bool status)).
*/
void ElementElectriqueDeBase::switchOnOff(){
	setOnOff(!isOnOff());
}
/* bool ElementElectriqueDeBase::isAlive() :
* Retourne true si l'�tat est etat_actif ou etat_stand_by.
*/
bool ElementElectriqueDeBase::isAlive(){
	return (this->getEtat() == etat_actif || this->getEtat() == etat_stand_by || this->getEtat() == etat_inactif);
}