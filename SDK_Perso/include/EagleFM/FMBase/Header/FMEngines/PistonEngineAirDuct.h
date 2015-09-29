#pragma once

#include "Base.h"

#include "FMEngines/PistonEngineAirDuctDefs.h"
#include "FMSpace/AirFlow.h"

namespace EagleFM {

class PistonEngine;

class FMBASE_API AirDuctPart
{
	friend FMBASE_API AirDuctPart& operator>>(AirDuctPart& prev, AirDuctPart& next);
public:

	AirDuctPart();

	virtual ~AirDuctPart();

	virtual double calc_amf(AirFlowState flow_in_st) = 0;

	double get_pressure_out() const { return StateOut_.P_; }
	double get_temperature_out() const { return StateOut_.T_; }

protected:
	AirDuctPart* next_pipe_;

	AirFlowState StateOut_; /*air flow params in out of the element*/
};

FMBASE_API AirDuctPart& operator>>(AirDuctPart& prev, AirDuctPart& next);

//////////////////////////////////////////////////////////////////////////
//Intercoolers
class FMBASE_API InterCoolerDelta : public AirDuctPart
{
public:

	InterCoolerDelta(double& itercooler_delta)
		: itercooler_delta_(itercooler_delta)
	{

	}

	virtual double calc_amf(AirFlowState flow_in_st) override
	{
		StateOut_ = flow_in_st;
		StateOut_.T_ += itercooler_delta_;

		return next_pipe_ ->calc_amf(StateOut_);
	}

private:
	double& itercooler_delta_;
};

class FMBASE_API InterCoolerK : public AirDuctPart
{
public:

	InterCoolerK(double& itercooler_K)
		: itercooler_K_(itercooler_K)
	{

	}

	virtual double calc_amf(AirFlowState flow_in_st) override
	{
		StateOut_ = flow_in_st;
		StateOut_.T_ *= (1.0 - itercooler_K_);

		return next_pipe_ ->calc_amf(StateOut_);
	}

private:
	double& itercooler_K_;
};

//////////////////////////////////////////////////////////////////////////

class FMBASE_API SuperchargerDuct : public AirDuctPart
{
public:

	SuperchargerDuct(PistonEngine* engine, JetCompressor* superCharger, 
		double OmegaTerminal,
		double& compr_rate,
		double& comp_power);

	virtual double calc_amf(AirFlowState flow_in_st) override;

protected:

	PistonEngine* engine_;
	double& compr_rate_;
	double& compr_power_;

	double beta_;

	const double Omega_Terminal_;  //до этих оборотов параметры компрессора лежат на рабочей линии 

	const double stallAirFlowK_;	//коэф-ты ухудшения параметров при неустойчивой работе компрессора
	const double stallPiK_;
	const double stallNuK_;

	JetCompressor* superCharger_;

	void calcOutFlow(const AirFlow& flow_in,double pi, double nu, AirFlowState& flow_out) const; /* flow state after compressor*/

#ifdef DEBUG
public:
    double storedDebugBeta;
    double storedDebugNu;
#endif
};

class FMBASE_API ManifoldDuct : public AirDuctPart
{
public:

	ManifoldDuct(PistonEngine* engine);

	virtual double calc_amf(AirFlowState flow_in_st) override;

protected:

	const PistonEngine* engine_;
};

class FMBASE_API ThrottleDuct : public AirDuctPart
{
public:

	ThrottleDuct(JetNozzle* throttleNozzle);

	virtual double calc_amf(AirFlowState flow_in_st) override;

protected:

	double calcThrottleAMF(const AirFlowState& flow_in_st, double pressure_out) const;

	const JetNozzle*    throttleNozzle_;
	const PistonEngine* engine_;
};
}