#pragma once

#include "Terrain.h"
#include "Common/Link.h"
#include "LandObjects\lLandObject.h"
#include "LandObjects\lObjectDynData.h"
#include "lStandartStructure.h"
#include "Renderer\RenderObject.h"
#include "osg/Vec3d"

// объекты электросети
#define IID_lETL_Cables Common::identify<lETL_Cables>()
#define IID_lCableDynData Common::identify<lCableDynData>()
#define IID_lETL_Substation Common::identify<lETL_Substation>()
#define IID_lETL_Farm Common::identify<lETL_Farm>()

class lETL;
class viObjectStorage;
// Загрузка ЛЭП, венет число созданых объектов
TERRAIN_API int LoadEtlScene(ed::vector<osg::Vec3d>& singleline, viObjectStorage* storage=0);

// провода
class TERRAIN_API lETL_Cables : public lLandObject, public LinkHost
{
private:
	// константы проводов
	static Graphics::Material*	material;	// материал
	static const int			segment_count; // количество сегментов в проводе
	static const float			lowering_factor; // константа величины провисания

    ObjectID linked[2];

	// мой графический объект
	Graphics::RenderObject robj;
	Position3 rpos;
	gBox box;

protected:
	// фабрика по созданию dyndata
	virtual void CreateDynamicData();

	// создать материал для проводов
	static void CreateMaterial();

	// создать индивидуальный провод (вершины добавятся в список)
	virtual void CreateWire(ed::vector<Vector3> &v, const Vector3 &p1, const Vector3 &p2);

	// просчитать провисание провода по параметру
	virtual float WireLowering(float length, float par);

	// получить высоту провода cable в параметре par
	virtual float WireHeight(int cable, float par);

public:
	static lLandObject* CreateObject();

	lETL_Cables();
	~lETL_Cables();
	
	// создать фигуру (для столкновений и отрисовки)
	virtual void CreateGShape();

	// создание flatshape
	virtual void ConstructFlatShape();

	// возвращает количество проводов
	virtual int CableCount();

	// возвращает точку крепления провода в ЛСК
	virtual void AttachPos(bool inbound, int i, Vector3 &p);

	// оборвать один провод
	virtual void CutCable(int cable);

	// существует ли провод
	virtual bool CableExists(int cable);

	// проверить, не пересекается ли провод с поверхностью земли
	bool CheckGroundIntersection();

	// проверка на пересечение с проводами
	bool CheckCableShapeIntersection(int cable, Graphics::ModelInstance &model, wsCollisionData& cdata);

	// проверка на пересечение с gShapeObject, cut_line - обрывать ли пересеченные провода
	// возвращает количество пересеченных проводов
	int CheckShapeIntersection(Graphics::ModelInstance &model, bool cut_line);

	// восстановить исходное состояние (если создавалась dyndata)
	virtual void ReinitState();

	// отрисовка
	virtual const gBox &LocalBox(wModelTime t);

	// парсинг
	virtual void Parse(Graphics::ModelParser& parser, const Position3& pos, Graphics::effectState* effects=0);

	// сериализация
	void serialize(Serializer &);
	
	// блоки отрисовки
//	bool AllowDrawBlocks();

    // связанные наземные объекты
    // [0,1] - опоры на концах
    virtual int getLinkedObjects(ObjectID *buff);
    virtual void setLinkedObjects(ObjectID *buff, int count);
};

// динамическая информация о обрыве проводов
class TERRAIN_API lCableDynData : public lObjectDynData
{
private:
	// маска оборванных проводов
	unsigned cablemask;

public:
	lCableDynData();

	void CutCable(int cable);
	virtual bool CableExists(int cable);

	void serialize(Serializer &);
};

// базовый класс для объекта - крепления проводов (подстанции и мачты)
class TERRAIN_API lETL : public lStandartStructure
{
protected:
	// входящие и исходящие провода
    ObjectID linked[2];

public:
	lETL();
	~lETL();

	// возвращает количество проводов
	virtual int CableCount() = 0;

	// возвращает точку крепления провода в ЛСК
	virtual void AttachPos(bool inbound, int i, cPoint &p) = 0;

	// Анимация уничтожения
	virtual bool FallDown();

	void serialize(Serializer &);

    // связанные наземные объекты
    // [0,1] - входящие и исходящие провода
    virtual int getLinkedObjects(ObjectID *buff);
    virtual void setLinkedObjects(ObjectID *buff, int count);
};

// подстанция
class TERRAIN_API lETL_Substation : public lETL
{
public:
	static lLandObject* CreateObject();

	// возвращает количество проводов
	virtual int CableCount();

	// возвращает точку крепления провода в ЛСК
	virtual void AttachPos(bool inbound, int i, cPoint &p);
};

// мачта
class TERRAIN_API lETL_Farm : public lETL
{
protected:
	// зажечь пожары после разрушения строения
	virtual void CreateFires();

public:
	static lLandObject* CreateObject();
	
	// возвращает количество проводов
	virtual int CableCount();

	// возвращает точку крепления провода в ЛСК
	virtual void AttachPos(bool inbound, int i, cPoint &p);
};
