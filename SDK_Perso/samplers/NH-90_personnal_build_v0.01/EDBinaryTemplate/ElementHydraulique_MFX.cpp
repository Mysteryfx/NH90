#include "stdafx.h"
#include "ElementHydraulique_MFX.h"

using namespace cockpit;
using namespace cockpit::element;

const float ElementHydraulique_MFX::PRESSIONMARCHE = 163.0f;

ElementHydraulique_MFX::ElementHydraulique_MFX(MFXDevice *systeme_maitre, float temperature_courante, float pression_courante, type_element_mfx type_element_hydraulique, etat_des_elements_de_base_mfx etat_element_hydraulique, float updateTime) :
	ElementMecanique_MFX(systeme_maitre, type_element_hydraulique, etat_element_hydraulique),
	temperature_courante(temperature_courante),
	pression_courante(pression_courante)
{
	switch (type_element_hydraulique) {
	case type_element_mfx::hydraulique_mfx:
		this->pression_nominal = (float)static_cast<int>(pression_hydraulique_nominal_mfx::PRESS_SYSTEME_HYDRAULIQUE_mfx);
		this->temperature_nominal = (float)static_cast<int>(temperature_hydraulique_nominal_mfx::TEMP_SYSTEME_HYDRAULIQUE_mfx);
		break;
	case type_element_mfx::robinet_sov_mfx:
		this->pression_nominal = (float)static_cast<int>(pression_hydraulique_nominal_mfx::PRESS_ROBINET_SOV_HYDRAULIQUE_mfx);
		this->temperature_nominal = (float)static_cast<int>(temperature_hydraulique_nominal_mfx::TEMP_ROBINET_SOV_HYDRAULIQUE_mfx);
		break;
	case type_element_mfx::tuyau_hydraulique_mfx:
		this->pression_nominal = (float)static_cast<int>(pression_hydraulique_nominal_mfx::PRESS_TUYAU_HYDRAULIQUE_mfx);
		this->temperature_nominal = (float)static_cast<int>(temperature_hydraulique_nominal_mfx::TEMP_TUYAU_HYDRAULIQUE_mfx);
		break;
	case type_element_mfx::verin_hydraulique_mfx:
		this->pression_nominal = (float)static_cast<int>(pression_hydraulique_nominal_mfx::PRESS_VERRIN_HYDRAULIQUE_mfx);
		this->temperature_nominal = (float)static_cast<int>(temperature_hydraulique_nominal_mfx::TEMP_VERRIN_HYDRAULIQUE_mfx);
		break;
	case type_element_mfx::pompe_mfx:
		this->pression_nominal = (float)static_cast<int>(pression_hydraulique_nominal_mfx::PRESS_POMPE_HYDRAULIQUE_mfx);
		this->temperature_nominal = (float)static_cast<int>(temperature_hydraulique_nominal_mfx::TEMP_POMPE_HYDRAULIQUE_mfx);
		break;
	case type_element_mfx::pompe_electrique_mfx:
		this->pression_nominal = (float)static_cast<int>(pression_hydraulique_nominal_mfx::PRESS_POMPE_ELECTRIQUE_HYDRAULIQUE_mfx);
		this->temperature_nominal = (float)static_cast<int>(temperature_hydraulique_nominal_mfx::TEMP_POMPE_ELECTRIQUE_HYDRAULIQUE_mfx);
		break;
	}
}
ElementHydraulique_MFX::~ElementHydraulique_MFX()
{
	this->setTemperature(-9999.9f);
	this->setPression(0.0f);
	//this->panne();
}

/* float getTemperature()
* Donne la température de l'élément.
*/
float ElementHydraulique_MFX::getTemperature(){
	return this->temperature_courante;
}
/* float getPression()
* Donne la pression de l'élément.
*/
float ElementHydraulique_MFX::getPression(){
	return this->pression_courante;
}
/* float getTemperatureNominal()
* Donne la température nominal.
*/
float ElementHydraulique_MFX::getTemperatureNominal() {
	return this->temperature_nominal;
}
/* float getPressionNominal()
* Donne la pression nominal.
*/
float ElementHydraulique_MFX::getPressionNominal() {
	return this->pression_nominal;
}
/* void setTemperature()
* Modifie la température de l'élément.
*/
void ElementHydraulique_MFX::setTemperature(float temperature){
	this->temperature_courante = temperature;
}
/* void setPression()
* Modifie la pression de l'élément.
*/
void ElementHydraulique_MFX::setPression(float pression){
	this->pression_courante = pression;
}

void ElementHydraulique_MFX::arret() {
	static bool pas_encore_arreter = true;
	if (pression_courante > 0.0f) {
		pression_courante -= (getPressionNominal() / 5.0f * updateTime);
		if (pas_encore_arreter && (pression_courante < 163)) {
			setEtatElementBase(etat_des_elements_de_base_mfx::etat_inactif_mfx);
			pas_encore_arreter = false;
		}
		if (pression_courante <= 0.0f) {
			pression_courante = 0.0f;
			pas_encore_arreter = true;
			actionEffectue();
		}
	}
}