#include "stdafx.h"
#include "BoutonTroisPosition.h"

using namespace cockpit;
using namespace cockpit::element;

BoutonTroisPosition::BoutonTroisPosition()
{
}


BoutonTroisPosition::~BoutonTroisPosition()
{
}

/* BoutonTroisPosition(int i_argument, ccMainPanel* main_panel = Verrou());
* Instanciation d'un BoutonMecanique mécanique, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
* Instanciation d'un BoutonMecanique mécanique sans valeur.
* Verrou optionnel.
*/
BoutonTroisPosition::BoutonTroisPosition(int i_argument, type_bouton_trois_position type, ccMainPanel* main_panel) : 
BoutonMecanique(i_argument, main_panel){
	this->type = type;
}
/* BoutonTroisPosition(int i_argument, int i_value, cockpit::ccMainPanel* main_panel = Verrou());
* Instanciation d'un bouton mécanique, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
* Instanciation d'un bouton mécanique sans valeur.
* Verrou optionnel.
*/
BoutonTroisPosition::BoutonTroisPosition(int i_argument, int i_value, type_bouton_trois_position type, cockpit::ccMainPanel* main_panel) :
BoutonMecanique(i_argument, main_panel){
	this->type = type;
	setValue(i_value);
}

/* void setValue(int i_value);
* Donne une valeur à 'value' à partir d'un integer.
* Modifie aussi la position du bouton.
*/
void BoutonTroisPosition::setValue(int i_value){
	BoutonMecanique::setValue(i_value);
	switch (type) {
	case type_bouton_trois_position::switch_mode_bouton:
		if (i_value > 0) {
			setPosition(static_cast<int>(enum_switch_mode_bouton::stand_by_switch_mode_bouton));
		}
		else if (i_value < 0) {
			setPosition(static_cast<int>(enum_switch_mode_bouton::data_switch_mode_bouton));
		}
		else {
			setPosition(static_cast<int>(enum_switch_mode_bouton::normal_switch_mode_bouton));
		}
		break;
	case type_bouton_trois_position::test_voyant_bouton:
		if (i_value > 0) {
			setPosition(static_cast<int>(enum_test_voyant_bouton::hold_test_voyant_bouton));
		}
		else if (i_value < 0) {
			setPosition(static_cast<int>(enum_test_voyant_bouton::tempo_test_voyant_bouton));
		}
		else {
			setPosition(static_cast<int>(enum_test_voyant_bouton::off_test_voyant_bouton));
		}
		break;
	}
}
/* void setValue(float f_value);
* Donne une valeur à 'value' à partir d'un nombre flotant.
* Modifie aussi la position du bouton.
*/
void BoutonTroisPosition::setValue(float f_value){
	setValue((int)(f_value * 100));
}
/* virtual void switch_bouton(float f_value);
* Gestion de l'appuie sur le bouton par l'utilisateur.
*/
void BoutonTroisPosition::switch_bouton(float f_value) {
	switch (type) {
	case type_bouton_trois_position::switch_mode_bouton:
		switch_bouton_switch_mode();
		break;
	case type_bouton_trois_position::test_voyant_bouton:
		switch_bouton_test_voyant();
		break;
	}
	animate();
}
/* virtual void switch_bouton(float f_value);
* Gestion de l'appuie sur le bouton par l'utilisateur pour un bouton de type switch_mode_bouton.
*/
void BoutonTroisPosition::switch_bouton_switch_mode() {
	static int precedante_position = enum_switch_mode_bouton::stand_by_switch_mode_bouton;
	if (position != static_cast<int>(enum_switch_mode_bouton::normal_switch_mode_bouton)) {
		precedante_position = position;
		setValue(static_cast<int>(enum_switch_mode_bouton::normal_switch_mode_bouton));
	}
	else {
		switch (precedante_position) {
		case static_cast<int>(enum_switch_mode_bouton::data_switch_mode_bouton) :
			precedante_position = position;
			setValue(static_cast<int>(enum_switch_mode_bouton::stand_by_switch_mode_bouton));
			break;
		case static_cast<int>(enum_switch_mode_bouton::stand_by_switch_mode_bouton) :
			precedante_position = position;
			setValue(static_cast<int>(enum_switch_mode_bouton::data_switch_mode_bouton));
			break;
		}
	}
}
/* virtual void switch_bouton(float f_value);
* Gestion de l'appuie sur le bouton par l'utilisateur pour un bouton de type test_voyant_bouton.
*/
void BoutonTroisPosition::switch_bouton_test_voyant() {
	static int precedante_position = enum_test_voyant_bouton::hold_test_voyant_bouton;
	if (position != static_cast<int>(enum_test_voyant_bouton::off_test_voyant_bouton)) {
		precedante_position = position;
		setValue(static_cast<int>(enum_test_voyant_bouton::off_test_voyant_bouton));
	}
	else {
		switch (precedante_position) {
		case static_cast<int>(enum_test_voyant_bouton::hold_test_voyant_bouton) :
			precedante_position = position;
			setValue(static_cast<int>(enum_test_voyant_bouton::tempo_test_voyant_bouton));
			break;
		case static_cast<int>(enum_test_voyant_bouton::tempo_test_voyant_bouton) :
			precedante_position = position;
			setValue(static_cast<int>(enum_test_voyant_bouton::hold_test_voyant_bouton));
			break;
		}
	}
}