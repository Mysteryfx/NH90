#include "stdafx.h"
#include "VoyantDeuxEtat.h"
#include "Batterie.h"
#include "SystemeElectrique.h"

using namespace cockpit;
using namespace cockpit::element;
using namespace cockpit::NH_90;

VoyantDeuxEtat::VoyantDeuxEtat() : Voyant() {}
VoyantDeuxEtat::~VoyantDeuxEtat(){}
VoyantDeuxEtat::VoyantDeuxEtat(int i_argument, type_voyant_deux_etat type, SystemeElectrique* systeme_electrique) : Voyant(i_argument, systeme_electrique){
	this->type = type;
	setValue(static_cast<int>(voyant_defaut::courant_off));
	animate();
}

/* void mise_a_jour_voyant(int etat);
* Mets à jour les informations du voyant.
*/
void VoyantDeuxEtat::mise_a_jour_voyant(int etat){
	switch (type) {
	case type_voyant_deux_etat::batterie:
		switch (etat){
		case static_cast<int>(etat_des_elements_electrique::etat_actif):
			setValue(static_cast<int>(enum_voyant_batterie::on_voyant_batterie));
			break;
		default:
			setValue(static_cast<int>(enum_voyant_batterie::off_panne_voyant_batterie));
			break;
		}
		break;
	}
}
/* void afficheEtat();
* Affiche toutes les informations du verrou.
*/
void VoyantDeuxEtat::afficheEtat(){
	Debuggueur::log("VoyantDeuxEtat::afficheEtat => 42");
	Debuggueur::log("Argument : " + Utilitaire::convertIntToString(this->i_argument) + " |||||| Valeur : " + Utilitaire::convertIntToString(this->i_value) + " || test_voyant = -100, on_voyant et pas_de_courant_voyant = 0, off_panne_voyant = 100");
}