#ifndef __waSatellite_h__
#define __waSatellite_h__

#include "Space.h"
#include "wTime.h"
#include "cPosition.h"

#include "viObject.h"
#include "MovingObjects\MovingHandle.h"
#include "GraphMath/box.h"
#include "Modeler/gGeometry.h"

#define IID_WASATELLITE Common::identify<waSatellite>()

enum enum_GNSS
{
	GNSS_GLONASS,
	GNSS_GPS
};

struct SPACE_API AlmanacGPS
{
	AlmanacGPS()
	{
		number = 0;
		type = 0;
		Health = 0;
		Eccentricity = 0;
		Time_of_Applicability = 0;
		Orbital_Inclination = 0;
		Rate_of_Right_Ascen = 0;
		SQRT_A = 0;
		Right_Ascen_at_Week = 0;
		Argument_of_Perigee = 0;
		Mean_Anom = 0;
		Af0 = 0;
		Af1 = 0;
		week = 0;
	}
	int number;
	unsigned char type;
	int Health;
	double Eccentricity;
	double Time_of_Applicability;
	double Orbital_Inclination;
	double Rate_of_Right_Ascen;
	double SQRT_A;
	double Right_Ascen_at_Week;
	double Argument_of_Perigee;
	double Mean_Anom;
	double Af0;
	double Af1;
	int week;
};

class SPACE_API waSatellite:	public viObject,
	public MovingHandle
{
public:
	waSatellite();
	waSatellite(int sat_id_, unsigned char system_id_);
	waSatellite(int sat_id_, unsigned char system_id_, const char* name_);
	~waSatellite();

	// интерфейс viObject
	cPosition&  	Position(wModelTime t)
	{
		return cpos;
	}
	const gBox&     LocalBox(wModelTime t)
	{
		return gbox;
	}
	// интерфейс MovingHandle
	float			GetSize()
	{
		return gbox.R;
	}

	void			update2(double timeDays, double dtSec = 5.0);
	void			update(double dtSec = 5.0);
	static dVector	calcVelocity(AlmanacGPS almanac_, double timeDays, double dtSec = 5.0);
	dVector			calcVelocity(double time0days, double dtSec) const;
	static dVector	calcPosition(AlmanacGPS almanac_, double time0days);
	dVector			calcPosition(double time) const;

	int				get_number() const
	{
		return sat_num;
	}
	unsigned char	getSystem() const
	{
		return system;
	}
	const dVector& getWGS84Position() const
	{
		return posWGS84;
	}
	const dVector& getWGS84Velocity() const
	{
		return velWGS84;
	}
	const AlmanacGPS& 	getAlmanac() const
	{
		return almanac;
	}

	void			set_number(int sat_num_)
	{
		sat_num = sat_num_;
	}
	void			setSystem(unsigned char system_)
	{
		system = system_;
	}
	void			setPosition(const dVector& posWGS84_);
	void			setVelocity(const dVector& velWGS84_);
	void			setAlmanac(AlmanacGPS almanac_)
	{
		almanac = almanac_;
	}

	//void set_available(bool available_) {available = available_};
	//bool is_available() const {return available;}
private:
	static const double GPS_TIME_ZERO;
	//
	int				sat_num;
	ed::string		name;
	unsigned char	system;
	AlmanacGPS		almanac;
	//
	dVector			posWGS84; //в WGS-84
	dVector			velWGS84;
	cPosition		cpos;
	Box				box;
	gBox			gbox;
};

typedef ed::list<waSatellite*>waSatelliteList;

class SPACE_API SatelliteStorage
{
public:
	SatelliteStorage();
	~SatelliteStorage();
	void Init();
	void updateSatellitesPos();
	void updateSatellitesPos(double time);
	void clear();
	void setSA(bool SA_)
	{
		SA_mode = SA_;
	}
	void setAS(bool AS_)
	{
		AS_mode = AS_;
	}
	bool getSA() const
	{
		return SA_mode;
	}
	bool getAS() const
	{
		return AS_mode;
	}

	waSatellite*			createSatellite(int sat_num_, unsigned char sat_system_);
	waSatellite*			getSatellite(int sat_num_, unsigned char sat_system_);
	void				notifySatelliteDeath(waSatellite* satellite);
private:
	bool	SA_mode;
	bool	AS_mode;
	waSatelliteList satellites_list;
	static const double GPS_COMMIT_DATE;
	static const double GLONASS_COMMIT_DATE;
};

extern SPACE_API SatelliteStorage theSatelliteStorage;

#endif