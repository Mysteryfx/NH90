#pragma once

namespace cockpit {
	namespace NH_90 {
		class SystemeElectrique;
		enum etat_des_elements_electrique;
	}
	namespace element{
		typedef enum voyant_defaut{
			test_voyant = -100,
			courant_off = 0
		};
		class Voyant
		{
		public:
			Voyant();		//Constructeur par d�faut.
			~Voyant();		//Destructeur par d�faut.
			/* Voyant(int i_argument, cockpit::ccMainPanel* main_panel);
			* Instanciation d'un voyant avec son argument et le panel.
			*/
			Voyant(int i_argument, cockpit::NH_90::SystemeElectrique* systeme_electrique);

			/* void setValue(int i_value);
			* Donne une valeur � 'i_value' en integer.
			*/
			virtual void setValue(int i_value);
			/* void setValue(float i_value);
			* Donne une valeur � 'i_value' � partir d'un nombre flotant.
			*/
			virtual void setValue(float f_value) { setValue((int)(f_value * 100)); }
			/* float getValue();
			* Donne la valeur de 'i_value'
			*/
			virtual float getValue();
			/* bool isVoyant();
			* Dit si oui ou non, c'est un voyant.
			*/
			virtual bool isVoyant();
			/* void update(int etat);
			* Mets � jour les informations du voyant.
			*/
			virtual void update(int etat);
			/* virtual void update();
			* Il ne faut pas oublier que s'il n'y a pas de courant dans l'appareil, il n'y a pas de voyant.
			*/
			virtual void update();
			/* virtual void test_voyant();
			* Affiche l'�tat teste du voyant.
			*/
			virtual void test_voyant() { setValue(static_cast<int>(voyant_defaut::test_voyant)); animate(); }
			/* virtual void test_voyant(int i_value);
			* Affiche le voyant � un �tat donn� sous forme d'un entier.
			*/
			virtual void test_voyant(int i_value) { setValue(i_value); animate(); }

		protected:
			int i_argument;												//Argument du voyant.
			int i_value;												//Valeur du voyant.
			
			cockpit::NH_90::SystemeElectrique* systeme_electrique;		//Syst�me �lectrique. Utilis� pour savoir si le voyant est aliment� et pour faire les animations.

			/* void animate();
			* Anime l'�l�ment dans le cockpit.
			*/
			void animate();
			/* void update(int etat);
			* Mets � jour les informations du voyant.
			*/
			virtual void mise_a_jour_voyant(int etat) = 0;
		};
	}
}