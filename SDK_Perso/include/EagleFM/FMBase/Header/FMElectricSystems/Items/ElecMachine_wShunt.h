#pragma once

#include "FMElectricSystems/Items/Source.h"

namespace EagleFM { namespace Elec {

class VoltVarChunk
{
public:
	VoltVarChunk() : parameter(0.0), voltage(0.0) {};
	VoltVarChunk(double parameterIn, double voltageIn) : parameter(parameterIn), voltage(voltageIn) {};

	double parameter, voltage;
};

typedef ed::vector<VoltVarChunk> VoltVarVec;

class FMBASE_API VoltVar
{
public:
	//bool l_read(Lua::Config& config, const char* field);

	VoltVarVec variation;
};

class FMBASE_API ElecMachine_wShunt : public Source
{
public:

	enum ElecMachine_polarity_enum
	{
		NEGATIVE_POL = -1,
		POSITIVE_POL = 1
	};

	ElecMachine_wShunt(double R_ArmIn, double R_StartIn, double R_ExcIn, double cKem,
					   double maxU, double kReg, double tau);

	virtual ~ElecMachine_wShunt() {}

	double simulate(double dt, double t_ambC, double omega, double sumMoment);

	double GetMoment() const;
	VoltVar& getMaxU_Varied() {return maxU_Varied;}

	void setR_StartOnOff(bool flag);
	void set_polarity(ElecMachine_polarity_enum polarity) { polarity_ = polarity; }

	// Cooling is optional.
	// Use it if you want low work time when it works as a starter, and high work time when it works as a generator.
	void setCoolerParameters(bool hasIt, double fanK);
	void updateTemperature(double dt, double t_ambC, double power, double omega);

#ifdef _ED_HIDDEN_DEBUG 
	float D_Iarm_;
#endif

private:
	void updateRLoads();
	double getRStartActual() const;

	const double m_tau;
	const double R_Arm;       // Arm resistance
	const double R_Start;     // Augmentation de la résistance (de départ)
	const double R_Exc;       // La résistance de l'enroulement de champ

	const double m_cKem;

	double m_moment;
	double m_kContr;

	double m_delta_U;

	double m_maxU;            // maximum voltage
	VoltVar maxU_Varied;      // maximum voltage can be varied with some parameter (usually temperature)

	const double m_kReg;
	bool R_StartOn;           // Start impendance is enabled

	char polarity_;
	bool hasCooler;
	double KtempFan;
};

} }
