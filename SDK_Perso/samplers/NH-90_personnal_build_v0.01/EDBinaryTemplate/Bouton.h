#pragma once

namespace cockpit {
	class ccMainPanel;
	namespace element {
		class Bouton
		{
		public:
			Bouton();		//Constructeur par d�faut.
			~Bouton();		//Destructeur par d�faut.

			/* Bouton(int i_argument, cockpit::ccMainPanel* main_panel);
			* Instanciation d'un bouton, le minimum � savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un bouton sans valeur.
			* Verrou optionnel.
			*/
			Bouton(int i_argument, cockpit::ccMainPanel* main_panel);
			/* Bouton(int i_argument, int i_value, cockpit::ccMainPanel* main_panel);
			* Instanciation d'un bouton, le minimum � savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un bouton dont on conna�t l'entier de la valeur.
			* Verrou optionnel.
			*/
			Bouton(int i_argument, int i_value, cockpit::ccMainPanel* main_panel);
			/* Bouton(int i_argument, float f_value, cockpit::ccMainPanel* main_panel);
			* Instanciation d'un bouton, le minimum � savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un bouton dont on conna�t la valeur sous forme de nombre flotant.
			* Verrou optionnel.
			*/
			Bouton(int i_argument, float f_value, cockpit::ccMainPanel* main_panel);

			/* void setValue(int i_value);
			* Donne une valeur � 'value' � partir d'un integer.
			*/
			virtual void setValue(int i_value);
			/* void setValue(float f_value);
			* Donne une valeur � 'value' � partir d'un float.
			*/
			virtual void setValue(float f_value);
			/* float getValue();
			* Donne la valeur de 'value' en float.
			*/
			virtual float getValue() = 0;
			/* virtual void update(float f_value = 0);
			* Mais � jour la valeur du bouton.
			*/
			virtual void update(float f_value = 0) = 0;
			/* void afficheEtat();
			* Inscrit l'�tat de l'objet dans les fichiers log.
			* Fonction utilis� pendant les phases de d�bogage.
			*/
			void afficheEtat();

		protected:
			int i_value = 0;				//Valeur appliqu� � l'argument de -100 � 100. Vaaleur par d�faut 0.

			/* void animate();
			* Animation du bouton.
			*/
			virtual void animate();
			/* virtual void switch_bouton(float f_value);
			* Gestion de l'appuie sur le bouton par l'utilisateur.
			*/
			virtual void switch_bouton(float f_value) = 0;
			/* virtual int getArgument();
			* Donne l'argument du bouton.
			*/
			virtual int getArgument() { return i_argument; }

		private:
			int i_argument;					//Argument de l'objet dans le cockpit.
			cockpit::ccMainPanel* main_panel;		//Utilis� pour faire les diff�rentes annimations dans le cockpit.

			/* void init_bouton(int i_argument, cockpit::ccMainPanel* main_panel);
			* Initialisation des attribtus d'un bouton.
			*/
			void init_bouton(int i_argument, cockpit::ccMainPanel* main_panel);
		};
	}
}