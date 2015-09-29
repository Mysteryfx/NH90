#include "stdafx.h"
#include "BoutonMecaniqueAVerrou.h"

using namespace cockpit;
using namespace cockpit::element;

BoutonMecaniqueAVerrou::BoutonMecaniqueAVerrou()
{
}


BoutonMecaniqueAVerrou::~BoutonMecaniqueAVerrou()
{
}

/* BoutonMecaniqueAVerrou(int i_argument, ccMainPanel* main_panel, Verrou verrou = Verrou());
* Instanciation d'un BoutonMecanique m�canique, le minimum � savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
* Instanciation d'un BoutonMecanique m�canique sans valeur.
* Verrou optionnel.
*/
BoutonMecaniqueAVerrou::BoutonMecaniqueAVerrou(int i_argument, ccMainPanel* main_panel) : BoutonMecanique(i_argument, main_panel){}
/* BoutonMecaniqueAVerrou(int i_argument, int i_value, ccMainPanel* main_panel);
* Instanciation d'un BoutonMecanique m�canique, le minimum � savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
* Instanciation d'un BoutonMecanique m�canique dont on conna�t l'entier de la valeur.
*/
BoutonMecaniqueAVerrou::BoutonMecaniqueAVerrou(int i_argument, int i_value, ccMainPanel* main_panel) :
	BoutonMecanique (i_argument, i_value, main_panel){
}
/* BoutonMecaniqueAVerrou(int i_argument, float f_value, ccMainPanel* main_panel);
* Instanciation d'un BoutonMecanique m�canique, le minimum � savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
* Instanciation d'un BoutonMecanique m�canique on conna�t la valeur en nombre flottant.
*/
BoutonMecaniqueAVerrou::BoutonMecaniqueAVerrou(int i_argument, float f_value, ccMainPanel* main_panel) :
	BoutonMecanique (i_argument, f_value, main_panel){
}

/* virtual void update(float f_value = 0);
* Mets � jour les informations du BoutonMecanique.
*/
void BoutonMecaniqueAVerrou::update(float f_value){
	if (isLocked()) {
		unlock();
	}
	else {
		switch_bouton(f_value);
	}
	animate();
}
