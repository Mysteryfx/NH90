#include "stdafx.h"
#include "SystemeTemplate.h"

#include "ccMainPanel.h"
#include "WorldManager.h"

using namespace cockpit;
using namespace cockpit::NH_90;

//Instruction register world factory ex permet de mettre cette classe � disposition.
//Si cette instruction n'est pas pr�sente dans la classe faisant office de Device, celle-ci ne pourra �tre utilis� avec le lua.
//REGISTER_WOLRD_FACTORY_EX(new WorldFactory<appel complet de la classe>, nom d'appel de la classe souhait�);
REGISTER_WORLD_FACTORY_EX(new WorldFactory<cockpit::NH_90::SystemeTemplate>, SystemeTemplate);

static const int timer_activity = 0.5; //Temps entre chaque mise � jours du syst�me (par d�faut : 0.5 seconde);

SystemeTemplate::SystemeTemplate(){
}

void SystemeTemplate::initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name){
	//Appel de la fonction initialize du parent avDevice.
	avDevice::initialize(ID, Name, script_name); //Ligne obligatoire dans la fonction intialize.

	//TODO : Initialiser les �l�ments concern�.
	//Exemple d'initialisation
	arg_bouton_fuel = 12;
	arg_mfd_fuel = 13;
	arg_voyant_fuel = 11;

	make_default_activity(timer_activity);
}

void SystemeTemplate::SetCommand(int command, float value){
	/* Rappel :
	Un switch case �quivaut � un if () {} esle if () {}
	Ici switch command �quivaut � :
	if (command == 3001) {
	} else if (command == 3002) {
	} else if (command == 3003) {
	} else { //correspond � default
	}
	*/
	switch (command) {
	case 3001:
		//Do some actions
	default:
		//Si la commande n'est pas trouv�, le programme execute tout le code impl�ment� ici.
	}
}

void SystemeTemplate::update() {
	//TODO : Mettre � jours les �l�ments correspondants, utilisation de timer possible ici.
}

bool SystemeTemplate::checkCallbackKeyboard(int command, float& value) {
	//ATTENTION : Cette fonction attend un retour de type bool (boolean).
	//Obligation dans tous les cas d'avoir un retour (instruction return) vrai ou faux (true, false)
	switch (command) {
	/*case 3001:
		//Do some actions
		return ?;
		*/
	default:
		return true;
	}
}

bool SystemeTemplate::checkCallbackClickable(int command, float& value){
	//ATTENTION : Cette fonction attend un retour de type bool (boolean).
	//Obligation dans tous les cas d'avoir un retour (instruction return) vrai ou faux (true, false)
	switch (command) {
		/*case 3001:
		//Do some actions
		return ?;
		*/
	default:
		return true;
	}
}

bool SystemeTemplate::checkAnimationConditions(int arg_num, int command, float& value){
	//ATTENTION : Cette fonction attend un retour de type bool (boolean).
	//Obligation dans tous les cas d'avoir un retour (instruction return) vrai ou faux (true, false)
#if 1 //Mettre � z�ro pour d�sactiver.
	switch (command) {
		/*case 3001:
		//Do some actions
		return ?;
		*/
	default:
		return true;
	}
#endif
#if 0 //Mettre � un pour activer.
	switch (arg_num) {
		/*case 17:
		//Do some actions
		return ?;
		*/
	default:
		return true;
	}
#endif
}

void SystemeTemplate::performClickableAction(int command, float value, bool echo){
	switch (command) {
		/*case 3001:
		//Do some actions
		return ?;
		*/
	default:
		avDevice::performClickableAction(command, value, false);
	}
}

void SystemeTemplate::SetArgument(int argument, float value) {
	get_context()->panel->SetArgument(argument, value);
}

float SystemeTemplate::GetArgument(int argument) {
	return get_context()->panel->GetArgument(argument);
}