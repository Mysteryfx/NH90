#pragma once
#include "Avionics\avDevice.h"
#include "ElementDeBase_MFX.h"

namespace cockpit {
	typedef enum type_de_systeme {
		systeme_electrique_mfx,
		systeme_hydraulique_mfx,
	};
	/** class MFXDevice : public avDevice
	* Classe personnalisé de 'Device' qui porte l'information d'un type de système.
	*/
	class MFXDevice :
		public avDevice
	{
	public:
		/* MFXDevice(type_de_systeme type)
		* Création d'un device de type particulier.
		*/
		MFXDevice(type_de_systeme type);
		~MFXDevice();

		/* type_de_systeme getType()
		* Donne le type de système.
		*/
		type_de_systeme getType() { return this->type; }

		/* void elementAChangerEtat(element::type_element_mfx type)
		* Un élément informe le système qu'il à changer d'état.
		* On retrouve les éléments à vérifié par le type de celui-ci.
		*/
		virtual void elementAChangerEtat(element::type_element_mfx type) = 0;

	private:
		type_de_systeme type; //Type de système.
	};
}