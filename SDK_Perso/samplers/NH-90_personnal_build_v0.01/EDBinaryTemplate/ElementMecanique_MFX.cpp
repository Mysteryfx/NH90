#include "stdafx.h"
#include "ElementMecanique_MFX.h"

using namespace cockpit;
using namespace cockpit::element;

ElementMecanique_MFX::ElementMecanique_MFX(MFXDevice *systeme_maitre, type_element_mfx type_element_mecanique, etat_des_elements_de_base_mfx etat_element_mecanique) :
ElementDeBase_MFX(systeme_maitre, type_element_mecanique, etat_element_mecanique)
{}
ElementMecanique_MFX::~ElementMecanique_MFX()
{
	panne();
}
