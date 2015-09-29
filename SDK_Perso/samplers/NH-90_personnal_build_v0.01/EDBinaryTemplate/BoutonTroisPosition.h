#pragma once
#include "BoutonMecanique.h"

namespace cockpit {
	namespace element {
		typedef enum enum_switch_mode_bouton {
			data_switch_mode_bouton		= -100 ,
			normal_switch_mode_bouton	= 0,
			stand_by_switch_mode_bouton	= 100
		};
		typedef enum enum_test_voyant_bouton {
			tempo_test_voyant_bouton	= -100,
			off_test_voyant_bouton		= 0,
			hold_test_voyant_bouton		= 100
		};
		typedef enum type_bouton_trois_position {
			switch_mode_bouton,
			test_voyant_bouton
		};
		class BoutonTroisPosition :
			public BoutonMecanique
		{
		public:
			BoutonTroisPosition();
			~BoutonTroisPosition();
			/* BoutonTroisPosition(int i_argument, cockpit::ccMainPanel* main_panel);
			* Instanciation d'un bouton mécanique, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un bouton mécanique sans valeur.
			* Verrou optionnel.
			*/
			BoutonTroisPosition(int i_argument, type_bouton_trois_position type, cockpit::ccMainPanel* main_panel);
			/* BoutonTroisPosition(int i_argument, int i_value, cockpit::ccMainPanel* main_panel);
			* Instanciation d'un bouton mécanique, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un bouton mécanique sans valeur.
			* Verrou optionnel.
			*/
			BoutonTroisPosition(int i_argument, int i_value, type_bouton_trois_position type, cockpit::ccMainPanel* main_panel);

			/* int getPosition();
			* Donne sous forme d'entier la position du bouton.
			*/
			int getPosition() { return position; }
			/* void setPosition(int position);
			* Modifie la position du bouton.
			*/
			void setPosition(int position) { this->position = position; }
			/* void setValue(int i_value);
			* Donne une valeur à 'value' à partir d'un integer.
			* Modifie aussi la position du bouton.
			*/
			virtual void setValue(int i_value);
			/* void setValue(float f_value);
			* Donne une valeur à 'value' à partir d'un nombre flotant.
			* Modifie aussi la position du bouton.
			*/
			virtual void setValue(float f_value);

		protected:
			/* virtual void switch_bouton(float f_value);
			* Gestion de l'appuie sur le bouton par l'utilisateur.
			*/
			virtual void switch_bouton(float f_value);

			type_bouton_trois_position type;			//Type de bouton trois position.
			int position;								//Position actuelle du voyant en entier.

		private:
			/* virtual void switch_bouton(float f_value);
			* Gestion de l'appuie sur le bouton par l'utilisateur pour un bouton de type switch_mode_bouton.
			*/
			void switch_bouton_switch_mode();
			/* virtual void switch_bouton(float f_value);
			* Gestion de l'appuie sur le bouton par l'utilisateur pour un bouton de type test_voyant_bouton.
			*/
			void switch_bouton_test_voyant();
		};
	}
}