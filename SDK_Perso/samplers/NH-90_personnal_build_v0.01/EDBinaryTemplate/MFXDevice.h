#pragma once
#include "Avionics\avDevice.h"
#include "ElementDeBase_MFX.h"

namespace cockpit {
	typedef enum type_de_systeme {
		systeme_electrique_mfx,
		systeme_hydraulique_mfx,
	};
	/** class MFXDevice : public avDevice
	* Classe personnalis� de 'Device' qui porte l'information d'un type de syst�me.
	*/
	class MFXDevice :
		public avDevice
	{
	public:
		/* MFXDevice(type_de_systeme type)
		* Cr�ation d'un device de type particulier.
		*/
		MFXDevice(type_de_systeme type);
		~MFXDevice();

		/* type_de_systeme getType()
		* Donne le type de syst�me.
		*/
		type_de_systeme getType() { return this->type; }

		/* void elementAChangerEtat(element::type_element_mfx type)
		* Un �l�ment informe le syst�me qu'il � changer d'�tat.
		* On retrouve les �l�ments � v�rifi� par le type de celui-ci.
		*/
		virtual void elementAChangerEtat(element::type_element_mfx type) = 0;

	private:
		type_de_systeme type; //Type de syst�me.
	};
}