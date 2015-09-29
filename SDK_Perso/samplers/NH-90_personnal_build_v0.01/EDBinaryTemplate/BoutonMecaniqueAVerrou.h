#pragma once
#include "BoutonMecanique.h"

namespace cockpit {
	namespace element {
		class BoutonMecaniqueAVerrou :
			public BoutonMecanique
		{
		public:
			BoutonMecaniqueAVerrou();		//Constructeur par d�faut.
			~BoutonMecaniqueAVerrou();		//Destructeur par d�faut.
			/* BoutonMecaniqueAVerrou(int i_argument, cockpit::ccMainPanel* main_panel, Verrou verrou = Verrou());
			* Instanciation d'un bouton m�canique, le minimum � savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un bouton m�canique sans valeur.
			* Verrou optionnel.
			*/
			BoutonMecaniqueAVerrou(int i_argument, cockpit::ccMainPanel* main_panel);
			/* BoutonMecaniqueAVerrou(int i_argument, int i_value, cockpit::ccMainPanel* main_panel);
			* Instanciation d'un bouton m�canique, le minimum � savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un bouton m�canique dont on conna�t l'entier de la valeur.
			*/
			BoutonMecaniqueAVerrou(int i_argument, int i_value, cockpit::ccMainPanel* main_panel);
			/* BoutonMecaniqueAVerrou(int i_argument, float f_value, cockpit::ccMainPanel* main_panel);
			* Instanciation d'un bouton m�canique, le minimum � savoir est l'argument sur lequel on interragit et le panel qui permet de faire les interractions.
			* Instanciation d'un bouton m�canique on conna�t la valeur en nombre flottant.
			*/
			BoutonMecaniqueAVerrou(int i_argument, float f_value, cockpit::ccMainPanel* main_panel);

			/* virtual void lock(bool bouton_peut_etre_verrouille = false) = 0;
			* Fonction qui permet de verrouill� le bouton si celui-ci est dans une position qui le permet (bool bouton_peut_etre_verrouille).
			*/
			virtual void lock(bool bouton_peut_etre_verrouille = false) = 0;
			/* virtual void unlock() = 0;
			* Fonction de d�verrouillage du bouton.
			*/
			virtual void unlock() = 0;
			/* virtual void lock_unlock(bool bouton_peut_etre_verrouille) = 0;
			* Fonction qui alterne entre verrouillage et d�verrouillage du bouton.
			**** bool bouton_peut_etre_verrouille, dis si le bouton est en position de verrouillage.
			**** Utilis� en passant le param�tre � la fonction lock(bool bouton_peut_etre_verrouille.
			*/
			virtual void lock_unlock(bool bouton_peut_etre_verrouille) = 0;
			/* virtual bool isLocked() = 0;
			* Dis si le bouton est verrouille.
			*/
			virtual bool isLocked() = 0;
			/* virtual void update(float f_value = 0);
			* Mets � jour les informations du bouton.
			*/
			virtual void update(float f_value = 0) override;

		protected:
			/* virtual void switch_bouton(float f_value);
			* Gestion de l'appuie sur le bouton par l'utilisateur.
			*/
			virtual void switch_bouton(float f_value) = 0;
		};
	}
}