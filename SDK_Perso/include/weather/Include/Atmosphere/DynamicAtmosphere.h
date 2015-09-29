//! \ file
//! Dynamic atmosphere implementation

//! This is new DCS atmosphere with different weather at different parts of map.
//! Should be dynamic but is not yet.
#ifndef DynamicAtmosphere_h__
#define DynamicAtmosphere_h__

#include <ed/map.h>
#include <osg/Vec3d>

#include "Atmosphere/IAtmosphere.h"
#include "Randomizer/RandomObject.h"

namespace Weather
{

//! Meteo formation or weather type
enum METEO_FORMATION_TYPE
{
	MFTYPE_CYCLONE = -1, //!< Cyclone as meteo formation or cyclonic weather type
	MFTYPE_NEUTRAL,		 //!< Neutral weather type
	MFTYPE_ANTICYCLONE,	 //!< Anticyclone as meteo formation or anticyclonic weather type
};

//#define WIND_CACHE_SIZE 3

//! Auxiliary class
class BaricSystem;

//! Atmosphere model

//! Provides access to atmosphere parameters like sound speed, pressure,
//! temperature, density, wind etc.
//! Must be a singleton during simulation.
//!
//! This is new DCS atmosphere with different weather at different parts of map.
//! Should be dynamic but is not yet.
class WEATHER_API DynamicAtmosphere : public IAtmosphere
{
public:
	DynamicAtmosphere();
	virtual ~DynamicAtmosphere(){};

    virtual ATMO_TYPES getAtmoType() const;

	virtual void	clear();

    //! \name Weather condition setting methods
    //! \{

    //! Baric system
    void    addBaricSystem(const BaricSystem &sys);

    //! Temperature in point
    void    addPointAtmoTemperature(const Math::Vec3d &Position_w, double Time, double _AtmosphereTemperature);

    //! Turbulence near ground
    void    addGroundTurbulence(double Turbulence);

	// \}

    //! \name Atmosphere parameters calculation methods
    //! \{
	
    //! Temperature [K] and geopotential altitude [m].
	virtual void	getAtmoTemperature(const Math::Vec3d &Position_w, double *GeopotentialAltitude, double *AtmosphereTemperature) const;

    //! Temperature [K], pressure [Pa] and geopotential altitude [m]. It's impossible to calсulate pressure whithout temperature calculation, 
    //! so there's no function getAtmoPressure
	virtual void	getAtmoTemperatureAndPressure(const Math::Vec3d &Position_w, double *GeopotentialAltitude, double *AtmosphereTemperature, double *AtmospherePressure) const;

    //! Temperature [K], pressure [Pa], density [kg/m^3] and geopotential altitude [m]. It's impossible to calсulate pressure whithout temperature calculation
    //! and density without pressure, so there's no functions getAtmoPressure and  getAtmoDensity
	virtual void	getAtmoTemperatureAndPressureAndDensity(const Math::Vec3d &Position_w, double *GeopotentialAltitude, double *AtmosphereTemperature, double *AtmospherePressure, double *AtmosphereDensity) const;
	
    //! Sound speed [m/s]
	virtual double	getAtmoSoundSpeed(double AtmosphereTemperature) const;

    //! Basic wind [m/s], without turbulence. Use it by default. Don't forget to set AltitudeAboveGround argument to correct value
    //! if you need really correct wind, especially near the ground.
	virtual Math::Vec3d	getBaseWind_w(const Math::Vec3d &Position_w, const double AltitudeAboveGround = 1000.0) const;
	
    //! Wind [m/s] with turbulence. Use it if and only if you're sure that you need turbulence. Don't forget to set AltitudeAboveGround argument to correct value
    //! if you need really correct wind, especially near the ground.
    virtual Math::Vec3d	getAtmoWind_w(const Math::Vec3d &Position_w, const double AltitudeAboveGround = 1000.0) const;

    //! Get clouds density in point ( 0...10 ) (Бальность облачности)
    double	getCloudsDensity(const Math::Vec3d &Position_w)const;

    //! Precipitation type
	virtual int	getPrecipitation(const Math::Vec3d &Position_w) const;

    //! Ceiling (нижняя кромка облаков) [m]. If there is no clouds near Position_w, the ceiling will be 100000 meters.
    virtual double getCeiling(const Math::Vec3d &Position_w) ;

    //! Upper edge of clouds
    virtual double getCloudsUpperEdge(const Math::Vec3d &Position_w);

    //! Relative humidity (0...1)
    virtual double getRelativeHumidity(const Math::Vec3d &Position_w) const;

    //! \}

	//расчет адиабатики динамического торможения (увеличение температуры и давления при гашении скорости Velocity)
	virtual void	calcDynamicSlowdown(double Velocity, double &Temperature, double &Pressure) const;	
	virtual void	calcDynamicSlowdown(double Velocity, double &Temperature) const; //то же, только без давления

	virtual double	calcIcingRate(const Math::Vec3d &Position_w, double Velocity) const; //расчет скорости обледенения/таяния на данной высоте и скорости (изменение слоя льда за единицу времени [мм/мин])
	virtual double	calcIcingRate(double TemperatureH, double AtmoDensity, double Velocity) const; //расчет скорости обледенения/таяния при известной температуре и плотности атмосферы

	//! Atmosphere simulation
	virtual void	simulate(double dTime);

private:

    //! Average temperature in segment from 0 m to altitude
    double  getAverageTemperature(double altitude)const;

    //! Excess from normal pressure
   	double	getPressureExcess(const Math::Vec3d &Position_w)const;

    //! Pressure grid for gradient calculation
    void	fillPressureGrid(const osg::Vec3d &Point, double *Press)const; 

    void    updateBaricSystems(double dTime);

private:        
    RandomObject				rndNormal;          //!< Randomizer.

    //! \name Baric systems
    //! \{
	ed::vector<BaricSystem>     baricSystems;             //!< Baric systems array.
	//int							nCyclones;                //!< Baric systems quantity.
    //! \name Baric systems update
    //! \{
    double                      constEastVelocity;        //!< Baric systems move to the east
    double                      baricSystemsLastUpdate;   //
    double                      baricSystemsUpdatePeriod; //
    //! \}
    //! \}

	int                         gradToWind;         //!< Coefficient for wind calculation from pressure gradient.

    //! \name Jet stream parameters
    //! \{
    Math::Vec3d                 jetStream;          //!< Jet stream direction.
    double                      jetStreamHeight;    //!< Jet stream height.
    double                      jetStreamThickness; //!< Jet stream thickness.
    //! \}


    //! name Wind cache
/*
    //! \{
    mutable Math::Vec3d cachedBaseWind[WIND_CACHE_SIZE];
    mutable Math::Vec3d cachedBaseWindPosition[WIND_CACHE_SIZE];
    mutable double cachedBaseWindAltAGL[WIND_CACHE_SIZE];
    mutable int baseWindCacheIndex;
    //! \}
//*/
    
    //! \}
};

//! Baric system, cyclone or anticyclone - auxiliary class.
class BaricSystem
{
public:
    BaricSystem():center(0,0,0), rotation(0), ellipticity(0),
		pressureExcess(0), pressureSpread(0), centerVelocity(0,0,0), clone(false), linkedSystem(-1){}
	~BaricSystem(){}

	void	setCenter(osg::Vec3d c) {center = c;}
	void	setRotation(double r) {rotation = r;}
	void	setEllipticity(double e) {ellipticity = e;}
	void	setPressureExcess(double pe) {pressureExcess = pe;}
	void	setPressureSpread(double ps) {pressureSpread = ps;}
	void	setCenterVelocity(osg::Vec3d cv) {centerVelocity = cv;}

    osg::Vec3d	getCenter() const {return center;}
	double	getRotation() const {return rotation;}
	double	getEllipticity() const {return ellipticity;}
	double	getPressureExcess() const {return pressureExcess;}
	double	getPressureSpread() const {return pressureSpread;}
    osg::Vec3d	getCenterVelocity() const {return centerVelocity;}

	double	getPressureExcessInPoint(const osg::Vec3d &Point) const;

    void    makeClone(int parent){linkedSystem = parent; clone = true;}
    void    makeIndependent(){linkedSystem = -1; clone = false;}
    void    setClone(int clone){linkedSystem = clone; clone = false;}
    void    unlink(){linkedSystem = -1; clone = false;}
    bool    isClone(){return clone;}
    int     getLinkedSystem(){return linkedSystem;}
    void    decreaseLinkedSystemNumber(){linkedSystem--;}

	void	move(double dTime){center += centerVelocity*dTime;}

private:
	osg::Vec3d	center;			// Координаты центра
	double	rotation;		    // Угол поворота
	double	ellipticity;	    // Эллиптичность
	double	pressureExcess;	    // Смещение давления от "нормального"
	double	pressureSpread;     // Протяжённость циклона
	osg::Vec3d	centerVelocity;	// Скорость центра 

    bool clone;
    int linkedSystem;
};


}
#endif // DynamicAtmosphere_h__
