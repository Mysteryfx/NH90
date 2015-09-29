#pragma once
#include "edTerrain.h"
#include "landscape3/lPathFile.h"
//#include "ISceneSource.h"
#include "lPointOnSurface.h"

namespace NM_ROAD
{
	struct lRoadFile;
	struct Road;
	struct Crossroad;

	// Точка на дороге c вычислением высоты
	class EDTERRAIN_API PointOnRoadH : public PointOnRoad
	{
		mutable landscape::lPointOnSurface pons;
	public:
		PointOnRoadH();
		PointOnRoadH(const PointOnRoad& por);
		PointOnRoadH(const PointOnRoad& por, float offset);
		PointOnRoadH(const PointOnRoadH& por);
		PointOnRoadH(const PointOnRoadH& por, float offset);
		// Высота (с учетом мостов)
		float getHeigth() const;
		// Нормаль (с учетом мостов)
		void getNormal(cVector& normal) const;
	};

}

namespace path
{
	// Точка на дороге
	class Path;
	class EDTERRAIN_API PointOnPath : public NM_ROAD::PointOnRoadH
	{
	public:
		Path* path;				// Дорога
		PointOnPath();
	};

	// Дорога
	class EDTERRAIN_API Path
	{
	// Данные
	public:
		virtual NM_ROAD::Road* GetRoad()=0;

		// Начало/конец
		PointOnPath GetStart() const;
		PointOnPath GetEnd() const;
		// Сдвинуть точку по дороге
		void MovePoint(PointOnPath& point, float distance) const;

		// Длина дороги
		float GetRoadLength() const;
		// Число точек стыковки сегментов
		int GetPointCount() const;
		// Точки стыковки сегментов
		const cVector& GetPoint(int i) const;

		// цвет (для отладки)
		virtual unsigned int getColor()=0;
		/*/
	// Операции
	public:
		// Построить эквидистанту к дороге (исходная дорога не изменяется)
		Path* Offset(
			PointOnPath& start_pt,
			PointOnPath& end_pt,
			float distance) const; // эквидистантное смещения (вправо от дороги)

		// получить проекцию точки на дорогу.
		bool GetProjection(
			const cVector &p, 
			PointOnPath& pop);
		/*/
		virtual ~Path(){};
	};
	// Пересечение дорог
	class EDTERRAIN_API Cross
	{
	public:
		virtual NM_ROAD::Crossroad* GetCross()=0;

		virtual const cVector& getPosition() const = 0;
		virtual ~Cross(){};
	};

	// Перекресток на аэродроме
	class RunWayCrossCross : public Cross
	{
	public:
	};

	// Дорожная сеть
	class EDTERRAIN_API PathNetwork // : public ISceneSource
	{
	public:
		// Открыть файл
		virtual NM_ROAD::lRoadFile* Open(const char* filename)=0;
		virtual bool Open(NM_ROAD::lRoadFile* plRoadFile)=0;
		virtual void Close()=0;

		// Дороги
		virtual int getRoadCount() const=0;
		virtual NM_ROAD::Road* getRoad(int index)=0;
		virtual int getRoadIndex(NM_ROAD::Road*) const=0;

		// Перекрестки
		virtual int getCrossCount()=0;
		virtual NM_ROAD::Crossroad* getCross(int i)=0;
		virtual int getCrossIndex(NM_ROAD::Crossroad*) const=0;

		// Объекты
		virtual int getObjectCount()=0;
		virtual NM_ROAD::SceneObject* getObject(int i)=0;
		virtual int getObjectIndex(NM_ROAD::SceneObject*) const=0;

		// Выбрать дороги проходящие в квадрате
		virtual void getRoadsBySquare(
			intpoint sqindex,			// square index
			ed::vector<int>& roads,	// OUT: roads indicies in square
			ed::vector<int>& crosses	// OUT: crossroads indicies in square
			)=0;

		// Точка на дороге?
		virtual bool IsPointOnRoad(const cVector& pt)=0;
		// Получить ближайшую к точке дорогу 
		virtual Path* GetClosestRoad(
			const cVector& pt, 
			PointOnPath& point, 
			bool bExcludeBridgeAndTunnels=false)=0;	// Не возвращать мосты и туннели
		// Получить ближайшую к точке дорогу 
		virtual Path* GetClosestRoad(
			const cVector& pt, 
			PointOnPath& point, 
			NM_ROAD::ICheckRoadSink* checkproc)=0;	// Не возвращать мосты и туннели
		virtual Path* GetClosestRoad(
			const osg::Vec3d& pt, 
			PointOnPath& point, 
			NM_ROAD::ICheckRoadSink* checkproc		// позволяет фильтровать поиск
			)=0;	// Не возвращать мосты и туннели
		virtual Path* GetClosestRoad(
			const osg::Vec3d& pt, 
			PointOnPath& point, 
			bool bExcludeBridgeAndTunnels=false)=0;	// Не возвращать мосты и туннели
		// ближайший к точке перекресток
		virtual NM_ROAD::Crossroad* GetClosestCross(
			const osg::Vec3d& pt,
			NM_ROAD::ICheckRoadSink* checkproc=0	// позволяет фильтровать поиск
			)=0;

		// получить путь от точки до точки используя дорожную сеть
		// возвращает 0 если нельзя доехать, указатель на путь, если можно
		virtual NM_ROAD::Road* GetPath(
			const cVector& start,	// начальная точка пути
			const cVector& end,		// конечная точка пути
			float roffset,			// смещение от центра дороги
			// Выходные параметры
			PointOnPath& popStart,  // точка выезда на дорогу
			PointOnPath& popEnd,	// точка съезда с дороги
			NM_ROAD::ICheckRoadSink* checkproc=0	// позволяет фильтровать поиск
			)=0;

		// получить путь от точки до точки используя дорожную сеть
		// возвращает 0 если нельзя доехать, указатель на путь, если можно
		virtual NM_ROAD::Road* GetPath(
			const PointOnPath& popStart,  // начальная точка 
			const PointOnPath& popEnd,	// конечная точка 
			float roffset,
			NM_ROAD::ICheckRoadSink* checkproc=0	// позволяет фильтровать поиск
			)=0;	

		// получить путь от перекрестка до перекрестка используя дорожную сеть
		// возвращает 0 если нельзя доехать, указатель на путь, если можно
		virtual NM_ROAD::Road* GetPath(
			NM_ROAD::Crossroad* start,	// начальная перекресток
			NM_ROAD::Crossroad* end,	// начальная перекресток
			float roffset,			// смещение от центра дороги
			// Выходные параметры
			PointOnPath& popStart,  // точка выезда на дорогу
			PointOnPath& popEnd,		// точка съезда с дороги
			NM_ROAD::ICheckRoadSink* checkproc=0	// позволяет фильтровать поиск
			)=0;	

		/*/
		// получить путь из перекрестка crossroad_from в перекресток crossroad_to
		Path* GetPath(
			int crossroad_from, 
			int crossroad_to, 
			bool isBigAircraft);

		// создать соединительный путь между двумя точками, с учетом направления движения
		// с учетом радиуса разворота группы
		Path* CreatePathConnection(
			const cVector &A, //исходная точка вне дороги
			const cVector &Av, //направление выезда из исходной точки вне дороги (нормированное)
			const cVector &B, //точка выезда на дорогу
			const cVector &Bv, //направление в точке выезда на дорогу (нормированное)
			float turnR, float maxR); // минимальный и максимальный радиус разворота

		Path* GetPath(
			cVector *ps, // начальная точка пути
			cVector *pe, // конечная точка пути
			bool computeStayPoints); // рассчитать точки остановок

		// создать прямую односегментную дорогу
		Path* StraightPath(const cPoint &A, const cPoint &B);
		/*/

		// Удалить временную дорогу
		virtual void DeletePath(NM_ROAD::Road* path)=0;

		// чтоб удалялось
		virtual ~PathNetwork(){};
	};

	EDTERRAIN_API PathNetwork* CreatePathNetwork();

};
