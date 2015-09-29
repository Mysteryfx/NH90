//! \ file
//! Standard atmosphere implementation

//! This is old Lockon and DCS atmosphere similar to ISA,
//! with constant parameters and wind at 0, 2000 and 8000 meters.

#ifndef StandardAtmosphere_h__
#define StandardAtmosphere_h__

#include "Atmosphere/IAtmosphere.h"

typedef double wModelTime;

namespace Weather
{
//! Standard atmosphere model

//! Provides access to atmosphere parameters like sound speed, pressure,
//! temperature, density, wind etc.
//! Must be a singleton during simulation.
//!
//! This is an old Lockon and DCS atmosphere similar to ISA,
//! with constant parameters and wind at 0, 2000 and 8000 meters.
class WEATHER_API StandardAtmosphere : public IAtmosphere
{
public:
	StandardAtmosphere();
	virtual ~StandardAtmosphere(){};

    virtual ATMO_TYPES getAtmoType() const;

	virtual void	clear();

    //! \name Weather condition setting methods
    //! \{

    //! Temperature in point
	void	addPointAtmoTemperature(const Math::Vec3d &Position_w, double Time, double AtmosphereTemperature);

    //! Pressure in point
	void	addPointAtmoPressure(const Math::Vec3d &Position_w, double Time, double AtmospherePressure);

    //! Wind and turbulence in point (at altitude, indeed)
	void	addPointAtmoWind_w(const Math::Vec3d &Position_w, double Time, const Math::Vec3d &AtmosphereWind_w, double AtmosphereTurbulence);

    //! Set ground wind to make wind at 10-12 m and wind "at ground level" in mission editor equal.
    void	addPointGroundWind_w(const Math::Vec3d &Position_w, double Time, const Math::Vec3d &AtmosphereWind_w, double AtmosphereTurbulence);

    //! Clouds density [0:10]
    void    setCloudsDensity(const int density);

    //! Presipitation in point (whole map, indeed)
	void	addPrecipitation(const Math::Vec3d &Position_w, int Precipitation);

    //! Ceiling (нижняя кромка облаков), [m]
    void    addCeiling(double Ceiling);

    //! Clouds upper edge, [m]
    void    addCloudsUpperEdge(double Edge);

    //! Overcast or cumulus
    void    setOvercast(bool Overcast);

    //! \}

	//! \name Atmosphere parameters calculation methods
    //! \{

    //! Temperature [K] and geopotential altitude [m].
	virtual void	getAtmoTemperature(const Math::Vec3d &Position_w, double *GeopotentialAltitude, double *AtmosphereTemperature) const;

    //! Temperature [K] and pressure [Pa] and geopotential altitude [m]. It's impossible to calсulate pressure whithout temperature calculation, 
    //! so there's no function getAtmoPressure
	virtual void	getAtmoTemperatureAndPressure(const Math::Vec3d &Position_w, double *GeopotentialAltitude, double *AtmosphereTemperature, double *AtmospherePressure) const;

    //! Temperature [K], pressure [Pa] and density [kg/m^3] and geopotential altitude [m]. It's impossible to calсulate pressure whithout temperature calculation
    //! and density without pressure, so there's no functions getAtmoPressure and  getAtmoDensity
	virtual void	getAtmoTemperatureAndPressureAndDensity(const Math::Vec3d &Position_w, double *GeopotentialAltitude, double *AtmosphereTemperature, double *AtmospherePressure, double *AtmosphereDensity) const;
	
    //! Sound speed [m/s]
	virtual double	getAtmoSoundSpeed(double AtmosphereTemperature) const;

    //! Basic wind [m/s], without turbulence. Use it by default. Don't forget to set AltitudeAboveGround argument to correct value
    //! if you need really correct wind, especially near the ground.
	virtual Math::Vec3d	getBaseWind_w(const Math::Vec3d &Position_w, const double AltitudeAboveGround = 500.0) const;

    //! Wind [m/s] with turbulence. Use it if and only if you're sure that you need turbulence. Don't forget to set AltitudeAboveGround argument to correct value
    //! if you need really correct wind, especially near the ground.
	virtual Math::Vec3d	getAtmoWind_w(const Math::Vec3d &Position_w, const double AltitudeAboveGround = 500.0) const;

    //! Get clouds type: overcast or cumulus
    bool isOvercast();

    //! Get clouds density in point ( 0...10 ) (Бальность облачности)
    virtual double getCloudsDensity(const Math::Vec3d &Position_w)const;

    //! Precipitation type
	virtual int getPrecipitation(const Math::Vec3d &Position_w) const;

    //! Ceiling (нижняя кромка облаков) [m]. If there is no clouds near Position_w, the ceiling will be 100000 meters.
    virtual double getCeiling(const Math::Vec3d &Position_w);

    //! Upper edge of clouds
    virtual double getCloudsUpperEdge(const Math::Vec3d &Position_w);
        
    //! Relative humidity (0...1)
    virtual double getRelativeHumidity(const Math::Vec3d &Position_w) const;

    // \}

	//расчет адиабатики динамического торможения (увеличение температуры и давления при гашении скорости Velocity)
	virtual void	calcDynamicSlowdown(double Velocity, double &Temperature, double &Pressure) const;	
	virtual void	calcDynamicSlowdown(double Velocity, double &Temperature) const; //то же, только без давления

	virtual double	calcIcingRate(const Math::Vec3d &Position_w, double Velocity) const; //расчет скорости обледенения/таяния на данной высоте и скорости (изменение слоя льда за единицу времени [мм/мин])
	virtual double	calcIcingRate(double TemperatureH, double AtmoDensity, double Velocity) const; //расчет скорости обледенения/таяния при известной температуре и плотности атмосферы

	//! Atmosphere simulation
	virtual void	simulate(double dTime); 

private:
    
    double	AtmosphereWind_w[3][4];	//!< Wind speed vectors (at 0 - 2000 - 8000 meters)
    
//	double	getWindGroundFactor(double AbsoluteAltitude, const double &AltitudeAboveGround)const; //Возвращает коэффициент влияния земли на силу ветра. Диапазон 0..1

private:

    bool overcast;

    double cloudsDensity;

    double ceiling;
    double upperEdge;
    
    
};

}
#endif // StandardAtmosphere_h__
