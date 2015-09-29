#include "stdafx.h"
#include "Element.h"
#include "SystemeElectrique.h"
#include "ccMainPanel.h"

using namespace cockpit::NH_90;

Element::Element(std::string name, int argument, type_elem type, SystemeElectrique* electrique_systeme){
	this->name = name;
	this->argument = argument;
	this->type = type;
	switch (this->type){
	case type_elem::bat:
		this->value = 100;
		break;
	default :
		this->value = 0;
	}
	this->electrique_systeme = electrique_systeme;
}
Element::Element(std::string name, int argument, type_elem type, SystemeElectrique* electrique_systeme, Element* secours, int argumen_systeme_secour) : Element(name, argument, type, electrique_systeme) {
	this->secours = secours;
	this->argumen_systeme_secour = argumen_systeme_secour;
}
Element::~Element(){}

void Element::setSecour(int argument_secour, Element* secour){
	this->argumen_systeme_secour = argument_secour;
	this->secours = secour;
}

void Element::change(float value) {
	switch (this->type){
	case alt:
		alternateurChange(value);
		break;
	case bat:
		batterieChange(value);
		break;
	case acb:
		break;
	case egen:
		break;
	case tru:
		transforedresseurChange(value);
		break;
	case emerb:
	case essb:
	case dbat:
	case sbmb:
	case dcb:
	case dcsb:
		dc_multipprise_change(value);
		break;
	}
}

float Element::getValue(){
	switch (this->type){
	case alt:
	case bat:
		return ((float)this->value) / 100;
	case tru:
		return this->getTruVal();
	case emerb:
	case essb:
	case dbat:
	case sbmb:
	case dcb:
	case dcsb:
		return this->getDCVal();
	case acb:
		return this->getAcVal();
	default:
		return 0.0f;
	}
}

int Element::getArgument(){
	return this->argument;
}

type_elem Element::getType(){
	return this->type;
}

bool Element::comparaison(float value){
	return (this->value == (int)(value*100));
}

void Element::batterieChange(float value){
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
	} else {
		if (getName().compare("Batterie_externe") != 0) {
			this->value = static_cast<int>(arg_poss_bat::bat_off);
		}
		else {
			this->value = static_cast<int>(arg_poss_external_source::ext_src_off);
		}
	}
	updateMfd();
}

bool Element::batterieIsActivate(float value){
	if (value > 0.0f) {
		return true;
	}
	else {
		return false;
	}
}

std::string Element::getName(){
	return this->name;
}

void Element::update(){
	switch (type){
	case type_elem::bat:
		updateBat();
		break;
	}
}

void Element::updateBat(){
	batterieChange(getValue());
}

void Element::updateMfd(){
	electrique_systeme->get_context()->panel->SetArgument(this->argument, this->getValue());
}

void Element::dc_multipprise_change(float value){
	if (value != 0.0f) {
		this->value = static_cast<int>(arg_poss_gen::gen_on);
	}
	else {
		this->value = static_cast<int>(arg_poss_gen::gen_off);
	}
}

float Element::getDCVal(){
	if (isDcAlimented()){
		value = static_cast<int>(arg_poss_gen::gen_on);
	}
	else {
		value = static_cast<int>(arg_poss_gen::gen_off);
	}
	if (type == type_elem::dcb && value == static_cast<int>(arg_poss_gen::gen_off)) {
		dc_secourable(isDcSecourable());
	}
	return ((float)value) / 100;
}

bool Element::isDcAlimented() {
	if (electrique_systeme->isDcepuAlimented(this)) {
		return true;
	}
	else {
		return (electrique_systeme->isSwitched(this) && electrique_systeme->isDcAlimented(this));
	}
}

void Element::dc_secourable(bool value){
	if (value) {
		this->value = static_cast<int>(arg_poss_gen::gen_on);
		electrique_systeme->get_context()->panel->SetArgument(this->argumen_systeme_secour, ((float)static_cast<int>(arg_poss_gen::gen_on) / 100));
	}
	else {
		this->value = static_cast<int>(arg_poss_gen::gen_off);
		electrique_systeme->get_context()->panel->SetArgument(this->argumen_systeme_secour, ((float)static_cast<int>(arg_poss_gen::gen_off) / 100));
	}
}

bool Element::isDcSecourable(){
	return secours->isDcAlimented();
}

void Element::alternateurChange(float value){
	if (value > 0.0f) {
		if (getName().compare("Alternateur_externe") != 0) {
			this->value = static_cast<int>(arg_poss_gen::gen_on);
		}
		else {
			this->value = static_cast<int>(arg_poss_external_source::ext_src_on);
		}
	}
	else {
		if (getName().compare("Alternateur_externe") != 0) {
			this->value = static_cast<int>(arg_poss_gen::gen_off);
		}
		else {
			this->value = static_cast<int>(arg_poss_external_source::ext_src_off);
		}
	}
	updateMfd();
}

float Element::getAcVal(){
	if (isAcAlimented()){
		value = static_cast<int>(arg_poss_gen::gen_on);
	}
	else {
		value = static_cast<int>(arg_poss_gen::gen_off);
	}
	return ((float)value) / 100;
}

bool Element::isAcAlimented(){
	return electrique_systeme->isAcAlimented(this);
}

void Element::transforedresseurChange(float value){
	if (value > 0.0f) {
		this->value = static_cast<int>(arg_poss_tru::tru_on);
	}
	else {
		this->value = static_cast<int>(arg_poss_tru::tru_off);
	}
}

float Element::getTruVal(){
	if (isTruAlimented()) {
		if (this->value == static_cast<int>(arg_poss_tru::tru_off)) {
			return ((float)static_cast<int>(arg_poss_tru::tru_standby)) / 100;
		}
		else {
			return ((float)this->value) / 100;
		}
	}
	else {
		if (electrique_systeme->isTruDcepuAlimented(this)) {
			return ((float)static_cast<int>(arg_poss_tru::standby_dcepu)) / 100;
		}
		else {
			return ((float)static_cast<int>(arg_poss_tru::tru_off)) / 100;
		}
	}
}

bool Element::isTruAlimented(){
	return electrique_systeme->isTruAlimented(this);
}