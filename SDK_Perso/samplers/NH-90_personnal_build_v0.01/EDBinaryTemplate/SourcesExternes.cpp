#include "stdafx.h"
#include "SourcesExternes.h"
#include "SystemeElectrique.h"

using namespace cockpit;
using namespace cockpit::NH_90;
using namespace cockpit::element;

SourcesExternes::SourcesExternes() : Sources(-1.0, etat_des_elements_electrique::etat_panne), bouton(), voyant() {}
SourcesExternes::SourcesExternes(double nominalVolt) : Sources(nominalVolt) {}
SourcesExternes::SourcesExternes(double nominalVolt, etat_des_elements_electriques_externe etat, type_alim_externe type, SystemeElectrique* systeme_maitre, element::BoutonDeuxPosition bouton, element::VoyantTroisEtat voyant) : 
	Sources(nominalVolt),
	bouton(bouton),
	voyant(voyant){
	setEtatExterne(etat);
	this->type = type;
	this->systeme_maitre = systeme_maitre;
}
SourcesExternes::~SourcesExternes()
{
}

/* virtual etat_des_elements_electrique getEtat() override;
* Donne la correspondance de l'"état de l'équipement électrique externe" en "état des éléments électriques de base".
*/
etat_des_elements_electrique SourcesExternes::getEtat() {
	return etat;
}
/* virtual etat_des_elements_electriques_externe getEtatExterne();
* Donne l'état de l'équipement électrique externe.
*/
etat_des_elements_electriques_externe SourcesExternes::getEtatExterne() {
	return etat_externe;
}
/* int get_etat_externe();
* Donne l'état de la source externe sous forme d'entier.
*/
int SourcesExternes::get_etat_externe(){
	return static_cast<int>(getEtatExterne());
}
/* virtual type_alim_externe getType();
* Donne le type de source externe.
*/
type_alim_externe SourcesExternes::getType(){
	return type;
}
/* void setEtatExterne(etat_des_elements_electriques_externe etat_externe);
* Donne une valeur aux différents états (etat_externe et etat en faisant la correspondance entre les deux type de donné).
*/
void SourcesExternes::setEtatExterne(etat_des_elements_electriques_externe etat_externe) {
	this->etat_externe = etat_externe;
	switch (etat_externe) {
	case etat_des_elements_electriques_externe::element_branche_utilise:
		setEtat(etat_des_elements_electrique::etat_actif);
		break;
	case etat_des_elements_electriques_externe::element_branche_utilise_non_prioritaire:
		setEtat(etat_des_elements_electrique::etat_stand_by);
		break;
	case etat_des_elements_electriques_externe::element_branche_non_utilise:
		setEtat(etat_des_elements_electrique::etat_inactif);
		break;
	default:
		setEtat(etat_des_elements_electrique::etat_panne);
		break;
	}
	voyant.update(get_etat_externe());
}
/* void ElementElectriqueDeBase::setOnOff(bool status) :
* Si l'état n'est pas etat_panne :
*		- Affecte la valeur etat_actif si status true,
*		- Affecte la valeur etat_stand_by si status false.
*/
void SourcesExternes::setOnOff(bool status) {
	if (isAlive()) {
		if (status) {
			checkingUsage();
		}
		else {
			setEtatExterne(etat_des_elements_electriques_externe::element_branche_non_utilise);
		}
	}
}

/* bool isOnOff() :
* Retourne true si l'équipement est on.
*/
bool SourcesExternes::isOnOff() {
	return (etat_externe == etat_des_elements_electriques_externe::element_branche_utilise);
}
/* bool ElementElectriqueDeBase::isAlive() :
* Retourne true si l'équipement externe est branché et pas en panne, peu importe son utilisation.
*/
bool SourcesExternes::isAlive() {
	return ((getEtatExterne() != etat_des_elements_electriques_externe::element_non_branche) && (getEtatExterne() != etat_des_elements_electriques_externe::element_panne));
}
/* bool isUsed();
* Demande au systeme_maitre si l'équipement est utilisé.
*/
bool SourcesExternes::isUsed(){
	return systeme_maitre->externalAlimUsed(getType());
}

/* void branche();
* Branche l'équipement externe.
*/
void SourcesExternes::branche(){
	Debuggueur::log("SourcesExternes::branche => 106");
	if (etat_externe == etat_des_elements_electriques_externe::element_non_branche) {
		setEtatExterne(etat_des_elements_electriques_externe::element_branche_non_utilise);
	}
}
/* void debranche();
* Débranche l'équipement externe.
*/
void SourcesExternes::debranche(){
	Debuggueur::log("SourcesExternes::debranche => 115");
	if (etat_externe != etat_des_elements_electriques_externe::element_non_branche) {
		setEtatExterne(etat_des_elements_electriques_externe::element_non_branche);
	}
}
/* void userChangeEtat(float value) :
* Si le changement d'état vient de l'utilisateur, il faut animé l'action approppriée dans le cockpit.
*/
void SourcesExternes::userChangeEtat(float value){
	if (systeme_maitre->isOnOff()) {
		if (isAlive()) {
			setOnOff (value > 0.0f);
		}
	}
	bouton.update((float)static_cast<int>(getEtat()));
}
/* void update();
* Mets à jour l'état de la source externe (selon ordre de priorité).
*/
void SourcesExternes::update(){
	if (systeme_maitre->isOnOff() && (etat_externe == etat_des_elements_electriques_externe::element_branche_utilise || etat_externe == etat_des_elements_electriques_externe::element_branche_utilise_non_prioritaire)) {
		checkingUsage();
	}
	else {
		voyant.update(get_etat_externe());
	}
}
/* virtual void checkingUsage();
* Vérifie le mode de fonction de la source externe.
*/
void SourcesExternes::checkingUsage(){
	if (isUsed()) {
		setEtatExterne(etat_des_elements_electriques_externe::element_branche_utilise);
	}
	else {
		setEtatExterne(etat_des_elements_electriques_externe::element_branche_utilise_non_prioritaire);
	}
}