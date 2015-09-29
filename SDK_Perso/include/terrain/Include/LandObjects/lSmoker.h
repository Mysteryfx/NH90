#pragma once

#include "Terrain.h"
#include "lStandartStructure.h"
#include "Utility\DistanceKiller.h"
#include "graphicEffect.h"

#define IID_lSmoker Common::identify<lSmoker>()
#define IID_lBoilerHouse Common::identify<lBoilerHouse>()

// базовый класс для объекта с дымами
class TERRAIN_API lSmoker : public lStandartStructure, public DistanceKiller
{
protected:
	typedef ed::vector< gePointer > smokeList;
	smokeList smokes;

	virtual void AddSmoke(float power, const Vector3& local_pos);

	void CreateSmokesBase();
	virtual void DestroySmokes();

	// по умолчанию - воспринимает коннекторы с именем "smoke"
	virtual void CreateSmokes();

public:
	static lLandObject* CreateObject();

	lSmoker();

	// Для присоединенных дымов. Создать такие объекты
	virtual void InitConnectedObjects();

	// парсинг
	// create smokes on demand
	virtual void Parse(Graphics::ModelParser& parser, const Position3& pos, Graphics::effectState* effects=0);


	// восстановить исходное состояние (если создавалась dyndata)
	virtual void ReinitState();

	// если объект получил достаточно повреждений, чтобы деградировать
	virtual void Degrade(bool silent);

	// получить координаты объекта
	virtual const Vector3& GetKillerPos();

	// при покидании зоны видимости
	virtual void OnKillByDistance();
	
	// блоки отрисовки
//	bool AllowDrawBlocks();
};


// Котельная
class TERRAIN_API lBoilerHouse : public lSmoker
{
protected:
	virtual void CreateSmokes();
public:
	static lLandObject* CreateObject();
};
