#pragma once
#include <string>

namespace cockpit{
	class Utilitaire
	{
	public:
		static std::string convertFloatToString(float floatant_convertir_x);
		static std::string convertDoubleToString(double double_convertir_x);
		static std::string convertIntToString(int integer_convertir_i);
	};
}