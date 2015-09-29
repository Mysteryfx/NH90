#pragma once
#include "ElementDeBase_MFX.h"

namespace cockpit {
	namespace element {
		typedef enum type_de_fluide_mfx {
			huile_mfx,
			fuel_mfx,
		};
		class ElementFluide_MFX :
			public ElementDeBase_MFX
		{
		public:
			/* ElementFluide_MFX(type_de_fluide_mfx type_fluide, float reservoir, bool remplie, type_element_mfx type_element_fluide, etat_des_elements_de_base_mfx etat_element_fluide)
			* Pour créer un élément de type fluide, il faut :
			*			- type_de_fluide_mfx type_fluide, le type de fluide utilisé;
			*			- float reservoir, la capacité du réservoir;
			*			- bool remplie, si le réservoir est plein ou non (par défaut : vrai);
			*			- type_element_mfx type_element_fluide, le type de l'élément (par défaut : fluide_mfx);
			*			- etat_des_elements_de_base_mfx etat_element_fluide, l'état de l'élément (par défaut : etat_inactif_mfx).
			*/
			ElementFluide_MFX(MFXDevice *systeme_maitre, type_de_fluide_mfx type_fluide, float reservoir, bool remplie = true, type_element_mfx type_element_fluide = type_element_mfx::fluide_mfx, etat_des_elements_de_base_mfx etat_element_fluide = etat_des_elements_de_base_mfx::etat_inactif_mfx);
			ElementFluide_MFX();
			~ElementFluide_MFX();

			/* type_de_fluide_mfx getType()
			* Donne le type de fluide utilisé.
			*/
			type_de_fluide_mfx getType();
			/* float getQuantite()
			* Donne la quantité de fluide actuelle.
			*/
			float getQuantite();
			/* float getReservoir()
			* Donne la quantité de fluide maximal dans le reservoir.
			*/
			float getReservoir();

			/* bool isFull()
			* Dit si le reservoir est plein.
			*/
			bool isFull() { return (quantite == reservoir); }
			/* bool isEmpty()
			* Dit si le reservoir est vide.
			*/
			bool isEmpty() { return (quantite == 0.0f); }

			/* float ajusterQuantiteDe(float quantite_ajuster)
			* Ajuster la quantite d'une somme donne.
			*/
			virtual void ajusterQuantiteDe(float quantite_ajuster);
			/* float ajusterQuantiteA(float quantite_voulue)
			* Ajuste la quantite jusqu'a une quantite souhaité.
			*/
			virtual void ajusterQuantiteA(float quantite_voulue);

		protected:
			float quantite;						//Quantité de fluide actuelle.

			/* void setQuantite(float quantite)
			* Modifie la quantité actuelle de fluide.
			*/
			virtual void setQuantite(float quantite);

			/* virtual float aQuantiteDe(float quantite_ajuster) = 0;
			* Force la définition de l'ajustement de la quantité de par la classe fille.
			* Un réservoir d'huile moteur se vide différemment d'un réservoir de fuel.
			*/
			virtual void aQuantiteDe(float quantite_ajuster) = 0;
			/* virtual float aQuantiteA(float quantite_voulue) = 0;
			* Force la définition de l'ajustement de la quantité à par la classe fille.
			* Un réservoir d'huile moteur se vide différemment d'un réservoir de fuel.
			*/
			virtual void aQuantiteA(float quantite_voulue) = 0;

		private:
			type_de_fluide_mfx type;			//Type de fluide utilisé.
			float reservoir;					//Capacité maximal de fluide possible.
			float quantite_a_atteindre;			//Utilisé pour la modification de la valeur de la quantité.
		};
	}
}