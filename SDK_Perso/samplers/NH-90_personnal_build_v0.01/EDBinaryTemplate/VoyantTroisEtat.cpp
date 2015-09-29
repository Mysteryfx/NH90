#include "stdafx.h"
#include "VoyantTroisEtat.h"
#include "Alternateur.h"
#include "SourcesExternes.h"
#include "SystemeElectrique.h"

using namespace cockpit;
using namespace cockpit::element;
using namespace cockpit::NH_90;

VoyantTroisEtat::VoyantTroisEtat() : Voyant() {}
VoyantTroisEtat::~VoyantTroisEtat(){}
VoyantTroisEtat::VoyantTroisEtat(int i_argument, type_voyant_trois_etat type, SystemeElectrique* systeme_electrique) : Voyant(i_argument, systeme_electrique){
	this->type = type;
	setValue(static_cast<int>(voyant_defaut::courant_off));
	animate();
}

/* void mise_a_jour_voyant(int etat);
* Mets à jour les informations du voyant.
*/
void VoyantTroisEtat::mise_a_jour_voyant(int etat){
	switch (type) {
	case type_voyant_trois_etat::alimentation_externe:
		majAlimExterne(etat);
		break;
	case type_voyant_trois_etat::alternateur:
		majAlternateur(etat);
		break;
	case type_voyant_trois_etat::transforedresseur:
		majTransforedresseur(etat);
		break;
	}
}
/* void majAlimExterne(int etat);
* Mets à jours les informations du voyant en ce basant sur les états d'une alimentation externe.
*/
void VoyantTroisEtat::majAlimExterne(int etat){
	switch (etat){
	case static_cast<int>(etat_des_elements_electriques_externe::element_branche_utilise) :
		setValue(static_cast<int>(enum_voyant_alim_externe::fleche_voyant_alim_externe));
		break;
	case static_cast<int>(etat_des_elements_electriques_externe::element_branche_non_utilise) :
		setValue(static_cast<int>(enum_voyant_alim_externe::bouton_voyant_alim_externe));
		break;
	case static_cast<int>(etat_des_elements_electriques_externe::element_branche_utilise_non_prioritaire) :
		setValue(static_cast<int>(enum_voyant_alim_externe::stand_by_voyant_alim_externe));
		break;
	case static_cast<int>(etat_des_elements_electriques_externe::element_non_branche) :
		setValue(static_cast<int>(enum_voyant_alim_externe::off_voyant_alim_externe));
		break;
	default:
		setValue(static_cast<int>(enum_voyant_alim_externe::panne_voyant_alim_externe));
		break;
	}
}
/* void majAlternateur(int etat);
* Mets à jours les informations du voyant en ce basant sur les états d'un alternateur.
*/
void VoyantTroisEtat::majAlternateur(int etat){
	switch (etat){
	case static_cast<int>(etat_des_elements_electrique::etat_actif) :
		setValue(static_cast<int>(enum_voyant_alternateur::on_voyant_alternateur));
		break;
	case static_cast<int>(etat_des_elements_electrique::etat_inactif) :
		setValue(static_cast<int>(enum_voyant_alternateur::off_voyant_alternateur));
		break;
	case static_cast<int>(etat_des_elements_electrique::etat_panne) :
		setValue(static_cast<int>(enum_voyant_alternateur::panne_voyant_alternateur));
		break;
	case static_cast<int>(etat_des_elements_electrique::etat_stand_by) :
		setValue(static_cast<int>(enum_voyant_alternateur::stand_by_voyant_alternateur));
		break;
	}
}
/* void majTransforedresseur(int etat);
* Mets à jours les informtaions du voyant en ce basant sur les états d'un transforedresseur.
*/
void VoyantTroisEtat::majTransforedresseur(int etat){
	switch (etat){
	case static_cast<int>(etat_des_elements_electrique::etat_actif) :
		setValue(static_cast<int>(enum_voyant_transforedresseur::on_voyant_transforedresseur));
		break;
	case static_cast<int>(etat_des_elements_electrique::etat_inactif) :
		setValue(static_cast<int>(enum_voyant_transforedresseur::off_voyant_transforedresseur));
		break;
	case static_cast<int>(etat_des_elements_electrique::etat_panne) :
		setValue(static_cast<int>(enum_voyant_transforedresseur::panne_voyant_transforedresseur));
		break;
	case static_cast<int>(etat_des_elements_electrique::etat_stand_by) :
		setValue(static_cast<int>(enum_voyant_transforedresseur::stand_by_voyant_transforedresseur));
		break;
	}
}
/* void afficheEtat();
* Affiche toutes les informations du verrou.
*/
void VoyantTroisEtat::afficheEtat(){
	Debuggueur::log("VoyantTroisEtat::afficheEtat => 35");
	Debuggueur::log("Argument : " + Utilitaire::convertIntToString(this->i_argument) + " |||||| Valeur : " + Utilitaire::convertIntToString(this->i_value) + " || test_voyant = -100, on_voyant et pas_de_courant_voyant = 0, off_panne_voyant = 100");
}