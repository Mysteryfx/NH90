#pragma once
#include "Verrou.h"


namespace cockpit {
	namespace element {
		class VerrouDeSecurite :
			public Verrou
		{
		public:
			VerrouDeSecurite();
			~VerrouDeSecurite();

			/* VerrouDeSecurite(int i_argument, cockpit::ccMainPanel* main_panel, float f_value = 0);
			* Instancie un VerrouDeSecurite.
			* Obligation de fournir l'argument du verrou ainsi que le panel qui servira pour les animations.
			*/
			VerrouDeSecurite(int i_argument, cockpit::ccMainPanel* main_panel, float f_value = 0);
			/* VerrouDeSecurite(int i_argument, cockpit::ccMainPanel* main_panel, int i_value = 0);
			* Instancie un Verrou.
			* Obligation de fournir l'argument du verrou ainsi que le panel qui servira pour les animations.
			*/
			VerrouDeSecurite(int i_argument, cockpit::ccMainPanel* main_panel, int i_value);
			/* VerrouDeSecurite(int i_argument, cockpit::ccMainPanel* main_panel, float position_verrouillage_bouton, float f_value = 0);
			* Instancie un VerrouDeSecurite.
			* Obligation de fournir l'argument du verrou ainsi que le panel qui servira pour les animations.
			*/
			VerrouDeSecurite(int i_argument, cockpit::ccMainPanel* main_panel, int i_argument_bouton, float position_verrouillage_bouton, float f_value);
			/* VerrouDeSecurite(int i_argument, cockpit::ccMainPanel* main_panel, int i_argument_bouton, float position_verrouillage_bouton, int i_value);
			* Instancie un Verrou.
			* Obligation de fournir l'argument du verrou ainsi que le panel qui servira pour les animations.
			*/
			VerrouDeSecurite(int i_argument, cockpit::ccMainPanel* main_panel, int i_argument_bouton, float position_verrouillage_bouton, int i_value);

			/* void lock_unLock(bool bouton_peut_etre_verrouiller);
			* Verrouille/Déverrouille le bouton.
			**** bool bouton_peut_etre_verrouiller, utilisé si l'on veut verrouillé le bouton.
			**** Passé en paramètre à la fonction lock(bool bouton_peut_etre_verrouiller);
			*/
			virtual void lock_unLock(bool bouton_peut_etre_verrouiller) override;
			/* void lock(bool bouton_peut_etre_verrouiller);
			* Vérouille le bouton. Si celui-ci est en position de verrouillage.
			*/
			virtual void lock(bool bouton_peut_etre_verrouiller) override;
			/* void initBouton(int i_argument_bouton, float position_verrouillage_bouton);
			* Initialise une fois les informations du bouton.
			*/
			void initBouton(int i_argument_bouton, float position_verrouillage_bouton);

		protected:
			/* virtual void verrouillage_du_bouton();
			* Verrouille le bouton.
			*/
			virtual void verrouillage_du_bouton();

		private:
			int i_argument_bouton;					//Argument du bouton lié au verrou de sécurité.
			float position_verrouillage_bouton;		//Position de verrouillage du bouton.
			bool is_bouton_ok;						//Dis si les informations du bouton sont correctement renseigné.

			/* void setBouton(int i_argument_bouton, float position_verrouillage_bouton);
			* Enregistre les informations du boutons.
			*/
			void setBouton(int i_argument_bouton, float position_verrouillage_bouton);
		};
	}
}