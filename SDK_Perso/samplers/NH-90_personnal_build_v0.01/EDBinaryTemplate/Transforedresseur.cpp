#include "stdafx.h"
#include "Transforedresseur.h"
#include "SystemeElectrique.h"

using namespace cockpit;
using namespace cockpit::NH_90;
using namespace cockpit::element;

Transforedresseur::Transforedresseur() : ElementElectriqueDeBase(etat_inactif) {}
Transforedresseur::Transforedresseur(std::string id, double voltSortie, etat_des_elements_electrique etat, SystemeElectrique* systeme_maitre, element::BoutonElectrique bouton, element::VoyantTroisEtat voyant) :
	ElementElectriqueDeBase(etat),
	bouton(bouton),
	voyant(voyant)
{
	this->id = id;
	this->systeme_maitre = systeme_maitre;
	this->setSortie(voltSortie);
}

Transforedresseur::~Transforedresseur(){}

/* void setEntree(double voltEntree);
* Modifie le voltage d'entrée.
*/
void Transforedresseur::setEntree(double voltEntree){
	this->voltEntree = voltEntree;
}
/* void setSortie(double voltSortie);
* Modifie le voltage de sortie.
*/
void Transforedresseur::setSortie(double voltSortie){
	this->voltSortie = voltSortie;
}
/* double getEntree();
* Donne le voltage en entrée.
*/
double Transforedresseur::getEntree(){
	return this->voltEntree;
}
/* double getSortie();
* Donne le voltage en sortie.
*/
double Transforedresseur::getSortie(){
	return this->voltSortie;
}
/* void userChangeEtat(float value) :
* Si le changement d'état vient de l'utilisateur, il faut animé l'action approppriée dans le cockpit.
*/
void Transforedresseur::userChangeEtat(float value){
	if (systeme_maitre->isOnOff()) {
		if (systeme_maitre->isACBAlimented(id)) {
			switchOnOff();
			bouton.update((float)static_cast<int>(getEtat()));
		}
		voyant.update((float)static_cast<int>(getEtat()));
	}
}
/* void update();
* Mets à jours les informations de l'équipement.
*/
void Transforedresseur::update(){
	if (systeme_maitre->isACBAlimented(id)) {
		if (getEtat() == etat_des_elements_electrique::etat_inactif) {
			setEtat(etat_des_elements_electrique::etat_actif);
		}
	}
	else {
		if (getEtat() == etat_des_elements_electrique::etat_actif || getEtat() == etat_des_elements_electrique::etat_stand_by) {
			setEtat(etat_des_elements_electrique::etat_inactif);
		}
	}
	voyant.update((float)static_cast<int>(getEtat()));
}