#include "stdafx.h"
#include "Alternateur.h"
#include "SystemeElectrique.h"
#include "BoutonElectrique.h"

using namespace cockpit;
using namespace cockpit::NH_90;
using namespace cockpit::element;

Alternateur::Alternateur() : Sources(-1.0, etat_des_elements_electrique::etat_panne), bouton(), voyant() {}

Alternateur::Alternateur(double nominalVolt) : Sources(nominalVolt, etat_inactif){}

Alternateur::Alternateur(double nominalVolt, etat_des_elements_electrique etat, SystemeElectrique* systeme_maitre, element::BoutonElectrique bouton, element::VoyantTroisEtat voyant) :
	Sources(nominalVolt, etat, systeme_maitre),
	bouton(bouton),
	voyant(voyant){
}

Alternateur::~Alternateur(){}

/* int get_etat();
* Donne l'état sous forme d'entier.
*/
int Alternateur::get_etat(){
	return static_cast<int>(getEtat());
}
/* void setEtat(etat_des_elements_electrique etat);
* Modifie l'état de l'élement électrique.
*/
void Alternateur::setEtat(etat_des_elements_electrique etat){
	if (this->getEtat() != etat){
		if (etat == etat_panne || etat == etat_inactif) {
			setVoltCourant(0.0);
		}
		Sources::setEtat(etat);
		voyant.update(get_etat());
	}
}
/* void userChangeEtat(float value) :
* Si le changement d'état vient de l'utilisateur, il faut animé l'action approppriée dans le cockpit.
*/
void Alternateur::userChangeEtat(float value){
	if (systeme_maitre->isOnOff() && systeme_maitre->getMoteur()) {
		switchOnOff();
	}
	voyant.update((float)static_cast<int>(getEtat()));
	bouton.update((float)static_cast<int>(getEtat()));
}
/* void update();
* Mets à jours les informations de l'équipement.
*/
void Alternateur::update(){
	if (systeme_maitre->getMoteur()) {
		if (getEtat() == etat_des_elements_electrique::etat_inactif) {
			setEtat(etat_des_elements_electrique::etat_actif);
		}
	}
	else {
		setEtat(etat_des_elements_electrique::etat_inactif);
	}
	voyant.update((float)static_cast<int>(getEtat()));
}
/* void afficheEtat();
* Affiche l'état de l'alternateur.
* Fonction utilisé pendant de la phase de débogage.
*/
void Alternateur::afficheEtat(){
	Debuggueur::log("Alternateur::afficheEtat => 69");
	bouton.afficheEtat();
}