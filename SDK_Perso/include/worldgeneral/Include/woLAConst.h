#pragma once

#include <stdint.h>

//Режимы работы радаров
const unsigned __int32 RadarModeScan	= 1<<0;	//Поиск
const unsigned __int32 RadarModeTWS		= 1<<1;	//Сопровождение во время поиска
const unsigned __int32 RadarModeLock	= 1<<2;	//Захват и наведение
//Типы радаров по дальности
const unsigned __int32 RadarTypeEWR		= 1<<3;
const unsigned __int32 RadarTypeLongRange = 1<<4;
const unsigned __int32 RadarTypeMiddleRange = 1<<5;
const unsigned __int32 RadarTypeShortRange = 1<<6;
const unsigned __int32 RadarTypeAirBorn = 1<<7;

// кол-во ячеек(частей) самолета (для обработки столкновения)
const int NumCells   = 200;
// макс. количество дымов на самолете
const int MaxNumTail = 5;

const float GunFireRate = 0.04f; // сек. темп огня из пушки - 25 выстрелов/сек. 

const unsigned SmokeArraySize = 11; // размер массива дымов на самолете

const unsigned short int drawArgsMax = 1600; // количество аргументов рисования самолетов и вертолетов

union gun_index_t
{
	gun_index_t(char station, char gun)
		{ index_st_.gun_ = gun; index_st_.station_ = station; }

	struct 
	{
		char gun_;
		char station_;
	} index_st_;

	short index_;
};


struct gun_index_cmp_t {
	bool operator() (const gun_index_t& lhs, const gun_index_t& rhs) const
	{
		return lhs.index_ < rhs.index_;
	}
};

typedef std::map<gun_index_t,float,gun_index_cmp_t> GunsLoadFactors;

enum PlaneCommonFailures
{ 
	FlagFailureACS,
	FlagFailureAutopilot,
	FlagFailureRadar,
	FlagFailureEOS,
	FlagFailureMLWS,
	FlagFailureRWS,
	FlagFailureHydraulics,
	FlagFailureLeftEngine,
	FlagFailureRightEngine,
	FlagFailureHelmet,
	FlagFailureHUD,
	FlagFailureMFD,
	FlagFailureECM,
	FlagFailureGear,
	FlagTankDamage,
	PlaneCommonFailuresCount
};
