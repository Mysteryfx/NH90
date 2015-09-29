#pragma once

#include "Terrain.h"
#include "woLAConst.h"
#include "LandObjects/lLandObject.h"

#define IID_lLandPlane Common::identify<lLandPlane>()

const int PlaneArguments = drawArgsMax; 
struct Aircraft_Descriptor;
// Самолетик, как статический объект
class TERRAIN_API lLandPlane : public lLandObject
{
private:
	
	// раздает и инициирует draw - аргументы
	class ArgumentController
	{
	private:
		Graphics::DParamList Def;
		Graphics::DParamList Heli;

		Graphics::DParamList SU27;
		Graphics::DParamList SU34;
		Graphics::DParamList SU24;
		Graphics::DParamList SU17;

		void CreateDef();
		void CreateHeli();
		void CreateSU27();
		void CreateSU34();
		void CreateSU24();
		void CreateSU17();

	public:
		ArgumentController();

		// выдать указатель на таблицу аргументов по имени shape
		Graphics::DParamList* GetArgs(unsigned char name);
	};

	static ArgumentController drawArgsController;
	int flightNumber;

	Graphics::DParamList		 localArgs;
	const Aircraft_Descriptor *info;

public:
	static lLandObject* CreateObject();
	lLandPlane();

    virtual void Init(wShape *_type, const cPosition& _pos, viFlag status = viUnknown, void* context=NULL,double _scale = 1.0);

	// вызывается для повреждений (должна создать динамическую информацию)
	virtual void TakeDamage(double amount, ObjectID hitman);

	virtual void Degrade(bool silent);

	virtual const Graphics::DParamList& GetDrawArguments();

	// сериализация
	void serialize(Serializer &);

	virtual void setStaticData(const StaticObjectData & staticDataIn);
};
