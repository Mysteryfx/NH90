#pragma once
#include "Avionics/avDevice.h"

namespace cockpit {
	namespace NH_90 {
		class SystemeTemplate : public avDevice
		{
		public:
			SystemeTemplate();
			~SystemeTemplate(){}

			//Impl�mentation de l'interface avDevice
			/* Fonction d'initialisation.
			* Possibilit� d'initialis� tous les objets dans cette fonction.
			*/
			virtual void initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name);
			/* Permet de sp�cifi� un traitement sur une commande selon une valeur donn�e.
			*/
			virtual void SetCommand(int command, float value = 0);
			/* Fonction de mise � jours.
			* Permet de faire des timers ou des traitements selon un temps donn�e.
			*/
			virtual void update();

			/* Permet de faire un test sur une entr�e clavier.
			* Si la commande re�ue d'une entr�e clavier remplie les conditions voulues, renvoyer vrai "true" pour faire les actions souhait�.
			*/
			virtual bool checkCallbackKeyboard(int command, float& value);
			/* Permet de faire un test sur un clique de la souris.
			* Si la commande re�ue d'un clique de la souris remplie les conditions voulues, renvoyer vrai "true" pour faire les actions souhait�.
			*/
			virtual bool checkCallbackClickable(int command, float& value);
			/* Permet de faire un test sur les animations.
			* Si la commande, l'argument ou la valeur ne convient pas au condition souhait�, renvoyer faux "false" pour ne pas faire l'animation.
			*/
			virtual bool checkAnimationConditions(int arg_num, int command, float& value);
			/* Permet de faire une animation sans faire appel � la fonction SetCommand.
			*/
			void performClickableAction(int command, float value, bool echo = false /*set to true if you only want to move element w/o SetCommand call*/);

		private:
			//Possibilit� pour la partie graphique de n'utilis� que les arguments des �l�ments 3D pour faire une simulation graphique de l'utilisation des divers syst�mes.
			int arg_bouton_fuel, arg_voyant_fuel, arg_mfd_fuel;

			/* Modification de la valeur d'un argument.
			*/
			void SetArgument(int argument, float value);
			/* R�cup�ration de la valeur d'un argument.
			*/
			float GetArgument(int argument);
		};
	}
}