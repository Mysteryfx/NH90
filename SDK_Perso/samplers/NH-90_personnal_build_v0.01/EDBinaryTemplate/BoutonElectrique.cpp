#include "stdafx.h"
#include "BoutonElectrique.h"
#include "SystemeElectrique.h"

#include <thread>

using namespace cockpit;
using namespace cockpit::element;
using namespace cockpit::NH_90;

BoutonElectrique::BoutonElectrique()
{
}


BoutonElectrique::~BoutonElectrique()
{
}

/* BoutonElectrique(int i_argument, SystemeElectrique* systeme_maitre = Verrou());
* Instanciation d'un bouton mécanique, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
* Instanciation d'un bouton mécanique sans valeur.
* Verrou optionnel.
*/
BoutonElectrique::BoutonElectrique(int i_argument, SystemeElectrique* systeme_maitre) : Bouton(i_argument, systeme_maitre->get_context()->panel)
{
	this->systeme_maitre = systeme_maitre;
	setValue(0.0f);
}
/* virtual void update(float f_value = 0);
* Mets à jour les informations du bouton.
*/
void BoutonElectrique::update(float f_value){
	if (systeme_maitre->isOnOff()) {
		switch_bouton(f_value);
	}
	animate();
}
/* void animate();
* Animation du bouton.
*/
void BoutonElectrique::animate(){
	/*
	int cycle_par_seconde = 30;
	for (float i = 0; i < 100.0f ; i = i + (100 / (cycle_par_seconde / 2))) {
		setValue(i / 100);
		Bouton::animate();
	}
	for (float i = 100; i > 0.0f; i = i - (100 / (cycle_par_seconde / 2))) {
		setValue(i / 100);
		Bouton::animate();
	}
	setValue(0);
	*/
	setValue((getValue() == 0.0f ? 1.0f : 0.0f));
}