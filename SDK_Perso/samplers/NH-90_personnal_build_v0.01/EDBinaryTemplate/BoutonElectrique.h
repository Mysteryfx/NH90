#pragma once
#include "Bouton.h"


namespace cockpit {
	namespace NH_90 { class SystemeElectrique; }
	namespace element {
		class BoutonElectrique :
			public Bouton
		{
		public:
			BoutonElectrique();
			~BoutonElectrique();

			/* BoutonElectrique(int i_argument, cockpit::NH_90::SystemeElectrique* systeme_maitre);
			* Instanciation d'un BoutonElectrique, le minimum � savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un BoutonElectrique sans valeur.
			* Verrou optionnel.
			*/
			BoutonElectrique(int i_argument, cockpit::NH_90::SystemeElectrique* systeme_maitre);
			/* BoutonElectrique(int i_argument, int i_value, cockpit::NH_90::SystemeElectrique* systeme_maitre);
			* Instanciation d'un BoutonElectrique, le minimum � savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un BoutonElectrique dont on conna�t l'entier de la valeur.
			* Verrou optionnel.
			*/
			BoutonElectrique(int i_argument, int i_value, cockpit::NH_90::SystemeElectrique* systeme_maitre);
			/* BoutonElectrique(int i_argument, float f_value, cockpit::NH_90::SystemeElectrique* systeme_maitre);
			* Instanciation d'un BoutonElectrique, le minimum � savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un BoutonElectrique on conna�t la valeur en nombre flottant.
			* Verrou optionnel.
			*/
			BoutonElectrique(int i_argument, float f_value, cockpit::NH_90::SystemeElectrique* systeme_maitre);

			/* virtual void update(float f_value = 0);
			* Mais � jour la valeur du bouton.
			*/
			virtual void update(float f_value = 0);
			/* float getValue();
			* Donne la valeur de 'value' en float.
			*/
			virtual float getValue() { return 0.0f; }

		protected:
			/* virtual void switch_bouton(float f_value);
			* Gestion de l'appuie sur le bouton par l'utilisateur.
			*/
			virtual void switch_bouton(float f_value) {}
			/* void animate();
			* Animation du bouton.
			*/
			virtual void animate();

			cockpit::NH_90::SystemeElectrique* systeme_maitre;		//Systeme �lectrique, il ne faut pas oubli� qu'un bouton de type �lectrique n'envoie d'information que s'il y a du courant.
		};
	}
}