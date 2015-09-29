#pragma once
#include "ElementDeBase_MFX.h"

namespace cockpit {
	namespace element {
		class ElementMecanique_MFX :
			public ElementDeBase_MFX
		{
		public:
			/* ElementMecanique_MFX(type_element_mfx type_element_mecanique, etat_des_elements_de_base_mfx etat_element_mecanique = etat_des_elements_de_base_mfx::etat_inactif_mfx)
			* Pour cr�er un ElementMecanique_MFX, il faut :
			*				- type_element_mfx type_element_mecanique, le type d'�l�ment m�canique (par d�faut : mecanique_mfx),
			*				- etat_des_elements_de_base_mfx etat_element_mecanique, l'�tat de l'�l�ment m�canique (par d�faut : etat_actif_mfx).
			*/
			ElementMecanique_MFX(MFXDevice *systeme_maitre, type_element_mfx type_element_mecanique = type_element_mfx::mecanique_mfx, etat_des_elements_de_base_mfx etat_element_mecanique = etat_des_elements_de_base_mfx::etat_actif_mfx);
			~ElementMecanique_MFX();

		};
	}
}