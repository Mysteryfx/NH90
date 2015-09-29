#include "stdafx.h"
#include "Utilitaire.h"
#include <sstream>

using namespace cockpit;
std::string Utilitaire::convertFloatToString(float floatant_convertir_x)
{
	std::ostringstream oss;
	oss << floatant_convertir_x;
	return oss.str();
}

std::string Utilitaire::convertDoubleToString(double double_convertir_x)
{
	std::ostringstream oss;
	oss << double_convertir_x;
	return oss.str();
}

std::string Utilitaire::convertIntToString(int integer_convertir_i)
{
	std::ostringstream oss;
	oss << integer_convertir_i;
	return oss.str();
}