#include "stdafx.h"
#include "MFXDevice.h"

using namespace cockpit;
/* MFXDevice(type_de_systeme type)
* Cr�ation d'un device de type particulier.
*/
MFXDevice::MFXDevice(type_de_systeme type){
	this->type = type;
}
MFXDevice::~MFXDevice()
{
}