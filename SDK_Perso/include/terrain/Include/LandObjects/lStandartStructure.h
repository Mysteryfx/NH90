#pragma once

#include "Terrain.h"
#include "LandObjects\lObjectDynData.h"
#include "Randomizer\RandomObject.h"
#include "Modeler\AnimHolder.h"
#include "LandObjects\lLandObject.h"

#define IID_lStandartStructure Common::identify<lStandartStructure>()
#define IID_lStandartDynData Common::identify<lStandartDynData>()

class lDamageStructActivity;
class lObjectFire;
class lStandartStructure;

//Решаем проблему с зависимости при компиляции с сохранением при этом LinkPtr
class FireHolders;

struct TERRAIN_API lStandartDynData : public lObjectDynData
{
	// Падающий объект
	lStandartStructure* fallobject;

	// Класть на бок
	bool bIncline;
	// Вокруг какой оси крутить
	cVector inclineVec;

	// Исходная высота
	float source_heigth;
	// Высота здания
	float max_heigth;

	// анимированные аргументы
	wModelTime fallDownT, argsT;
	Graphics::AnimHolder args;

	//Борьба с лишними инклудами, а то ParticleApi везде попадает
	FireHolders*  fires;

	lStandartDynData();
	~lStandartDynData();
};


// класс, реализующий lLandObject для стандартных объектов наземной застройки (домов)
class TERRAIN_API lStandartStructure : public lLandObject
{
friend class lDamageStructActivity;
protected:
	static wModelTime argsTime;
	static Graphics::DParamList globalArgs;

	static void RecalculateArgs(wModelTime t);

protected:
	// фабрика по созданию dyndata
	virtual void CreateDynamicData();

	// зажечь пожары после разрушения строения
	virtual void CreateFires();

	void AddFire(float		  power,
		 		 const cVector & pnt,
				 bool		     pnt_local = true,
				 const char    * custom_effect_name = nullptr,
				 float		     custom_effect_scale = -1);

	// пылевое облако на месте падения дома
	virtual void CreateSmokeCloud();

public:
	static RandomObject randomEvenly;
	static lLandObject* CreateObject();
	lStandartStructure();
	~lStandartStructure();

	// argument animation
	void SetDrawArg(int arg, double value);
	void PlayArg(int arg, double start, double stop, double speed);

	virtual cPosition &Position(wModelTime t);
	virtual gShapeObject *Shape(wModelTime t);
	
	virtual const Position3& GetPosition();
	virtual Graphics::Model *GetModel();

	//virtual viDrawArguments DrawArguments(wModelTime t, viArgumentPurpose purpose);
	virtual const Graphics::DParamList& GetDrawArguments();

	virtual void Degrade(bool silent);

	virtual void PostCollisionProcessing(wsCollisionData *info, ObjectID obj);

	// Анимация уничтожения
	virtual bool FallDown();
	bool IsFallInProgress();

	// получить значение аргумента рисования
	static float DrawArgValue();
	
	// блоки отрисовки
//	bool AllowDrawBlocks();
};

