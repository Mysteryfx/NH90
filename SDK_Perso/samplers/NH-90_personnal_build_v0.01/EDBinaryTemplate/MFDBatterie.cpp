#include "stdafx.h"
#include "MFDBatterie.h"
#include "SystemeElectrique.h"

using namespace cockpit;
using namespace cockpit::NH_90;
using namespace cockpit::ecran;

MFDBatterie::MFDBatterie(){}
MFDBatterie::~MFDBatterie(){}
MFDBatterie::MFDBatterie(std::string name, int argument, cockpit::NH_90::SystemeElectrique* electrique_systeme) : 
	MFD(name, argument, mfd_type::bat, electrique_systeme){

}
MFDBatterie::MFDBatterie(std::string name, int argument, cockpit::NH_90::SystemeElectrique* electrique_systeme, MFD* secours, int argumen_systeme_secour) : 
	MFD(name, argument, mfd_type::bat, electrique_systeme, secours, argumen_systeme_secour) {

}

void MFDBatterie::batterieChange(float value){
	if (batterieIsActivate(value)) {
		if (getName().compare("Batterie_externe") != 0) {
			if (electrique_systeme->batterieUsed(this)) {
				switch (electrique_systeme->getSwitchMode()){
				case switch_mode::data:
					this->value = static_cast<int>(arg_poss_bat::on_data);
					break;
				case switch_mode::normal:
					this->value = static_cast<int>(arg_poss_bat::on_normal);
					break;
				case switch_mode::stand_by:
					this->value = static_cast<int>(arg_poss_bat::on_stand_by);
					break;
				}
			}
			else {
				this->value = static_cast<int>(arg_poss_bat::bat_stand_by);
			}
		}
		else {
			this->value = static_cast<int>(arg_poss_external_source::ext_src_on);
		}
	}
	else {
		if (getName().compare("Batterie_externe") != 0) {
			this->value = static_cast<int>(arg_poss_bat::bat_off);
		}
		else {
			this->value = static_cast<int>(arg_poss_external_source::ext_src_off);
		}
	}
	updateMfd();
}

bool MFDBatterie::batterieIsActivate(float value){
	if (value > 0.0f) {
		return true;
	}
	else {
		return false;
	}
}