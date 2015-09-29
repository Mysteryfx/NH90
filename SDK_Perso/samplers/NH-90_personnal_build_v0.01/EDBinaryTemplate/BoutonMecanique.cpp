#include "stdafx.h"
#include "BoutonMecanique.h"


using namespace cockpit::element;

BoutonMecanique::BoutonMecanique(){}
BoutonMecanique::~BoutonMecanique(){}

/* BoutonMecanique(int i_argument, cockpit::ccMainPanel* main_panel);
* Instanciation d'un BoutonMecanique, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
* Instanciation d'un BoutonMecanique sans valeur.
* Verrou optionnel.
*/
BoutonMecanique::BoutonMecanique(int i_argument, cockpit::ccMainPanel* main_panel) : Bouton(i_argument, main_panel){

}
/* BoutonMecanique(int i_argument, int i_value, cockpit::ccMainPanel* main_panel);
* Instanciation d'un BoutonMecanique, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
* Instanciation d'un BoutonMecanique dont on connaît l'entier de la valeur.
* Verrou optionnel.
*/
BoutonMecanique::BoutonMecanique(int i_argument, int i_value, cockpit::ccMainPanel* main_panel) : Bouton(i_argument, i_value, main_panel) {
	animate();
}
/* BoutonMecanique(int i_argument, float f_value, cockpit::ccMainPanel* main_panel);
* Instanciation d'un BoutonMecanique, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
* Instanciation d'un BoutonMecanique dont on connaît la valeur sous forme de nombre flotant.
* Verrou optionnel.
*/
BoutonMecanique::BoutonMecanique(int i_argument, float f_value, cockpit::ccMainPanel* main_panel) : Bouton(i_argument, f_value, main_panel) {
	animate();
}

/* virtual float getValue();
* Donne la valeur de 'value' en nombre flottant.
*/
float BoutonMecanique::getValue(){
	return ((float)i_value) / 100;
}

/* virtual void update(float f_value = 0);
* Mets à jour les informations du BoutonMecanique.
*/
void BoutonMecanique::update(float f_value){
	switch_bouton(f_value);
	animate();
}