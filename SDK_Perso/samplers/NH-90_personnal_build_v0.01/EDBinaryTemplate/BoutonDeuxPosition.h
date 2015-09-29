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
			* Instanciation d'un BoutonDeuxPosition, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un BoutonDeuxPosition sans valeur.
			* Verrou optionnel.
			*/
			BoutonDeuxPosition(int i_argument, cockpit::ccMainPanel* main_panel);
			/* BoutonDeuxPosition(int i_argument, int i_value, cockpit::ccMainPanel* main_panel);
			* Instanciation d'un BoutonDeuxPosition, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un BoutonDeuxPosition dont on connaît l'entier de la valeur.
			* Verrou optionnel.
			*/
			BoutonDeuxPosition(int i_argument, int i_value, cockpit::ccMainPanel* main_panel);
			/* BoutonDeuxPosition(int i_argument, float f_value, cockpit::ccMainPanel* main_panel);
			* Instanciation d'un BoutonDeuxPosition, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un BoutonDeuxPosition dont on connaît la valeur sous forme de nombre flotant.
			* Verrou optionnel.
			*/
			BoutonDeuxPosition(int i_argument, float f_value, cockpit::ccMainPanel* main_panel);

			/* void initiale_state(bouton_deux_position bdp_etat);
			* Permet de mettre à jour l'état initiale du bouton.
			* Si cette fonction n'est pas appelé l'état initiale du bouton sera off.
			* ATTENTION : Cette méthode ne fonctionnera qu'une seule et unique fois.
			*/
			void initiale_state(bouton_deux_position bdp_etat);
			/* void setValue(bouton_deux_position bdp_new);
			* Donne une valeur à 'value' à partir d'un état de bouton à deux position.
			*/
			virtual void setValue(bouton_deux_position bdp_new);
			/* bouton_deux_position getEtat();
			* Donne l'état du bouton.
			*/
			virtual bouton_deux_position getEtat();
			/* void afficheEtat();
			* Affiche toutes les informations du verrou.
			* Fonction utilisé lors des phases de débogage.
			*/
			void afficheEtat();

		protected:
			/* virtual void switch_bouton(float f_value);
			* Gestion de l'appuie sur le bouton par l'utilisateur.
			*/
			virtual void switch_bouton(float f_value);

		private:
			bouton_deux_position etat;	//Etat du bouton.
			bool is_initiale_done;		//Dit si l'état initiale a déjà été donné.

			/* void constructeur_initiale_state();
			* Cette fonction initialise l'état du bouton dans les différents constructeurs (bouton.etat = off).
			* Elle fait appel à initiale_state(bouton_deux_position bdp_etat).
			* Il faut prendre en compte le fait que cette fonction doît pouvoir être appelé de l'extérieur une fois.
			* Elle remet donc la valeur de is_initiale_done à faux à la fin de l'appel.
			*/
			void constructeur_initiale_state();
		};
	}
}