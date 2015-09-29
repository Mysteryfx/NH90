#ifndef __MGRS__
#define __MGRS__

#include "WorldGeneral.h"

namespace MapCoordsSpace
{
	enum ELLIPSOID
	{
		// spheroids from A10C manual
		ELL_AC50 = 0, // ARC 1950 (Clarke 1880)
		ELL_ADIN, // ADINDAN (Clarke 1880)
        ELL_AU66, // AUSTRALIAN GEODETIC 1984 (Australian)
        ELL_BRIM, // BUKIT RIMPAH (Bessel)
        ELL_CAST, // CAMP AREA ASTRO (International)
        ELL_DIAK, // DJAKARTA (Bessel)
        ELL_EU50, // EUROPEAN 1950 (International)
        ELL_GB36, // ORD SURVEY GREAT BRITAIN 1936 (Airy)
        ELL_GD49, // GEODETIC DATUM 1949 (International)
        ELL_GHAN, // GHANA (WGS 84)
        ELL_GU63, // GUAM 1963 (Clarke 1866)
        ELL_HERA, // HERAT NORTH (International)
        ELL_HJOR, // HJORSEY 1955 (International)
        ELL_INDI, // INDIAN (Everest)
        ELL_IR65, // IRELAND 1965 (Modified Airy)
        ELL_KAUA, // OLD HAWAIIAN KAUAI (International)
        ELL_KERT, // KERTAU (Modified Everest)
        ELL_LI64, // LIBERIA 1964 (Clarke 1880)
        ELL_LOWE, // MONTJONG LOWE (WGS 84)
        ELL_LUZO, // LUZON (Clarke 1866)
        ELL_MAUI, // OLD HAWAIIAN MAUI (International)
        ELL_MERC, // MERCHICH (Clarke 1880)
        ELL_NIGE, // NIGERIA (Clarke 1880)
		ELL_N27A, // NORTH AMERICAN 1927 (ALASKA AND CANADA) (Clarke 1866)
		ELL_N27C, // NORTH AMERICAN 1927 (CONUS) (Clarke 1866)
        ELL_OAHU, // OLD HAWAIIAN OAHU (International)
        ELL_QORN, // QORNOQ (International)
		ELL_SACA, // SOUTH AMERICAN CORREGO ALEGRE (International)
		ELL_SAP,  // SOUTH AMERICAN PROV 1956 (International)
        ELL_SACH, // SOUTH AMERICAN ASTRO (International)
		ELL_SACI, // SOUTH AMERICAN INCHAUSPE (International)
		ELL_SAY,  // SOUTH AMERICAN YACARE (International)
        ELL_SDIN, // SD INDIAN SPECIAL (Everest)
        ELL_SDLU, // SD LUZON SPECIAL (Clarke 1866)
        ELL_SDTO, // SD TOKYO SPECIAL (Bessel)
        ELL_SD84, // SD WGS 84 SPECIAL (WGS 84)
        ELL_SEGA, // G SEGARA (Bessel)
        ELL_SERI, // G SERINDUNG (WGS 84)
        ELL_SHAN, // HU-TZU-SHAN (International)
        ELL_SL60, // SIERRA LEONE 1960 (WGS 84)
        ELL_TIMB, // TIMBALAI (Bessel)
        ELL_TO25, // TANANARIVE OBS 1925 (International)
        ELL_TOKY, // TOKYO (Bessel)
        ELL_USER, // USER ENTERED
        ELL_VOIR, // VOIROL (WGS 84)
		// ------
		ELL_AIRY,
		ELL_AUSTRALIAN_NATIONAL,
		ELL_BESSEL_1841,
		ELL_BESSEL_1841_NAMBIA, 
		ELL_CLARKE_1866,
		ELL_CLARKE_1880,
		ELL_EVEREST,
		ELL_FISCHER_1960,
		ELL_FISCHER_1968,
		ELL_GRS_1967,
		ELL_GRS_1980,
		ELL_HELMERT_1906,
		ELL_HOUGH,
		ELL_INTERNATIONAL,
		ELL_KRASSOVSKY,
		ELL_MODIFIED_AIRY,
		ELL_MODIFIED_EVEREST,
		ELL_MODIFIED_FISCHER,
		ELL_SOUTH_AMERICAN,
		ELL_WGS_60,
		ELL_WGS_66,
		ELL_WGS_72,
		ELL_WGS_84,
		ELL_NONE
	};

	class Ellipsoid
	{
	public:
		Ellipsoid(){};
		Ellipsoid(ELLIPSOID Id, double radius, double ecc)
		{
			id = Id; EquatorialRadius = radius; eccentricitySquared = ecc;
		}

		int id;
		double EquatorialRadius; 
		double eccentricitySquared;  
	};

	static Ellipsoid ellipsoid[] = 
	{
		Ellipsoid( ELL_AC50,				6378137, 0.00669438),
		Ellipsoid( ELL_ADIN,				6378137, 0.00669438),
		Ellipsoid( ELL_AU66,				6378137, 0.00669438),
		Ellipsoid( ELL_BRIM,				6378137, 0.00669438),
		Ellipsoid( ELL_CAST,				6378137, 0.00669438),
		Ellipsoid( ELL_DIAK,				6378137, 0.00669438),
		Ellipsoid( ELL_EU50,				6378137, 0.00669438),
		Ellipsoid( ELL_GB36,				6378137, 0.00669438),
		Ellipsoid( ELL_GD49,				6378137, 0.00669438),
		Ellipsoid( ELL_GHAN,				6378137, 0.00669438),
		Ellipsoid( ELL_GU63,				6378137, 0.00669438),
		Ellipsoid( ELL_HERA,				6378137, 0.00669438),
		Ellipsoid( ELL_HJOR,				6378137, 0.00669438),
		Ellipsoid( ELL_INDI,				6378137, 0.00669438),
		Ellipsoid( ELL_IR65,				6378137, 0.00669438),
		Ellipsoid( ELL_KAUA,				6378137, 0.00669438),
		Ellipsoid( ELL_KERT,				6378137, 0.00669438),
		Ellipsoid( ELL_LI64,				6378137, 0.00669438),
		Ellipsoid( ELL_LOWE,				6378137, 0.00669438),
		Ellipsoid( ELL_LUZO,				6378137, 0.00669438),
		Ellipsoid( ELL_MAUI,				6378137, 0.00669438),
		Ellipsoid( ELL_MERC,				6378137, 0.00669438),
		Ellipsoid( ELL_NIGE,				6378137, 0.00669438),
		Ellipsoid( ELL_N27A,				6378137, 0.00669438),
		Ellipsoid( ELL_N27C,				6378137, 0.00669438),
		Ellipsoid( ELL_OAHU,				6378137, 0.00669438),
		Ellipsoid( ELL_QORN,				6378137, 0.00669438),
		Ellipsoid( ELL_SACA,				6378137, 0.00669438),
		Ellipsoid( ELL_SAP,				    6378137, 0.00669438),
		Ellipsoid( ELL_SACH,				6378137, 0.00669438),
		Ellipsoid( ELL_SACI,				6378137, 0.00669438),
		Ellipsoid( ELL_SAY,				    6378137, 0.00669438),
		Ellipsoid( ELL_SDIN,				6378137, 0.00669438),
		Ellipsoid( ELL_SDLU,				6378137, 0.00669438),
		Ellipsoid( ELL_SDTO,				6378137, 0.00669438),
		Ellipsoid( ELL_SD84,				6378137, 0.00669438),
		Ellipsoid( ELL_SEGA,				6378137, 0.00669438),
		Ellipsoid( ELL_SERI,				6378137, 0.00669438),
		Ellipsoid( ELL_SHAN,				6378137, 0.00669438),
		Ellipsoid( ELL_SL60,				6378137, 0.00669438),
		Ellipsoid( ELL_TIMB,				6378137, 0.00669438),
		Ellipsoid( ELL_TO25,				6378137, 0.00669438),
		Ellipsoid( ELL_TOKY,				6378137, 0.00669438),
		Ellipsoid( ELL_USER,				6378137, 0.00669438),
		Ellipsoid( ELL_VOIR,				6378137, 0.00669438),
		//-----------------------
		Ellipsoid( ELL_AIRY,				6377563, 0.00667054),
		Ellipsoid( ELL_AUSTRALIAN_NATIONAL,	6378160, 0.006694542),
		Ellipsoid( ELL_BESSEL_1841,			6377397, 0.006674372),
		Ellipsoid( ELL_BESSEL_1841_NAMBIA,	6377484, 0.006674372),
		Ellipsoid( ELL_CLARKE_1866,			6378206, 0.006768658),
		Ellipsoid( ELL_CLARKE_1880,			6378249, 0.006803511),
		Ellipsoid( ELL_EVEREST,				6377276, 0.006637847),
		Ellipsoid( ELL_FISCHER_1960,		6378166, 0.006693422),
		Ellipsoid( ELL_FISCHER_1968,		6378150, 0.006693422),
		Ellipsoid( ELL_GRS_1967,			6378160, 0.006694605),
		Ellipsoid( ELL_GRS_1980,			6378137, 0.00669438),
		Ellipsoid( ELL_HELMERT_1906,		6378200, 0.006693422),
		Ellipsoid( ELL_HOUGH,				6378270, 0.00672267),
		Ellipsoid( ELL_INTERNATIONAL,		6378388, 0.00672267),
		Ellipsoid( ELL_KRASSOVSKY,			6378245, 0.006693422),
		Ellipsoid( ELL_MODIFIED_AIRY,		6377340, 0.00667054),
		Ellipsoid( ELL_MODIFIED_EVEREST,	6377304, 0.006637847),
		Ellipsoid( ELL_MODIFIED_FISCHER,	6378155, 0.006693422),
		Ellipsoid( ELL_SOUTH_AMERICAN,		6378160, 0.006694542),
		Ellipsoid( ELL_WGS_60,				6378165, 0.006693422),
		Ellipsoid( ELL_WGS_66,				6378145, 0.006694542),
		Ellipsoid( ELL_WGS_72,				6378135, 0.006694318),
		Ellipsoid( ELL_WGS_84,				6378137, 0.00669438),
		Ellipsoid( ELL_NONE,				      0, 0.0)
	};

	class WORLDGENERAL_API MGRS
	{
	public:
		static void LLtoMGRS(const double Lat, const double Long, 
			char* UTMZone, char* MGRSDigraph, int &Easting, int &Northing, 
			ELLIPSOID ReferenceEllipsoid = ELL_WGS_84);
		static void MGRStoLL(const char* UTMZone, const char* MGRSDigraph, 
			const int Easting, const int Northing, double& Lat, double& Long, 
			ELLIPSOID ReferenceEllipsoid = ELL_WGS_84);
		static double Distance(const double Lat1, const double Long1,
			const double Lat2, const double Long2,
			ELLIPSOID ReferenceEllipsoid = ELL_WGS_84);
		static double DistanceOnSphere(const double Lat1, const double Long1,
			const double Lat2, const double Long2,
			ELLIPSOID ReferenceEllipsoid = ELL_WGS_84);
		static double Bearing(const double Lat1, const double Long1,
			const double Lat2, const double Long2,
			ELLIPSOID ReferenceEllipsoid = ELL_WGS_84);
		static double BearingOnSphere(const double Lat1, const double Long1,
			const double Lat2, const double Long2,
			ELLIPSOID ReferenceEllipsoid = ELL_WGS_84);
		static double Bearing_from_x_z(const double x1, const double z1, 
			const double x2, const double z2);
		static void CalcLL(const double Lat1, const double Long1,
			const double Bearing, const double Distance, double& Lat2, double& Long2,
			ELLIPSOID ReferenceEllipsoid = ELL_WGS_84);
		static void CalcLL_OnSphere(const double Lat1, const double Long1,
			const double Bearing, const double Distance, double& Lat2, double& Long2,
			ELLIPSOID ReferenceEllipsoid = ELL_WGS_84);
		static double getMiddleLatForUTMZone(char utmLetter);
		static double getMiddleLongForUTMZone(int zoneNumber);

	private:
		static char GetUTMLetter(double Lat);
		static void GetMGRSDigraph(int ZoneNumber, int Easting, int Northing, 
			char* MGRSDigraph);
		static int GetOffsetEasting(const char* MGRSDigraph);
		static int GetOffsetNothing(int Northing, const char* MGRSDigraph, 
			const int ZoneNumber, const char ZoneLetter);
	};
}

#endif