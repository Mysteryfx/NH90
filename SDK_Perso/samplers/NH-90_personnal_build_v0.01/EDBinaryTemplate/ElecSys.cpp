#include "stdafx.h"
#include "ElecSys.h"

#include "ccMainPanel.h"
#include "WorldManager.h"

using namespace cockpit::NH_90;

ElecSys::ElecSys() : alt1("Alternateur1", 905, type_elem::alt, this), alt2("Alternateur2", 906, type_elem::alt, this), alt3("Alternateur3", 907, type_elem::alt, this), 
acepu("Alternateur_externe", 904, type_elem::alt, this), dcepu("Batterie_externe", 919, type_elem::bat, this),
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
	argument_switch = 3 ;
	mode_du_switch = switch_mode::normal;
	ancienne_valeur_switch = static_cast<int>(mode_du_switch);
	first_activate_update = true;
}


ElecSys::~ElecSys()
{

}

void ElecSys::initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name) {
	avDevice::initialize(ID, Name, script_name);

	dcb1.setSecour(918, &dcb2);
	dcb2.setSecour(918, &dcb1);

	init_multiprise();

	listen_command_range(this, 3001, 3030);

	make_default_activity(0.04);
}

void ElecSys::init_cockpit_mfd(){
	bat1.change(((float)static_cast<int>(arg_poss_bat::on_normal)) / 100);
	bat2.change(((float)static_cast<int>(arg_poss_bat::on_normal)) / 100);

	tru1.change(((float)static_cast<int>(arg_poss_tru::tru_off)) / 100);
	tru2.change(((float)static_cast<int>(arg_poss_tru::tru_off)) / 100);

	alt1.change(((float)static_cast<int>(arg_poss_alt::alt_off)) / 100);
	alt2.change(((float)static_cast<int>(arg_poss_alt::alt_off)) / 100);
	alt3.change(((float)static_cast<int>(arg_poss_alt::alt_off)) / 100);

	dcepu.change(((float)static_cast<int>(arg_poss_external_source::ext_src_off)) / 100);
	acepu.change(((float)static_cast<int>(arg_poss_external_source::ext_src_off)) / 100);
}

void ElecSys::init_cockpit_switch(){
	get_context()->panel->SetArgument(1, 1.0f);
	get_context()->panel->SetArgument(4, 1.0f);
	get_context()->panel->SetArgument(3, (float)static_cast<int>(mode_du_switch));
	get_context()->panel->SetArgument(13, 1.0f);
}

void ElecSys::init_multiprise(){
	element_change(&dcb1, 1.0f); element_change(&dcb2, 1.0f);
	element_change(&dbat1, 1.0f); element_change(&dbat2, 1.0f);
	element_change(&dcsb1, 1.0f); element_change(&dcsb2, 1.0f);
	element_change(&essb1, 1.0f); element_change(&essb2, 1.0f);
	element_change(&emerb1, 1.0f); element_change(&emerb2, 1.0f);
	element_change(&sbmb1, 1.0f); element_change(&sbmb2, 1.0f);
}

void ElecSys::SetCommand(int command, float value /*= 0*/) {
	switch (command){
	case 3001:
		switch_mode_change(value);
		break;
	case 3002:
		element_change(&bat1, value);
		break;
	case 3003:
		element_change(&bat2, value);
		break;
	case 3004:
		element_change(&alt1, value);
		break;
	case 3005:
		element_change(&alt2, value);
		break;
	case 3006:
		element_change(&alt3, value);
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
		element_change(&tru1, value);
		break;
	case 3012:
		element_change(&tru2, value);
		break;
	case 3013:
		element_change(&acepu, value);
		break;
	case 3014:
		element_change(&dcepu, value);
		break;
	case 3015:
		element_change(&egen, value);
		break;
	}
}

void ElecSys::switch_mode_change(float value){
	if (value < 0.0f) {
		this->mode_du_switch = switch_mode::data;
	}
	else if (value > 0.0f) {
		this->mode_du_switch = switch_mode::stand_by;
	}
	else {
		this->mode_du_switch = switch_mode::normal;
	}
	bat1.update();
	bat2.update();
}

void ElecSys::element_change(Element* element, float value){
	if (!element->comparaison(value)) {
		element->change(value);
	}
}

switch_mode ElecSys::getSwitchMode(){
	return this->mode_du_switch;
}

bool ElecSys::batterieUsed(Element* batterie){
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

void ElecSys::update(){
	if (get_context()->panel->is_active()) {
		if (first_activate_update) {
			init_cockpit_mfd();
			init_cockpit_switch();
			first_activate_update = false;
		}
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
	}
}

bool ElecSys::isSwitched(Element* multiprise){
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

bool ElecSys::isDcepuAlimented(Element* multiprise){
	if (dcepu.getValue() > 0.0f && multiprise->getName().compare(0, multiprise->getName().size() - 1, "MultipriseDCSB") != 0) {
		return true;
	}
	else {
		return false;
	}
}

bool ElecSys::isDcAlimented(Element* multiprise){
	if (multiprise->getName().compare(multiprise->getName().size() - 1, 1, "1") == 0) {
		if (tru1.getValue() > 0.0f || bat1.getValue() > 0.0f) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		if (tru2.getValue() > 0.0f || bat2.getValue() > 0.0f) {
			return true;
		}
		else {
			return false;
		}
	}
}

bool ElecSys::isAcAlimented(Element* multiprise){
	if (acepu.getValue() > 0.0f) {
		return true;
	}
	else {
		if (multiprise->getName().compare(multiprise->getName().size() - 1, 1, "1") == 0) {
			if (alt1.getValue() > 0.0f || alt2.getValue() > 0.0f) {
				if (alt1.getValue() == 0.0f) {
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
			if (alt2.getValue() > 0.0f || alt1.getValue() > 0.0f) {
				if (alt2.getValue() == 0.0f) {
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
			if (alt3.getValue() > 0.0f || (alt1.getValue() > 0.0f && alt2.getValue() > 0.0f)) {
				if (alt3.getValue() == 0.0f) {
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
			if (alt2.getValue() > 0.0f && alt1.getValue() > 0.0f) {
				if (alt1.getValue() == 0.0f) {
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

bool ElecSys::isTruAlimented(Element* transforedresseur){
	if (transforedresseur->getName().compare(transforedresseur->getName().size() - 1, 1, "1") == 0) {
		return (acb1.getValue() > 0.0f ? true : false);
	} else {
		return (acb2.getValue() > 0.0f ? true : false);
	}
}

bool ElecSys::isTruDcepuAlimented(Element* transforedresseur){
	if (transforedresseur->getName().compare(transforedresseur->getName().size()-1, 1, "2") == 0) {
		return (dcepu.getValue() > 0.0f ? true : false);
	}
	else {
		return false;
	}
}

REGISTER_WORLD_FACTORY_EX(new WorldFactory<cockpit::NH_90::ElecSys>, ElecSys);