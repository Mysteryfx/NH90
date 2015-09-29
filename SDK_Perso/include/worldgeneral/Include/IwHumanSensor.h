// IwHumanSensor.h: interface for the IwHumanSensor class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "WorldGeneral.h"

enum base_sensor
{
	base_sensor_RadarAltitude,
	base_sensor_BarometricAltitude,
	base_sensor_AngleOfAttack,
	base_sensor_AngleOfSlide,
	base_sensor_VerticalVelocity,
	base_sensor_TrueAirSpeed,
	base_sensor_IndicatedAirSpeed,
	base_sensor_MachNumber,
	base_sensor_VerticalAcceleration,
	base_sensor_HorizontalAcceleration,
	base_sensor_LateralAcceleration,
	base_sensor_RateOfRoll,
	base_sensor_RateOfYaw,
	base_sensor_RateOfPitch,
	base_sensor_Roll,
	base_sensor_MagneticHeading,
	base_sensor_Pitch,
	base_sensor_Heading,
	base_sensor_EngineLeftFuelConsumption,
	base_sensor_EngineRightFuelConsumption,
	base_sensor_EngineLeftTemperatureBeforeTurbine,
	base_sensor_EngineRightTemperatureBeforeTurbine,
	base_sensor_EngineLeftRPM,
	base_sensor_EngineRightRPM,
	base_sensor_WOW_RightMainLandingGear,
	base_sensor_WOW_LeftMainLandingGear,
	base_sensor_WOW_NoseLandingGear,
	base_sensor_RightMainLandingGearDown,
	base_sensor_LeftMainLandingGearDown,
	base_sensor_NoseLandingGearDown,
	base_sensor_RightMainLandingGearUp,
	base_sensor_LeftMainLandingGearUp,
	base_sensor_NoseLandingGearUp,
	base_sensor_LandingGearHandlePos,
	base_sensor_StickRollPosition,
	base_sensor_StickPitchPosition,
	base_sensor_RudderPosition,
	base_sensor_ThrottleLeftPosition,
	base_sensor_ThrottleRightPosition,
	base_sensor_HelicopterCollective,
	base_sensor_HelicopterCorrection,
	base_sensor_CanopyPos,
	base_sensor_CanopyState,
	base_sensor_FlapsRetracted,
	base_sensor_SpeedBrakePos,
	base_sensor_FlapsPos,
	base_sensor_TotalFuelWeight,
	base_sensor_count
};

class WORLDGENERAL_API IwHumanSensor  
{
public:
	virtual ~IwHumanSensor() = 0;
	virtual double getSensorValue(unsigned index) const = 0;
	//TEMP DEBUG!!!
	virtual double getFlowRotorParams(int key) const = 0;
};

