#ifndef SPACE_H
#define SPACE_H

#ifdef _WINDOWS
	#ifdef SPACE_EXPORTS
		#define SPACE_API __declspec(dllexport)
	#else
		#define SPACE_API __declspec(dllimport)
	#endif
#else
	#define SPACE_API
#endif

#include <stdio.h>
#include <ed/string.h>
#include <ed/vector.h>
#include "osg/Vec3f"
#include "osg/Vec4f"

#ifndef EDGE
#include "position.h"
#include "Iwasun.h"
class SPACE_API Space : public IwaSun
#else
class SPACE_API Space
#endif
{
public:
	enum PlanetID
	{
		SUN=0, MOON, MERCURY, VENUS, MARS, JUPITER, SATURN, URANUS, NEPTUNE
	};

	struct OrbitalElements
	{
		ed::string _name;
		double N;
		double i;
		double w;
		double a;
		double e;
		double M;
		double brightness;
	};

	struct StarData
	{
		int starID;
		ed::string name;
		double azimuth;
		double elevation;
		double brightness;
	};
	typedef ed::vector<StarData> StarsList;
	typedef ed::vector<osg::Vec4f> StarsSoords;

protected:
	static Space* _instance;

public:
	static void init();
	static void done();
	static Space* getInstance();
	static bool initialized()
	{
		return _instance != NULL;
	};


protected:
	double _Year;
	double _Month;
	double _Day;
	double _Hour;
	double _Minute;
	double _Second;

	double _d;

	double _Lat;		// Широта места
	double _Lon;		// Долгота места

	double _rs;			// distance to Sun
	double _lonsun;		// longitude of the Sun
	double _Ms;			// Sun's  mean anomaly
	double _Ls;			// Sun's mean longitude
	double _ecl;		// obliquity of the ecliptic

	double _timeShift;	// смещение времени относительно Гринвича в часах
	float _winterTimeDelta;
	float _summerTimeDelta;

	Space();
	~Space();

	double getD();
	OrbitalElements getOrbitalElements(Space::PlanetID planetID);
	void update();
	void azimuthElevation(double RA, double Dec, double& azimuth, double& height);
public:
	double siderialTime(double lon = 0.0);
	double solarTime();

public:
	osg::Vec3f getDir(double azimuth, double height);
	void setGreenwichDateTime(double Year, double Month, double Day, double Hour, double Minute, double Second);
	void getDateTime(double& Year, double& Month, double& Day, double& Hour, double& Minute, double& Second);

	void setLatLon(double Lat, double Lon);
	void getLatLon(double& Lat, double& Lon);

	void getSun(double& azimuth, double& elevation);
	void getMoon(double& azimuth, double& elevation);
	void getPlanet(PlanetID planetID, double& azimuth, double& elevation, double& brightness);
	osg::Vec3f getPlanet(PlanetID planetID, double& brightness);
	int getPlanetsCount()
	{
		return 9;
	}
	void getStar(int starID, double& azimuth, double& elevation, double& brightness);

	StarsList getStarsList();

	double getSunZenithAngleR();
	double getSunZenithAngleD();
	osg::Vec3f getSunVector();
	void getSunVector(float& x, float& y, float& z);

	virtual float Get_Sun_Zenit(void);

	void getStars(StarsSoords &starts, double Year = 0, double Month = 0, double Day = 0);

	void updateTimeShift();	//обновить смещение времени

	double getTimeShift(); // смещение времени относительно Гринвича в часах
	float getSummerTimeDelta()
	{
		return _summerTimeDelta;
	}
	float getWinterTimeDelta()
	{
		return _winterTimeDelta;
	}

#ifdef EDGE
	void setLatLon(osg::Vec3f viewerPos);
#else
	void getSun(Position3& pos);
	void getMoon(Position3& pos);
	Vector3 Space::getStar(int planetID);
#endif


};

#endif
