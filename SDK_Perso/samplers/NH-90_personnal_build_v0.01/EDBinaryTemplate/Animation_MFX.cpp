#include "stdafx.h"
#include "Animation_MFX.h"
#include "ccMainPanel.h"

using namespace cockpit;
using namespace cockpit::animation;

/* Animation_MFX(int argument, ccMainPanel * panel)
* Pour créer une animation, il faut :
*		- argument (nombre entier), l'argument de l'objet animable dans le cockpit;
*		- panel (pointer vers ccMainPanel), le panel avec lequel on fera les animations.
*/
Animation_MFX::Animation_MFX(int argument, ccMainPanel * panel)
{
	this->argument = argument;
	this->panel = panel;
}
Animation_MFX::~Animation_MFX()
{
	this->argument = 0;
	this->panel = nullptr;
}

/* float getValeur()
* Donne la valeur de l'argument.
*/
float Animation_MFX::getValeur(){
	return this->panel->GetArgument(this->argument);
}
/* int getArgument()
* Donne l'argument.
*/
int Animation_MFX::getArgument() {
	return this->argument;
}
/* void setValeur(float valeur)
* Modifie la valeur de l'argument.
*/
void Animation_MFX::setValeur(float valeur){
	this->panel->SetArgument(this->argument, valeur);
}
