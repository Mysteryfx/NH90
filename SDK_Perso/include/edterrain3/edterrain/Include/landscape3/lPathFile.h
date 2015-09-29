#ifndef __LANDSCAPE3_LPATHFILE_H
#define __LANDSCAPE3_LPATHFILE_H
#include "../landscape3_config.h"

#include "edTerrain.h"
#include "lTerraBasic.h"
#include "GraphMath/trigonometry.h"

#include "landscape3/lSuperficial.h"
#include "landscape3/lModel.h"
#include "misc/misc_bitarray.h"
#include "osg/Vec3d"

#include "offshore/scene.h"

namespace roads3
{
// флаги сегментов дорог
const unsigned int SEGMENT_INVERT_DIR     = 0x00000001;
const unsigned int SEGMENT_INVISIBLE      = 0x00000008;
const unsigned int SEGMENT_IS_BRIDGE      = 0x00000002;
const unsigned int SEGMENT_IN_TOWN        = 0x00000004;

const unsigned int SEGMENT_NONPHYSICAL    = 0x00000010;	// Сегмент НЕ на физ. дороге
const unsigned int SEGMENT_DIVERSION      = 0x00000020;	// Сегмент на Объездн. дороге

const unsigned int SEGMENT_MOUND          = 0x00000040;	// Сегмент-насыпь перед мостом

// флаги дорог
const unsigned int ROAD_GIRDER			 = 0x00000001; // используется только для железных дорог
const unsigned int ROAD_DEAD			 = 0x00000002; // используется только для аэродромов
const unsigned int ROAD_FOR_BIG_AIRPLANES = 0x00000004; // используется только для аэродромов
const unsigned int ROAD_VIRTUAL		 = 0x00000008; // ???
const unsigned int ROAD_RUNWAY			 = 0x00000010; // используется только для аэродромов
const unsigned int ROAD_SLOPE			 = 0x00000020;	// построение откосов для дорог (используется только в процессе экспорта)
const unsigned int ROAD_TAKEOFF			 = 0x00000040;	// ВПП (взлет)
const unsigned int ROAD_LANDING			 = 0x00000080;	// ВПП (посадка)
const unsigned int ROAD_ONEWAY			 = 0x00000100;	// Односторонняя

const unsigned int ROAD_TYPE_MASK		 = 0xFF000000; // используется только для авт.дорог
const unsigned int ROAD_TYPE_BIG		 = 0x01000000; // широкая дорога

// Флаги перекрестков
const int CROSSROAD_ON_RUNWAY    = 0x00000001;	// Флаг "Vpp"
const int CROSSROAD_SHELTER      = 0x00000002;	// Флаг "shelter"
const int CROSSROAD_FOR_BIG_AIR  = 0x00000008;	// Флаг "forBigAirplanes"
const int CROSSROAD_IN_TOWN      = 0x00000004;	// Флаг "town"
const int CROSSROAD_DEAD         = 0x00000010;	// Флаг "isDead"
const int CROSSROAD_STAND        = 0x00000020;	// Флаг "stand"	(стоянка)
const int CROSSROAD_STAND_HELICOPTER_ONLY = 0x00000040;	// Флаг "стоянка только для вертолетов"

enum road_type
{
	AUTO = 0,		// Авто дороги
	RAIL = 1,		// Железные дороги
	AIR  = 2,		// Аэродромные
	SEA  = 3		// Морские пути
};

struct PointOnRoad;
struct RoadSceneObject;
struct Crossroad;
struct Road;
struct RoadAnchor;
struct Segment;
struct SceneObject;
struct CrossroadType;
struct CrossroadTypeFile;

// Точка на дороге
struct EDTERRAIN_API PointOnRoad
{
protected:
	friend struct Segment;
	friend struct Road;
	mutable dVector point;			// точка
	mutable cVector tangent;		// точка

	void SetToLength(float length);
public:
	Road* pRoad;			// Дорога
	unsigned short segment; // номер сегмента
	float param;			// локальный параметр на сегменте [0, 1]

	PointOnRoad();
	PointOnRoad(const PointOnRoad& por);
	PointOnRoad(const PointOnRoad& por, float length);
	PointOnRoad(Road* pRoad, float length);
	PointOnRoad(Road* pRoad, unsigned short segment, float param);

	// Сдвинуть точку (инкриментный поиск от текущей позиции)
	void MoveTo(float length);
	void Move(float delta);

	// Расстояние от начала дороги
	float getDistFromBegin() const;
	// Сегментный параметр точки
	float getSegmentParam() const;
	// Точка на физ.дороге
	bool isOnPhysicalRoad() const;
	// Точка на объездной дороге
	bool isDiversion() const;
	// Признаки
	bool isTunnel()const;
	bool isBridge()const;
	bool isTown()const;
	bool isTunnelOrBridge()const;

	// Аэродромные флаги
	bool isDead()const;
	bool isForBigAirplanes()const;
	bool isVirtual()const;
	bool isRunway(bool* landing=0, bool* takeoff=0)const;

	// Точка
	dVector& GetPoint() const;
	void GetPoint(osg::Vec3d& p) const;
	// Сдвинутая точка
	dVector getOffsetPoint(float offset) const;
	void getOffsetPoint(float offset, osg::Vec3d& p) const;
	// Направление
	cVector& getTangent() const;
	void getTangent(osg::Vec3d& tg) const;
	// Радиус поворота
	float getTurningRadius() const;

	// Расстояние до точки привязки
	float getDistanceTo(const PointOnRoad& ra) const;
	// Расстояние до точки привязки
	float getDistanceTo(RoadAnchor* ra) const;
	// Кто ближе к началу
	bool operator <(const PointOnRoad& arg)const;
	// Сравнение
	bool operator ==(const PointOnRoad& arg)const;
	bool operator !=(const PointOnRoad& arg)const;
};


/////////////////////////////////////////////
// Сегмент дороги
struct EDTERRAIN_API Segment
{
	typedef enum
	{
		UNKNOWN_TYPE_LINE = -1,
		SEGMENT_TYPE_LINE = 0,
		ARC_TYPE_LINE = 1,
		BSPLINE_TYPE_LINE = 2,
	} enType;

	enType Type; // line(0) or arc(1)
	float length;    // накопленная длина
	cVector p1;		 // нач. точка
	cVector p2;		 // кон. точка

	// (используется только для окружности)
	cVector cen;	 // центр
	float radius;    // радиус дуги (используется только для окружности)
	float st_angle;  // нач. угол (против ЧС) (используется только для окружности)
	float en_angle;  // кон. угол (используется только для окружности)
	// (используется только для B-spline)
	cVector p1_b;		 // точка p1_B
	cVector p2_b;		 // точка p2_B
	// Сегментный параметр (ГРИША)
	float segmentParam;
	// высота моста
	float bridgeHeight;	// Высота моста

	uint32_t flags;// бит[0] - 1 - обратное 0 - прямое - (направление вхождения в дорогу) (используется только для окружности)
	// бит[1] - 1 - мост, 0 - нет
	// бит[2] - 1 - дорога в городе, 0 - нет
	// бит[3] - 1 - дорога невидима. 0 - видима
	// Функции
public:
	Segment();
	Segment(const Segment& arg);
	Segment(const cVector& p1, const cVector& p2);
	Segment& operator=(const Segment& seg);

	void SetFlags(unsigned int flags);
	// Функции
public:
	bool IsArc()const
	{
		return Type==ARC_TYPE_LINE;
	}
	// Начало/конец
	const cVector& getStartPoint() const
	{
		return p1;
	}
	const cVector& getEndPoint() const
	{
		return p2;
	}
	const cVector& getPoint(bool bStart) const
	{
		return bStart?p1:p2;
	}
	cVector& getPoint(bool bStart)
	{
		return bStart?p1:p2;
	}
	// Направление
	cVector getStartTangent() const;
	cVector getEndTangent() const;
	// BSpline point
	cVector& getBsPoint(bool bStart)
	{
		return bStart?p1_b:p2_b;
	}

	bool isTunnel()const
	{
		return (flags&SEGMENT_INVISIBLE)!=0;
	};
	bool isBridge()const
	{
		return (flags&SEGMENT_IS_BRIDGE)!=0;
	};
	bool isTown()const
	{
		return (flags&SEGMENT_IN_TOWN)!=0;
	};
	bool isTunnelOrBridge()const
	{
		return (flags&(SEGMENT_INVISIBLE|SEGMENT_IS_BRIDGE))!=0;
	};
	bool isMound()const
	{
		return (flags&SEGMENT_MOUND)!=0;
	};
	bool isTunnelBridgeOrMound()const
	{
		return (flags&(SEGMENT_INVISIBLE|SEGMENT_IS_BRIDGE|SEGMENT_MOUND))!=0;
	};


	// Длина сегмента
	float getLength() const;
	// Расстояние вдоль сегмента param<=0 param>=1
	float getLengthByParam(float param) const;
	// Точка на сегменте
	dVector getPointByParam(float param) const;
	// Направление
	cVector getTangentByParam(float param) const;
	// Бокс
	void BuildBox(Box& box);

	// Вырожденный сегмент
	bool IsDegradateSegment();

	// Расчитать тангенты для безье
	void getBezierTangents(osg::Vec3d& starttangent, osg::Vec3d& endtangent) const;

	// Функции (только для дуг)
public:
	// Угловой размер дуги
	float ArcAngle() const
	{
		if(st_angle <= en_angle) return (en_angle - st_angle);
		return (float)(en_angle + (M_PI * 2.0) - st_angle);
	}
	// Обратное направление сегмента
	bool IsArcInverted() const
	{
		return (flags & SEGMENT_INVERT_DIR);
	};
	void SetArcInverted()
	{
		flags |= SEGMENT_INVERT_DIR;
	}
	// Угол по параметру (используется только для окружности)
	float getAngleByParam(float param) const;

	// проверочные ф-ции
public:
	// Расстояние до точки
	float DistanceFromPoint(const dVector& pt) const;
	// Проекция точки
	void ProjectPoint(const dVector& pt, PointOnRoad& point) const;
	// Проверка точка на дороге?
	bool IsPointOnRoad(const dVector& matchpt, float roadwidht) const;

	// Операции
public:
	// Изменить направление на обратное
	void Reverse();
	// Выделить часть сегмента
	void Cut(float param1, float param2, Segment& dst) const;
	// сдвинуть сегмент направо (окружность увеличиться или уменьшиться, длина может измениться)
	bool Offset(float off);

	// Очень маленькая дуга - в отрезок
	bool ConvertSmallArc2Seg();
	// Притянуть точку к pt
	// lengthfactor - показывает мак. изменение отн. длины сегмента
	const cVector& MovePoint(bool bStart, const cVector& pt, float lengthfactor=0.01f);
	// Сдвинуть
	void Translate(const cVector& pos);
	// Повернуть
	void Rotate(const cVector& cnt, float angle);
public:
	// Аппроксимация ломаной (вернет число отрезков аппроксимации)
	// tolerance - макс. отклонение от кривой
	int getNumApproxSeg(float tolerance, float offset=0) const;
	// Аппроксимация сегмента
	void Approx(float tolerance, ed::vector<cVector>& approx, float offset=0) const;
	// Текстура сегмента
	const char* getSegmentTexture(int roadnetworktype, int roadtype);
	// Пересечения
public:
	// получить точки пересечения отрезка [ab] с дорогой, возвращает два первых пересечения
	// возвращает количество найденных пересечений,
	int SegmentIntersection(const cVector& a, const cVector& b, PointOnRoad* pors /*[2]*/) const;
	// получить точку пересечения окружности с сегментом, возвращает параметр и с какой стороны от сегмента центр
	// возвращает количество найденных пересечений [0,2]
	int CircleIntersection(const cVector& center, float r, PointOnRoad* pors /*[2]*/) const;
	// получить точки пересечения прямоугольника с дорогой
	// возвращает количество найденных пересечений, p - массив углов
	int BoxIntersection(const cVector* p, ed::vector<PointOnRoad>& intpts) const;
	// возвращает количество найденных пересечений,
	int SegmentIntersection(const Segment& seg, PointOnRoad* pors) const;
public:
	// Создание
	void SetLineSeg(const cVector& p1, const cVector& p2);
	void SetArc(float a1, float a2, const cVector& center, float radius, bool bInvert);

	// Построить дугу по точке, двум тангенсам и радиусу
	void BuildArc(const cVector& pt1, const cVector& tangent1, const cVector& tangent2, float radius);
	void BuildArc(const cVector& pt1, const cVector& tangent1, const cVector& tangent2, float radius, bool bLeft);

	// построить сегмент - скругление в точке стыковки сегментов (исходные сегменты - меняются)
	bool BuildFillet(Segment& s1, Segment& s2, float radius);

	// построить максимально допустимое скругление для 2х сегментов (исходные сегменты - меняются)
	// расстояние до исходной кривой не должно превышать maxerror
	bool BuildFilletMax(Segment& s1, Segment& s2, float maxerror);
};

// объект сцены леж.на дороге
struct EDTERRAIN_API RoadSceneObject
{
	SceneObject* obj;	// объект
	uint32_t objindex;

	uint32_t flags;
	int32_t segment;			// Параметр на дороге
	float param;				// ---

	RoadSceneObject();
	PointOnRoad getPointOnRoad();
};

// Привязка дороги к перекрестку
struct EDTERRAIN_API RoadAnchor
{
public:
	Crossroad* cross;	// Перекресток
	Road* road;		// Дорога
	int32_t crossindex;
	//		int32_t roadindex;
	bool bStart;				// Каким Концом подходит или началом
public:
	bool isStart()const
	{
		return bStart;
	};
	bool isEnd()const
	{
		return !bStart;
	};
	// Перекресток
	Crossroad* getCross()const
	{
		return cross;
	};
	// Дорога
	Road* getRoad()const
	{
		return road;
	};
	// Точка привязки
	cVector getPoint() const;
	// Направление от перекрестка
	cVector getTangent() const;
	// Точка на дороге
	PointOnRoad getPOR() const;
	// Противоположная привязка
	RoadAnchor* getOponentAnchor();
	// Противоположный пер-ток
	Crossroad* getOponentCross() const;
public:
	RoadAnchor();
	//		RoadAnchor(const RoadAnchor& arg);
	//		RoadAnchor& operator=(const RoadAnchor& arg);
	// Иниц.
	void Init(Road* pRoad, bool bStart);
	//
	void setCross(Crossroad* cross);
public:
	// Для работы CrossroadType
	// Проверить можно ли подцепить дорогу к перекрестку
	bool CheckType(unsigned int roadtype);
};

// Дорога
struct EDTERRAIN_API Road
{
	// символьное имя дороги
	ed::string name;
	// ширина проезжей части дороги
	float width;
	// перекресток начала дороги
	RoadAnchor st_cross;
	// перекресток конца дороги
	RoadAnchor en_cross;
	// флаги дороги [0] бит == 1, если вдоль железной дороги нужно ставить столбы
	uint32_t flags;
	// Бокс
	Box box;
	// for GRISHA
	float zeroSegmentParam;

	// Список сегментов
	ed::vector<Segment> segments;
	// Список объектов на дороге
	ed::vector<RoadSceneObject> objects;

	// Функции
public:
	dVector getPoint(const PointOnRoad& pt) const 
	{
		return pt.GetPoint();
	}

	int getSegmentsCount() const
	{
		return segments.size();
	}
	// Точки
	const cVector& getStartPoint()const
	{
		return segments[0].getStartPoint();
	};
	const cVector& getEndPoint()const
	{
		return segments[segments.size()-1].getEndPoint();
	};
	void getStartPoint(PointOnRoad& pt)const
	{
		pt = PointOnRoad((Road*)this, 0, 0);
	};
	void getEndPoint(PointOnRoad& pt)const
	{
		pt = PointOnRoad((Road*)this, (unsigned short)(segments.size()-1), 1);
	};
	PointOnRoad getStart() const
	{
		return PointOnRoad((Road*)this, 0, 0);
	};
	PointOnRoad getEnd() const
	{
		return PointOnRoad((Road*)this, (unsigned short)(segments.size()-1), 1);
	};
	// Направление
	cVector getStartTangent() const;
	cVector getEndTangent() const;
	// Точка по индексу
	const cVector& getPoint(int ptn) const;
	// Точка по длине
	bool getPointByLength(float length, PointOnRoad& pt)const;
	bool getPointByLength(float length, cVector& pt)const;
	// Точка по сегментрому параметру
	bool getPointBySegParam(float sp, PointOnRoad& pt)const;
	// Сегмент
	Segment* getStartSegment()
	{
		return &segments[0];
	}
	Segment* getEndSegment()
	{
		return &segments[segments.size()-1];
	}
	Segment* getSegment(int i);
	// Полная длина
	float getLength()const
	{
		return segments.back().length;
	}
	// длина по сегментному параметру
	float getSegmentLength()const
	{
		return segments.back().segmentParam;
	}
	// Бокс
	const Box& getBox() const;

	// Вырожденный сегмент?
	bool IsDegradateSegment(int s, cVector* vc);
	// должен вернуть true только если весь участок на реальной дороге
	bool isOnPhysicalRoad(PointOnRoad& from, PointOnRoad& to) const;
	// должен вернуть false если на интервале встречается мост или туннель
	bool isDiversionAvailable(PointOnRoad& from, PointOnRoad& to) const;
	// dead
	bool isDead()const
	{
		return (flags&ROAD_DEAD)!=0;
	};
	// Valid
	bool isValidRoad() const;
	// Операции
public:
	// Аппроксимация ломаной 3d
	void Approx3d(float tolerance, ed::vector<cVector>& approx, float offset=0, int startseg=0, int endseg=-1, float angleTolerance = Pi/6);
	// Аппроксимация ломаной 2d
	void Approx2d(float tolerance, ed::vector<cVector>& approx, float offset=0);

	// Упрощённая аппрксимация 3d
	void SimpleApprox3d(ed::vector<cVector>& approx);

	// Построить эквидистанту к дороге
	bool Offset(float distance); // эквидистантное смещения (вправо от дороги)
	// копировать дорогу
	Road* Copy();
	// Выделить часть дороги
	bool Cut(const PointOnRoad& start_pt, const PointOnRoad& end_pt, Road& dstroad);
	Road* CutRoad(const PointOnRoad& start_pt, const PointOnRoad& end_pt);
	// Вставить одну дорогу в середину другой
	Road* InsertRoad(const PointOnRoad& start_pt, const PointOnRoad& end_pt, Road& insertedroad);

	// Изменить направление на обратное
	void Reverse();
	// расчитать длины сегментов
	void CalcLength();
	// Иниц. сегментных параметров (предв. дб вызвана CalcLength)
	void InitSegmentParams();
	void InitSegmentParams(float beginSP, float endSP);
	// сместить сегментные параметры
	void setStartSegmentParam(float param);

	// Посчитать бокс
	void BuildBox();
	// Изменить флаги сегментов
	void ModifySegmentsFlag(unsigned int set, unsigned int mask);

	// Сдвинуть
	void Translate(const cVector& pos);
	// Повернуть
	void Rotate(const cVector& cnt, float angle);

	// Собрать из списка дорог
	void BuildFromList(
	    ed::list<Road*>& src, ed::list<bool>& srcinvert);
	// Удалить сегменты
	void DeleteAllSegments();
	// Добавить сегменты
	void InsertSegments(Segment* segs, int count, int to);
	// Притянуть точку ptn к pt
	// lengthfactor - показывает мак. изменение отн. длины сегмента
	bool MovePoint(
	    int ptn, const cVector& pt,
	    float lengthfactor=0.01f,
	    bool bMoveEndPoint=false);

	// Удалить вырожденные сегменты
	void RemoveDegradateSegments();

	// Собрать из списка
	static Road* BuildFromList(ed::list<Road*>& src);
	// построить линейную дорогу
	static Road* BuildLineSeg(const cVector& a, const cVector& b, unsigned int flags=0xFAC);
	// построить дорогу из одного сегмента
	static Road* BuildFromSegment(const Segment& src);

	// Пострить выезд с одной дороги на другую по заданному радиусу
	// Направление: road1->fillet->road2
	static bool BuildRoadsFillet(
	    const Road& road1,	// road1->
	    const Road& road2,	// ->road2
	    float radius,		// radius
	    Segment& fillet,	// out
	    PointOnRoad& por1,	// Точка съезда с road1
	    PointOnRoad& por2);	// Точка выезда на road2

	// проверочные ф-ции
public:
	// Расстояние до точки
	float DistanceFromPoint(const cVector& pt) const;
	// Проекция точки
	void ProjectPoint(const cVector& pt, PointOnRoad& point) const;
	void ProjectPoint(const cVector& pt, PointOnRoad& point, double startFrom) const;
	// Проверка точка на дороге?
	bool IsPointOnRoad(const dVector& matchpt) const;
	// Пересечение
public:
	// получить точки пересечения сегмента [ab] с дорогой
	// возвращает количество найденных пересечений,
	int SegmentIntersection(const cVector& a, const cVector& b, ed::vector<PointOnRoad>& pors) const;
	// получить точки пересечения окружности с дорогой
	// возвращает количество найденных пересечений (offset - смещение дороги)
	int CircleIntersection(const cVector& center, float r, ed::vector<PointOnRoad>& pors) const;
	int CircleIntersection(float offset, const cVector& center, float r, ed::vector<PointOnRoad>& pors) const;
	// получить точки пересечения прямоугольника с дорогой
	// возвращает количество найденных пересечений, p - массив углов
	int BoxIntersection(const cVector& sw, const cVector& ne, ed::vector<PointOnRoad>& intpts) const;
	// Геометрия
public:
	// Накладные объекты
	geometry3::GeometryItem* BuildCenterLine(
	    const cVector& sqpos,
	    float tolerance,
	    material3::Material* material);
	geometry3::GeometryItem* BuildMesh(
	    int s1, int s2,
	    const cVector& sqpos,
	    float tolerance,
	    float wigth,
	    float factorLtoV,
	    float roadV,
	    material3::Material* material);
public:
	Road();
	~Road();
	// Задать имя
	void SetName(const char* name);
	// Копировать
	void CopyFrom(const Road& src, misc::mmf_write* file=NULL);
	// Сравнить типы дорог
	bool equalType(Road& arg);
	//
	void setObjects(int count);

	// Сериализация
public:
	// Требуемый объем для сериализации
	//		int SerializeSize() const;
	// сериализация
	//		bool Serialize(char* data, int datasize, bool bSave);
	//
	bool serialize(io::Stream& stream);

	// Инициализация после загрузки
	void InitAfterLoad(
	    Crossroad* crosses,
	    SceneObject* objects
	);
	// Инициализация перед сохранением
	void InitBeforeSave(
	    Crossroad* crosses,
	    SceneObject* objects
	);

};

// Перекресток
struct EDTERRAIN_API Crossroad
{
	// Тип перекрестка
	CrossroadType* type;
	int32_t typeind;

	ed::string name;	// crossroad name (for taxiways)

	uint32_t nTexture;	// Текстурный индекс перекрестка (archaic)
	uint32_t flag;		// Флаги перекрестка
	cPosition pos;			// Позиция перекрестка
	float rotateangle;		// Угол поворота (тип перекрестка)

	// объекты сцены
	ed::vector< SceneObject* > objects;
	ed::vector< uint32_t > objectinds;

	// Массив дорог (отсортирован по углам)
	ed::vector< RoadAnchor* > roads;
	ed::vector< uint32_t > roadinds;

public:
	// Найти дорогу
	RoadAnchor* FindRoadAnchor(Road* road);
public:
	Crossroad();

	// dead
	bool isDead()const
	{
		return (flag&CROSSROAD_DEAD)!=0;
	};
	bool isOnRunway()const
	{
		return (flag&CROSSROAD_ON_RUNWAY)!=0;
	};
	bool isShelter()const
	{
		return (flag&CROSSROAD_SHELTER)!=0;
	};
	bool isForBigAirPlaines()const
	{
		return (flag&CROSSROAD_FOR_BIG_AIR)!=0;
	};
	bool isInTown()const
	{
		return (flag&CROSSROAD_IN_TOWN)!=0;
	};
	bool isStand()const
	{
		return (flag&CROSSROAD_STAND)!=0;
	};
	bool isStandHelicopterOnly()const
	{
		return (flag&CROSSROAD_STAND_HELICOPTER_ONLY)!=0;
	};

	// Инициализация после загрузки
	void InitAfterLoad(
	    CrossroadType* types,
	    Road* roads,
	    SceneObject* objects
	);
	// Инициализация перед сохранением
	void InitBeforeSave(
	    CrossroadType* types,
	    Road* roads,
	    SceneObject* objects
	);

	// Привязать перекрестки
	void InitRoadAnchors(RoadAnchor** anchor, int count);
	// Копировать
	void CopyFrom(const Crossroad& src);
	// Индекс привязки
	int getAnchorIndex(const RoadAnchor*) const;
	//
	void setObjects(int count);
	// Проверка точка на дороге?
	bool IsPointOnCross(const dVector& matchpt) const;
};

// Привязка дороги
struct EDTERRAIN_API RoadLink
{
	// номер дороги
	Road* pRoad;
	uint32_t roadindex;
	// Номера сегментов леж. в квадрате
	ed::vector<uint32_t> segments;
	// Битовый массив номеров сегментов
	//		misc::bitarray bits;
	// Бокс
	Box box;

	RoadLink();
	//
	bool IsInside(const cVector& pt)const
	{
		Vector3 v(pt);
		return box.isInsideXZ(v);
	};
	//
	void BuildBox();

	// Инициализация после загрузки
	void InitAfterLoad(
	    Road* roads
	);
	// Инициализация перед сохранением
	void InitBeforeSave(
	    Road* roads
	);
};

// Процедура проверки типа сегмента
struct ICheckRoadSink
{
	// для поиска ближайшей дороги
	virtual bool IsValidSegment(
	    const NM_ROAD::Segment& seg
	)
	{
		return true;
	}
	virtual bool IsValidRoad(
	    const NM_ROAD::Road& road
	)
	{
		return true;
	}
	// для поиска ближайшего перекрестка
	virtual bool IsValidCross(
	    const NM_ROAD::Crossroad& cross
	)
	{
		return true;
	}
	// для поиска пути
	virtual bool CanMoveRoad(
	    NM_ROAD::Road* road,
	    NM_ROAD::RoadAnchor* from,
	    NM_ROAD::RoadAnchor* to
	)
	{
		return true;
	}
	virtual bool CanMoveCross(
	    NM_ROAD::Crossroad* cross,
	    NM_ROAD::RoadAnchor* from,
	    NM_ROAD::RoadAnchor* to
	)
	{
		return true;
	}

};

// Квадрат
struct EDTERRAIN_API Square
{
	// Координаты квадрата
	intpoint squareindex;
	// массив привязку дорог
	ed::vector<RoadLink> roads;
	// Список перекресков
	ed::vector< Crossroad* > crosses;
	ed::vector< uint32_t> crossinds;
	// Функции
public:
	// Проверка точка на дороге?
	bool IsPointOnRoad(
	    const dVector& matchpt,
	    PointOnRoad* por) const;	// Результат
	// Найти ближайший сегмент
	Road* FindClosestRoad(
	    const cVector& pt,
	    unsigned int& findsegment,
	    float& dist,
	    ICheckRoadSink* checksegmentproc=NULL
	);
	// Найти ближайший перекресток
	Crossroad* FindClosestCross(
	    const osg::Vec3d& pt,
	    float& dist,
	    ICheckRoadSink* checksegmentproc=NULL
	);

	// Инициализация после загрузки
	void InitAfterLoad(
	    Road* roads,
	    Crossroad* crosses
	);
	// Инициализация перед сохранением
	void InitBeforeSave(
	    Road* roads,
	    Crossroad* crosses
	);
};

// Описание объекта сцены
struct EDTERRAIN_API SceneObject
{
	// символьное имя типа объекта
	ed::string name;
	// Позиция объекта
	cPosition pos;
	// Дорога к которой привязан
	Road* road;
	int32_t roadindex;
	// Перекресток к которому привязан
	Crossroad* cross;
	int32_t crossindex;

	SceneObject();
	SceneObject(const SceneObject& arg);
	SceneObject& operator=(const SceneObject& seg);

	// Инициализация после загрузки
	void InitAfterLoad(
	    Road* roads,
	    Crossroad* crosses
	);
	// Инициализация перед сохранением
	void InitBeforeSave(
	    Road* roads,
	    Crossroad* crosses
	);
};



// Тип перекрестка
struct EDTERRAIN_API CrossroadType
{
	// Имя типа
	ed::string name;
	// Тип дорожной сети
	roads3::road_type rn_type;
	// Модель
	//		models3::lModel model;
	offshore::Scene model;
	// Дороги
	ed::vector<Road> roads;
	// Перекрестки (точки привязки)
	ed::vector<Crossroad> crosses;
	// Углы привязки
	struct EDTERRAIN_API CrossAngle
	{
		Crossroad* cross;	// Пер-ток
		uint32_t crossindex;	// serialize

		float angle;				// Угол привязки (от центра)
		uint32_t roadtype;		// Тип дороги

		CrossAngle();
		//			CrossAngle(const CrossAngle& arg);
		//			CrossAngle& operator=(const CrossAngle& arg);

		// Вектор стыковки дороги
		cVector getOutPoint();
		cVector getOutTangent();

		// Инициализация после загрузки
		void InitAfterLoad(
		    Crossroad* crosses
		);
		// Инициализация перед сохранением
		void InitBeforeSave(
		    Crossroad* crosses
		);

	};
	ed::vector<CrossAngle> crossangles;

public:
	CrossroadType();

	// Копировать
	void CopyFrom(const CrossroadType& src);
	// Собрать дороги
	void BuildRoads(ed::list<Road*>& src);

public:
	// Проверить и получить вектора привязки
	bool CheckType(
	    roads3::road_type, // Тип дорог
	    Crossroad& cross,
	    float& currenterror,	 // среднюю ошибку
	    float& crossrotateangle, // Угол поворота перекрестка
	    int& roads_shift		 // На сколько надо повернуть список дорог
	);
	// Можно ли пересечь перекресток в заданном направлении
	Road* getRoad(
	    Crossroad* cross,
	    RoadAnchor* from, RoadAnchor* to, bool& bReverse);

	// Инициализация после загрузки
	void InitAfterLoad(
	    material3::Material* materials	// Материалы
	);
	// Инициализация перед сохранением
	void InitBeforeSave(
	);

};
// Файл типов пер-ков
struct EDTERRAIN_API CrossroadTypeFile
{
	// Тип файла
	//		char filetype[4];
	// Версия
	//		unsigned int version;

	// Типы
	ed::vector< CrossroadType > types;

	// Материалы для моделей
	ed::vector< material3::Material> materials;
	// Функции
public:
	CrossroadTypeFile();

	// Инициализация после загрузки
	void InitAfterLoad(
	);
	// Инициализация перед сохранением
	void InitBeforeSave(
	);

	// Собрать
	void Build(ed::list<roads3::CrossroadType*>& dsttypes);
	// Копировать
	//		void BuildCopy(
	//			CrossroadTypeFile* src, misc::mmf_write& file);
	// Функции
public:
	// Открыть файл
	bool Load(
	    io::MmfStream& mmfStream
	);
	bool Save(
	    const char* filename
	);
	bool Serialize(io::Stream& file);

	// Тип по имени
	//		CrossroadType* getType(const char* name);
};


// Файл
struct EDTERRAIN_API lRoadFile
{
public:
	// Тип файла
	//		char filetype[4];
	// Версия
	//		unsigned int version;
	// Тип (0-авто, 1-делезнай, 2-аэродром)
	road_type type;


	// Границы
	intpoint sqmin, sqmax;
	// Типы перекрестков
	CrossroadTypeFile crosstypes;
	// Дороги
	ed::vector<Road> roads;
	// Перекрестки
	ed::vector<Crossroad> crosses;
	// Массив квадратов (указателей) размерность X*Z
	ed::vector< Square > squarepool;
	ed::vector< int32_t > pSquares;
	// объекты сцены
	ed::vector<SceneObject> sceneobjects;
	// Геометрия для отрисовки
	superficial3::lSuperficialFile pSuperficials;

	// Имя файла дорог
	ed::string filename;

	// Функции
public:
	// Открыть файл
	bool Load(
	    io::MmfStream& mmfStream
	);
	bool Save(
	    const char* filename
	);
	bool Serialize(io::Stream& file);

	// Инициализация после загрузки
	void InitAfterLoad(
	);
	// Инициализация перед сохранением
	void InitBeforeSave(
	);

	/*/
	// Открыть файл
	static lRoadFile* Open(
		const char* filename,
		misc::mmf& file);
	/*/

	// получить точки пересечения сегмента [ab] с дорогами
	// возвращает количество найденных пересечений,
	int SegmentIntersection(const cVector& a, const cVector& b, ed::vector<PointOnRoad>& pors);

	// Проверка точка на дороге?
	bool IsPointOnRoad(const dVector& matchpt, PointOnRoad* por) const;

	// Квадрат
	Square* getSquare(intpoint sqindex)
	{
		if(sqindex.x<sqmin.x || sqindex.x>=sqmax.x)
			return 0;
		if(sqindex.z<sqmin.z || sqindex.z>=sqmax.z)
			return 0;
		int i = (sqindex.x-sqmin.x)*(sqmax.z-sqmin.z) + (sqindex.z-sqmin.z);
		int32_t poolindex = pSquares[i];
		if(poolindex<0 || poolindex>(int)squarepool.size())
			return 0;
		return &squarepool[poolindex];
	}
	void setSquare(intpoint sqindex, int32_t poolindex)
	{
		if(sqindex.x<sqmin.x || sqindex.x>=sqmax.x)
			return;
		if(sqindex.z<sqmin.z || sqindex.z>=sqmax.z)
			return;
		int i = (sqindex.x-sqmin.x)*(sqmax.z-sqmin.z) + (sqindex.z-sqmin.z);
		pSquares[i] = poolindex;
	}

	// Функции создания
public:
	lRoadFile();
	lRoadFile(road_type type, const char* srcfilename);

	// Построить файл
	void Build(
	    ed::list<Road*>& list,
	    ed::list<SceneObject*>& dstobjects);

	static void Build(
	    ed::list<Road*>& src,
	    ed::vector<Road>& roads,
	    ed::vector<Crossroad>& crosses);

	// построить привязку к квадратам
	void BuildSquares();
	// построить накладные объекты
	void BuildSuperficial();

public:
	// номер дороги по указателю
	// -1 ошибка
	int getRoadIndex(Road* pRoad);
	// номер перекрестка по указателю
	// -1 ошибка
	int getCrossIndex(Crossroad* pCrossroad);

protected:
	// Инициализация
	void InitSquares(
	    intpoint sqmin, intpoint sqmax);
};

}

#endif