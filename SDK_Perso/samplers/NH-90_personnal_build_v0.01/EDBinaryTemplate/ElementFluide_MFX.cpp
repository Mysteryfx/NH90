#include "stdafx.h"
#include "ElementFluide_MFX.h"

using namespace cockpit;
using namespace cockpit::element;

ElementFluide_MFX::ElementFluide_MFX(MFXDevice *systeme_maitre, type_de_fluide_mfx type_fluide, float reservoir, bool remplie, type_element_mfx type_element_fluide, etat_des_elements_de_base_mfx etat_element_fluide) :
ElementDeBase_MFX(systeme_maitre, type_element_fluide, etat_element_fluide),
	type(type_fluide), reservoir(reservoir), quantite((remplie ? reservoir : 0.0f))
{}
ElementFluide_MFX::~ElementFluide_MFX()
{
	this->panne();
	this->setQuantite(0.0f);
}

/* type_de_fluide_mfx getType()
* Donne le type de fluide utilisé.
*/
type_de_fluide_mfx ElementFluide_MFX::getType(){
	return this->type;
}
/* float getQuantite()
* Donne la quantité de fluide actuelle.
*/
float ElementFluide_MFX::getQuantite(){
	return this->quantite;
}
/* float getReservoir()
* Donne la quantité de fluide maximal dans le reservoir.
*/
float ElementFluide_MFX::getReservoir(){
	return this->reservoir;
}
/* void setQuantite(float quantite)
* Modifie la quantité actuelle de fluide.
*/
void ElementFluide_MFX::setQuantite(float quantite){
	if (quantite > reservoir) {
		quantite = reservoir;
	}
	else if (quantite < 0.0f) {
		quantite = 0.0f;
	}
	this->quantite = quantite;
}
/* float ajusterQuantiteDe(float quantite_ajuster)
* Ajuster la quantite d'une somme donne.
*/
void ElementFluide_MFX::ajusterQuantiteDe(float quantite_ajuster) {
	if (!isAlive()) {
		quantite_ajuster -= 10.0f;
	}
	if ( (quantite_ajuster + quantite) > reservoir) {
		quantite_ajuster = reservoir - quantite;
	}
	else if ( (quantite_ajuster + quantite) < 0.0f) {
		quantite_ajuster = 0.0f - quantite;
	}
	if ((isFull() && (quantite_ajuster < 0.0f)) || (isEmpty() && (quantite_ajuster > 0.0f))) {
		aQuantiteDe(quantite_ajuster);
	}
}
/* float ajusterQuantiteA(float quantite_voulue)
* Ajuste la quantite jusqu'a une quantite souhaité.
*/
void ElementFluide_MFX::ajusterQuantiteA(float quantite_voulue) {
	if (!isAlive()) {
		quantite_voulue -= 50.0f;
	}
	if (quantite_voulue > reservoir) {
		quantite_voulue = reservoir;
	}
	else if (quantite_voulue < 0.0f) {
		quantite_voulue = 0.0f;
	}
	if (quantite_voulue != quantite) {
		aQuantiteA(quantite_voulue);
	}
}