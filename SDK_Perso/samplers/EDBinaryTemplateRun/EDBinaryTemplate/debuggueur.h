#pragma once
#include <string>

namespace EagleFM{namespace Elec{
	class Lamp;
	class Battery;
}}

namespace cockpit {
	class Debuggueur
	{
	public:
		static void log(std::string texte_log_string);
		static void log_float(float floatant_log_x);
		static void log_double(double double_log_x);
		static void log_boolean(bool boolean_log_b);
		static void log_param_batterie(EagleFM::Elec::Battery battery, std::string name_string);

	private:
		static std::string texte_log_string_;
		
		static void t_log();
		static void t_set(std::string texte);
		static void t_add(std::string texte);
		static void t_add_int(int integer_i);
		static void t_add_float(float floatant);
		static void t_add_double(double double_x);
		static void t_add_boolean(bool boolean);
		
		static void log_param_batterie(EagleFM::Elec::Battery battery);
		static void log_param_batterie_lampe(EagleFM::Elec::Battery battery, EagleFM::Elec::Lamp lampe);
		static void t_battery_is_on(bool boolean);
	};
}