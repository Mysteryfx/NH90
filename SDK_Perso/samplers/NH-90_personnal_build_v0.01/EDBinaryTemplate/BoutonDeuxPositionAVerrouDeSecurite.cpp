#include "stdafx.h"
#include "BoutonDeuxPositionAVerrouDeSecurite.h"

using namespace cockpit;
using namespace cockpit::element;

BoutonDeuxPositionAVerrouDeSecurite::BoutonDeuxPositionAVerrouDeSecurite(){}
BoutonDeuxPositionAVerrouDeSecurite::~BoutonDeuxPositionAVerrouDeSecurite(){}
/* BoutonDeuxPositionAVerrouDeSecurite(int i_argument, ccMainPanel* main_panel, VerrouDeSecurite verrou);
* Instanciation d'un bouton à deux position, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
* Instanciation d'un bouton à deux position sans valeur.
* Verrou optionnel.
*/
BoutonDeuxPositionAVerrouDeSecurite::BoutonDeuxPositionAVerrouDeSecurite(int i_argument, ccMainPanel* main_panel, VerrouDeSecurite verrou) :
	BoutonDeuxPosition(i_argument, main_panel),
	BoutonMecaniqueAVerrou(i_argument, main_panel),
	verrou(verrou){
	verrou.initBouton(getArgument(), getValue());
}
/* BoutonDeuxPositionAVerrouDeSecurite(int i_argument, int i_value, ccMainPanel* main_panel, int i_argument_verrou, float f_value_verrou = 0);
* Instanciation d'un bouton à deux position, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
* Instanciation d'un bouton à deux position dont on connaît l'entier de la valeur.
*/
BoutonDeuxPositionAVerrouDeSecurite::BoutonDeuxPositionAVerrouDeSecurite(int i_argument, int i_value, ccMainPanel* main_panel, int i_argument_verrou, float f_value_verrou) : 
	BoutonDeuxPosition(i_argument, i_value, main_panel),
	BoutonMecaniqueAVerrou(i_argument, i_value, main_panel),
	verrou(i_argument_verrou, main_panel, f_value_verrou){
	verrou.initBouton(getArgument(), getValue());
}
/* BoutonDeuxPositionAVerrouDeSecurite(int i_argument, float f_value, ccMainPanel* main_panel, int i_verrou_argument, float f_value_verrou);
* Instanciation d'un bouton à deux position, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
* Instanciation d'un bouton à deux position on connaît la valeur en nombre flottant.
*/
BoutonDeuxPositionAVerrouDeSecurite::BoutonDeuxPositionAVerrouDeSecurite(int i_argument, float f_value, ccMainPanel* main_panel, int i_argument_verrou, float f_value_verrou) :
	BoutonDeuxPosition(i_argument, f_value, main_panel),
	BoutonMecaniqueAVerrou(i_argument, f_value, main_panel),
	verrou(i_argument_verrou, main_panel, this->getArgument(), this->getValue(), f_value_verrou) {
}

/* void setValue(int i_value);
* Donne une valeur à 'value' à partir d'un integer.
*/
void BoutonDeuxPositionAVerrouDeSecurite::setValue(int i_value) {
	BoutonMecaniqueAVerrou::setValue(i_value);
	BoutonDeuxPosition::setValue(static_cast<bouton_deux_position>(i_value));
}
/* void setValue(float f_value);
* Donne une valeur à 'value' à partir d'un float.
*/
void BoutonDeuxPositionAVerrouDeSecurite::setValue(float f_value) {
	BoutonMecaniqueAVerrou::setValue(f_value);
	BoutonDeuxPosition::setValue(static_cast<bouton_deux_position>((int)(f_value * 100)));
}

/* virtual void lock(bool bouton_peut_etre_verrouille);
* Fonction qui permet de verrouillé le bouton si celui-ci est dans une position qui le permet (bool bouton_peut_etre_verrouille).
*/
void BoutonDeuxPositionAVerrouDeSecurite::lock(bool bouton_peut_etre_verrouille){
	verrou.lock(bouton_peut_etre_verrouille);
}
/* virtual void unlock();
* Fonction de déverrouillage du bouton.
*/
void BoutonDeuxPositionAVerrouDeSecurite::unlock(){
	verrou.unlock();
}
/* virtual void lock_unlock(bool bouton_peut_etre_verrouille);
* Fonction qui alterne entre verrouillage et déverrouillage du bouton.
**** bool bouton_peut_etre_verrouille, dis si le bouton est en position de verrouillage.
**** Utilisé en passant le paramètre à la fonction lock(bool bouton_peut_etre_verrouille.
*/
void BoutonDeuxPositionAVerrouDeSecurite::lock_unlock(bool bouton_peut_etre_verrouille){
	if (isLocked()) {
		unlock();
	}
	else {
		lock();
	}
}
/* virtual bool isLocked();
* Dis si le bouton est verrouille.
*/
bool BoutonDeuxPositionAVerrouDeSecurite::isLocked(){
	return !verrou.isUnlock();
}