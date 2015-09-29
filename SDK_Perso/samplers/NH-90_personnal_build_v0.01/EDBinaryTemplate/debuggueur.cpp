#include "stdafx.h"
#include "Debuggueur.h"
#include "WorldManager.h"
#include "Utilitaire.h"
#include <EagleFM/FMBase/Header/FMElectricSystems/Items/Battery.h>
#include <EagleFM/FMBase/Header/FMElectricSystems/Items/Lamp.h>

using namespace cockpit;

std::string Debuggueur::texte_log_string_ = "";
//{ Modifie le texte a afficher
void Debuggueur::t_set(std::string texte)
{
	Debuggueur::texte_log_string_ = texte;
}
void Debuggueur::t_add(std::string texte)
{
	Debuggueur::texte_log_string_ += texte;
}
void Debuggueur::t_add_boolean(bool boolean)
{
	if (boolean)
	{
		t_add("Vrai");
	}
	else
	{
		t_add("Faux");
	}
}
void Debuggueur::t_add_int(int integer_i)
{
	t_add(Utilitaire::convertIntToString(integer_i));
}

void Debuggueur::t_add_float(float floatant)
{
	t_add(Utilitaire::convertFloatToString(floatant));
}
void Debuggueur::t_add_double(double double_x)
{
	t_add(Utilitaire::convertDoubleToString(double_x));
}
void Debuggueur::t_log()
{
	ED_INFO(Debuggueur::texte_log_string_.c_str());
}

//{ Ecrit dans les fichiers de log
void Debuggueur::log(std::string texte_log_string)
{
	ED_INFO(texte_log_string.c_str());
}
void Debuggueur::log_boolean(bool boolean_log_b)
{
	if (boolean_log_b)
	{
		log("Vrai");
	} else
	{
		log("Faux");
	}
}
void Debuggueur::log_float(float floatant_log_x)
{
	log(Utilitaire::convertFloatToString(floatant_log_x));
}
void Debuggueur::log_double(double double_log_x)
{
	log(Utilitaire::convertDoubleToString(double_log_x));
}
//}

//{ Ecrits touss les paramètres d'une batterie dans les fichiers log
void Debuggueur::t_battery_is_on(bool boolean)
{
	if (boolean)
	{
		t_add("Activée");
	}
	else
	{
		t_add("Désactivée");
	}
}
void Debuggueur::log_param_batterie(EagleFM::Elec::Battery battery)
{
	t_set("Status : "); t_battery_is_on(battery.getIsOn()); t_log();
	t_set("Is Online : "); t_add_boolean(battery.getIsOnline()); t_log();
	t_set("Is powered : "); t_add_boolean(battery.isPowered()); t_log();
	t_set("Battery.getEMF : "); t_add_double(battery.getEMF()); t_log();
	t_set("Battery.getIsSourceLine : "); t_add_boolean(battery.getIsSourceLine()); t_log();
	t_set("Voltage : "); t_add_double(battery.getVoltage()); t_log();
	t_set("Nominal Voltage : "); t_add_double(battery.getNominalVoltage()); t_log();
	t_set("Type de batterie : "); t_add_int(battery.getType()); t_log();
	t_set("Puissance nominale : "); t_add_double(battery.getNominalCapacity()); t_log();
	t_set("Puisssance courante : "); t_add_double(battery.GetCurrentCapacity()); t_log();
	t_set("Battery.getCurrent : "); t_add_double(battery.getCurrent()); t_log();
	t_set("Internal Conductance : "); t_add_double(battery.getInternalConductance()); t_log();
	t_set("Internal Impedance : "); t_add_double(battery.getInternalImpedance()); t_log();
	t_set("Modeling Time : "); t_add_double(battery.getModelingTime()); t_log();
	t_set("Battery.get_tC : "); t_add_double(battery.get_tC()); t_log();
	t_set("Conductance : "); t_add_double(battery.getConductance()); t_log();
}
void Debuggueur::log_param_batterie_lampe(EagleFM::Elec::Battery battery, EagleFM::Elec::Lamp lampe)
{
	Debuggueur::log_param_batterie(battery);
	t_set("Lampe !"); t_log();
	t_set("Status : "); t_add_boolean(lampe.isPowered()); t_log();
}

void Debuggueur::log_param_batterie(EagleFM::Elec::Battery battery, std::string name_string)
{
	log(name_string);
	log_param_batterie(battery);
}

