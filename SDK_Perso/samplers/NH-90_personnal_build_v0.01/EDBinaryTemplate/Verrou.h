#pragma once

namespace cockpit {
	class ccMainPanel;
	namespace element {
		typedef enum enum_verrou {
			verrouille		= 0,
			deverrouille	= 1
		};
		class Verrou
		{
		public:
			Verrou();
			~Verrou();

			/* Verrou(int i_argument, ccMainPanel* main_panel, float f_value = 0);
			* Instancie un Verrou.
			* Obligation de fournir l'argument du verrou ainsi que le panel qui servira pour les animations.
			*/
			Verrou(int i_argument, ccMainPanel* main_panel, float f_value = 0);
			/* Verrou(int i_argument, ccMainPanel* main_panel, int i_value = 0);
			* Instancie un Verrou.
			* Obligation de fournir l'argument du verrou ainsi que le panel qui servira pour les animations.
			*/
			Verrou(int i_argument, ccMainPanel* main_panel, int i_value = 0);

			/* bool isLocked();
			* Dit si le verrou est en place ou non.
			*/
			virtual bool isUnlock();
			/* void lock_unLock(bool bouton_peut_etre_verrouiller);
			* Verrouille/Déverrouille le bouton.
			**** bool bouton_peut_etre_verrouiller, utilisé si l'on veut verrouillé le bouton.
			**** Passé en paramètre à la fonction lock(bool bouton_peut_etre_verrouiller);
			*/
			virtual void lock_unLock(bool bouton_peut_etre_verrouiller);
			/* void unlock();
			* Déverrouille le bouton.
			*/
			virtual void unlock();
			/* void lock(bool bouton_peut_etre_verrouiller);
			* Vérouille le bouton. Si celui-ci est en position de verrouillage.
			*/
			virtual void lock(bool bouton_peut_etre_verrouiller);
			/* float getValue();
			* Donne la valeur de 'value' en nombre flottant.
			*/
			virtual float getValue();
			/* void setValue(int i_value);
			* Donne une valeur à 'value' en integer.
			*/
			virtual void setValue(int i_value);
			/* void setValue(float f_value);
			* Donne une valeur à 'value' à partir d'un nombre flottant.
			*/
			virtual void setValue(float f_value);
			/* void afficheEtat();
			* Affiche toutes les informations du verrou.
			*/
			virtual void afficheEtat();

		protected:
			int i_argument = -1;			//Argument du verrou dans le cockpit.
			int i_value;					//Valeur du verrou.

			ccMainPanel* main_panel;		//Utilisé pour faire les différentes annimations dans le cockpit.

			/* void animate();
			* Anime l'élément dans le cockpit.
			*/
			void animate();

		private:

			/* void init_verrou(int i_argument, ccMainPanel* main_panel);
			* Initialise les attributs obligatoires du verrou.
			*/
			void init_verrou(int i_argument, ccMainPanel* main_panel);
		};
	}
}