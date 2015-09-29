#include "stdafx.h"
#include "Voyant.h"

#include "SystemeElectrique.h"
#include "ccMainPanel.h"

using namespace cockpit;
using namespace cockpit::element;

Voyant::Voyant()
{
	i_argument = -1;
}
Voyant::~Voyant()
{
}
/* Voyant(int i_argument, cockpit::ccMainPanel* main_panel);
* Instanciation d'un voyant avec son argument et le panel.
*/
Voyant::Voyant(int i_argument, cockpit::NH_90::SystemeElectrique* systeme_electrique){
	this->i_argument = i_argument;
	this->systeme_electrique = systeme_electrique;
}

/* void setValue(int i_value);
* Donne une valeur à 'value' en integer.
*/
void Voyant::setValue(int i_value){
	this->i_value = i_value;
}
/* float getValue();
* Donne la valeur de 'i_value'
*/
float Voyant::getValue(){
	return ((float)i_value) / 100;
}
/* bool isVoyant();
* Dit si oui ou non, c'est un voyant.
*/
bool Voyant::isVoyant(){
	return i_argument != -1;
}
/* void animate();
* Anime l'élément dans le cockpit.
*/
void Voyant::animate(){
	systeme_electrique->get_context()->panel->SetArgument(i_argument, getValue());
}
/* virtual void update();
* Il ne faut pas oublier que s'il n'y a pas de courant dans l'appareil, il n'y a pas de voyant.
*/
void Voyant::update() {
	if (!systeme_electrique->isOnOff()) {
		setValue(static_cast<int>(voyant_defaut::courant_off));
		animate();
	}
}
/* void update(int etat);
* Mets à jour les informations du voyant.
*/
void Voyant::update(int etat) {
	if (isVoyant()) {
		if (systeme_electrique->isOnOff() && systeme_electrique->isDCBAlimented()) {
			mise_a_jour_voyant(etat);
		}
		else {
			setValue(static_cast<int>(voyant_defaut::courant_off));
		}
		animate();
	}
}