#include "stdafx.h"
#include "SystemeElectrique.h"

#include "SystemeHydraulique.h"

#include "ElementElectriqueDeBase.h"
#include "WorldManager.h"

using namespace cockpit::NH_90;
using namespace cockpit::element;
using namespace cockpit;


SystemeElectrique::SystemeElectrique() : MFXDevice(type_de_systeme::systeme_electrique_mfx),
	alt1("Alternateur1", 905, type_elem::alt, this), alt2("Alternateur2", 906, type_elem::alt, this), alt3("Alternateur3", 907, type_elem::alt, this),
	acepu_mfd("Alternateur_externe", 904, type_elem::alt, this), dcepu_mfd("Batterie_externe", 919, type_elem::bat, this),
	bat1("Batterie1", 912, type_elem::bat, this), bat2("Batterie2", 913, type_elem::bat, this),
	dbat1("MultipriseDBAT1", 914, type_elem::dbat, this), dbat2("MultipriseDBAT2", 915, type_elem::dbat, this),
	acb1("MultipriseACB1", 900, type_elem::acb, this), acb2("MultipriseACB2", 901, type_elem::acb, this), acb3("MultipriseACB3", 902, type_elem::acb, this), acb4("MultipriseACB4", 903, type_elem::acb, this),
	tru1("Transforedresseur1", 931, type_elem::tru, this), tru2("Transforedresseur2", 932, type_elem::tru, this),
	dcb1("MultipriseDCB1", 916, type_elem::dcb, this), dcb2("MultipriseDCB2", 917, type_elem::dcb, this),
	dcsb1("MultipriseDCSB1", 920, type_elem::dcsb, this), dcsb2("MultipriseDCSB2", 921, type_elem::dcsb, this),
	egen("BatterieSecour", 922, type_elem::egen, this), emerb1("MultipriseEMERB1", 923, type_elem::emerb, this), emerb2("MultipriseEMERB2", 924, type_elem::emerb, this),
	essb1("MultipriseESSB1", 925, type_elem::essb, this), essb2("MultipriseESSB2", 926, type_elem::essb, this),
	sbmb1("MultipriseSBMS1", 929, type_elem::sbmb, this), sbmb2("MultipriseSBMS2", 930, type_elem::sbmb, this)
{
	argument_switch = 3;
	mode_du_switch = switch_mode::normal;
	ancienne_valeur_switch = static_cast<int>(mode_du_switch);
	first_activate_update = true;
	b_test_voyant = false;
	test_voyant_time = 0.0f;
	b_moteur = false;
	is_mco_cut = false;
}
SystemeElectrique::~SystemeElectrique()
{

}

void SystemeElectrique::initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name) {
	avDevice::initialize(ID, Name, script_name);

	dcb1.setSecour(918, &dcb2);
	dcb2.setSecour(918, &dcb1);

	init_multiprise();

	listen_command_range(this, 3001, 3040);

	make_default_activity(0.2);
}

void SystemeElectrique::init_cockpit_mfd(){
	bat1.change(((float)static_cast<int>(arg_poss_bat::bat_off)) / 100);
	bat2.change(((float)static_cast<int>(arg_poss_bat::bat_off)) / 100);

	tru1.change(((float)static_cast<int>(arg_poss_tru::tru_off)) / 100);
	tru2.change(((float)static_cast<int>(arg_poss_tru::tru_off)) / 100);

	alt1.change(((float)static_cast<int>(arg_poss_alt::alt_off)) / 100);
	alt2.change(((float)static_cast<int>(arg_poss_alt::alt_off)) / 100);
	alt3.change(((float)static_cast<int>(arg_poss_alt::alt_off)) / 100);

	dcepu_mfd.change(((float)static_cast<int>(arg_poss_external_source::ext_src_off)) / 100);
	acepu_mfd.change(((float)static_cast<int>(arg_poss_external_source::ext_src_off)) / 100);
}
void SystemeElectrique::init_cockpit_switch(){
	bouton_emergency		= BoutonDeuxPositionAVerrouDeSecurite(14, 0, get_context()->panel, 13);
	//bouton_pompe_electrique = BoutonDeuxPosition(428, get_context()->panel, Verrou(28, get_context()->panel, true));

	bouton_switch_mode	= BoutonTroisPosition(3, 0, type_bouton_trois_position::switch_mode_bouton, get_context()->panel);
	bouton_test_voyant	= BoutonTroisPosition(47, 0, type_bouton_trois_position::test_voyant_bouton, get_context()->panel);

	get_context()->panel->SetArgument(3, (float)static_cast<int>(mode_du_switch));
}
void SystemeElectrique::init_multiprise(){
	element_change(&dcb1, 1.0f); element_change(&dcb2, 1.0f);
	element_change(&dbat1, 1.0f); element_change(&dbat2, 1.0f);
	element_change(&dcsb1, 1.0f); element_change(&dcsb2, 1.0f);
	element_change(&essb1, 1.0f); element_change(&essb2, 1.0f);
	element_change(&emerb1, 1.0f); element_change(&emerb2, 1.0f);
	element_change(&sbmb1, 1.0f); element_change(&sbmb2, 1.0f);
}
void SystemeElectrique::init_sources(){
	///////////////////////////////////////////////////////Batterie
	batterie1 = Batterie(28, etat_des_elements_electrique::etat_inactif, this,
												BoutonDeuxPosition(2, get_context()->panel),
												VoyantDeuxEtat(1, type_voyant_deux_etat::batterie, this));
	batterie2 = Batterie(28, etat_des_elements_electrique::etat_inactif, this,
												BoutonDeuxPosition(4, get_context()->panel),
												VoyantDeuxEtat(5, type_voyant_deux_etat::batterie, this));
	///////////////////////////////////////////////////////Alternateur
	alternateur1 = Alternateur(115, etat_inactif, this,
												BoutonElectrique(423, this),
												VoyantTroisEtat(17, type_voyant_trois_etat::alternateur, this));
	alternateur2 = Alternateur(115, etat_inactif, this,
												BoutonElectrique(426, this),
												VoyantTroisEtat(20, type_voyant_trois_etat::alternateur, this));
	alternateur3 = Alternateur(115, etat_inactif, this,
												BoutonElectrique(424, this),
												VoyantTroisEtat(18, type_voyant_trois_etat::alternateur, this));
	///////////////////////////////////////////////////////Alimentation externe
	acepu = SourcesExternes(200, etat_des_elements_electriques_externe::element_non_branche, type_alim_externe::acepu_alimentation, this, 
												BoutonDeuxPosition(425, get_context()->panel),
												VoyantTroisEtat(19, type_voyant_trois_etat::alimentation_externe, this));
	dcepu = SourcesExternes(28, etat_des_elements_electriques_externe::element_non_branche, type_alim_externe::dcepu_alimentation, this,
												BoutonDeuxPosition(420, get_context()->panel),
												VoyantTroisEtat(8, type_voyant_trois_etat::alimentation_externe, this));
	///////////////////////////////////////////////////////Transforedresseur
	transforedresseur1 = Transforedresseur("1", 29, etat_des_elements_electrique::etat_inactif, this,
												BoutonElectrique(421, this),
												VoyantTroisEtat(11, type_voyant_trois_etat::transforedresseur, this));
	transforedresseur2 = Transforedresseur("2", 29, etat_des_elements_electrique::etat_inactif, this,
												BoutonElectrique(422, this),
												VoyantTroisEtat(16, type_voyant_trois_etat::transforedresseur, this));
}

void SystemeElectrique::SetCommand(int command, float value /*= 0*/) {
	Debuggueur::log("SystemeElectrique::SetCommand => 93");
	switch (command){
	case 3001:
		bouton_switch_mode.update(value);
		switch_mode_change(bouton_switch_mode.getPosition());
		break;
	case 3002:
		Debuggueur::log("Case 3002 - Batterie 1");
		batterie1.userChangeEtat(value);
		element_change(&bat1, (batterie1.isOnOff() ? 1.0f : 0.0f));
		break;
	case 3003:
		Debuggueur::log("Case 3003 - Batterie 2");
		batterie2.userChangeEtat(value);
		element_change(&bat2, (batterie2.isOnOff() ? 1.0f : 0.0f));
		break;
	case 3004:
		Debuggueur::log("Case 3004 - Alternateur 1");
		alternateur1.userChangeEtat(value);
		element_change(&alt1, (alternateur1.isOnOff() ? 1.0f : 0.0f));
		break;
	case 3005:
		Debuggueur::log("Case 3005 - Alternateur 2");
		alternateur2.userChangeEtat(value);
		element_change(&alt2, (alternateur2.isOnOff() ? 1.0f : 0.0f));
		break;
	case 3006:
		Debuggueur::log("Case 3006 - Alternateur 3");
		alternateur3.userChangeEtat(value);
		element_change(&alt3, (alternateur3.isOnOff() ? 1.0f : 0.0f));
		alternateur3.afficheEtat();
		break;
	case 3007:
		element_change(&acb1, value);
		break;
	case 3008:
		element_change(&acb2, value);
		break;
	case 3009:
		element_change(&acb3, value);
		break;
	case 3010:
		element_change(&acb4, value);
		break;
	case 3011:
		Debuggueur::log("Case 3011 - Transforedresseur 1");
		transforedresseur1.userChangeEtat(value);
		element_change(&tru1, (transforedresseur1.isOnOff() ? 1.0f : 0.0f));
		break;
	case 3012:
		Debuggueur::log("Case 3012 - Transforedresseur 2");
		transforedresseur2.userChangeEtat(value);
		element_change(&tru2, (transforedresseur2.isOnOff() ? 1.0f : 0.0f));
		break;
	case 3013:
		Debuggueur::log("Case 3013 - Alimentation externe alternatif");
		acepu.userChangeEtat(value);
		element_change(&acepu_mfd, (acepu.isOnOff() ? 1.0f : 0.0f));
		break;
	case 3014:
		Debuggueur::log("Case 3014 - Alimentation externe continue");
		dcepu.userChangeEtat(value);
		element_change(&dcepu_mfd, (dcepu.isOnOff() ? 1.0f : 0.0f));
		break;
	case 3015:
		Debuggueur::log("Case 3015 - Emergency On/Off");
		bouton_emergency.update(value);
		element_change(&egen, bouton_emergency.getValue());
		break;
	case 3016:
		Debuggueur::log("Case 3016 - Emergency cache");
		bouton_emergency.lock_unlock();
		break;
	case 3030:
		Debuggueur::log("Case 3030 - Branché Alimentation externe alternatif");
		acepu.branche();
		break;
	case 3031:
		Debuggueur::log("Case 3031 - Débranché Alimentation externe alternatif");
		acepu.debranche();
		break;
	case 3032:
		Debuggueur::log("Case 3032 - Branché Alimentation externe continue");
		dcepu.branche();
		break;
	case 3033:
		Debuggueur::log("Case 3033 - Débranché Alimentation externe continue");
		dcepu.debranche();
		break;
	case 3035:
		//bouton_pompe_electrique.update(value);
		//bouton_pompe_electrique.lock_unlock();
		break;
	case 3038:
		b_moteur = !b_moteur;
		break;
	case 3039:
		is_mco_cut = true;
		get_context()->panel->SetArgument(64, 1.0f);
		break;
	}

	update_lights();

	switch (command) {
	case 3034:
		bouton_test_voyant.update(value);
		switch (bouton_test_voyant.getPosition()) {
		case static_cast<int>(enum_test_voyant_bouton::off_test_voyant_bouton) :
			b_test_voyant = false;
			test_voyant_time = 0.0f;
			update_lights();
			break;
		default:
			b_test_voyant = true;
			break;
		}
		break;
	}
	static_cast<SystemeHydraulique*>(get_context()->devices.get("SYSTEMEHYDRAULIQUE"))->electriciteChange(isOnOff());
}

void SystemeElectrique::switch_mode_change(int value){
	switch (value) {
	case static_cast<int>(enum_switch_mode_bouton::data_switch_mode_bouton) :
		this->mode_du_switch = switch_mode::data;
		break;
	case static_cast<int>(enum_switch_mode_bouton::stand_by_switch_mode_bouton) :
		this->mode_du_switch = switch_mode::stand_by;
		break;
	case static_cast<int>(enum_switch_mode_bouton::normal_switch_mode_bouton) :
		this->mode_du_switch = switch_mode::normal;
		break;
	}
}
void SystemeElectrique::element_change(Element* element, float value){
	if (!element->comparaison(value)) {
		element->change(value);
	}
}
switch_mode SystemeElectrique::getSwitchMode(){
	return this->mode_du_switch;
}
bool SystemeElectrique::batterieUsed(Element* batterie){
	if (batterie->getName().compare("Batterie1") == 0){
		if (tru1.getValue() == ((float)static_cast<int>(arg_poss_tru::tru_on)) / 100) {
			return false;
		}
		else {
			return true;
		}
	}
	else {
		if (tru2.getValue() == ((float)static_cast<int>(arg_poss_tru::tru_on)) / 100) {
			return false;
		}
		else {
			return true;
		}
	}
}

void SystemeElectrique::update(){
	if (get_context()->panel->is_active()) {
		if (first_activate_update) {
			init_cockpit_mfd();
			init_cockpit_switch();
			init_sources();
			first_activate_update = false;
		}
		if (isOnOff()) {
			if (!b_test_voyant) {
				//Batteries
				batterie1.update();
				batterie2.update();
			}
			else {
				test_voyant_change(bouton_test_voyant.getPosition());
			}
		}
		else {
			update_lights();
		}
	}
}
bool SystemeElectrique::isSwitched(Element* multiprise){
	switch (multiprise->getType()) {
	case type_elem::essb:
	case type_elem::emerb:
		return (this->mode_du_switch == switch_mode::normal ? true : false);
	case type_elem::dcb:
		if (this->mode_du_switch == switch_mode::normal || this->mode_du_switch == switch_mode::data) {
			return true;
		}
		else {
			return false;
		}
	case type_elem::dcsb:
		return false;
	case type_elem::dbat:
	case type_elem::sbmb:
		return true;
	default:
		return false;
	}
}
bool SystemeElectrique::isDcepuAlimented(Element* multiprise){
	if (dcepu_mfd.getValue() > 0.0f && multiprise->getName().compare(0, multiprise->getName().size() - 1, "MultipriseDCSB") != 0) {
		return true;
	}
	else {
		return false;
	}
}
bool SystemeElectrique::isDcAlimented(Element* multiprise){
	if (multiprise->getName().compare(multiprise->getName().size() - 1, 1, "1") == 0) {
		if (tru1.getValue() > 0.0f || batterie1.isOnOff()) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		if (tru2.getValue() > 0.0f || batterie2.isOnOff()) {
			return true;
		}
		else {
			return false;
		}
	}
}
bool SystemeElectrique::isAcAlimented(Element* multiprise){
	if (acepu.isOnOff()) {
		return true;
	}
	else {
		if (multiprise->getName().compare(multiprise->getName().size() - 1, 1, "1") == 0) {
			if (alternateur1.isOnOff() || alternateur2.isOnOff()) {
				if (!alternateur1.isOnOff()) {
					get_context()->panel->SetArgument(908, 1.0f);
				}
				else {
					get_context()->panel->SetArgument(908, 0.0f);
				}
				return true;
			}
			else {
				return false;
			}
		}
		else if (multiprise->getName().compare(multiprise->getName().size() - 1, 1, "2") == 0) {
			if (alternateur2.isOnOff() || alternateur1.isOnOff()) {
				if (!alternateur2.isOnOff()) {
					get_context()->panel->SetArgument(909, 1.0f);
				}
				else {
					get_context()->panel->SetArgument(909, 0.0f);
				}
				return true;
			}
			else {
				return false;
			}
		}
		else if (multiprise->getName().compare(multiprise->getName().size() - 1, 1, "3") == 0) {
			if (alternateur3.isOnOff() || (alternateur1.isOnOff() && alternateur2.isOnOff())) {
				if (!alternateur3.isOnOff()) {
					get_context()->panel->SetArgument(910, 1.0f);
				}
				else {
					get_context()->panel->SetArgument(910, 0.0f);
				}
				return true;
			}
			else {
				return false;
			}
		}
		else if (multiprise->getName().compare(multiprise->getName().size() - 1, 1, "4") == 0) {
			if (alternateur2.isOnOff() && alternateur1.isOnOff()) {
				if (!alternateur1.isOnOff()) {
					get_context()->panel->SetArgument(911, 1.0f);
				}
				else {
					get_context()->panel->SetArgument(911, 0.0f);
				}
				return true;
			}
			else {
				return false;
			}
		}
		else {
			get_context()->panel->SetArgument(908, 0.0f);
			get_context()->panel->SetArgument(909, 0.0f);
			get_context()->panel->SetArgument(910, 0.0f);
			get_context()->panel->SetArgument(911, 0.0f);
			return false;
		}
	}
}
bool SystemeElectrique::isTruAlimented(Element* transforedresseur){
	if (transforedresseur->getName().compare(transforedresseur->getName().size() - 1, 1, "1") == 0) {
		return (acb1.getValue() > 0.0f ? true : false);
	}
	else {
		return (acb2.getValue() > 0.0f ? true : false);
	}
}
bool SystemeElectrique::isTruDcepuAlimented(Element* transforedresseur){
	if (transforedresseur->getName().compare(transforedresseur->getName().size() - 1, 1, "2") == 0) {
		return (dcepu_mfd.getValue() > 0.0f ? true : false);
	}
	else {
		return false;
	}
}
bool SystemeElectrique::isOnOff(){
	return (!is_mco_cut && (batterie1.isOnOff() || batterie2.isOnOff() || alternateur1.isOnOff() || alternateur2.isOnOff() || alternateur3.isOnOff() || dcepu.isOnOff() || acepu.isOnOff()));
}
double SystemeElectrique::getUpdateTime(){
	return (double)p_device_timer->GetDeltaTime();
}
bool SystemeElectrique::isDCBAlimented(){
	if (dcepu.isOnOff() || transforedresseur1.isOnOff() || transforedresseur2.isOnOff()) {
		return true;
	}
	else {
		return (mode_du_switch != switch_mode::stand_by ? (batterie1.isOnOff() || batterie2.isOnOff()) : false);
	}
}
bool SystemeElectrique::isACBAlimented(std::string id){
	return (id.compare("1") == 0 ? acb1.getValue() > 0.0f : acb2.getValue() > 0.0f);
}
bool SystemeElectrique::externalAlimUsed(type_alim_externe type){
	switch (type){
	case type_alim_externe::dcepu_alimentation:
		return !(transforedresseur1.isOnOff() || transforedresseur2.isOnOff());
	case type_alim_externe::acepu_alimentation:
		return !(alternateur1.isOnOff() || alternateur2.isOnOff());
	default:
		return false;
	}
}
void SystemeElectrique::test_voyant_change(int value) {
	if (isOnOff()) {
		batterie1.teste_voyant(); batterie2.teste_voyant();
		alternateur1.teste_voyant(); alternateur2.teste_voyant(); alternateur3.teste_voyant();
		transforedresseur1.teste_voyant(); transforedresseur2.teste_voyant();
		acepu.teste_voyant(); dcepu.teste_voyant();
	}
	if (value == static_cast<int>(enum_test_voyant_bouton::tempo_test_voyant_bouton)) {
		teste_voyant();
	}
}
void SystemeElectrique::teste_voyant(){
	static const int max_time_test_voyant = 5;
	if (isOnOff()) {
		if (((float)max_time_test_voyant) > test_voyant_time) {
			test_voyant_time += getUpdateTime();
		}
		else {
			test_voyant_time = 0;
			b_test_voyant = false;
			bouton_test_voyant.update(0.0f);
			update_lights();
		}
	}
	else {
		test_voyant_time = 0;
		b_test_voyant = false;
		update_lights();
	}
}
void SystemeElectrique::update_lights(){
	//DCSwitch
	get_context()->panel->SetArgument(dbat1.getArgument(), dbat1.getValue()); get_context()->panel->SetArgument(dbat2.getArgument(), dbat2.getValue());
	get_context()->panel->SetArgument(dcb1.getArgument(), dcb1.getValue()); get_context()->panel->SetArgument(dcb2.getArgument(), dcb2.getValue());
	get_context()->panel->SetArgument(dcsb1.getArgument(), dcsb1.getValue()); get_context()->panel->SetArgument(dcsb2.getArgument(), dcsb2.getValue());
	get_context()->panel->SetArgument(essb1.getArgument(), essb1.getValue()); get_context()->panel->SetArgument(essb2.getArgument(), essb2.getValue());
	get_context()->panel->SetArgument(emerb1.getArgument(), emerb1.getValue()); get_context()->panel->SetArgument(emerb2.getArgument(), emerb2.getValue());
	get_context()->panel->SetArgument(sbmb1.getArgument(), sbmb1.getValue()); get_context()->panel->SetArgument(sbmb2.getArgument(), sbmb2.getValue());
	//ACSwitch
	get_context()->panel->SetArgument(acb1.getArgument(), acb1.getValue()); get_context()->panel->SetArgument(acb2.getArgument(), acb2.getValue());
	get_context()->panel->SetArgument(acb3.getArgument(), acb3.getValue()); get_context()->panel->SetArgument(acb4.getArgument(), acb4.getValue());
	//TRU
	get_context()->panel->SetArgument(tru1.getArgument(), tru1.getValue()); get_context()->panel->SetArgument(tru2.getArgument(), tru2.getValue());
	
	//Alternateur
	alternateur1.update();
	alternateur2.update();

	//SourcesExternes (ACEPU)
	acepu.update();

	alternateur3.update();		//ACEPU prioritaire sur Alternateur 3

	//Transforedresseur
	transforedresseur1.update();
	transforedresseur2.update();

	//SourcesExternes (DCEPU)
	dcepu.update();

	//Batteries
	batterie1.update();
	batterie2.update();
}

REGISTER_WORLD_FACTORY_EX(new WorldFactory<cockpit::NH_90::SystemeElectrique>, SystemeElectrique);