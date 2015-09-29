#include "stdafx.h"
#include "Verrou.h"

#include "ccMainPanel.h"

using namespace cockpit;
using namespace cockpit::element;

Verrou::Verrou(){}


Verrou::~Verrou()
{
}

/* Verrou(int i_argument, ccMainPanel* main_panel, bool b_isRemovable = false, float f_value = 0);
* Instancie un Verrou.
* Obligation de fournir l'argument du verrou ainsi que le panel qui servira pour les animations.
* Si vous souhaitez informer i_argument et f_value, veuillez utiliser le constructeur Verrou(int i_argument, float f_value, bool b_isRemovable).
*/
Verrou::Verrou(int i_argument, ccMainPanel* main_panel, float f_value){
	init_verrou(i_argument, main_panel);
	setValue(f_value);
}
/* Verrou(int i_argument, ccMainPanel* main_panel, int i_value = 0);
* Instancie un Verrou.
* Obligation de fournir l'argument du verrou ainsi que le panel qui servira pour les animations.
*/
Verrou::Verrou(int i_argument, ccMainPanel* main_panel, int i_value){
	init_verrou(i_argument, main_panel);
	setValue(i_value);
}

/* void init_verrou(int i_argument, ccMainPanel* main_panel);
* Initialise les attributs obligatoires du verrou.
*/
void Verrou::init_verrou(int i_argument, ccMainPanel* main_panel){
	this->i_argument = i_argument;
	this->main_panel = main_panel;
}

/* bool isLocked();
* Dit si le verrou est en place ou non.
*/
bool Verrou::isUnlock(){
	return i_value == static_cast<int>(enum_verrou::deverrouille);
}
/* void lock_unLock(bool bouton_peut_etre_verrouiller);
* Verrouille/Déverrouille le bouton.
**** bool bouton_peut_etre_verrouiller, utilisé si l'on veut verrouillé le bouton.
**** Passé en paramètre à la fonction lock(bool bouton_peut_etre_verrouiller);
*/
void Verrou::lock_unLock(bool bouton_peut_etre_verrouiller){
	if (!isUnlock()) {
		unlock();
	}
	else {
		lock(bouton_peut_etre_verrouiller);
	}
}
/* float getValue();
* Donne la valeur de 'value' en nombre flottant.
*/
float Verrou::getValue() {
	return (float)i_value;
}
/* void animate();
* Anime l'élément dans le cockpit.
*/
void Verrou::animate(){
	main_panel->SetArgument(i_argument, this->getValue());
}
/* void setValue(int i_value);
* Donne une valeur à 'value' en integer.
*/
void Verrou::setValue(int i_value){
	this->i_value = i_value;
}
/* void setValue(float f_value);
* Donne une valeur à 'value' à partir d'un nombre flottant.
*/
void Verrou::setValue(float f_value){
	this->i_value = (int)f_value;
}
/* void unlock();
* Déverrouille le bouton.
*/
void Verrou::unlock(){
	setValue(static_cast<int>(enum_verrou::deverrouille));
	animate();
}
/* void lock(bool bouton_peut_etre_verrouiller);
* Vérouille le bouton. Si celui-ci est en position de verrouillage.
*/
void Verrou::lock(bool bouton_peut_etre_verrouiller){
	if (bouton_peut_etre_verrouiller) {
		setValue(static_cast<int>(enum_verrou::verrouille));
	}
	animate();
}
/* void afficheEtat();
* Affiche toutes les informations du verrou.
*/
void Verrou::afficheEtat() {
	Debuggueur::log("Verrou::afficheEtat => 101");
	Debuggueur::log("Argument : " + Utilitaire::convertIntToString(this->i_argument) + " |||||| Valeur : " + Utilitaire::convertIntToString(this->i_value));
}