#include "stdafx.h"
#include "ReservoirHuile_MFX.h"

using namespace cockpit;
using namespace cockpit::fluide;
using namespace cockpit::element;

ReservoirHuile_MFX::ReservoirHuile_MFX(MFXDevice *systeme_maitre, type_reservoir_huile_mfx type_reservoir, float reservoir, bool remplie) :
	ElementFluide_MFX(systeme_maitre, type_de_fluide_mfx::huile_mfx, reservoir, remplie),
	type_reservoir_huile(type_reservoir)
{}

ReservoirHuile_MFX::~ReservoirHuile_MFX()
{
	setQuantite(0.0f);
}

/* float aQuantiteDe(float quantite_ajuster)
* Ajuste la quantité dans le réservoir de la quantité demandé.
*/
void ReservoirHuile_MFX::aQuantiteDe(float quantite_ajuster){
	if ((quantite + quantite_ajuster) > getReservoir()) {
		setQuantite(getReservoir());
	}
	else if ((quantite + quantite_ajuster) < 0.0f) {
		setQuantite(0.0f);
	}
	else {
		setQuantite(quantite + quantite_ajuster);
	}
}
/* float aQuantiteA(float quantite_voulue)
* Ajuste la quantité dans le réservoir à la quantité voulue.
*/
void ReservoirHuile_MFX::aQuantiteA(float quantite_voulue) {
	float increment;
	switch (type_reservoir_huile) {
	case type_reservoir_huile_mfx::hydraulique_mfx:
		increment = 15.0f;
		break;
	case type_reservoir_huile_mfx::moteur_mfx:
		increment = 0.01f;
		break;
	}
	if (quantite_voulue < quantite) {
		increment = (-increment);
	}
	while (quantite != quantite_voulue) {
		aQuantiteDe(increment);
		if ((increment < 0) && (quantite < quantite_voulue)) {
			setQuantite(quantite_voulue);
		}
		else if (quantite > quantite_voulue) {
			setQuantite(quantite_voulue);
		}
	}
}

/* void update();
* Mets à jours les informations de l'équipement.
*/
void ReservoirHuile_MFX::update(){
	float ajustement = 0.0f;
	switch (type_reservoir_huile) {
	case type_reservoir_huile_mfx::moteur_mfx:
		if (isAlive()) {
			ajustement = 0.1f / (8.0f * (60.0f * 60.0f) / 0.1f);		//Perte de 0.1 litre d'huile moteur pour 8 heure de vol.
		}
		else {
			ajustement = 0.0f;
		}
		break;
	}
	ajusterQuantiteDe(ajustement);
}