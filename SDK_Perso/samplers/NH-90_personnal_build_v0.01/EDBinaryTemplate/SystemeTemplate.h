#pragma once
#include "Avionics/avDevice.h"

namespace cockpit {
	namespace NH_90 {
		class SystemeTemplate : public avDevice
		{
		public:
			SystemeTemplate();
			~SystemeTemplate(){}

			//Implémentation de l'interface avDevice
			/* Fonction d'initialisation.
			* Possibilité d'initialisé tous les objets dans cette fonction.
			*/
			virtual void initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name);
			/* Permet de spécifié un traitement sur une commande selon une valeur donnée.
			*/
			virtual void SetCommand(int command, float value = 0);
			/* Fonction de mise à jours.
			* Permet de faire des timers ou des traitements selon un temps donnée.
			*/
			virtual void update();

			/* Permet de faire un test sur une entrée clavier.
			* Si la commande reçue d'une entrée clavier remplie les conditions voulues, renvoyer vrai "true" pour faire les actions souhaité.
			*/
			virtual bool checkCallbackKeyboard(int command, float& value);
			/* Permet de faire un test sur un clique de la souris.
			* Si la commande reçue d'un clique de la souris remplie les conditions voulues, renvoyer vrai "true" pour faire les actions souhaité.
			*/
			virtual bool checkCallbackClickable(int command, float& value);
			/* Permet de faire un test sur les animations.
			* Si la commande, l'argument ou la valeur ne convient pas au condition souhaité, renvoyer faux "false" pour ne pas faire l'animation.
			*/
			virtual bool checkAnimationConditions(int arg_num, int command, float& value);
			/* Permet de faire une animation sans faire appel à la fonction SetCommand.
			*/
			void performClickableAction(int command, float value, bool echo = false /*set to true if you only want to move element w/o SetCommand call*/);

		private:
			//Possibilité pour la partie graphique de n'utilisé que les arguments des éléments 3D pour faire une simulation graphique de l'utilisation des divers systèmes.
			int arg_bouton_fuel, arg_voyant_fuel, arg_mfd_fuel;

			/* Modification de la valeur d'un argument.
			*/
			void SetArgument(int argument, float value);
			/* Récupération de la valeur d'un argument.
			*/
			float GetArgument(int argument);
		};
	}
}