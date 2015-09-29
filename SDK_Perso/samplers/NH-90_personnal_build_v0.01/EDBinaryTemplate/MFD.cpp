#include "stdafx.h"
#include "MFD.h"
#include "SystemeElectrique.h"

using namespace cockpit;
using namespace cockpit::NH_90;
using namespace cockpit::ecran;

MFD::MFD(){}
MFD::MFD(std::string name, int argument, mfd_type type, SystemeElectrique* electrique_systeme){
	this->name = name;
	this->argument = argument;
	this->type = type;
	switch (this->type){
	case mfd_type::bat:
		this->value = 100;
		break;
	default:
		this->value = 0;
	}
	this->electrique_systeme = electrique_systeme;
}
MFD::MFD(std::string name, int argument, mfd_type type, SystemeElectrique* electrique_systeme, MFD* secours, int argumen_systeme_secour) : MFD(name, argument, type, electrique_systeme) {
	this->secours = secours;
	this->argumen_systeme_secour = argumen_systeme_secour;
}
MFD::~MFD(){}

void MFD::setSecour(int argument_secour, MFD* secour){
	this->argumen_systeme_secour = argument_secour;
	this->secours = secour;
}


int MFD::getArgument(){
	return this->argument;
}

mfd_type MFD::getType(){
	return this->type;
}

bool MFD::comparaison(float value){
	return (this->value == (int)(value * 100));
}

std::string MFD::getName(){
	return this->name;
}