#include "stdafx.h"
#include "Batterie.h"
#include "SystemeElectrique.h"

#include "ccMainPanel.h"

using namespace cockpit::NH_90;

Batterie::Batterie() : Sources(-1.0, etat_des_elements_electrique::etat_panne), bouton(), voyant() {}
Batterie::~Batterie(){}
/* Batterie(int i_voltNominal, etat_des_elements_electrique etat, SystemeElectrique* systeme_maitre, cockpit::element::BoutonDeuxPosition bouton, cockpit::element::VoyantDeuxEtat voyant);
* Instancie une batterie avec un bouton et un voyant.
*/
Batterie::Batterie(int i_voltNominal, etat_des_elements_electrique etat, /*double temperature,*/ SystemeElectrique* systeme_maitre, BoutonDeuxPosition bouton, VoyantDeuxEtat voyant) :
	Sources(i_voltNominal),
	bouton(bouton),
	voyant(voyant){
	initBatterie(etat, systeme_maitre);
}

/* void initBatterie(etat_des_elements_electrique etat, SystemeElectrique* systeme_maitre) :
* Initialisation des propriétés de la batterie.
*/
void Batterie::initBatterie(/*double temperature,*/ etat_des_elements_electrique etat, SystemeElectrique* systeme_maitre){
	this->systeme_maitre	= systeme_maitre;
	this->inCharge			= false;
	this->life				= 0;
	//this->setTemperature(temperature);
	this->setEtat(etat);
	//Etat d'origine des batteries = etat_actif par défaut, le bouton de la batterie doit correspondre à cet état.
	if (isOnOff()){
		bouton.initiale_state(cockpit::element::bouton_deux_position::on);
	}
	else {
		bouton.initiale_state(cockpit::element::bouton_deux_position::off);
	}
}

/* void setTemperature(double temperature) :
* Modification de la température (en degrée Celcius).
*/
/*void Batterie::setTemperature(double temperature) {
	this->temperature = temperature;
}*/

/* double getTemperature() :
* Fournis la température (en degrée Celcius).
*/
/*double Batterie::getTemperature() {
	return this->temperature;
}*/

void Batterie::setEtat(etat_des_elements_electrique etat){
	if (this->getEtat() != etat){
		if (etat == etat_panne || etat == etat_inactif) {
			setVoltCourant(0.0);
		}
		Sources::setEtat(etat);
		voyant.update(static_cast<int>(getEtat()));
	}
}

/* bool isMcoCut() :
* Retourne true si la manette MCO qui gouverne le système électrique à été coupé.
*/
bool Batterie::isMcoCut(){
	return false;
}
/* void update() :
* Cette fonction va agire directement sur la durée de vie de la batterie.
*/
void Batterie::update(){
	//Debuggueur::log("Batterie::update()");
	if (isOnOff()  && !isInCharge()) {
		switch (systeme_maitre->getSwitchMode()){
		case switch_mode::data:
			life += systeme_maitre->getUpdateTime()*(Batterie::lifetime / (1.5 * 60 * 60)); //Durée de vie en continu en mode data = 1H30.
			break;
		case switch_mode::normal:
			life += systeme_maitre->getUpdateTime();										//Durée de vie en continu en mode normal = 10 minutes.
			break;
		case switch_mode::stand_by:
			life += systeme_maitre->getUpdateTime()*(Batterie::lifetime / (4 * 60 * 60));	//Durée de vie en mode stand by = 4H.
			break;
		}
		if ((int)life >= Batterie::lifetime) {
			life = Batterie::lifetime;
			setEtat(etat_des_elements_electrique::etat_inactif); //Si la batterie n'a plus de durée vie (à atteins sa limite) alors elle est vide (une batterie vide est dite inactive).
		}
		if ((int)life >= Batterie::lifetime - 30) {
			setVoltCourant(getNominalVolt() * ( (life / Batterie::lifetime - 1) != 0 ? (((life / Batterie::lifetime - 1) * (-18)) + 1 - (26/28)) : 0));
		}
		else {
			setVoltCourant(getNominalVolt() * ((28 / 27.5 - (life / Batterie::lifetime)) > 1 ? (28 / 27.5 - (life / Batterie::lifetime)) : (1 - ((26/28) - ((Batterie::lifetime - life) / Batterie::lifetime)) / 100)));
		}
	}
	else if (isAlive() && isInCharge()) {
		life -= (life != 0 ? systeme_maitre->getUpdateTime()*(Batterie::lifetime / (1.5 * 60 * 60)) : 0);
		if (life < 0) {
			life = 0;
		}
	}
	voyant.update(static_cast<int>(getEtat()));
}
/* void charge(double voltCharge) :
* Cette fonction dit à la batterie qu'elle est en charge.
*/
void Batterie::charge(double voltCharge){
	if(!isInCharge()){ //Si la batterie n'est pas déjà en cours de rechargement, on peut la rechargé.
		this->voltCharge = voltCharge;
		inCharge = true;
	}
}
/* bool isInCharge() :
* Cette fonction dit si la batterie est en recharge.
*/
bool Batterie::isInCharge(){
	return inCharge;
}
/* void userChangeEtat(float value) :
* Si le changement d'état vient de l'utilisateur, il faut animé l'action appropriée dans le cockpit.
*/
void Batterie::userChangeEtat(float valeur){
	setOnOff(valeur > 0.0f);
	bouton.update((float)static_cast<int>(getEtat()));
	voyant.update(static_cast<int>(getEtat()));
}
/* cockpit::NH_90::switch_mode getSwitchedMode() :
* Retourne le mode d'utilisation de la batterie.
*/
cockpit::NH_90::switch_mode Batterie::getSwitchedMode(){
	return systeme_maitre->getSwitchMode();
}
/* bool isOnOff() :
* Fonctionnement différent de ce qui était initialement prévue.
* Retourne toujours true si etat_actif et false sinon.
*/
bool Batterie::isOnOff() {
	if (getNominalVolt() < 0.0) { //Par défaut, si le voltage nominal n'est pas renseigné, il est initialisé à -1.
		return false;
	}
	else {
		return Sources::isOnOff();
	}
}
/* void afficheEtat();
* Affiche toutes les informations du verrou.
*/
void Batterie::afficheEtat(){
	Debuggueur::log("Batterie::afficheEtat => 185");
	Debuggueur::log("Etat de la batterie : " + Utilitaire::convertIntToString(static_cast<int>(this->etat)));
	Debuggueur::log("Est-ce que la batterie est en charge ?");
	Debuggueur::log_boolean(this->inCharge);
	Debuggueur::log("Life : " + Utilitaire::convertIntToString(this->life));
	Debuggueur::log("Nominal volt : " + Utilitaire::convertFloatToString(this->nominalVolt) + " |||||| Volt courant : " + Utilitaire::convertFloatToString(this->voltCourant));
	Debuggueur::log("Volt charge : " + Utilitaire::convertFloatToString(this->voltCharge));
	bouton.afficheEtat();
	voyant.afficheEtat();
}