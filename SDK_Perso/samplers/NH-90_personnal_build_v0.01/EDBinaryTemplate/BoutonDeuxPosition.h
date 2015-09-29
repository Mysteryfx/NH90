#pragma once
#include "BoutonMecanique.h"


namespace cockpit {
	namespace element {
		typedef enum bouton_deux_position{
			off = 0,
			on = 100
		};
		class BoutonDeuxPosition :
			public BoutonMecanique
		{
		public:
			BoutonDeuxPosition();
			~BoutonDeuxPosition();

			/* BoutonDeuxPosition(int i_argument, cockpit::ccMainPanel* main_panel);
			* Instanciation d'un BoutonDeuxPosition, le minimum � savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un BoutonDeuxPosition sans valeur.
			* Verrou optionnel.
			*/
			BoutonDeuxPosition(int i_argument, cockpit::ccMainPanel* main_panel);
			/* BoutonDeuxPosition(int i_argument, int i_value, cockpit::ccMainPanel* main_panel);
			* Instanciation d'un BoutonDeuxPosition, le minimum � savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un BoutonDeuxPosition dont on conna�t l'entier de la valeur.
			* Verrou optionnel.
			*/
			BoutonDeuxPosition(int i_argument, int i_value, cockpit::ccMainPanel* main_panel);
			/* BoutonDeuxPosition(int i_argument, float f_value, cockpit::ccMainPanel* main_panel);
			* Instanciation d'un BoutonDeuxPosition, le minimum � savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un BoutonDeuxPosition dont on conna�t la valeur sous forme de nombre flotant.
			* Verrou optionnel.
			*/
			BoutonDeuxPosition(int i_argument, float f_value, cockpit::ccMainPanel* main_panel);

			/* void initiale_state(bouton_deux_position bdp_etat);
			* Permet de mettre � jour l'�tat initiale du bouton.
			* Si cette fonction n'est pas appel� l'�tat initiale du bouton sera off.
			* ATTENTION : Cette m�thode ne fonctionnera qu'une seule et unique fois.
			*/
			void initiale_state(bouton_deux_position bdp_etat);
			/* void setValue(bouton_deux_position bdp_new);
			* Donne une valeur � 'value' � partir d'un �tat de bouton � deux position.
			*/
			virtual void setValue(bouton_deux_position bdp_new);
			/* bouton_deux_position getEtat();
			* Donne l'�tat du bouton.
			*/
			virtual bouton_deux_position getEtat();
			/* void afficheEtat();
			* Affiche toutes les informations du verrou.
			* Fonction utilis� lors des phases de d�bogage.
			*/
			void afficheEtat();

		protected:
			/* virtual void switch_bouton(float f_value);
			* Gestion de l'appuie sur le bouton par l'utilisateur.
			*/
			virtual void switch_bouton(float f_value);

		private:
			bouton_deux_position etat;	//Etat du bouton.
			bool is_initiale_done;		//Dit si l'�tat initiale a d�j� �t� donn�.

			/* void constructeur_initiale_state();
			* Cette fonction initialise l'�tat du bouton dans les diff�rents constructeurs (bouton.etat = off).
			* Elle fait appel � initiale_state(bouton_deux_position bdp_etat).
			* Il faut prendre en compte le fait que cette fonction do�t pouvoir �tre appel� de l'ext�rieur une fois.
			* Elle remet donc la valeur de is_initiale_done � faux � la fin de l'appel.
			*/
			void constructeur_initiale_state();
		};
	}
}