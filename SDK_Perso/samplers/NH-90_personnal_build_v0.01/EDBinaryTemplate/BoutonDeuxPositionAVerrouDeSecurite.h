#pragma once
#include "BoutonDeuxPosition.h"
#include "BoutonMecaniqueAVerrou.h"
#include "VerrouDeSecurite.h"

namespace cockpit {
	namespace element {
		class BoutonDeuxPositionAVerrouDeSecurite :
			public BoutonMecaniqueAVerrou, public BoutonDeuxPosition
		{
		public:
			BoutonDeuxPositionAVerrouDeSecurite();		//Constructeur par défaut.
			~BoutonDeuxPositionAVerrouDeSecurite();		//Destructeur par défaut.

			/* BoutonDeuxPositionAVerrouDeSecurite(int i_argument, cockpit::ccMainPanel* main_panel, VerrouDeSecurite verrou);
			* Instanciation d'un bouton à deux position, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un bouton à deux position sans valeur.
			* Verrou optionnel.
			*/
			BoutonDeuxPositionAVerrouDeSecurite(int i_argument, cockpit::ccMainPanel* main_panel, VerrouDeSecurite verrou);
			/* BoutonDeuxPositionAVerrouDeSecurite(int i_argument, int i_value, cockpit::ccMainPanel* main_panel, int i_argument_verrou, float f_value_verrou = 0);
			* Instanciation d'un bouton à deux position, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un bouton à deux position dont on connaît l'entier de la valeur.
			*/
			BoutonDeuxPositionAVerrouDeSecurite(int i_argument, int i_value, cockpit::ccMainPanel* main_panel, int i_argument_verrou, float f_value_verrou = 0.0f);
			/* BoutonDeuxPositionAVerrouDeSecurite(int i_argument, float f_value, cockpit::ccMainPanel* main_panel, int i_verrou_argument, float f_value_verrou = 0);
			* Instanciation d'un bouton à deux position, le minimum à savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un bouton à deux position on connaît la valeur en nombre flottant.
			*/
			BoutonDeuxPositionAVerrouDeSecurite(int i_argument, float f_value, cockpit::ccMainPanel* main_panel, int i_argument_verrou, float f_value_verrou);

			/* virtual void lock(bool bouton_peut_etre_verrouille) = 0;
			* Fonction qui permet de verrouillé le bouton si celui-ci est dans une position qui le permet (bool bouton_peut_etre_verrouille).
			*/
			virtual void lock(bool bouton_peut_etre_verrouille = true) override;
			/* virtual void unlock() = 0;
			* Fonction de déverrouillage du bouton.
			*/
			virtual void unlock() override;
			/* virtual void lock_unlock(bool bouton_peut_etre_verrouille = false) = 0;
			* Fonction qui alterne entre verrouillage et déverrouillage du bouton.
			**** bool bouton_peut_etre_verrouille, dis si le bouton est en position de verrouillage.
			**** Utilisé en passant le paramètre à la fonction lock(bool bouton_peut_etre_verrouille.
			*/
			virtual void lock_unlock(bool bouton_peut_etre_verrouille = true) override;
			/* virtual bool isLocked() = 0;
			* Dis si le bouton est verrouille.
			*/
			virtual bool isLocked() override;
			/* virtual void update(float value);
			* Gère les mises à jours produites par l'utilisateur.
			*/
			virtual void update(float value) { BoutonMecaniqueAVerrou::update(value); } //Définition de l'utilisation de la fonction update du BoutonMéecaniqueAVerrou.
			/* virtual float getValue();
			* Donne la valeur de 'value'
			*/
			virtual float getValue() { return BoutonDeuxPosition::getValue(); }
			/* void setValue(int i_value);
			* Donne une valeur à 'value' à partir d'un integer.
			*/
			virtual void setValue(int i_value);
			/* void setValue(float f_value);
			* Donne une valeur à 'value' à partir d'un float.
			*/
			virtual void setValue(float f_value);
			/* void setValue(bouton_deux_position bdp_new);
			* Donne une valeur à 'value' à partir d'un état de bouton à deux position.
			*/
			void setValue(bouton_deux_position bdp_new) { BoutonDeuxPosition::setValue(bdp_new); BoutonMecaniqueAVerrou::setValue(static_cast<int>(bdp_new)); }
			/* void afficheEtat();
			* Inscrit l'état de l'objet dans les fichiers log.
			* Fonction utilisé pendant les phases de débogage.
			*/
			void afficheEtat() { Debuggueur::log("BoutonDeuxPositionAVerrouDeSecurite::afficheEtat => 82");  BoutonDeuxPosition::afficheEtat(); }

		protected:
			/* virtual void switch_bouton(float f_value);
			* Gestion de l'appuie sur le bouton par l'utilisateur.
			*/
			virtual void switch_bouton(float f_value) { BoutonDeuxPosition::switch_bouton(f_value); animate(); }
			/* void animate();
			* Animation du bouton.
			*/
			virtual void animate() { BoutonDeuxPosition::animate(); }
			/* virtual int getArgument();
			* Donne l'argument du bouton.
			*/
			virtual int getArgument() { return BoutonDeuxPosition::getArgument(); }

		private:
			VerrouDeSecurite verrou;			//Verrou du bouton.

		};
	}
}