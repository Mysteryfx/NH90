#include "stdafx.h"
#include "Sources.h"

using namespace cockpit::NH_90;

Sources::Sources(double nominalVolt) : ElementElectriqueDeBase()
{
	this->nominalVolt = nominalVolt;
	this->setVoltCourant(this->getNominalVolt());
}

Sources::Sources(double nominalVolt, etat_des_elements_electrique etat) : ElementElectriqueDeBase(etat) {
	this->nominalVolt = nominalVolt;
	this->setVoltCourant(this->getNominalVolt());
}

Sources::Sources(double nominalVolt, etat_des_elements_electrique etat, SystemeElectrique* systeme_maitre) : ElementElectriqueDeBase(etat) {
	this->nominalVolt = nominalVolt;
	this->setVoltCourant(this->getNominalVolt());
	this->systeme_maitre = systeme_maitre;
}

Sources::~Sources()
{
	this->nominalVolt = 0;
	this->voltCourant = 0;
}

/* void setVoltCourant(double volt);
* Modifie le voltage courant (sortie).
*/
void Sources::setVoltCourant(double volt){
	this->voltCourant = volt;
}
/* double getVoltCourant();
* Donne le voltage courant (sortie).
*/
double Sources::getVoltCourant() {
	return this->voltCourant;
}
/* double getNominalVolt();
* Donne le voltage nominal (Volt normal).
*/
double Sources::getNominalVolt() {
	return this->nominalVolt;
}