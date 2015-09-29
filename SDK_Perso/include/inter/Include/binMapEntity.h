#ifndef _BIN_MAP_ENTITY_H_
#define _BIN_MAP_ENTITY_H_

#include <ed/map.h>
#include <ed/string.h>
#include <ed/list.h>

#define MAX_ENTITY 30

struct binMapEntity
{
	binMapEntity() : iType(0)
	{};
	
	int iType;	//type для cigi протокола
};

struct binEntityCigi
{
	binEntityCigi():iType(0)
		, iParentId(65535)
		, iId(0)
		, iCount(0)
		, dLat(0.0)
		, dLon(0.0)
		, dAlt(0.0)
		, dYaw(0.0)
		, dPitch(0.0)
		, dRoll(0.0)
		, bUpdate(false)
		, bExplode(false)
		, iDelay(180)
		, fAlfa(0.0f)
	{};

	int iType;
	int iParentId;
	int iId;
	int iCount;
	int iDelay;	//задержка перед удалением

	double dLat;
	double dLon;
	double dAlt;
	double dYaw;
	double dPitch;
	double dRoll;

	float fAlfa;	//альфу используем для передачи потока воздуха винта

	bool bUpdate;
	bool bExplode;	//признак взрыва
};

//ключом является имя объекта
typedef ed::map<ed::string, binMapEntity> tMapEntity;

//ключом является id из модели, значанием данные для отправки в протокол
typedef ed::map<unsigned int, binEntityCigi> tMapCigi;

//список объектов для отправки (нужен для отправки эффектов)
typedef ed::list<binEntityCigi> tListCigi;

#endif	//_BIN_MAP_ENTITY_H_
