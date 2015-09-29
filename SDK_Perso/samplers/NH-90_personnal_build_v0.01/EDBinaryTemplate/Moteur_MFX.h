#pragma once
#include "ElementMecanique_MFX.h"

namespace cockpit {
	namespace moteur {
		class Moteur_MFX :
			public element::ElementMecanique_MFX
		{
		public:
			Moteur_MFX(MFXDevice *systeme_maitre) : ElementMecanique_MFX(systeme_maitre, element::type_element_mfx::moteur_mfx, element::etat_des_elements_de_base_mfx::etat_actif_mfx) { puissance = 100.0f; }
			~Moteur_MFX();

			/* float getPuissance()
			* Donne la puissance.
			*/
			float getPuissance() { return this->puissance; }

			void setPuissance(float puissance) { this->puissance = puissance; }

			/* void recuperation()
			* Procédure de récupération des éléments de base.
			*/
			virtual void recuperation() {}

		protected:
			float puissance;			//Puissance du moteur.
		};
	}
}