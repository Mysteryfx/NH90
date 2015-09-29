#ifndef __BasicAtmosphere_h__
#define __BasicAtmosphere_h__

#include "IBasicAtmosphere.h"

namespace EagleFM
{

//! Basic atmosphere model

//! Provides access to atmosphere parameters like sound speed, pressure,
//! temperature, density, wind etc.
//! Must be a singleton.
class BasicAtmosphere : public IBasicAtmosphere
{
public: // methods
    BasicAtmosphere();
    virtual ~BasicAtmosphere(){};

    virtual void    clear();

    //! \name Weather condition setting methods
    //! \{
    
    //! Temperature in point
	void	addPointAtmoTemperature(const Vec3 &Position_w, double Time, double AtmosphereTemperature);

    //! Pressure in point
	void	addPointAtmoPressure(const Vec3 &Position_w, double Time, double AtmospherePressure);

    //! Wind and turbulence in point (at altitude, indeed)
	void	addPointAtmoWind_w(const Vec3 &Position_w, double Time, const Vec3 &AtmosphereWind_w, double AtmosphereTurbulence);

    //! Presipitation in point (whole map, indeed)
    void	addPrecipitation(const Vec3 &Position_w, int Precipitation);
    //! \}
    
    
    //! \name Atmosphere parameters calculation methods
    //! \{
	
    //! Temperature [K] and geopotential altitude [m].
    virtual void    getAtmoTemperature(const Vec3 &Position_w, double *GeopotentialAltitude, double *AtmosphereTemperature) const;
    
    //! Temperature [K]? pressure [Pa] and geopotential altitude [m]. It's impossible to calсulate pressure whithout temperature calculation, 
    //! so there's no function getAtmoPressure
    virtual void    getAtmoTemperatureAndPressure(const Vec3 &Position_w, double *GeopotentialAltitude, double *AtmosphereTemperature, double *AtmospherePressure) const;

    //! Temperature [K], pressure [Pa], density [kg/m^3] and geopotential altitude [m]. It's impossible to calсulate pressure whithout temperature calculation
    //! and density without pressure, so there's no functions getAtmoPressure and  getAtmoDensity
    virtual void    getAtmoTemperatureAndPressureAndDensity(const Vec3 &Position_w, double *GeopotentialAltitude, double *AtmosphereTemperature, double *AtmospherePressure, double *AtmosphereDensity) const;

    //! Sound speed [m/s]
    virtual double  getAtmoSoundSpeed(double AtmosphereTemperature) const;

    //! Basic wind [m/s], without turbulence. Use it by default. Don't forget to set AltitudeAboveGround argument to correct value
    //! if you need really correct wind, especially near the ground.
    virtual Vec3    getBaseWind_w(const Vec3 &Position_w, const double AltitudeAboveGround = 500.0) const;

    //! Wind [m/s] with turbulence. Use it if and only if you're sure that you need turbulence. Don't forget to set AltitudeAboveGround argument to correct value
    //! if you need really correct wind, especially near the ground.
    virtual Vec3    getAtmoWind_w(const Vec3 &Position_w, const double AltitudeAboveGround = 500.0) const;
   
    //! Precipitation type
    virtual int     getPrecipitation(const Vec3 &Position_w) const;

    //! Relative humidity (0...1)
    virtual double  getRelativeHumidity(const Vec3 &Position_w) const;

    //! Dew point [K]
    virtual double  getDewPoint(const Vec3 &Position_w) const;

    //! \}

    //расчет адиабатики динамического торможения (увеличение температуры и давления при гашении скорости Velocity)
    virtual void    calcDynamicSlowdown(double Velocity, double &Temperature, double &Pressure) const;	
    virtual void    calcDynamicSlowdown(double Velocity, double &Temperature) const; //то же, только без давления

    virtual double  calcIcingRate(const Vec3 &Position_w, double Velocity) const; //расчет скорости обледенения/таяния на данной высоте и скорости (изменение слоя льда за единицу времени [мм/мин])
    virtual double  calcIcingRate(double TemperatureH, double AtmoDensity, double Velocity) const; //расчет скорости обледенения/таяния при известной температуре и плотности атмосферы

	//! Atmosphere simulation
    virtual void    simulate(double dTime); 

private: // methods

    //! Layered turbulence at high altitude
    double  getLayeredTurbulence(const Vec3 &Position_w)const;

    //! Layered turbulence at low altitude
    double  getLayeredTurbulence(const Vec3 &Position_w, const double AltitudeAboveGround)const;

    void	readTurbulenceData();
	Vec3	getTurbulenceInLayer(const Vec3 &Position_w, const double CellSize)const;
	double	getWindGroundFactor(double AbsoluteAltitude, const double &AltitudeAboveGround)const; //Возвращает коэффициент влияния земли на силу ветра. Диапазон 0..1

    // Temperature in [K]
    double getDewPoint(double temperature, double relativeHumidity) const;


private: // fields

	double	BasicGeopotentialAltitude[8],		//!< Basic altitudes
			AtmospherePressure[8],				//!< Atmosphere pressure at basic altitudes
			AtmosphereTemperature[8],			//!< Temperature at basic altitudes
			AtmosphereTemperatureGradient[7],	//!< Temperature gradient between basic altitudes
			Time;

	int		Precipitation;

    static Vec3	WindNoise[CellNum][CellNum];

	double	AtmosphereWind_w[3][4],				// Вектор скорости ветра
			GroundTurbulence,					// Приземная турбулентность
			LayerTurbulence,					// Межслойная турбулентность
			CloudTurbulence;					// Турбулентность в облачном слое

	Vec3	TurbulencePosition;					// Вектор смещения в турбулентности от времени
};

}

#endif
