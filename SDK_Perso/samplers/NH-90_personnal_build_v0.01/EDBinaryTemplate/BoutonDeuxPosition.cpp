#include "stdafx.h"
#include "BoutonDeuxPosition.h"

using namespace cockpit::element;

BoutonDeuxPosition::BoutonDeuxPosition(){}
BoutonDeuxPosition::~BoutonDeuxPosition(){}

/* BoutonDeuxPosition(int i_argument, cockpit::ccMainPanel* main_panel);
* Instanciation d'un BoutonDeuxPosition, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
* Instanciation d'un BoutonDeuxPosition sans valeur.
* Verrou optionnel.
*/
BoutonDeuxPosition::BoutonDeuxPosition(int i_argument, cockpit::ccMainPanel* main_panel) : BoutonMecanique(i_argument, main_panel){
	constructeur_initiale_state();
}
/* BoutonDeuxPosition(int i_argument, int i_value, cockpit::ccMainPanel* main_panel);
* Instanciation d'un BoutonDeuxPosition, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
* Instanciation d'un BoutonDeuxPosition dont on connaît l'entier de la valeur.
* Verrou optionnel.
*/
BoutonDeuxPosition::BoutonDeuxPosition(int i_argument, int i_value, cockpit::ccMainPanel* main_panel) : BoutonMecanique (i_argument, i_value, main_panel){
	constructeur_initiale_state();
}
/* BoutonDeuxPosition(int i_argument, float f_value, cockpit::ccMainPanel* main_panel);
* Instanciation d'un BoutonDeuxPosition, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
* Instanciation d'un BoutonDeuxPosition dont on connaît la valeur sous forme de nombre flotant.
* Verrou optionnel.
*/
BoutonDeuxPosition::BoutonDeuxPosition(int i_argument, float f_value, cockpit::ccMainPanel* main_panel) : BoutonMecanique(i_argument, f_value, main_panel) {
	constructeur_initiale_state();
}

/* void setValue(bouton_deux_position bdp_new);
* Donne une valeur à 'value' à partir d'un état de bouton à deux position.
*/
void BoutonDeuxPosition::setValue(bouton_deux_position bdp_new){
	BoutonMecanique::setValue(static_cast<int>(bdp_new));
	etat = bdp_new;
}
/* bouton_deux_position getEtat();
* Donne l'état du bouton.
*/
bouton_deux_position BoutonDeuxPosition::getEtat(){
	return etat;
}

/* void initiale_state(bouton_deux_position bdp_etat);
* Etat initiale du bouton.
* Si cette fonction n'est pas appelé l'état initiale du bouton sera off.
*/
void BoutonDeuxPosition::initiale_state(bouton_deux_position bdp_etat) {
	if (!is_initiale_done) {
		setValue(bdp_etat);
		animate();
		is_initiale_done = true;
	}
}
/* void constructeur_initiale_state();
* Cette fonction initialise l'état du bouton dans les différents constructeurs (bouton.etat = off).
* Elle fait appel à initiale_state(bouton_deux_position bdp_etat).
* Il faut prendre en compte le fait que cette fonction doît pouvoir être appelé de l'extérieur une fois.
* Elle remet donc la valeur de is_initiale_done à faux à la fin de l'appel.
*/
void BoutonDeuxPosition::constructeur_initiale_state(){
	is_initiale_done = false;
	initiale_state(bouton_deux_position::off);
	is_initiale_done = false;
}


/* virtual void switch_bouton(float f_value);
* Gestion de l'appuie sur le bouton par l'utilisateur.
*/
void BoutonDeuxPosition::switch_bouton(float f_value) {
	switch (getEtat()){
	case bouton_deux_position::off:
		setValue(bouton_deux_position::on);
		break;
	case bouton_deux_position::on:
		setValue(bouton_deux_position::off);
		break;
	}
}
/* void afficheEtat();
* Affiche toutes les informations du verrou.
*/
void BoutonDeuxPosition::afficheEtat(){
	Debuggueur::log("BoutonDeuxPosition::afficheEtat => 103");
	Debuggueur::log("Etat du bouton : " + Utilitaire::convertIntToString(static_cast<int>(this->etat)) + " || 100 = on, 0 = off");
	Debuggueur::log("A été initialisé ?");
	Debuggueur::log_boolean(this->is_initiale_done);
	Debuggueur::log("Valeur : " + Utilitaire::convertIntToString(this->i_value));
	BoutonMecanique::afficheEtat();
}