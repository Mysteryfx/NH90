#pragma once

#include <ed/string.h>
#include <ed/map.h>
#include <stdlib.h>

#include "WRadio.h"
#include "wMovingObject.h"
#include "Mem/Shared.h"

typedef int(*getIdxPtr)(int);

enum DL_identity_
{
	DL_IDENT_UNK,
	DL_IDENT_PEND,
	DL_IDENT_ASSUM_FRND,
	DL_IDENT_FRND,
	DL_IDENT_NEUT,
	DL_IDENT_SUSP,
	DL_IDENT_HOST,
};

enum DL_platform_
{
	DL_PLAT_UNK,
	DL_PLAT_FIGHTER,
	DL_PLAT_ATTACK,
	DL_PLAT_BOMBER,
	DL_PLAT_ADART,
	DL_PLAT_SAM,
	DL_PLAT_TANK,
	DL_PLAT_TROOP,
	DL_PLAT_LNDMARK,
	DL_PLAT_SHIP,
};

enum DL_environment_
{
	DL_ENV_UNK,
	DL_ENV_AIR,
	DL_ENV_LAND,
	DL_ENV_SURF,
};

enum
{
	DL_SID_AttackingEngaging  = 1,
	DL_SID_Investigating      = 2,
	DL_SID_MIF_WeaponReleased = 3,
	DL_SID_LocalTargetReport  = 4,
	DL_SID_CancelReport       = 5,
	DL_SID_Disengage          = 7,
	DL_SID_PDT                = 10,
	DL_SID_Covering           = 12,
	DL_SID_BDA_UNK            = 45,
	DL_SID_BDA_TGT_DESTR      = 46,
	DL_SID_BDA_PART_DESTR     = 47,
	DL_SID_BDA_REATTACK       = 48,
};

enum
{
	DL_MAD_Wilco               = 3,
	DL_MAD_CantComply          = 4,
	DL_MAD_Engage              = 5,
	DL_MAD_BreakEngagement     = 7,
	DL_MAD_Investigate         = 8,
	DL_MAD_ClearToDrop         = 9,
	DL_MAD_Cease_DontDrop      = 10,
	DL_MAD_ContactPoint        = 20,
	DL_MAD_InitialPoint        = 21,
	DL_MAD_ClosestFriendly     = 22,
	DL_MAD_CantProcess         = 23,
	DL_MAD_Cover               = 31,
	DL_MAD_Attack              = 41,
	DL_MAD_CeaseAttack         = 42,
	DL_MAD_CeaseMission        = 63,
	DL_MAD_PriorityKill        = 64,
	DL_MAD_BreakPriorityKill   = 65,
	DL_MAD_AttackFEA           = 66,
	DL_MAD_VID                 = 67,
	DL_MAD_AerialRecon         = 68,
	DL_MAD_Shadow              = 69,
	DL_MAD_Intervene           = 71,
	DL_MAD_ArmedRecon          = 72,
	DL_MAD_FighterSweep        = 73,
	DL_MAD_SEAD                = 74,
	DL_MAD_LaserDesign_Bombing = 75,
	DL_MAD_RTB                 = 76,
	DL_MAD_Divert              = 77,
	DL_MAD_Recall              = 78,
	DL_MAD_Orbit               = 79,
	DL_MAD_Refuel              = 80,
	DL_MAD_Escort              = 81,
	DL_MAD_SAR                 = 82,
	DL_MAD_CAP                 = 83,
	DL_MAD_CAS                 = 84,
	DL_MAD_Salvo_ClearAircraft = 85,
	DL_MAD_GoToVoice           = 86,
};

class WRADIO_API AirPlatformStatus : public Mem::Shared
{
public:
	AirPlatformStatus() : FuelOnboard(-1) {storesZeroize();}

	void storesZeroize();

	bool operator==(const AirPlatformStatus& in)
	{
		if (FuelOnboard != in.FuelOnboard)
			return false;

		if (GunCapable != in.GunCapable)
			return false;

		if (StoreType1 != in.StoreType1)
			return false;

		if (StoreType2 != in.StoreType2)
			return false;

		if (StoreType3 != in.StoreType3)
			return false;

		if (StoreType4 != in.StoreType4)
			return false;

		if (StoreCount1 != in.StoreCount1)
			return false;

		if (StoreCount2 != in.StoreCount2)
			return false;

		if (StoreCount3 != in.StoreCount3)
			return false;

		if (StoreCount4 != in.StoreCount4)
			return false;

		return true;
	}

	bool operator!=(const AirPlatformStatus& in)
	{
		if (*this == in)
			return false;

		return true;
	}

	ed::string PlatformSpecificType;
	int stype;
	int platform;

	int FuelOnboard;

	bool GunCapable;

	int StoreType1;
	int StoreType2;
	int StoreType3;
	int StoreType4;

	int StoreCount1;
	int StoreCount2;
	int StoreCount3;
	int StoreCount4;

	ed::string WeaponsString;
};


class WRADIO_API SADL_msgsParser
{
public:

	SADL_msgsParser();

	enum
	{
		// msg type
		TYPE,  // Message type
		// addresses
		TN,    // Source track number 
		TNDST, // Track Number, Destination 
		IDXN,  // Index Number
		TNOBJ, // Track Number, Objective
		TNIO,  // Track Number, Index Originator
		INTHP, // Index Number, Third Party
		TNMC,  // Track Number, Mission Commander
		TNFL,  // Track Number, Flight Lead
		TNREF, // Track Number, Reference
		URN,   // Unit Reference Number (originator)
		URNR,  // URN reference
		URNOB, // Unit Reference Number, Objective
		TADCI, // TADIL C Address Indicator
		TADC,  // TADIL C Address. 3 characters string.
		// discretes
		MAD,   // Mission assignment discrete
		MADSC, // Mission assignment discrete type description (text)
		SID,   // Status informatin discrete
		TDP,   // Time discrete parameter
		// Status ID
		NPSI,  // Network Participation Status Indicator
		EXI,   // Exercise Indicator
		SIMI,  // Simulation Indicator
		FTELI, // Force Tell Indicator
		C2I,   // C2 Indicator
		ARBNI, // Airborne Indicator
		// type ID
		ENV,   // Environment
		IDENT, // Identity
		PLAT,  // Platform
		PACT,  // Air Platform Activity. Int
		STYPE, // Platform specific type
		M1,    // Mode 1 Code
		M2,    // Mode 2 Code
		M3,    // Mode 3 Code
		ETYPE, // Emergency Type
		// flight ID
		CALLI, // Voice Callsign Indicator
		CALL,  // Callsign
		FLI,   // Flight leader indicator
		// Emergency
		EMERI, // Emergency indicator
		BAILI, // Bailout Indicator
		// position
		LAT,   // Latitude
		LON,   // Longitude
		ALT,   // Altitude
		ALTO,  // Objective Altitude
		// orientation
		CRS,   // True heading
		RH,    // Run in heading (true, degrees)
		EH,    // Egress heading (true, degrees)
		// stores
		FUEL,  // Fuel onboard
		FFUNC, // Fuel Function
		GUN,   // Gun capable
		STOR1, // Weapon 1 type
		STOR2, // Weapon 2 type
		STOR3, // Weapon 3 type
		STOR4, // Weapon 4 type
		SNUM1, // Weapon 1 quantity
		SNUM2, // Weapon 2 quantity
		SNUM3, // Weapon 3 quantity
		SNUM4, // Weapon 4 quantity
		RSTT,  // Required stores type
		RSTQ,  // Required stores quantity
		SUMI,  // Stores Summation Indicator
		// other data
		GS,    // Ground speed
		TFP,   // Time Function Parameter
		TIME,  // Time
		BDA,   // Battle damage assessment
		TEXT,  // Text
		FRCHI, // Voice Frequency/Channel Indicator
		FRCH,  // Voice Frequency/Channel. int
		CC,    // Control Channel. Int
		RTTRI, // RTT Reply Status Indicator
		NET,   // Net Number, NonC2-to-NonC2 NPG
		NETS,  // NonC2-to-NonC2 NPG Status
		ARWI,  // Active Relay Indicator, WAN
		ARVI,  // Active Relay Indicator, Voice Channel
		ARCI,  // Active Relay Indicator, Control Channel
		MC1,   // Mission Correlator, 1
		MC2,   // Mission Correlator, 2
		MC3,   // Mission Correlator, 3
		ISPT,  // Is Point
		LCODE, // Laser Code
		TDEF,  // Target Defenses
		FIELDS_COUNT
	};

	enum
	{
		SYS_TYPE,
		SYS_TN,
		SYS_NET,
		SYS_SHAPE,
		SYS_NTSTR,
		SYS_FIELDS_COUNT
	};


	static void prepare();

	static void place(int i, const ed::string& ItemToAppend);
	static void place(int i, const char* ItemToAppend);
	static void place(int i, int ItemToAppend);
	static void place(int i, bool ItemToAppend);
	static void place(int i, double ItemToAppend, int Signs);
	static void place(int i, float ItemToAppend, int Signs);

	static void append(ed::string& Res, const ed::string& ItemToAppend);
	static bool parse(const ed::string& str_in);

	// The field is NS (No Statement)
	static const bool fieldIsNS(int i) {return ParsedMessage[i].empty();}

	static const ed::string& getStr(int i) {return ParsedMessage[i];}
	static const int getInt(int i) {return atoi(ParsedMessage[i].c_str());}
	static const double getDbl(int i) {return atof(ParsedMessage[i].c_str());}
	static const bool getBool(int i) {return atoi(ParsedMessage[i].c_str());}

	static ed::string getMsgAsString(int LastIdx);

	class WRADIO_API Msg
	{
	protected:
		static void initHeader(ed::map<int, int>& Indexes)
		{
			Msg::addIdx(SADL_msgsParser::TYPE, Indexes);
			Msg::addIdx(SADL_msgsParser::TN, Indexes);
		}

		static int getIdx(int i, const ed::map<int, int>& Indexes)
		{
			ed::map<int, int>::const_iterator cit = Indexes.find(i);
			if (cit != Indexes.end())
				return cit->second;
			else
				return 0;
		}

		static void addIdx(int i, ed::map<int, int>& Indexes)
		{
			Indexes[i] = Indexes.size();
		}
	};

	class WRADIO_API SysNet : public Msg
	{
	public:
		SysNet();
		static int getIdx(int i) {return Msg::getIdx(i, SysNet::Indexes);}
		static int getLastIdx() {return SysNet::Indexes.size();}
	private:
		static ed::map<int, int> Indexes;
	};

	class WRADIO_API J2_0 : public Msg
	{
	public:
		J2_0();
		static int getIdx(int i) {return Msg::getIdx(i, J2_0::Indexes);}
		static int getLastIdx() {return J2_0::Indexes.size();}
	private:
		static ed::map<int, int> Indexes;
	};

	class WRADIO_API J2_2 : public Msg
	{
	public:
		J2_2();
		static int getIdx(int i) {return Msg::getIdx(i, J2_2::Indexes);}
		static int getLastIdx() {return J2_2::Indexes.size();}
	private:
		static ed::map<int, int> Indexes;
	};

	class WRADIO_API J2_3 : public Msg
	{
	public:
		J2_3();
		static int getIdx(int i) {return Msg::getIdx(i, J2_3::Indexes);}
		static int getLastIdx() {return J2_3::Indexes.size();}
	private:
		static ed::map<int, int> Indexes;
	};

	class WRADIO_API J2_5 : public Msg
	{
	public:
		J2_5();
		static int getIdx(int i) {return Msg::getIdx(i, J2_5::Indexes);}
		static int getLastIdx() {return J2_5::Indexes.size();}
	private:
		static ed::map<int, int> Indexes;
	};

	class WRADIO_API J2_6 : public Msg
	{
	public:
		J2_6();
		static int getIdx(int i) {return Msg::getIdx(i, J2_6::Indexes);}
		static int getLastIdx() {return J2_6::Indexes.size();}
	private:
		static ed::map<int, int> Indexes;
	};

	class WRADIO_API J3_1 : public Msg
	{
	public:
		J3_1();
		static int getIdx(int i) {return Msg::getIdx(i, J3_1::Indexes);}
		static int getLastIdx() {return J3_1::Indexes.size();}
	private:
		static ed::map<int, int> Indexes;
	};

	class WRADIO_API J3_2 : public Msg
	{
	public:
		J3_2();
		static int getIdx(int i) {return Msg::getIdx(i, J3_2::Indexes);}
		static int getLastIdx() {return J3_2::Indexes.size();}
	private:
		static ed::map<int, int> Indexes;
	};

	class WRADIO_API J3_3 : public Msg
	{
	public:
		J3_3();
		static int getIdx(int i) {return Msg::getIdx(i, J3_3::Indexes);}
		static int getLastIdx() {return J3_3::Indexes.size();}
	private:
		static ed::map<int, int> Indexes;
	};

	class WRADIO_API J3_5 : public Msg
	{
	public:
		J3_5();
		static int getIdx(int i) {return Msg::getIdx(i, J3_5::Indexes);}
		static int getLastIdx() {return J3_5::Indexes.size();}
	private:
		static ed::map<int, int> Indexes;
	};

	class WRADIO_API K05_01 : public Msg
	{
	public:
		K05_01();
		static int getIdx(int i) {return Msg::getIdx(i, K05_01::Indexes);}
		static int getLastIdx() {return K05_01::Indexes.size();}
	private:
		static ed::map<int, int> Indexes;
	};

	class WRADIO_API K05_19 : public Msg
	{
	public:
		K05_19();
		static int getIdx(int i) {return Msg::getIdx(i, K05_19::Indexes);}
		static int getLastIdx() {return K05_19::Indexes.size();}
	private:
		static ed::map<int, int> Indexes;
	};

	class WRADIO_API J12_0 : public Msg
	{
	public:
		J12_0();
		static int getIdx(int i) {return Msg::getIdx(i, J12_0::Indexes);}
		static int getLastIdx() {return J12_0::Indexes.size();}
	private:
		static ed::map<int, int> Indexes;
	};

	class WRADIO_API J12_6 : public Msg
	{
	public:
		J12_6();
		static int getIdx(int i) {return Msg::getIdx(i, J12_6::Indexes);}
		static int getLastIdx() {return J12_6::Indexes.size();}
	private:
		static ed::map<int, int> Indexes;
	};

	class WRADIO_API J13_2 : public Msg
	{
	public:
		J13_2();
		static int getIdx(int i) {return Msg::getIdx(i, J13_2::Indexes);}
		static int getLastIdx() {return J13_2::Indexes.size();}
	private:
		static ed::map<int, int> Indexes;
	};

	class WRADIO_API J28_2 : public Msg
	{
	public:
		J28_2();
		static int getIdx(int i) {return Msg::getIdx(i, J28_2::Indexes);}
		static int getLastIdx() {return J28_2::Indexes.size();}
	private:
		static ed::map<int, int> Indexes;
	};

	static const ed::string Delimiter;

private:
	static const int SGrowSize;

	static ed::vector<ed::string> ParsedMessage;
};


class WRADIO_API SADL_msgsPacker
{
public:

	/*
	--------------------------------------------------------
	Convention:
	 alt - feet
	 lat, lon - MapCoordsSpace::MapCoords::Latitude()/Longitude()
	 head - degrees
	 gnd_spd - knots

	--------------------------------------------------------  
	*/


	// send air PPLI @ J2.2 SADL message format
	static int packAirPPLI(const ed::string& OwnTN,
		                    int platform,
	                        double alt,
	                        double lat,
				            double lon,
				            double head,
				            double gnd_spd,
				            const ed::string& callsign,
	                        ed::string& msg);

	// send ground PPLI @ K05.1 SADL message format
	static int packGroundPPLI(const ed::string& urn,
		                        int platform,
		                        double alt,
		                        double lat,
		                        double lon,
		                        double head,
		                        double gnd_spd,
		                        ed::string& msg);

	// send hostile surface track @ J3.3 SADL message format
	static int packSurfaceTrack(const ed::string& ReporterTN,
                                const ed::string& TargetTN, 
                                int platform,
                                int stype, // platform specific type
                                int ident, // identification: hostile, friendly
                                double Alt, 
                                double Lat, 
                                double Long, 
                                double Crs, 
                                double GndSpeed,
                                ed::string& msg);

	// send land track @ J3.5 SADL message format
	static int packLandTrack(const ed::string& ReporterTN,
		                     const ed::string& TargetTN, 
		                     int platform,
		                     int stype, // platform specific type
		                     int ident, // identification: hostile, friendly
		                     double Alt, 
		                     double Lat, 
		                     double Long, 
		                     double Crs, 
		                     double GndSpeed,
		                     ed::string& msg);


    // send air track @ J3.2 SADL message format
    static int packAirTrack(const ed::string& ReporterTN,
                            const ed::string& TargetTN, 
                            int platform,
                            int stype, // platform specific type
                            int ident, // identification: hostile, friendly
                            double Alt, 
                            double Lat, 
                            double Long, 
                            double Crs, 
                            double GndSpeed,
                            ed::string& msg);

	// send platform and sys status @ J13.2 SADL message format
	static int packPlatformAndSysStat(const ed::string& ownTN,
									  AirPlatformStatus& AirPlatformStatus,
		                              ed::string& msg);

	// J12.6
	static int packMiniSPI(const ed::string& ownTN,
		int sid,
		int enviroment,
		int local_idx,
		int ident,
		double lat,
		double lon,
		double alt,
		ed::string& msg);

	static int SADL_msgsPacker::pack9Liner(const ed::string& ownTN, // own TN
		const ed::string& destTN,  // detination TN
		const ed::string& ip_name, // Initial Point (IP) name
		double hdg,                 // heading from IP to TGT (degrees)    
		double dist,                // distance from IP to TGT (NM) 
		double elev,                // elevation of TGT (feet)
		const ed::string& desc,    // TGT description
		double lat,                 // latitude of TGT
		double lon,                 // longitude of TGT
		const ed::string& frnd,    // name of the closest to TGT friendly unit
		double egrs,                // heading after attack (degrees)
		const ed::string& mark_type,   // mark type
		ed::string& msg);

	// J12.0
	static int SADL_msgsPacker::packMissionAssignment(const ed::string& ownTN,
		const ed::string& destTN,
		double Lat,
		double Lon,
		double Alt,
		double course,
		double runin_head,
		int stype,
		ed::string& msg);

	static int         getUniqueIntTN(MovingObject *obj);
	static ed::string getUniqueStrTN(MovingObject *obj);
	static void getStypeAndPlatform(const wsType& tp, int& platform, int& stype);

	static void LongToString(double Long, ed::string& str);
	static void LatToString(double Long, ed::string& str);

	static const int        MAX_A10_GROUP_NUMBER; // max group number for A10C
	static ed::vector<int> group_map;            // map to arrange TN for A10C units for SADL identification

};





