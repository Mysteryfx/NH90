#ifndef __wsThreat_h__
#define __wsThreat_h__

//List of ground threats.

#include "WorldGeneral.h"
#include "wMovingObject.h"
#include <ed/map.h>

namespace AI
{

namespace Threat
{

//struct Desc
//{
//	enum Type
//	{
//		TypeInfantry = 1 << 0,
//		TypeVehicles = 1 << 1,
//		TypeAAA = 1 << 2,
//		TypeSAM = 1 << 3
//	} type;
//	enum Guidance
//	{
//		GuidanceIR = 1 << 0,
//		GuidanceRadar = 1 << 1
//	};
//	enum Range
//	{
//		RangeShort = 1 << 0,
//		RangeMedium = 1 << 1,
//		RangeLong = 1 << 2
//	};
//	bool mobile;
//};

//Threat types and properties

enum TypeFlag
{
	Nothing				= 0,
	Infantry			= 1 << 0,
	AntiAirArmedVehicles= 1 << 1,
	StaticAAA			= 1 << 2,
	MobileAAA			= 1 << 3,
	MANPAD				= 1 << 4,

	ShortRangeSAM		= 1 << 5,
	MediumRangeSAM		= 1 << 6,
	LongRangeSAM		= 1 << 7,

	IRGuidedSAM			= 1 << 8,
	RadarGuidedSAM		= 1 << 9,
	ATGMLauncher		= 1 << 10,
	//
	LightGuns		= Infantry | AntiAirArmedVehicles,
	AAA				= StaticAAA | MobileAAA,
	AntiAirCannons	= LightGuns | AAA,
	SAMvehicles		= ShortRangeSAM | MediumRangeSAM | LongRangeSAM,
	SAM				= MANPAD | SAMvehicles,
	AllThreats		= LightGuns | AAA | SAM
};

//A ground threat

class WORLDGENERAL_API AirdefenceData
{
public:
	AirdefenceData();	
	bool			isPointInWEZ(const cPoint & point, float distMin = 0.0f, float distReserve = 0.0f, cVector * pDir = NULL) const;
	bool			isPointInWEZ(const E2::Vector & point, float distMin = 0.0f, float distReserve = 0.0f, E2::Vector * pDir = NULL) const;
	bool			isLineInWEZ(const cPoint & startPoint, const cPoint & endPoint, float distMin = 0.0f, float distReserve = 0.0f, float * in = NULL, float * out = NULL) const;
	bool			isLineInWEZ(const E2::Vector & startPoint, const E2::Vector & endPoint, float distMin = 0.0f, float distReserve = 0.0f, float * in = NULL, float * out = NULL) const;
	inline float	getKillMaxHeight() const { return ptr->Position().p.y + killMaxHeight; }
	inline float	getKillMinHeight(const cPoint & point) const;
public:
	static bool		checkAndBuild(woPointer ptr_, unsigned int typeFlags_ = AllThreats, AirdefenceData * pAirdefenceData = NULL);
public:
	woPointer		ptr;
#ifdef DEBUG_TEXT
	ed::string		objTypeName;
#endif
	unsigned int	typeFlags;
	float			killDist;
	float			killMaxHeight;
};

//List of ground threats

class WORLDGENERAL_API AirdefenceUnitsData
{
public:
	bool			checkAndAdd(wControl * pControl, unsigned int typeFlags_ = AllThreats);
	bool			checkAndAdd(woPointer unit_ptr, unsigned int typeFlags_ = AllThreats);
	void			onUnitDeath(ObjectID ID);
	void			clearDeads();
	bool			empty() const { return airdefenceUnitsMap.empty(); }
	
	//Query as a predicate: uses predicate handler
	template<class Handler>
	inline bool	find(ObjectID threatID, Handler handler) const
	{
		AirdefenceUnitsMap::const_iterator itAUM = airdefenceUnitsMap.find(threatID);
		if(itAUM != airdefenceUnitsMap.end())
			return handler(itAUM->second);
		else
			return false;
	}

	//Query as a counter: uses predicate handler
	template<class Handler>
	inline unsigned int	find(Handler handler) const
	{
		struct HandlePair
		{
			HandlePair(Handler & handler) : handler_(handler) {;}
			inline bool operator()(const std::pair<ObjectID const, AirdefenceData> & pair) const { return handler_(pair.second); }
		private:
			Handler & handler_;
		};
		return std::count_if(airdefenceUnitsMap.begin(), airdefenceUnitsMap.end(), HandlePair(handler));
	}
private:
	typedef ed::map<ObjectID, AirdefenceData> AirdefenceUnitsMap;	
	AirdefenceUnitsMap airdefenceUnitsMap;
};

//Query functors

//Predicate functor. Returns false.
struct HandlerVoid
{
	inline bool operator()(const AirdefenceData & airdefenceData) const
	{
		return false;
	}
};


//Predicate functor. Checks of the 3D-point inside WEZ of the given threat
struct PointChecker
{
	PointChecker(const cPoint & point_, float distMin_ = 0.0f, float distReserve_ = 0.0f) : point(point_), distMin(distMin_), distReserve(distReserve_)
	{
	}
	inline bool operator()(const AirdefenceData & airdefenceData) const
	{
		return airdefenceData.isPointInWEZ(point, distMin, distReserve);
	}
private:
	cPoint point;
	float distMin;
	float distReserve;
};

//Predicate functor. Checks of the 2D-point inside WEZ of the given threat
struct PointChecker2D
{
	PointChecker2D(const E2::Vector & point_, float distMin_ = 0.0f, float distReserve_ = 0.0f) : point(point_), distMin(distMin_), distReserve(distReserve_)
	{
	}
	inline bool operator()(const AirdefenceData & airdefenceData) const
	{
		return airdefenceData.isPointInWEZ(point, distMin, distReserve);
	}
private:
	E2::Vector point;
	float distMin;
	float distReserve;
};

//Predicate functor. Checks of the line intersects WEZ of the given threat
struct LineChecker
{
	LineChecker(const cPoint & startPoint_, const cPoint & endPoint_, float distMin_ = 0.0f, float distReserve_ = 0.0f) : startPoint(startPoint_), endPoint(endPoint_), distMin(distMin_), distReserve(distReserve_)
	{
	}
	inline bool operator()(const AirdefenceData & airdefenceData) const
	{
		return airdefenceData.isLineInWEZ(startPoint, endPoint, distMin, distReserve);
	}
private:
	cPoint startPoint;
	cPoint endPoint;
	float distMin;
	float distReserve;
};

//Predicate functor. Checks if the threat is a threat of the given type(s)
struct CheckTypeFlag
{
	CheckTypeFlag(unsigned int flags) : flags_(flags) {;}
	inline bool operator()(const AirdefenceData & airdefenceData) const
	{
		return flags_ & airdefenceData.typeFlags;
	}
private:
	unsigned int flags_;
};

//Action functor. Collects threat types
struct CollectTypeFlags
{
	CollectTypeFlags(unsigned int & val) : val_(val) { val_ = 0; }
	inline void operator()(const AirdefenceData & airdefenceData)
	{
		val_ |= airdefenceData.typeFlags;
	}
	unsigned int & val_;
};

//Action functor. Collects threats
struct Collect
{
	Collect(const AirdefenceData ** begin, const AirdefenceData ** end) : curr_(begin), end_(end) {;}
	template<size_t size>
	Collect(const AirdefenceData *(&buff)[size]) : curr_(buff), end_(buff + size) {;}
	inline void operator()(const AirdefenceData & airdefenceData)
	{
		if(curr_ != end_)
			(*curr_++) = &airdefenceData;
	}
private:
	const AirdefenceData ** curr_;
	const AirdefenceData ** end_;
};

//Combines predicate functor and action functor in if-then operation
template<class Checker, class Handler>
struct IfThen
{
	IfThen(const Checker & checker, const Handler & handler)  : checker_(checker), handler_(handler) {;}
	inline bool operator()(const AirdefenceData & airdefenceData)
	{
		if(checker_(airdefenceData))
		{
			handler_(airdefenceData);
			return true;
		}
		else
			return false;
	}
private:
	Checker checker_;
	Handler handler_;
};

template<class Checker, class Handler>
inline IfThen<Checker, Handler> makeIfThen(const Checker & checker, const Handler & handler)
{
	return IfThen<Checker, Handler>(checker, handler);
}

//Combines two predicate functors in one with "OR"
template<class Handler1, class Handler2>
struct ComboOr
{
	ComboOr(const Handler1 & handler1, const Handler2 & handler2) : handler1_(handler1), handler2_(handler2) {;}
	inline bool operator()(const AirdefenceData & airdefenceData)
	{
		return	handler1_(airdefenceData) || handler2_(airdefenceData);
	}
private:
	Handler1 handler1_;
	Handler2 handler2_;
};

template<class Handler1, class Handler2>
inline ComboOr<Handler1, Handler2> makeComboOr(Handler1 & handler1, Handler2 & handler2)
{
	return ComboOr<Handler1, Handler2>(handler1, handler2);
}

//Combines two predicate functors in one with "AND"
template<class Handler1, class Handler2>
struct ComboAnd 
{
	ComboAnd(const Handler1 & handler1, const Handler2 & handler2) : handler1_(handler1), handler2_(handler2) {;}
	inline bool operator()(const AirdefenceData & airdefenceData)
	{
		return handler1_(airdefenceData) && handler2_(airdefenceData);
	}
private:
	Handler1 handler1_;
	Handler2 handler2_;
};
template<class Handler1, class Handler2>
inline ComboAnd<Handler1, Handler2> makeComboAnd(const Handler1 & handler1, const Handler2 & handler2)
{
	return ComboAnd<Handler1, Handler2>(handler1, handler2);
}

//Estimates risk due the threat
template<class RiskEstimator>
struct RiskEstimationAccumulator
{
	RiskEstimationAccumulator(float & risk, const RiskEstimator & riskEstimator) : risk_(risk), riskEstimator_(riskEstimator) {}
	inline void operator()(const AirdefenceData & airdefenceData)
	{
		risk_ += riskEstimator_(airdefenceData);
	}
private:
	float & risk_;
	const RiskEstimator riskEstimator_;
};

template<class RiskEstimator>
inline RiskEstimationAccumulator<RiskEstimator> makeRiskEstimationAccumulator(float & risk, const RiskEstimator & riskEstimator)
{
	return RiskEstimationAccumulator<RiskEstimator>(risk, riskEstimator);
}

//Distributes threats by altitudes wit the given minAltDifference

struct Level
{
	float altitude;
	float risk;
};
typedef ed::list<Level> Levels;

struct WORLDGENERAL_API DistributeThreatsBase
{
	DistributeThreatsBase(float minAltDifference, Levels & levels) : minAltDifference_(minAltDifference), levels_(levels) {;}
	inline void operator()(float risk, const AirdefenceData & airdefenceData);
private:
	float minAltDifference_;
	Levels & levels_;
};

template<class RiskEstimator>
struct DistributeThreats : DistributeThreatsBase
{
	DistributeThreats(const RiskEstimator & riskEstimator, float minAltDifference, Levels & levels) : DistributeThreatsBase(minAltDifference, levels), riskEstimator_(riskEstimator) {;}
	inline void operator()(const AirdefenceData & airdefenceData)
	{
		float risk = riskEstimator_(airdefenceData);
		DistributeThreatsBase::operator ()(risk, airdefenceData);
	}
private:
	RiskEstimator riskEstimator_;
};

struct DefaultRiskEstimator
{
	DefaultRiskEstimator(float operationalVel) : operationalVel_(operationalVel) {;}
	float operator()(const AirdefenceData & airdefenceData) const
	{
		if((airdefenceData.typeFlags & AntiAirArmedVehicles) != 0)
		{
			return 0.05f;
		}
		if((airdefenceData.typeFlags & AAA) != 0)
		{
			return  0.2f;
		}
		if((airdefenceData.typeFlags & SAM) != 0)
		{
			return 1.0f;
		}
		return false;
	}
private:
	float operationalVel_;
};

template<class RiskEstimator>
inline DistributeThreats<RiskEstimator> makeDistributeThreats(const RiskEstimator & riskEstimator, float minAltDifference, Levels & levels)
{
	return DistributeThreats<RiskEstimator>(riskEstimator, minAltDifference, levels);
}

//Returns minimal level that safe enough. It assumed risk grows by altitude
inline float findMinSafeLevel(const Levels & levels, float maxRisk)
{
	struct RiskLowerThan
	{
		RiskLowerThan(float maxRisk) : maxRisk_(maxRisk) {;}
		inline bool operator()(const Level & level) const { return level.risk < maxRisk_; }
	private:
		float maxRisk_;
	};
	Levels::const_iterator itL = std::find_if(levels.begin(), levels.end(), RiskLowerThan(maxRisk));
	if(itL != levels.end())
		return itL->altitude;
	else
		return levels.empty() ? 0.0f : levels.back().altitude;
}


//Finds best level, levels must be compared with each another by CompareLevel
template<class CompareLevel>
inline float findBestLevel(const Levels & levels, const CompareLevel & compareLevel)
{
	Levels::const_iterator itL = std::max_element(levels.begin(), levels.end(), compareLevel);
	if(itL != levels.end())
		return itL->altitude;
	else
		return levels.empty() ? 0.0f : levels.back().altitude;
}

}

}

#endif
