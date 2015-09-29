#include "stdafx.h"
#include "Bouton.h"

#include "ccMainPanel.h"

using namespace cockpit;
using namespace cockpit::element;

Bouton::Bouton(){}
Bouton::~Bouton(){}

/* Bouton(int i_argument, ccMainPanel* main_panel, Verrou verrou = Verrou());
* Instanciation d'un bouton, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
* Instanciation d'un bouton sans valeur.
* Verrou optionnel.
*/
Bouton::Bouton(int i_argument, ccMainPanel* main_panel){
	init_bouton(i_argument, main_panel);
}
/* Bouton(int i_argument, int i_value, ccMainPanel* main_panel);
* Instanciation d'un bouton, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
* Instanciation d'un bouton dont on connaît l'entier de la valeur.
*/
Bouton::Bouton(int i_argument, int i_value, ccMainPanel* main_panel){
	init_bouton(i_argument, main_panel);
	setValue(i_value);
}
/* Bouton(int i_argument, float f_value, cockpit::ccMainPanel* main_panel);
* Instanciation d'un bouton, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
* Instanciation d'un bouton dont on connaît la valeur sous forme de nombre flotant.
* Verrou optionnel.
*/
Bouton::Bouton(int i_argument, float f_value, cockpit::ccMainPanel* main_panel){
	init_bouton(i_argument, main_panel);
	setValue(f_value);
}

/* void init_bouton(int i_argument, ccMainPanel* main_panel);
* Initialisation des attribtus d'un bouton.
*/
void Bouton::init_bouton(int i_argument, ccMainPanel* main_panel){
	this->i_argument = i_argument;
	this->main_panel = main_panel;
}

/* void setValue(int i_value);
* Donne une valeur à 'value' à partir d'un integer.
* Rappel : value interval compris entre {-100 , 100} bornes comprises.
*/
void Bouton::setValue(int i_value){
	//Vérification que i_value correspond bien à l'interval fixé.
	if (i_value >= -100 && i_value <= 100) {
		this->i_value = i_value;
	}
	//Si i_value ne correspond pas à l'interval, cette valeur reste importante.
	//This->i_value doit resté dans l'innterval.
	else if (i_value < -100) {
		this->i_value = -100;
	}
	else {
		this->i_value = 100;
	}
}
/* void setValue(float f_value);
* Donne une valeur à 'value' à partir d'un float.
* Rappel : value interval compris entre {-100 , 100} bornes comprises.
*/
void Bouton::setValue(float f_value){
	//f_value interval compris entre {-1.0 , 1.0} bornes comprises.
	//Multiplication par 100 avant conversion en integer pour l'appel de la fonction setValue(int i_value).
	this->setValue((int)(f_value * 100));
}
/* void animate();
* Animation du bouton.
*/
void Bouton::animate(){
	main_panel->SetArgument(i_argument, getValue());
}
/* void afficheEtat();
* Inscrit l'état de l'objet dans les fichiers log.
* Fonction utilisé pendant les phases de débogage.
*/
void Bouton::afficheEtat(){
	Debuggueur::log("Bouton::afficheEtat => 145");
	Debuggueur::log("Argument : " + Utilitaire::convertIntToString(this->i_argument));
	Debuggueur::log("Valuer : " + Utilitaire::convertFloatToString(getValue()));
}