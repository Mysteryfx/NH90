#include "stdafx.h"
#include "ElementDeBase_MFX.h"
#include "MFXDevice.h"

using namespace cockpit;
using namespace cockpit::element;

ElementDeBase_MFX::ElementDeBase_MFX(MFXDevice *systeme_maitre, type_element_mfx type_element_base, etat_des_elements_de_base_mfx etat_element_base, float updateTime) :
	systeme_maitre(systeme_maitre),
	type_element_base(type_element_base),
	etat_element_base(etat_element_base),
	updateTime(updateTime)
{}

ElementDeBase_MFX::~ElementDeBase_MFX(){  }

/* etat_des_elements_de_base_mfx setEtatElementBase();
* Donne l'�tat de l'�l�ment.
*/
etat_des_elements_de_base_mfx ElementDeBase_MFX::getEtatElementBase() {
	return this->etat_element_base;
}
/* void etat_element_base(etat_des_elements_de_base_mfx etat_element_base);
* Modifie l'�tat de l'�lement.
*/
void ElementDeBase_MFX::setEtatElementBase(etat_des_elements_de_base_mfx etat_element_base) {
	if (isAlive()) {
		this->etat_element_base = etat_element_base;
		changementEtat();
	}
}
/* type_element_mfx getType()
* Donne le type de l'�l�ment.
*/
type_element_mfx ElementDeBase_MFX::getType(){
	return this->type_element_base;
}
/* void setType(type_element_mfx type_element_base)
* Modifie le type de l'�l�ment.
*/
void ElementDeBase_MFX::setType(type_element_mfx type_element_base){
	this->type_element_base = type_element_base;
}

/* bool ElementDeBase_MFX::isAlive() :
* Retourne true si l'�tat n'est pas en panne.
*/
bool ElementDeBase_MFX::isAlive(){
	return (getEtatElementBase() != etat_des_elements_de_base_mfx::etat_panne_mfx);
}
/* bool isOnOff() :
* Retourne true si l'�tat est etat_actif.
*/
bool ElementDeBase_MFX::isOnOff() {
	return (getEtatElementBase() == etat_des_elements_de_base_mfx::etat_actif_mfx);
}
/* void setOnOff(bool status) :
* Si l'�tat n'est pas etat_panne :
*		- Affecte la valeur etat_actif si status true,
*		- Affecte la valeur etat_inactif si status false.
*/
void ElementDeBase_MFX::setOnOff(bool status){
	action_demande = true;
	if (status) {
		action_a_execute = action_des_elements_mfx::action_demarrage_mfx;
	}
	else {
		action_a_execute = action_des_elements_mfx::action_arret_mfx;
	}
}
/* void switchOnOff() :
* Active ou d�sactive un �l�ment en fonction de son �tat actuelle (voir isOnOff() - voir setOnOff(bool status)).
*/
void ElementDeBase_MFX::switchOnOff(){
	setOnOff(!isOnOff());
}

/* void userAction(action_des_elements_mfx action_a_execute)
* Utilis� lorsque l'action est demand� par l'utilisateur.
* Mise � jour de la variable action_demande � la valeur vrai.
* L'action se fera dans la fonction update();
*/
void ElementDeBase_MFX::userAction(action_des_elements_mfx action_a_execute) {
	this->action_demande = true;
	this->action_a_execute = action_a_execute;
}
/* void actionEffectue()
* Lorsque l'action demand� par l'utilisateur est terminer, toutes les fonctions d'actions font appel � cette fonction.
* Elle valide � l'�l�ment que l'action s'est terminer.
*/
void ElementDeBase_MFX::actionEffectue() {
	this->action_demande = false;
}

/* void update();
* Mets � jours l'�l�ment.
*/
void ElementDeBase_MFX::update() {
	if (action_demande) {
		static int ancienne_etat = static_cast<int>(getEtatElementBase());
		if (getEtatElementBase() != etat_des_elements_de_base_mfx::etat_panne_mfx) {
			ancienne_etat = static_cast<int>(getEtatElementBase());
		}
		switch (action_a_execute) {
		case action_des_elements_mfx::action_arret_mfx:
			arret();
			break;
		case action_des_elements_mfx::action_demarrage_mfx:
			demarrage();
			break;
		case action_des_elements_mfx::action_panne_mfx:
			panne();
			break;
		case action_des_elements_mfx::action_recuperation_mfx:
			recuperation();
			break;
		case action_des_elements_mfx::action_restauration_mfx:
			restauration(ancienne_etat);
			break;
		}
	}
}
/* void demarrage()
* Proc�dure de d�marrage des �l�ments de base.
*/
void ElementDeBase_MFX::demarrage() {
	setEtatElementBase(etat_des_elements_de_base_mfx::etat_actif_mfx);
}
/* void arret()
* Proc�dure d'arr�t des �l�ments de base.
*/
void ElementDeBase_MFX::arret() {
	setEtatElementBase(etat_des_elements_de_base_mfx::etat_inactif_mfx);
}
/* void restauration(int ancienne_etat)
* Proc�dure de restauration des �l�ments de base.
* Remet l'�l�ment � l'ancienne �tat (avant �tat de panne).
*/
void ElementDeBase_MFX::restauration(int ancienne_etat) {
	this->etat_element_base = static_cast<etat_des_elements_de_base_mfx>(ancienne_etat);
	changementEtat();
}
/* void panne()
* Proc�dure de mise en panne des �l�ments de base.
*/
void ElementDeBase_MFX::panne() {
	setEtatElementBase(etat_des_elements_de_base_mfx::etat_panne_mfx);
}
/* void changementEtat()
* Fonction qui pr�vient le syst�me ma�tre que l'�tat de l'�l�ment � changer.
*/
void ElementDeBase_MFX::changementEtat(){
	systeme_maitre->elementAChangerEtat(getType());
}