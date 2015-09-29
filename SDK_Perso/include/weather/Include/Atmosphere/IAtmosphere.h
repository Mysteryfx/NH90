//! \file
//! Atmosphere interface

//! This interface inherits basic interface from EagleFM and extends it.
//! All methods are duplicated in this header to save atmosphere users
//! from need reading EagleFM header.

#ifndef __IAtmosphere_h__
#define __IAtmosphere_h__

#include "Weather.h"
#include "FMSpace/IBasicAtmosphere.h"
#include "Atmosphere/Clouds.h"

namespace Weather
{

enum ATMO_TYPES
{
    ATMO_STANDARD,
    ATMO_DYNAMIC,
};


//! Interface to atmosphere model

//! Provides access to atmosphere parameters like sound speed, pressure,
//! temperature, density, wind etc.
//! Implementation must be a singleton.
//!
//! This interface inherits basic interface from EagleFM and extends it.
//! All methods are duplicated in this header to save atmosphere users
//! from need reading EagleFM header.
//! If you need to know something about the atmosphere and you are not an FM part,
//! ask this class, do not use the FM one to avoid unnecessary dependencies
//!
//! Though it is above all an interface this class is not pure abstract:
//! it contains some variables and functions which are common
//! for standard and dynamic atmosphere.
class WEATHER_API IAtmosphere : public EagleFM::IBasicAtmosphere
{
public:
    IAtmosphere();
    ~IAtmosphere(){};

    virtual void clear() = 0;

    //! Atmosphere type: standard or dynamic.
    virtual ATMO_TYPES getAtmoType() const = 0;

    //! \name Atmosphere parameters calculation methods
    //! \{

	//! Temperature [K] and geopotential altitude [m].
    virtual void getAtmoTemperature(const Math::Vec3d &Position_w, double *GeopotentialAltitude, double *AtmosphereTemperature) const = 0;

    //! Temperature [K], pressure [Pa] and geopotential altitude [m]. It's impossible to calсulate pressure whithout temperature calculation, 
    //! so there's no function getAtmoPressure
    virtual void getAtmoTemperatureAndPressure(const Math::Vec3d &Position_w, double *GeopotentialAltitude, double *AtmosphereTemperature, double *AtmospherePressure) const = 0;

    //! Temperature [K], pressure [Pa], density [kg/m^3] and geopotential altitude [m]. It's impossible to calсulate pressure whithout temperature calculation
    //! and density without pressure, so there's no functions getAtmoPressure and getAtmoDensity
    virtual void getAtmoTemperatureAndPressureAndDensity(const Math::Vec3d &Position_w, double *GeopotentialAltitude, double *AtmosphereTemperature, double *AtmospherePressure, double *AtmosphereDensity) const = 0;
	
    //! Sound speed [m/s]
    virtual double getAtmoSoundSpeed(double AtmosphereTemperature) const = 0;

    //! Basic wind [m/s], without turbulence. Use it by default. Don't forget to set AltitudeAboveGround argument to correct value
    //! if you need really correct wind, especially near the ground.
    virtual Math::Vec3d getBaseWind_w(const Math::Vec3d &Position_w, const double AltitudeAboveGround = 500.0) const = 0;

    //! Wind [m/s] with turbulence. Use it if and only if you're sure that you need turbulence. Don't forget to set AltitudeAboveGround argument to correct value
    //! if you need really correct wind, especially near the ground.
    virtual Math::Vec3d getAtmoWind_w(const Math::Vec3d &Position_w, const double AltitudeAboveGround = 500.0) const = 0;

    //! Basic ground wind [m/s], without turbulence. Returns wind at 11 meters AGL, (it is the wind measured by weather station).
    Math::Vec3d getGroundWind_w(const Math::Vec3d &Position_w) const;

    //! Get clouds density in point ( 0...10 ) (Бальность облачности)
    virtual double	getCloudsDensity(const Math::Vec3d &Position_w)const = 0;

    //! Precipitation type
    virtual int getPrecipitation(const Math::Vec3d &Position_w) const = 0;

    //! Ceiling (нижняя кромка облаков) [m]. If there is no clouds near Position_w, the ceiling will be 100000 meters.
    virtual double getCeiling(const Math::Vec3d &Position_w) = 0;

    //! Upper edge of clouds
    virtual double getCloudsUpperEdge(const Math::Vec3d &Position_w) = 0;

    //! Relative humidity (0...1)
    virtual double getRelativeHumidity(const Math::Vec3d &Position_w) const = 0;

    //! Dew point [K]
    virtual double getDewPoint(const Math::Vec3d &Position_w) const;

    //! \}

    //! Get 20 km squares with clouds
    ed::map<osg::Vec2i, CloudSquare*>& getClouds();

	//расчет адиабатики динамического торможения (увеличение температуры и давления при гашении скорости Velocity)
    virtual void    calcDynamicSlowdown(double Velocity, double &Temperature, double &Pressure) const = 0;	
    virtual void    calcDynamicSlowdown(double Velocity, double &Temperature) const = 0; //то же, только без давления

    virtual double  calcIcingRate(const Math::Vec3d &Position_w, double Velocity) const = 0; //расчет скорости обледенения/таяния на данной высоте и скорости (изменение слоя льда за единицу времени [мм/мин])
    virtual double  calcIcingRate(double TemperatureH, double AtmoDensity, double Velocity) const = 0; //расчет скорости обледенения/таяния при известной температуре и плотности атмосферы

	//! Atmosphere simulation
    virtual void simulate(double dTime); 
    void synchronize(double newTime, double step);

    void setSquaresToGraphicUpdate(int xMin, int xMax, int zMin, int zMax);
    void graphicUpdate(bool updateAll = false, ed::map<osg::Vec2i, bool>*  visualizedSquares = NULL);

    //! \name Weather condition setting methods
    //! \{

    void setRelativeHumidityData(double humidityAverage, double humidityYearMagnitude, double humidityDailyMagnitude);
    void setTimeFromYearStart(double seconds){timeFromYearStart = seconds;}
    void setSecondsInYear(int secondsInYear){this->secondsInYear = secondsInYear;}
    void generateClouds();

    //! \}

protected:
    //! \name Turbulence methods
    //! \{

    //! Read turbulence data from file 
    void	readTurbulenceData();

    //! Layered turbulence magnitude(?) at high altitude
    double  getLayeredTurbulence(const Math::Vec3d &Position_w)const;

    //! Layered turbulence magnitude(?) at low altitude
    double  getLayeredTurbulence(const Math::Vec3d &Position_w, const double AltitudeAboveGround)const;

    //! Turbulence vector (addition to base wind)
	Math::Vec3d	getTurbulenceInLayer(const Math::Vec3d &Position_w, const double CellSize)const;

    //! Ground factor is damping coefficient for both wind in standard model
    //! and turbulence in standard and dynamic models (0...1)
	double	getWindGroundFactor(double AbsoluteAltitude, const double &AltitudeAboveGround)const;
    //! \}

    //! Basic relative humidity (0...1)
    double getBasicRelativeHumidity() const;
    double getYearHumidityDelta() const;
    double getDailyHumidityDelta() const;
 
    //! Vapor saturation pressure
    double getSaturationPressure(double temperature) const;
    //! Vapor current pressure by relative humidity and temperature
    double getVaporPressure(double relativeHumidity, double temperature) const;
    //! Dew point by pressure
    double getDewPoint(double pressure) const;
    //! Dew point by relative humidity and temperature
    double getDewPoint(double temperature, double relativeHumidity) const;

    void    updateClouds();

protected:
    static Math::Vec3d	WindNoise[CellNum][CellNum]; //!< Array for turbulence calculation.

    double	BasicGeopotentialAltitude[8],		//!< Basic altitudes.
			AtmospherePressure[8],				//!< Atmosphere pressure at basic altitudes.
			AtmosphereTemperature[8],			//!< Temperature at basic altitudes.
			AtmosphereTemperatureGradient[7];	//!< Temperature gradient between basic altitudes.

	double	Time;

	int		Precipitation;
    	
	double	GroundTurbulence;					//!< Turbulence near ground.
			//LayerTurbulence;					//!< Not in use yet (or, maybe, already...).
			//CloudTurbulence;					//!< Not in use yet.

	Math::Vec3d	TurbulencePosition;				//!< Turbulence displacement in time.	// Вектор смещения в турбулентности от времени

    //! \name Clouds
    //! \{
    ed::map<osg::Vec2i, CloudSquare*>  clouds;   //!< 20km squares map with clouds
    bool   cloudsGenerated;                      //!< Flag
    //! \name Clouds update
    //! \{
    ed::map<osg::Vec2i, CloudSquare*>::iterator updateIterator;
    int squaresPerTick;
    ed::vector<osg::Vec2i> cloudsToGraphicUpdate;
    int level1Idx;
    int level2Idx;
    int currentLevel;
    //! \}

    double humidityAverage;
    double humidityYearMagnitude;
    double humidityDailyMagnitude;

    int secondsInYear;
    double timeFromYearStart;

    
    friend class CloudSquare;
    friend class Cloud;
};

//! Atmosphere singleton pointer. 
extern WEATHER_API IAtmosphere* sAtmosphere;

double const AdiabaticConstantAir = 1/3.5;			// показатель степени в адиабатике воздуха
double const PressureMCA_SL = 101325;           	// Давление на уровне моря стандартной атмосферы
double const R = 287.0;
double const EarthRadius = 6340000.0;	            // Радиус Земли
double const HydrostaticConstant = 0.034;			// Газовая постоянная
double const SoundSpeedFactor = 20.0;  				// Коэффициент для расчета скорости звука
double const AirAdiab1 = 1.0/AdiabaticConstantAir;

}

#endif
