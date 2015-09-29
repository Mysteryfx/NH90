#ifndef LANDSCAPE4_LROAD_H
#define LANDSCAPE4_LROAD_H
#include "../landscape3_config.h"

#include "edTerrain.h"
#include "osg/Vec3d"
#include "osg/Matrixf"
#include "osg/Matrixd"
#include "osg/Vec2d"
#include <ed/vector.h>
#include "offshore/scene.h"

#ifdef EDGE
	#include "Core/io/STLSerializer.h"
#else
	#include "io/STLSerializer.h"
#endif

namespace roads4
{
	/** 
	 * Часть дороги, используется для задания моста, туннеля
	 */
	struct lRoadPart
	{
		// Тип участка
		enum TYPE
		{
			NONE = 0,
			BRIDGE = 1,
			TUNNEL = 2,
		};

		// Тип участка
		int32_t type;

		// Сегменты, ограничивающие участок
		int32_t startSegment;
		int32_t endSegment;

		// Точки, обозначающире границы
		osg::Vec3d startPoint;
		osg::Vec3d endPoint;

		// Направления на границах
		osg::Vec3d startDirection;
		osg::Vec3d endDirection;

		/**
		 * Оператор сравнения
		 */
		EDTERRAIN_API bool operator < (const lRoadPart& rhs) const
		{
			return (this->endSegment <= rhs.startSegment);
		}
	};

	/**
	 * Уравнение движения для отрезка дороги
	 */
	struct lRoadMotionEquation
	{
		//Параметры участка дороги
		double startParam;		//Начальный параметр
		double endParam;		//Конечный параметр

		//Параметры уравнения движения
		double startLength;		//Начальная длина
		double startVelocity;	//Начальная скорость
		double acceleration;	//Ускорение

		//Время движения по участку дороги
		double startTime;		//Начальное время участка
		double endTime;			//Конечное время участка
	};

	/**
	 * Полосы движения на дороге
	 */
	struct lRoadTrafficLanes
	{
		//Параметры полос движения
		ed::vector<double> velocities; 		//Скорости движения по полосе
		ed::vector<double> offsets;			//Смещения полос относительно осевой линии

		/**
		 * Добавить дорожную полосу
		 * @param velocity скорость движения по полосе
		 * @param offset смещение полосы относительно осевой линии
		 */
		EDTERRAIN_API void addTrafficLane(double velocity, double offset);

		/**
		 * Найти полосу движения с ближайшей скоростью
		 * @param velocity заданная скорость
		 * @return -1, если полос нет
		 */
		EDTERRAIN_API uint8_t getClosestTrafficLane(double velocity) const;

		/**
		 * Получить смещение по скорости
		 * @param velocity заданная скорость
		 * @param offset полученное смещение
		 * @return false если нет полосы с такой скоростью
		 */
		EDTERRAIN_API bool getOffsetByVelocity(double velocity, double& offset) const;
	};

	/**
	 * Структура задает точки, к которым нужно пристыковать концы страйпа дороги
	 */
	struct StripeAnchoringPoints
	{
		osg::Vec3d startLeft;	//Точка слева от начала
		osg::Vec3d startRight;	//Точка справа от начала
		
		osg::Vec3d endLeft;		//Точка слева от конца
		osg::Vec3d endRight;	//Точка справа от конца

		bool startIsSet;		//Начало задано
		bool endIsSet;			//Конец задан

		EDTERRAIN_API StripeAnchoringPoints() : startIsSet(false), endIsSet(false) {};

		/**
		 * Задать точки стыковки начала страйпа
		 */
		EDTERRAIN_API void setStart(const osg::Vec3d& left, const osg::Vec3d& right)
		{
			this->startLeft = left;
			this->startRight = right;
			this->startIsSet = true;
		}

		/**
		 * Задать точки стыковки начала страйпа
		 */
		EDTERRAIN_API void setEnd(const osg::Vec3d& left, const osg::Vec3d& right)
		{
			this->endLeft = left;
			this->endRight = right;
			this->endIsSet = true;
		}

		/**
		 * Умножить все точки на матрицу
		 */
		EDTERRAIN_API void multiply(const osg::Matrixd& m)
		{
			this->startLeft = this->startLeft * m;
			this->startRight = this->startRight * m;
			this->endLeft = this->endLeft * m;
			this->endRight = this->endRight * m;
		}

		/**
		 * Поменять местами начало и конец
		 */
		EDTERRAIN_API void revert()
		{
			std::swap(this->startIsSet, this->endIsSet);
			std::swap(this->startLeft, this->endRight);
			std::swap(this->startRight, this->endLeft);
		}
	};

	/**
	 * Структура, описывающая дорожное покрытие
	 */
	struct lRoadCovering
	{
		//Общие параметры
		bool isSet; 										//Задано ли покрытие
		bool is3d;											//Является ли 3d покрытием

		bool integralTiling;								//Нужно ли применять тайлинг целыми сэмплами

		double leftWidth;									//Ширина дороги слева от осевой
		double rightWidth;									//Ширина дороги справа от осевой

		double sampleLength;								//Длина сэмпла

		offshore::Mesh sample;								//Меш-образец покрытия дороги

		ed::string type;									//Тип дороги
		ed::string semantic;								//Семантика дороги

		offshore::Material material; 						//Назначенный материал
		ed::map<ed::string, ed::string> nodeParams;		//Параметры ноды

		//Параметры 1d покрытия
		ed::map<ed::string, osg::Matrixd> toUVMatrixes;	//Матрицы перехода из пространства Length, Width в uv и имя uvSet
		ed::vector<osg::Vec2d> offsets;						//Смещения геометрии относительно осевой, x - смещение по нормали, у - смещение по высоте

		//Параметры 3d покрытия
		ed::vector<std::pair<double, int32_t> > frontEdgeOffsets;
		ed::vector<std::pair<double, int32_t> > backEdgeOffsets;

		/**
		 * Конструктор
		 */
		EDTERRAIN_API lRoadCovering() : isSet(false), is3d(true) {};

		/**
		 * Обработать шаблон покрытия и получить данные о покрытии
		 * @param sample меш-образец геометрии покрытия
		 * @param centerLine осевая линия дороги
		 * @param material материал покрытия
		 * @param nodeParams параметры ноды меша-образца геометрии
		 */
		EDTERRAIN_API void parseSample(const offshore::Mesh& sample, const offshore::Spline& centerLine, const offshore::Material& material, const ed::map<ed::string, ed::string>& nodeParams);
		
		/**
		 * Deprecated
		 */
		EDTERRAIN_API void set(offshore::Mesh& sample, offshore::Spline& centerLine, offshore::Material& material, ed::map<ed::string, ed::string>& nodeParams);
		EDTERRAIN_API void set3d(const offshore::Mesh& sample, const offshore::Spline& centerLine, const offshore::Material& material, const ed::map<ed::string, ed::string>& nodeParams);

		/**
		 * Можно ли пристыковать дорогу с данным покрытием
		 */
		EDTERRAIN_API bool canAttach(const lRoadCovering& rhs) const;
	};

	class lRoad
	{
	public:
		enum ROADCLASS
		{
			NONE = 0,
			RAIL = 1,
			ROAD = 2,
			WATER = 3,
		};

	public:
		//Класс дороги
		uint8_t roadClass;

		//Тип дороги
		uint8_t roadType;

		//Перекресток, в котором начинается дорога, -1 - если нет такого перекрестка
		int32_t startCrossroad;
		//Перекресток, в котором заканчивается дорога, -1 - если нет такого перекрестка
		int32_t endCrossroad;
		
		// Параметры дороги
		ed::map<ed::string, ed::string> params;

		//Уравнение движения по дороге
		ed::vector<lRoadMotionEquation> motionEquation;

		EDTERRAIN_API void calculateMotionEquation(double acceleration, ed::vector<offshore::Notification>& notifications);

		EDTERRAIN_API bool addRoadStopMotionEquation(const ed::map<double, double>& paramsDurations, double acceleration, ed::vector<offshore::Notification>& notifications);

		// Участки дороги (мосты, туннели)
		ed::vector<lRoadPart> roadParts;

		EDTERRAIN_API lRoad();
		// Начало/конец дороги
		EDTERRAIN_API double getStartParam() const;
		EDTERRAIN_API double getEndParam() const;
		// Точка по параметру
		EDTERRAIN_API osg::Vec3d getPoint(double param) const;
		EDTERRAIN_API osg::Vec3d getPointOnSurface(double param) const;
		// Касательная по параметру
		EDTERRAIN_API osg::Vec3d getTangent(double param) const;
		EDTERRAIN_API osg::Vec3d getTangentOnSurface(double param) const;
		// Нормаль по параметру
		EDTERRAIN_API osg::Vec3d getNormal(double param);
		//Кривизна по параметру
		EDTERRAIN_API double getCurvature(double param);

		/**
		 * Задать параметр дороги
		 * @param param параметр
		 * @param value значение
		 */
		EDTERRAIN_API void setParam(const ed::string& param, const ed::string& value);
		EDTERRAIN_API void setParam(const ed::string& param, bool value = true);
		
		/**
		 * Получить параметр дороги
		 * @param param параметр
		 * @param value значение
		 * @param defaultValue значение по умолчанию
		 * @param return false если такого параметра нет и присвоено значение по умолчанию
		 */	
		EDTERRAIN_API bool getParam(const ed::string& param, ed::string& value, const ed::string& defaultValue = "") const;

		/**
		 * Есть ли параметр с заданным значением
		 * @param param параметр
		 * @param value значение
		 */
		EDTERRAIN_API bool isParam(const ed::string& param, const ed::string& value) const;
		EDTERRAIN_API bool isParam(const ed::string& param, bool value = true) const;

		/**
		 * Получить высоту из дорожных частей
		 * @param param параметр на дороге
		 * @param point точка 
		 * @return false если точка не находится внутри частей 
		 */
		EDTERRAIN_API bool getHeightFromRoadParts(double param, osg::Vec3d& point) const;

		/**
		 * Сделать дорогу мостом
		 * @param delta смещение точек дороги
		 */ 
		EDTERRAIN_API void makeBridge(double delta = 0.1);

		/**
		 * Сделать дорогу туннелем
		 * @param delta смещение точек туннеля
		 */
		EDTERRAIN_API void makeTunnel(double delta = 0.1);

		/**
		 * Получить полное время движения по дороге
		 */
		EDTERRAIN_API double getFullTrafficTime();

		EDTERRAIN_API void setMaxVelocity(double velocity);

		EDTERRAIN_API double cutRoadStopIntoRoad(double param, double offset);

		/**
		 * Добавить уравнения, описывающие движения на заданном участке
		 * @param endParam конечный параметр
		 * @param startVelocity начальная скорость
		 * @param maxVelocity максимальная скорость
		 * @param endVelocity конечная скорость
		 * @param acceleration ускорение
		 * @param errorMessage сообщение об ошибке
		 * @param return false если не удалось построить уравнение движение 
		 */
		EDTERRAIN_API bool addMotionEquation(double endParam, double startVelocity, double maxVelocity, double endVelocity, double acceleration, ed::string& errorMessage);

		/**
		 * Получить смещенные контрольные точки
		 * @param vertices смещенные контрольные точки
		 * @param return false если не удалось получить точки
		 */
		EDTERRAIN_API bool getOffsetedControlVertices(ed::vector<osg::Vec3d>& vertices) const;

		/**
		 * Получить длину дуги, пройденной за данное время
		 * @param time время
		 */
		EDTERRAIN_API double getLengthByTime(double time);

		// Длина кривой
		EDTERRAIN_API bool getLength(double& length) const;
		// Длина от начала по параметру
		EDTERRAIN_API bool getLength(double param, double& length, int limitSimpsonSteps = 10000) const;
		
		/**
		 * Поиск по параметра по длине от начала сплайна
		 * @param length длина от начала сплайна
		 * @param param найденный параметр
		 * @param startSegment начальный сегмент для поиска; значение -1 указывает, что не задан
		 * @param limitSimpsonIterations максимальное кол-во итераций в формуле Симпсона
		 * @param limitNewtonIterations максимальное кол-во итераций в методе Ньютона
		 * @return false при достижении ограничения
		 */
		EDTERRAIN_API bool getParamByLength(double length, double& param, int limitSimpsonIterations = 5, int limitNewtonIterations = 10);

		/**
		 * Сместить параметр на длину
		 * @param param исходный параметр
		 * @param length длина смещения
		 * @param newParam полученный параметр
		 * @param limitSimpsonIterations максимальное кол-во итераций в формуле Симпсона
		 * @param limitNewtonIterations максимальное кол-во итераций в методе Ньютона
		 * @return false при достижении ограничения
		 */
		EDTERRAIN_API bool moveParamByLength(double param, double length, double& newParam, int limitSimpsonIterations = 5, int limitNewtonIterations = 10);

		// Ближайшая точка на дороге (вернет параметр)
		// Не реализованно
		EDTERRAIN_API double findClosestPoint(osg::Vec3d point);

		/**
		 * Получить ближайшую точку на сегменте
		 * @param point исходная точка
		 * @param segment сегмент
		 * @return параметр на всем сплайне
		 */
		EDTERRAIN_API double getClosestPoint2d(const osg::Vec3d& point, int32_t segment) const;

		//Задание контрольных точек сплайна, с автоматическим дополнением по точке на каждом конце, для того чтобы сплайн
		//начинался и заканчивался в первой и последней точках
		EDTERRAIN_API void setVerts(const ed::vector<osg::Vec3d>& verts);

		//Задание offsets
		EDTERRAIN_API void setOffsets(const ed::vector<float>& offsets);
		EDTERRAIN_API void setOffset(float offset);
		EDTERRAIN_API void setOffset(float startOffset, float endOffset);
		
		/**
		 * Посчитать нормали для оффсетов
		 */
		EDTERRAIN_API void calculateNormals();

		//Подсчет матриц сегментов
		EDTERRAIN_API void calculateSegmentMatrixes();

		//Посчитать параметры сегментов, error - абсолютная погрешность вычислений
		EDTERRAIN_API bool calculateSegments(double error);

		/**
		 *  Посчитать дорожные участки
		 */
		EDTERRAIN_API void calculateRoadParts();

		/**
		 * Построить оффшорный меш дороги
		 * @param scene сцена, к которой добавить меш
		 * @param parentNode нода, к которой добавить меш
		 * @param roadCovering дорожное покрытие, по которому строить меш
		 * @param points точки пристыковки концов дорожного покрытия
		 * @param notification уведомления в ходе выполнения
		 */
		EDTERRAIN_API int32_t buildOffshoreMesh(offshore::Scene& scene, int32_t parentNode, const lRoadCovering& roadCovering, const StripeAnchoringPoints& points, ed::vector<offshore::Notification>& notifications);
		EDTERRAIN_API int32_t buildOffshoreMesh1d(offshore::Scene& scene, int32_t parentNode, const lRoadCovering& roadCovering, const StripeAnchoringPoints& points, ed::vector<offshore::Notification>& notifications);
		EDTERRAIN_API int32_t buildOffshoreMesh3d(offshore::Scene& scene, int32_t parentNode, const lRoadCovering& roadCovering, const StripeAnchoringPoints& points, ed::vector<offshore::Notification>& notifications);

		/**
		 * Отфильтровать (проредить точки)
		 * @param vertices точки
		 * @param minHLength высота выбрасываемого треугольника
		 * @param minEdgeLength если два сегмента сплайна, образующего треугольник по длине больше minEdgeLength, то треугольник оставляем
		 */
		EDTERRAIN_API void filtrate(ed::vector<osg::Vec3d>& vertices, double minHLength = 0.0, double minEdgeLength = 0.0);

		/**
		 * Применить subdivision к точкам, с последующей фильтрацией
		 * @param verts точки
		 * @param subdivstep кол-во итераций subdivision
		 */
		EDTERRAIN_API void subDivide(ed::vector<osg::Vec3d>& verts, int subdivstep = 2);

		EDTERRAIN_API void getSubRoadVerts(ed::vector<osg::Vec3d>& vertices, double startParam, double endParam);

		EDTERRAIN_API int32_t getSegmentsCount() const {return (verts.size() - 3);}

		EDTERRAIN_API osg::Rect getSegmentRect(int32_t segment) const;

		EDTERRAIN_API osg::Vec3d getControlVertex(double t) const;

		/**
		 * Строит дорогу объединяющию переданные
		 * @param roads дороги
		 * @param error погрешнось результирующей дороги
		 * @param return false если не удалось объединить дороги
		 */
		EDTERRAIN_API bool combineRoads(const ed::vector<lRoad>& roads);
		
		/**
		 * Построить offshore::Spline по дороге
		 * @param spline полученный сплайн
		 * @param distance расстояние между точками
		 * @return false если в ходе построения были ошибки
		 */
		EDTERRAIN_API bool buildOffshoreSplineFromRoad(offshore::Spline& spline, double distance = 10.0);

	public:
	/**
	 * Получить сплайн правой границы меша
	 * @param mesh меш дороги, полученный из метода buildOffshoreMesh
	 * @param roadCovering тип дорожного покрытия
	 * @param rightBorder полученная правая граница меша
	 */
	static EDTERRAIN_API void getRightBorderFromOffshoreMesh(const offshore::Mesh& mesh, const lRoadCovering& roadCovering, offshore::Spline& rightBorder);
	static EDTERRAIN_API void getRightBorderFromOffshoreMesh1d(const offshore::Mesh& mesh, const lRoadCovering& roadCovering, offshore::Spline& rightBorder);
	static EDTERRAIN_API void getRightBorderFromOffshoreMesh3d(const offshore::Mesh& mesh, const lRoadCovering& roadCovering, offshore::Spline& rightBorder);

	/**
	 * Проверить меш
	 * @param mesh меш для проверки
	 * @param notifications уведомления
	 * @return true если ошибок не найдено
	 */
	static EDTERRAIN_API bool checkOffshoreMesh(const offshore::Mesh& mesh, ed::vector<offshore::Notification>& notifications);

	// Контрольные вертексы
	//protected:
	public:
		ed::vector<osg::Vec3d> verts;	
		ed::vector<osg::Vec3d> normals;

		// Смещения контрольных точек от осевой. Может быть пустым!
		ed::vector<float> offsets;

		//Длины сегментов, номер сегмента равен вершине слева
		ed::vector<double> segmentLengths;

		//Максимальные скорости на сегментах
		ed::vector<double> segmentMaxVelocities;

		//Шаги изменения параметра на каждом сегменте для обеспечения нужной
		//точности измерения
		ed::vector<int> segmentSteps;

		//Матрицы сегментов сплайна
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  NOTE
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  Matrixes -> Matrices
		ed::vector<osg::Matrixf> segmentMatrixes;

		//Абсолютная погрешность вычислений
		double epsilon;

		//Подсчет длин сегментов с абсолютной погрешностью epsilon
		bool calculateSegmentLengths();

		//Возвращает номер сегмента, которому принадлежит точка, а param переводится в [0; 1]
		EDTERRAIN_API int splitParam(double& param) const;

		//Подинтегральная функция вычисления длины
		double integrand(double param, int segment) const;

		//Поиск длины дуги между точками численным интегрированием, используя квадратуру Симпсона
		//a, b - точки
		//segment - сегмент сплайна, по которому берется интеграл
		//n - минимальное число шагов при интегрировании(внутренний параметр)
		//error - абсолютная ошибка искомой и найденной длины
		bool lengthBySimpson(double a, double b, int segment, int &n, double error, double& length, int limitIterations) const;
	
		/**
		 * Поиск по параметра по длине от начала сплайна
		 * @param length длина от начала сплайна
		 * @param param найденный параметр
		 * @param startSegment начальный сегмент для поиска; значение -1 указывает, что не задан
         * @param limitSimpsonIterations максимальное кол-во итераций в формуле Симпсона
		 * @param limitNewtonIterations максимальное кол-во итераций в методе Ньютона
		 * @return false при достижении ограничения
		 */
		bool getParam(double length, double& param, int startSegment, int limitSimpsonIterations, int limitNewtonIterations);

		/**
		 * Получить номер сегмента в сплайне, по расстоянию от начала
		 * Сегменты нумеруются с нуля
		 * @param distances расстояния от начала сплайна до точек сплайна
		 * @param distance расстояние от начала
		 * @param delta абсолютная погрешность вычисления расстояния
		 * @return возвращает номер сегмента, если расстояние меньше минимального - -1, больше максимального - максимальный сегмент + 1
		 */
		int32_t getSegmentByDistance(const ed::vector<double>& distances, double distance, double delta = 0.01);
	
		/**
		 * Получить номер уравнения движения по времени
		 * @param time время от начала движения
		 * @return возвращает индекс уравнения движения
		 */
		int32_t getMotionEquationByTime(double time) const;

		/**
		 * Получить номер уравнения движения по параметру
		 * @param param параметр
		 * @return возвращает индекс уравнения движения
		 */
		int32_t getMotionEquationByParam(double param) const;
	};
}

template <typename S> S& operator >>(S& stream, roads4::lRoad& road)
{
	uint32_t version = 1;
	stream >> version;

	if (version >= 1)
	{
		stream >> road.roadClass;
		stream >> road.roadType;
		stream >> road.startCrossroad;
		stream >> road.endCrossroad;
		stream >> road.params;
		stream >> road.verts;
		stream >> road.normals;
		stream >> road.offsets;
		stream >> road.segmentLengths;
		stream >> road.segmentSteps;
		stream >> road.segmentMatrixes;
		stream >> road.epsilon;
		stream >> road.motionEquation;
		stream >> road.roadParts;
	}

	return stream;
}

template <typename S> S& operator >>(S& stream, roads4::lRoadPart& part)
{
	uint32_t version = 1;
	stream >> version;

	if (version >= 1)
	{
		stream >> part.type;
		stream >> part.startSegment;
		stream >> part.endSegment;
		stream >> part.startPoint;
		stream >> part.endPoint;
		stream >> part.startDirection;
		stream >> part.endDirection;
	}

	return stream;
}

template <typename S> S& operator >>(S& stream, roads4::lRoadMotionEquation& equation)
{
	uint32_t version = 1;
	stream >> version;

	if (version >= 1)
	{
		stream >> equation.startParam;		
		stream >> equation.endParam;		
		stream >> equation.startLength;		
		stream >> equation.startVelocity;	
		stream >> equation.acceleration;	
		stream >> equation.startTime;		
		stream >> equation.endTime;			
	}

	return stream;
}

template <typename S> S& operator >>(S& stream, roads4::lRoadTrafficLanes& trafficLanes)
{
	uint32_t version = 1;
	stream >> version;

	if (version >= 1)
	{
		stream >> trafficLanes.velocities;
		stream >> trafficLanes.offsets;
	}

	return stream;
}

#endif
