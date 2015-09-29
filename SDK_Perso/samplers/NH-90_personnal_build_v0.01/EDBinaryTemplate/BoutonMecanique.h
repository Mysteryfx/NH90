#pragma once
#include "Bouton.h"


namespace cockpit {
	namespace element {
		class BoutonMecanique :
			public Bouton
		{
		public:
			BoutonMecanique();
			~BoutonMecanique();

			/* BoutonMecanique(int i_argument, cockpit::ccMainPanel* main_panel);
			* Instanciation d'un BoutonMecanique, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un BoutonMecanique sans valeur.
			* Verrou optionnel.
			*/
			BoutonMecanique(int i_argument, cockpit::ccMainPanel* main_panel);
			/* BoutonMecanique(int i_argument, int i_value, cockpit::ccMainPanel* main_panel);
			* Instanciation d'un BoutonMecanique, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un BoutonMecanique dont on connaît l'entier de la valeur.
			* Verrou optionnel.
			*/
			BoutonMecanique(int i_argument, int i_value, cockpit::ccMainPanel* main_panel);
			/* BoutonMecanique(int i_argument, float f_value, cockpit::ccMainPanel* main_panel);
			* Instanciation d'un BoutonMecanique, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un BoutonMecanique dont on connaît la valeur sous forme de nombre flotant.
			* Verrou optionnel.
			*/
			BoutonMecanique(int i_argument, float f_value, cockpit::ccMainPanel* main_panel);

			/* virtual void update(float f_value = 0);
			* Mets à jour les informations du bouton.
			*/
			virtual void update(float f_value = 0);
			/* virtual float getValue();
			* Donne la valeur de 'value'
			*/
			virtual float getValue();

		protected:
			/* virtual void switch_bouton(float f_value);
			* Gestion de l'appuie sur le bouton par l'utilisateur.
			*/
			virtual void switch_bouton(float f_value) = 0;
		};
	}
}