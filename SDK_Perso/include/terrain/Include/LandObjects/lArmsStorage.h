#pragma once


#include "Randomizer\RandomObject.h"
#include "Terrain.h"
#include "LandObjects/lWarehouseOwner.h"

#define IID_lArmsStorage Common::identify<lArmsStorage>()
#define IID_lArmsStorageDynData Common::identify<lArmsStorageDynData>()

class TERRAIN_API lArmsStorageDynData : public lStandartDynData, public LinkHost
{
	bool started;
public:
	static RandomObject rnd;
    static RandomObject rnd_uniform;

	virtual void Fire();		// Зажечь еще один пожар
	virtual void Flare();		// Выпустить ловушку
	virtual void Shell();		// Запустить снаряд
	virtual void Explosion();	// Взрыв
	virtual void Dust();		// Пылевое облако

	virtual void OnDegrade();	// Начать феерверк

	lArmsStorageDynData();
};


// Склад боеприпасов
// Круто взрывается :)
class TERRAIN_API lArmsStorage : public lWarehouseOwner
{
public:
	static lLandObject* CreateObject();
protected:
	// фабрика по созданию dyndata
	virtual void CreateDynamicData();
	virtual void Degrade(bool silent);
	virtual unsigned	getStoreType() const {return 1;/*AS_WEAPON_STORE*/}

};
