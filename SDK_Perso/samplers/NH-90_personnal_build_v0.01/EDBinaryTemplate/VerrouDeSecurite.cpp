#include "stdafx.h"
#include "VerrouDeSecurite.h"
#include "ccMainPanel.h"

using namespace cockpit;
using namespace cockpit::element;

VerrouDeSecurite::VerrouDeSecurite(){}

VerrouDeSecurite::~VerrouDeSecurite(){}

/* VerrouDeSecurite(int i_argument, ccMainPanel* main_panel, float f_value = 0);
* Instancie un VerrouDeSecurite.
* Obligation de fournir l'argument du verrou ainsi que le panel qui servira pour les animations.
*/
VerrouDeSecurite::VerrouDeSecurite(int i_argument, ccMainPanel* main_panel, float f_value) : Verrou(i_argument, main_panel, f_value) {
	is_bouton_ok = false;
}
/* VerrouDeSecurite(int i_argument, ccMainPanel* main_panel, int i_value = 0);
* Instancie un Verrou.
* Obligation de fournir l'argument du verrou ainsi que le panel qui servira pour les animations.
*/
VerrouDeSecurite::VerrouDeSecurite(int i_argument, ccMainPanel* main_panel, int i_value) : Verrou(i_argument, main_panel, i_value) {
	is_bouton_ok = false;
}
/* VerrouDeSecurite(int i_argument, ccMainPanel* main_panel, int i_argument_bouton, float position_verrouillage_bouton, float f_value = 0);
* Instancie un VerrouDeSecurite.
* Obligation de fournir l'argument du verrou ainsi que le panel qui servira pour les animations.
*/
VerrouDeSecurite::VerrouDeSecurite(int i_argument, ccMainPanel* main_panel, int i_argument_bouton, float position_verrouillage_bouton, float f_value) : 
	Verrou(i_argument, main_panel, f_value){
	is_bouton_ok = false;
	initBouton(i_argument_bouton, position_verrouillage_bouton);
}
/* VerrouDeSecurite(int i_argument, ccMainPanel* main_panel, int i_argument_bouton, float position_verrouillage_bouton, int i_value = 0);
* Instancie un Verrou.
* Obligation de fournir l'argument du verrou ainsi que le panel qui servira pour les animations.
*/
VerrouDeSecurite::VerrouDeSecurite(int i_argument, ccMainPanel* main_panel, int i_argument_bouton, float position_verrouillage_bouton, int i_value) : 
	Verrou(i_argument, main_panel, i_value) {
	is_bouton_ok = false;
	initBouton(i_argument_bouton, position_verrouillage_bouton);
}

/* void lock_unLock(bool bouton_peut_etre_verrouiller);
* Verrouille/Déverrouille le bouton.
**** bool bouton_peut_etre_verrouiller, utilisé si l'on veut verrouillé le bouton.
**** Passé en paramètre à la fonction lock(bool bouton_peut_etre_verrouiller);
*/
void VerrouDeSecurite::lock_unLock(bool bouton_peut_etre_verrouiller) {
	if (!isUnlock()) {
		unlock();
	}
	else {
		lock(bouton_peut_etre_verrouiller);
	}
	animate();
}
/* void lock(bool bouton_peut_etre_verrouiller);
* Vérouille le bouton. Si celui-ci est en position de verrouillage.
*/
void VerrouDeSecurite::lock(bool bouton_peut_etre_verrouiller) {
	if (is_bouton_ok && bouton_peut_etre_verrouiller) {
		Verrou::lock(true);
		verrouillage_du_bouton();
	}
	animate();
}
/* virtual void verrouillage_du_bouton();
* Verrouille le bouton.
*/
void VerrouDeSecurite::verrouillage_du_bouton(){
	main_panel->SetArgument(i_argument_bouton, position_verrouillage_bouton);
}
/* void initBouton(int i_argument_bouton, float position_verrouillage_bouton);
* Initialise une fois les informations du bouton.
*/
void VerrouDeSecurite::initBouton(int i_argument_bouton, float position_verrouillage_bouton) {
	if (!is_bouton_ok) {
		setBouton(i_argument_bouton, position_verrouillage_bouton);
		is_bouton_ok = true;
	}
}
/* void setBouton(int i_argument_bouton, float position_verrouillage_bouton);
* Enregistre les informations du boutons.
*/
void VerrouDeSecurite::setBouton(int i_argument_bouton, float position_verrouillage_bouton) {
	this->i_argument_bouton = i_argument_bouton;
	this->position_verrouillage_bouton = position_verrouillage_bouton;
}