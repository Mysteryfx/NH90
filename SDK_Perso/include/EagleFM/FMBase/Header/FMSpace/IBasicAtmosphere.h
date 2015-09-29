//! \file
//! Basic atmosphere interface.

//! This interface is parent for EagleFM.dll internal basic
//! atmosphere implementation and for Weather.dll atmosphere interface.
//! All functions must be virual to avoid unnecessary dependeces.
//! Such inheritance structure was developed to ensure
//! both EagleFM.dll independence from other libraries
//! and other libraries independence from EagleFM (except header files).

#ifndef __IBasicAtmosphere_h__
#define __IBasicAtmosphere_h__

#include "Base.h"
#include "FMMathematics/math.h"

namespace EagleFM
{

#define CellNum		200			//количество ячеек в стороне поля

//! Interface to atmosphere model

//! Provides access to atmosphere parameters like sound speed, pressure,
//! temperature, density, wind etc.
//! Implementation must be a singleton.
//!
//! This interface is parent for EagleFM.dll internal basic
//! atmosphere implementation and for Weather.dll atmosphere interface.
//! All functions must be virual to avoid unnecessary dependeces.
//! Such inheritance structure was developed to ensure
//! both EagleFM.dll independence from other libraries
//! and other libraries independence from EagleFM (except header files).
//! If you need to know something about the atmosphere and you are not an FM part,
//! ask the class from Weather, do not use this one to avoid unnecessary dependencies.
class IBasicAtmosphere
{
public:
    virtual ~IBasicAtmosphere(){};

    virtual void    clear() = 0;

    //! \name Atmosphere parameters calculation methods
    //! \{
	//! Temperature [K] and geopotential altitude [m].
    virtual void    getAtmoTemperature(const Vec3 &Position_w, double *GeopotentialAltitude, double *AtmosphereTemperature) const = 0;

    //! Temperature [K] and pressure [Pa] and geopotential altitude [m]. It's impossible to calсulate pressure whithout temperature calculation, 
    //! so there's no function getAtmoPressure
    virtual void    getAtmoTemperatureAndPressure(const Vec3 &Position_w, double *GeopotentialAltitude, double *AtmosphereTemperature, double *AtmospherePressure) const = 0;

    //! Temperature [K], pressure [Pa] and density [kg/m^3] and geopotential altitude [m]. It's impossible to calсulate pressure whithout temperature calculation
    //! and density without pressure, so there's no functions getAtmoPressure and  getAtmoDensity
    virtual void    getAtmoTemperatureAndPressureAndDensity(const Vec3 &Position_w, double *GeopotentialAltitude, double *AtmosphereTemperature, double *AtmospherePressure, double *AtmosphereDensity) const = 0;
	
    //! Sound speed [m/s]
    virtual double  getAtmoSoundSpeed(double AtmosphereTemperature) const = 0;

    //! Basic wind [m/s], without turbulence. Use it by default. Don't forget to set AltitudeAboveGround argument to correct value
    //! if you need really correct wind, especially near the ground.
    virtual Vec3    getBaseWind_w(const Vec3 &Position_w, const double AltitudeAboveGround = 500.0) const = 0;

    //! Wind [m/s] with turbulence. Use it if and only if you're sure that you need turbulence. Don't forget to set AltitudeAboveGround argument to correct value
    //! if you need really correct wind, especially near the ground.
    virtual Vec3    getAtmoWind_w(const Vec3 &Position_w, const double AltitudeAboveGround = 500.0) const = 0;
    
    //! Precipitation type
    virtual int     getPrecipitation(const Vec3 &Position_w) const = 0;

    //! Relative humidity (0...1)
    virtual double  getRelativeHumidity(const Vec3 &Position_w) const = 0;

    //! Dew point [K]
    virtual double  getDewPoint(const Vec3 &Position_w) const = 0;

    //! \}

	//расчет адиабатики динамического торможения (увеличение температуры и давления при гашении скорости Velocity)
    virtual void    calcDynamicSlowdown(double Velocity, double &Temperature, double &Pressure) const = 0;	
    virtual void    calcDynamicSlowdown(double Velocity, double &Temperature) const = 0; //то же, только без давления

    virtual double  calcIcingRate(const Vec3 &Position_w, double Velocity) const = 0; //расчет скорости обледенения/таяния на данной высоте и скорости (изменение слоя льда за единицу времени [мм/мин])
    virtual double  calcIcingRate(double TemperatureH, double AtmoDensity, double Velocity) const = 0; //расчет скорости обледенения/таяния при известной температуре и плотности атмосферы

	//! Atmosphere simulation
    virtual void    simulate(double dTime) = 0; 
};

}

#endif
