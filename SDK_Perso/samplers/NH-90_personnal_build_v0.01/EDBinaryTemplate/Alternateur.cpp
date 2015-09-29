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
* Donne l'�tat sous forme d'entier.
*/
int Alternateur::get_etat(){
	return static_cast<int>(getEtat());
}
/* void setEtat(etat_des_elements_electrique etat);
* Modifie l'�tat de l'�lement �lectrique.
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
* Si le changement d'�tat vient de l'utilisateur, il faut anim� l'action approppri�e dans le cockpit.
*/
void Alternateur::userChangeEtat(float value){
	if (systeme_maitre->isOnOff() && systeme_maitre->getMoteur()) {
		switchOnOff();
	}
	voyant.update((float)static_cast<int>(getEtat()));
	bouton.update((float)static_cast<int>(getEtat()));
}
/* void update();
* Mets � jours les informations de l'�quipement.
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
* Affiche l'�tat de l'alternateur.
* Fonction utilis� pendant de la phase de d�bogage.
*/
void Alternateur::afficheEtat(){
	Debuggueur::log("Alternateur::afficheEtat => 69");
	bouton.afficheEtat();
}