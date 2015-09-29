#ifndef _ED_Geodetic__h_
#define _ED_Geodetic__h_

#include "MapCoords.h"
#include "cAlgorithm.h"

#include "dPosition.h"

namespace cockpit {

//ECEF - декартовая геоцентрическая система система координат
//OZ - на северный полюс
//OX - на точку пересечения экватора и гринвича
//OY - вправо
//прим: географические полярные координаты latitude и longitude В РАДИАНАХ!

enum enum_ELLIPSOID
{
	KRASSOVSKY,
	GRS_80
};

inline dVector /*COCKPITBASE_API*/ toECEFCoord(unsigned char coord_system, double latitude, double longitude, double height)
{
	dVector res = 0.0;
	//эллипсоид 
	double a = 0.0;
	double b = 0.0;
	switch(coord_system)
	{
		case KRASSOVSKY: //Пулково 1942
			a = 6378245.0;
			b = 6356863.0;
		break;
		case GRS_80: //WGS-84
			a = 6378137.0;
			b = 6356752.31425;
		break;
	}
	double e = (a*a - b*b)/a/a;
	double v = a / sqrt(1 - e*e * sin(latitude)*sin(latitude));
	res.x = (v + height) * cos(latitude) * cos(longitude);
	res.y = (v + height) * cos(latitude) * sin(longitude);
	res.z = ((1 - e * e) * v + height) * sin(latitude);
	return res;
}

inline void /*COCKPITBASE_API*/ toGeoCoord(unsigned char coord_system, const dVector & ecef_coord, double & latitude, double & longitude, double & height)
{
	//эллипсоид 
	double a = 0.0;
	double b = 0.0;
	switch(coord_system)
	{
		case KRASSOVSKY: //Пулково 1942
			a = 6378245.0;
			b = 6356863.0;
		break;
		case GRS_80: //WGS-84
			a = 6378137.0;
			b = 6356752.31425;
		break;
	}
	double e = (a*a - b*b)/a/a;
	double v = a / sqrt(1 - e*e * sin(TO_RAD(latitude))*sin(TO_RAD(latitude)));
	
	double rad_sec = 1.0 * M_PI / 180.0 / 3600.0; //угловая секунда в радианах
	
	longitude = atan2(ecef_coord.y, ecef_coord.x);	
	double latitude_prev = atan(ecef_coord.z / ((1-e*e)*sqrt(ecef_coord.x*ecef_coord.x + ecef_coord.y*ecef_coord.y)));	
	double latitude0 = latitude_prev;
	double N = a * sqrt(1 - e * e * sin(latitude_prev) * sin(latitude_prev));
	height = sqrt(ecef_coord.x * ecef_coord.x + ecef_coord.y * ecef_coord.y) / cos(latitude_prev) - N;
	latitude = atan(ecef_coord.z / sqrt(ecef_coord.x * ecef_coord.x + ecef_coord.y * ecef_coord.y) / (1 - e * e * N / (N + height)) );	
	while(fabs(latitude_prev - latitude) > rad_sec)
	{
		latitude_prev = latitude;
		N = a * sqrt(1 - e * e * sin(latitude_prev) * sin(latitude_prev));
		if(latitude0 < 0.785)
		{
			height = sqrt(ecef_coord.x * ecef_coord.x + ecef_coord.y * ecef_coord.y) / cos(latitude_prev) - N;
		}
		else
		{
			height = ecef_coord.z / sin(latitude_prev) - (1 - e * e) * N;
		}
		latitude = atan(ecef_coord.z / sqrt(ecef_coord.x * ecef_coord.x + ecef_coord.y * ecef_coord.y) / (1 - e * e * N / (N + height)) );		
	}	
}

inline void getCSparams(unsigned char coord_system, double & earth_rotate_rate, double & gravity_const)
{
	if(coord_system == GRS_80) //WGS-84
	{
		earth_rotate_rate = 7.2921151467E-5;
		gravity_const = 3.986005E14;
	}
}

inline dPoint convertECEFcoords(const dPoint & point, unsigned char from, unsigned char to)
{
	if(from == to)
	{
		return point;
	}

	double dX, dY, dZ, wx, wy, wz;

	if(from == KRASSOVSKY && to == GRS_80)
	{
		dX = 26.9;
		dY = -131.8;
		dZ = -74.8;
		wz = TO_RAD(0.233 / 3600.0);
		wx = TO_RAD(-0.416 / 3600.0);
		wy = TO_RAD(-0.906 / 3600.0);	
	}
	else if(from == GRS_80 && to == KRASSOVSKY)
	{
		dX = -26.9;
		dY = 131.8;
		dZ = 74.8;
		wz = TO_RAD(-0.233 / 3600.0);
		wx = TO_RAD(0.416 / 3600.0);;
		wy = TO_RAD(0.906 / 3600.0);;		
	}

	dVector shift;
	dPosition coeff;

	shift.x = dX;
	shift.y = dY;
	shift.z = dZ;

	coeff.x.x = 1; coeff.x.y = -wz; coeff.x.z = wy;
	coeff.y.x = wz; coeff.y.y = 1; coeff.y.z = -wx;
	coeff.z.x = -wy; coeff.z.y = wx; coeff.z.z = 1;

	dVector mult_point;
	mult(mult_point, coeff, point);
	return shift + mult_point;
}

//локальная прямоугольная система кординат:
//OX - параллельно меридиану локальной точки и на север
//OY - от центра Земли до локальной точки и вверх
//OZ - вправо

//получение локального базиса для точки в координатах WGS84
inline dPosition get_ECEF_local_basis(const dVector & ref_point_ECEF_coord) //dbg function
{
	double ecv_angle = atan2(ref_point_ECEF_coord.x, ref_point_ECEF_coord.y);
	double gr_angle = atan2(ref_point_ECEF_coord.z, sqrt(ref_point_ECEF_coord.x * ref_point_ECEF_coord.x + ref_point_ECEF_coord.y * ref_point_ECEF_coord.y));
	dPosition local_basis;
	local_basis.p = ref_point_ECEF_coord;
	local_basis.rotZ(-ecv_angle);
	local_basis.rotX(gr_angle);	
	cVector vec_x = local_basis.x;
	local_basis.x = local_basis.z;
	local_basis.z = -vec_x;
	return local_basis;
}

//расположение одной точки относительно опорной (второй отн. первой) в локальной СК
inline dVector get_local_dec_coord(const dVector & ref_point_ECEF_coord, const dVector & point_ECEF_coord)
{
	dPosition local_basis = get_ECEF_local_basis(ref_point_ECEF_coord);
	dVector ECEF_direction = point_ECEF_coord - ref_point_ECEF_coord;
	dVector local_coord;
	mult_transp(local_coord, local_basis, ECEF_direction);
	return local_coord;
}

//то же, только опорная задана географическими координатами
inline dVector get_local_dec_coord(unsigned char coord_system, float latitude, float longitude, float height, const dVector & point_ECEF_coord)
{
	dVector ref_point_ECEF_coord = toECEFCoord(coord_system, latitude, longitude, height);
	return get_local_dec_coord( ref_point_ECEF_coord, point_ECEF_coord);
}

inline void LOToGeoCoords(const cPoint & pos, double & latitude, double & longitude, double & height)
{
	double lat, lon;
	MapCoordsSpace::MapCoords::LatLon(pos.x, pos.z, lat, lon);

	latitude = TO_RAD(lat);
	longitude = TO_RAD(lon);
	height = pos.y;
}

inline cVector geoToLOCoords(double latitude, double longitude, double height)
{
	cVector LOcoords;
	MapCoordsSpace::MapCoords::GetCoords(TO_GRAD(latitude), TO_GRAD(longitude), LOcoords.x, LOcoords.z);
	LOcoords.y = height;
	return LOcoords;
}

inline dVector LOToECEFCoords(unsigned char coord_system, const cPoint & pos)
{
	double latitude, longitude, height;
	LOToGeoCoords(pos, latitude, longitude, height);
	return toECEFCoord(coord_system, latitude, longitude, height);
}

inline cVector ECEFtoLOCoords(unsigned char coord_system, const dVector & ECEFcoords)
{
	double latitude, longitude, height;
	toGeoCoord(coord_system, ECEFcoords, latitude, longitude, height);
	return geoToLOCoords(latitude, longitude, height);
}

}

#endif /* _ED_Geodetic__h_ */
