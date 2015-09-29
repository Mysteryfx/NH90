#ifndef LANDSCAPE4_LROADSTYPES_H
#define LANDSCAPE4_LROADSTYPES_H

#include "../landscape3_config.h"
#include "edTerrain.h"

#include "landscape4/lRoad.h"

namespace roads4
{

//Откуда брать высоту для класса дороги OTHER
enum ROADTYPEHEIGHTOPTION
{
	HEIGHTFROMSPLINE = 0,	//Из исходного сплайна
	SPLINEFROMMATRIX = 1,   //Выставить сплайн по матрице высот
	MESHFROMMATRIX = 2,   //Выставить страйп по матрице высот
};

/**
 * Типы дорог и перекрестков
 */
class lRoadsTypes
{
private:

	//Вспомогательные структуры
	
	/**
	 * Структура, хранящая данные о сплайне, используется в подборе перекрестка для пересечения сплайнов
	 */
	struct SplineInfo
	{
		//Индекс сплайна
		int spline;
		//Направление в плоскости XZ
		osg::Vec3d direction2d;
		//Имя сплайна
		ed::string name;
		//Полярный угол от -Pi до Pi
		double angle;
		//Приоритет сплайна
		int priority;
		//Сплайн входит в перекресток
		bool inGoing;

		/**
		 * Оператор сравнения по полярному углу
		 */
		bool operator < (const SplineInfo& rhs) const {return (angle < rhs.angle);}
	};

public:

	//Структуры дорожных деталей

	/**
	 * Структура, описывающая дорожный объект (фонари и т.д.)
	 */
	struct lRoadDetail
	{
		//Положение объекта
		osg::Matrixd transform;

		//Имя объекта (или любой другой идентификатор)
		ed::string name;

		lRoadDetail(const osg::Matrixd& transform, const ed::string& name) : transform(transform), name(name) {}

		/**
		 * Показывает, находится ли деталь внутри области ограниченной длиной или вне
		 * @param length длина огранчиенной области
		 * @return true если внутри границы
		 */
		bool withinLength(double length) const {return (distanceFromStart() <= length);}

		/**
		 * Возвращает расстояние от начала
		 */
		double distanceFromStart() const {return transform.getTrans().x();}
	};

	/**
	 * Структура, описывающая придорожную полосу
	 */
	struct lRoadWayside
	{
		//Имя придорожной полосы (или любой другой идентификатор)
		ed::string name;
		//Длина сегмента придорожной полосы
		double length;
		//Смещение придорожной полосы относительно осевой дороги
		double offset;

		//Направление придорожной полосы
		bool direction;

		//Точки придорожной полосы
		ed::vector<osg::Vec3d> vertices;

		//Параметры ноды
		ed::map<ed::string, ed::string> params;

		lRoadWayside(const ed::vector<osg::Vec3d>& vertices, const ed::string& name, const ed::map<ed::string, ed::string>& params) : vertices(vertices), name(name), params(params) {}

		/**
		 * Рассчитать параметры придорожной полосы относительно осевой линии
		 */
		void set(const offshore::Spline& centerLine);
	};

	/**
	 * Структура, описывающая геометрию дорожного объекта
	 */
	struct lRoadDetailGeometry
	{
		//Имя объекта (или любой другой идентификатор)
		ed::vector<ed::string> name;

		//Геометрия объекта
		offshore::Scene scene;
	};

	/**
	 * Структура, описывающая размещение деталей участка дороги
	 * Ось х расположена вдоль участка дороги, начало координат в начале участка дороги
	 */
	struct lRoadDetailsSample
	{
		//Детали
		ed::list<lRoadDetail> details;
		//Придорожные полосы
		ed::list<lRoadWayside> waysides;
		//Длина участка дороги
		double length;
	};
	
	//Структуры дорожных типов

	/**
	 * Тип дороги
	 */
	struct lRoadType
	{
		//Вид дороги
		enum ROADTYPE
		{
			NONE = 0,		//Нет типа дороги
			RAIL = 1,		//Железная дорога
			ROAD = 2,		//Автомобильная дорога
			RIVER = 3,		//Река
			SHORELINE = 4,	//Береговая линия
			OTHER = 5,		//Другое
			POWERLINE = 6,	//ЛЭП
		};

		bool stripeHeightFromMatrix;

		bool negativeHeightToZero;

		bool findMeshesIntersection;

		bool tunnel;

		bool findCrosses;

		//Индекс ноды типа дороги в сцене типов дорог
		int nodeIndex;

		//Имя типа дороги
		ed::string name;
		
		//Вид дороги
		int type;
		
		//Выставление высоты точек страйпа
		//Работает только для класса OTHER
		int heightOption;
		
		//Кол-во шагов применения алгоритма subdivision
		int subDivisionSteps;
		
		//Приоритет дороги
		int priority;

		//Параметр сглаживания по высоте
		double heightSmoothDelta;

		//Параметры фильтрации сабдива
		double minHLength;
		double minEdgeLength;

		//Тип покрытия дороги
		lRoadCovering covering;

		//Образец расстановки деталей дороги (фонарей и т.д.)
		lRoadDetailsSample roadDetailsSample;

		//Полосы движения
		lRoadTrafficLanes directTrafficLanes;	//Полосы движения в прямом направлении (от начала до конца)
		lRoadTrafficLanes backTrafficLanes;		//Полосы движения в обратном направлении (от конца до начала)

		/**
		 * Установить тип дороги по строке
		 */
		void setType(const ed::string& type);

		/**
		 * Установить опцию выставления высот
		 */
		void setHeightOption(const ed::string& option);
	};

	/**
	 * Тип перекрестка
	 */
	struct lCrossType
	{
		/**
		 * Якорь в типе перекрестка
		 */
		struct lAnchor
		{
			//Индекс ноды типа дороги в сцене типов дорог
			int nodeIndex;
			//Местоположение якоря (точка пристыковки оси дороги)
			osg::Vec3d position;
			//Направление дороги
			osg::Vec3d direction;
			//Точки пристыковки геометрии перекрестка и страйпа дороги
			osg::Vec3d leftPoint, rightPoint;
			//Допустимые типы дорог
			ed::vector<ed::string> availableRoads;
			//Полярный угол якоря в плоскости XZ, нужен для оператора сравнения
			double angle;

			/**
			 * Оператор сравнения по полярному углу
			 */
			bool operator < (const lAnchor& rhs) const {return (angle < rhs.angle);}

			/**
			 * Получить сцену с якорем, для отладки
			 * @param scene сцена, к которой добавить оффшор якоря
			 * @param name имя якоря
			 * @param offset смещение
			 */
			EDTERRAIN_API void getOffshore(offshore::Scene& scene, const ed::string& name = "anchor", const osg::Vec3d& offset = osg::Vec3d(0.0, 0.0, 0.0));
		};

		/**
		 * Структура, описывающая связь двух якорей
		 */
		struct lAnchorLink
		{
			//Трансформ перекрестка, нужен для указания мест ошибок
			osg::Matrixd crosstypeTransform;

			//Якорь начала связи
			int fromAnchor;
			//Якорь конца связи
			int toAnchor;

			//Точки связи
			ed::vector<osg::Vec3d> vertices;

			double velocity;

			lAnchorLink() {};
			lAnchorLink(int fromAnchor, int toAnchor) : fromAnchor(fromAnchor), toAnchor(toAnchor) {}

			/**
			 * Задать связь между якорями
			 * @param anchors список якорей
			 * @param vertices точки связи
			 * @param delta абсолютная погрешность измерения расстояния
			 */
			void set(const ed::vector<lAnchor>& anchors, const ed::vector<osg::Vec3d>& vertices, double delta = 1.0);
		};

		//Индекс ноды типа перекрестка в сцене типов дорог
		int nodeIndex;

		//Имя типа перекрестка
		ed::string name;

		//Фиктивный ли перекресток
		bool dummy;

		//Перекресток ориентированный или нет
		bool oriented;

		//Участвовать ли в пересечении мешей
		bool findMeshesIntersection;

		//Направление перекрестка
		osg::Vec3d direction;

		//Якоря типа перекрестка
		ed::vector<lAnchor> anchors;

		//Связи между якорями
		ed::vector<lAnchorLink> anchorLinks;

		//Вырезаный кусок с геометрией. Для экспорта
		offshore::Scene scene;

		//Вырезанный кусок с геометрией scene objects
		offshore::Scene sceneObjects;

	public:
		/**
		 * Подобрать наиболее подходящее положение перекрестка
		 * @param splinesInfo информация о сплайнах входящих в сплайн
		 * @param transform трансформ, который надо применить к перекрестку
		 * @param error ошибка соответствия перекрестка
		 * @param anchorsToSplines какой сплайн пристыкован к якорю: j = anchorsToSplines[i] - означает, что к i-ому якорю пристыковае j-ый сплайн
		 * @return true если перекресток соответсвует сплайнам
		 */
		bool fitSplines(const ed::vector<SplineInfo>& splinesInfo, osg::Matrixd& transform, double& error, ed::vector<int>& anchorsToSplines);
	};

	/**
	 * Тип моста
	 */
	struct lBridgeType
	{
		//Индекс ноды типа моста в сцене типов дорог
		int nodeIndex;

		//Имя типа моста
		ed::string name;

		double velocity;

		//Тип моста
		ed::vector<ed::string> availableRoads;

		//Осевая линия моста
		offshore::Spline centerline;

		//Вырезанный кусок с геометрией scene objects
		offshore::Scene sceneObjects;

		// Меш моста для вьюера
		offshore::Mesh bridgemesh;

		//Точки пристыковки дорог к мосту
		roads4::StripeAnchoringPoints anchoringPoints;

		//Дополнительный рельеф земли
		offshore::Scene surfaceScene;

		/**
		 * Проверить, можно ли разместить мост на дороге
		 */
		EDTERRAIN_API bool canPlace(const ed::string& road) const;
	};

public:
	/**
	 * Типы городов и полей
	 */
	struct lTownType
	{
		//Индекс ноды типа в сцене типов дорог
		int nodeIndex;
		//Имя типа
		ed::string name;
		//Матрица планарного мапинга
		osg::Matrixd mapping;
		//Меш шаблона
		offshore::Mesh mesh;
		offshore::Material material;
		ed::map<ed::string, ed::string> meshparams;
	};

public:
	/**
	 * Экземпляр перекрестка
	 */
	struct lCrossInstance
	{
		//Тип перекрестка
		lCrossType* crossType;
		//Каким якорям перекрестка соответствуют какие сплайны
		ed::vector<int> anchorsToSplines;
		//Трансформ перекрестка
		osg::Matrixd transform;
		//Ошибка соответсвия перекрестка
		double error;

		lCrossInstance() : crossType(NULL) {};

		/**
		 * Является ли перекресток фиктивным
		 */
		bool isDummy() const {return ((crossType == NULL) ? true : crossType->dummy);};
	};

private:
	//Выводить ли информацию о ходе выполнения
	bool verbose;

	//Сцена с типами дорог
	offshore::Scene roadTypeScene;

	//Типы дорог
	ed::map<ed::string, lRoadType> roadTypes;

	//Типы перекрестков
	ed::list<lCrossType> crossTypes;

	//Типы мостов
	ed::map<ed::string, lBridgeType> bridgeTypes;

	//Геометрия дорожных деталей
	ed::list<lRoadDetailGeometry> roadDetailGeometries;
	ed::map<ed::string, lRoadDetailGeometry*> roadDetailGeometriesByName;

	//Типы городов
	ed::map<ed::string, lTownType> townTypes;
	//Типы полей
	ed::map<ed::string, lTownType> fieldTypes;

public:
	lRoadsTypes(bool verbose = false) : verbose(verbose) {};

	/**
	 * Открыть оффшор с типами и парсить их
	 */
	EDTERRAIN_API void init(const ed::string& roadTypesFile);

	/**
	 * Получить тип дороги по имени
	 */
	EDTERRAIN_API lRoadType* getRoadType(const ed::string& roadType);

	/**
	 * Подобрать тип перекрестка
	 * @param scene сцена со сплайнами
	 * @param position координаты перекрестка
	 * @param splines индексы сплайнов, входящих в перекресток
	 */
	EDTERRAIN_API lCrossInstance getCrossType(offshore::Scene& scene, const osg::Vec3d& position, const ed::vector<int>& splines);
	EDTERRAIN_API lCrossInstance getRoadToRoadDummyCrossType(const osg::Vec3d& position, const ed::vector<int>& splines);
	EDTERRAIN_API lCrossInstance getEndRoadDummyCrossType(const osg::Vec3d& position, int spline);
	EDTERRAIN_API lCrossInstance getEmptyCrossType() {return lCrossInstance();};

	/**
	 * Получить тип моста
	 */
	EDTERRAIN_API lBridgeType* getBridgeType(offshore::Scene& scene, int node);
	EDTERRAIN_API lBridgeType* getBridgeType(const ed::string& type);

	/**
	 * Получить геометрию дорожной детали по ее имени
	 */
	EDTERRAIN_API lRoadDetailGeometry* getRoadDetailGeometry(const ed::string& name);

	/**
	 * get town/field type
	 */
	EDTERRAIN_API lTownType* getTownType(const ed::string& type);
	EDTERRAIN_API lTownType* getFieldType(const ed::string& type);


	// Get all road types. For editor
	const ed::map<ed::string, lRoadType>& getRoadTypes() const { return roadTypes;}

	// Get all bridge types. For editor
	const ed::map<ed::string, lBridgeType>& getBridgeTypes()const { return bridgeTypes;}
private:

	/**
	 * Построить образец расстановки деталей вдоль дороги
	 * @param line линия, относительно которой размещены детали
	 * @param details список дорожных объектов
	 * @param waysides список придорожных полос
	 * @param sample полученный образец
	 */
	void buildRoadDetailsSample(const offshore::Spline& line, const ed::list<lRoadDetail>& details, const ed::list<lRoadWayside>& waysides, lRoadDetailsSample& sample);

	/**
	 * Получить информацию о сплайнах входящих в перекресток
	 * Выходные данные отсортированны по полярному углу
	 * @param scene сцена со сплайнами
	 * @param position координаты перекрестка
	 * @param splines индексы сплайнов в сцене
	 * @param splinesInfo выходные данные
	 * @return true если входные данные корректны и успешно обработаны
	 */
	bool getSplinesInfo(offshore::Scene& scene, const osg::Vec3d& position, const ed::vector<int>& splines, ed::vector<SplineInfo>& splinesInfo);

	/**
	 * Получить параметр из списка параметров (not case-sensitive)
	 * @param params все параметры
	 * @param param нужный нам параметр
	 * @param value полученное значение
	 */
	template <typename T> bool getParam(const ed::map<ed::string, ed::string>& params, const ed::string& param, T& value);

	/**
	 * Парсить тип дороги
	 */
	void parseRoadType(int node);

	/**
	 * Парсить тип перекрестка
	 */
	void parseCrossType(int node);

	/**
	 * Парсить тип моста
	 */
	void parseBridgeType(int node);

	/**
	 * Парсить якорь
	 */
	void parseAnchor(int node, lCrossType::lAnchor& anchor);

	/**
	 * Парсить геометрию дорожной детали
	 */
	void parseRoadDetailGeometry(int node);

	/**
	 * Парсить тип города
	 */
	void parseTownType(int node, lTownType& towntype);

	/**
	 * Построить фиктивные перекрестки
	 */
	void buildFictiveCrossroads();
};

template <> bool lRoadsTypes::getParam<bool>(const ed::map<ed::string, ed::string>& params, const ed::string& param, bool& value);
template <> bool lRoadsTypes::getParam<ed::string>(const ed::map<ed::string, ed::string>& params, const ed::string& param, ed::string& value);

}

#endif
