#ifndef LANDSCAPE4_LROADNETWORK_H
#define LANDSCAPE4_LROADNETWORK_H

#include "../landscape3_config.h"

#include "edTerrain.h"

#include "landscape4/lRoad.h"
#include "landscape4/lCrossroad.h"

#include "osg/Vec3d"

#include "osg/RectQuadtree.h"

#ifdef EDGE
	#include "Core/io/STLSerializer.h"
#else
	#include "io/STLSerializer.h"
#endif

namespace roads4
{
	/**
	 * Структура, описывающая точку на дороге
	 */
	struct lPointOnRoad
	{
		//Индекс дороги в дорожной сети
		int32_t road;
		//Параметр точки на дороге
		double param;
	};

	/**
	 * Sink для определения возможности движения по дороге и перекрестку
	 */
	struct ICheckRoadSink
	{
		/**
		 * Можно ли двигаться по дороге
		 * @param road индекс дороги в дорожной сети
		 * @param startParam параметр на дороге, задающий точку, с которой начинаем движение
		 * @param endParam параметр на дороге, задающий точку, на которой заканчиваем движение
		 * @param return true если можно двигаться
		 */
		virtual bool canMoveAlongRoad(int32_t road, double startParam, double endParam) = 0;

		/**
		 * Можно ли двигаться по перекрестку
		 * @param crossroad индекс перекрестка в дорожной сети
		 * @param startRoad индекс дороги, с которой начинаем движение по перекрестку
		 * @param endRoad индекс дороги, на которой заканчиваем движение по перекрестку
		 */
		virtual bool canMoveAlongCrossroad(int32_t crossroad, int32_t startRoad, int32_t endRoad) = 0;

		/**
		 * Можно ли брать точку на дороге
		 * @param road индекс дороги
		 */
		virtual bool canGetPointOnRoad(int32_t road) = 0;
	};

	struct lRoadNetwork
	{
		struct RoadGraphNode
		{
			//Вершина, описывает точку входа дороги в перекресток
			int32_t crossroad;
			int32_t road;

			//Соседние вершины, first - индекс ноды, second - расстояние до нее
			ed::map<int32_t, int32_t> neighbours;

			RoadGraphNode(int32_t crossroad = -1, int32_t road = -1) : crossroad(crossroad), road(road) {}

			void addNeighbour(int32_t node, double length) {neighbours[node] = length;}
		};

		struct lRoadSegment
		{
			int32_t road;
			int32_t segment;

			lRoadSegment(int32_t road = -1, int32_t segment = -1) : road(road), segment(segment) {}
		};

		//Типы дорог
		ed::vector<ed::string> roadTypes;
		//Дорожные полосы
		ed::vector<lRoadTrafficLanes> directTrafficLanes;
		ed::vector<lRoadTrafficLanes> backTrafficLanes;

		//Ноды графа дороги
		ed::vector<RoadGraphNode> nodes;

		ed::vector<lRoad> roads;

		//Перекрестки
		ed::vector<lCrossroad> crossroads;
		//Ноды графа дороги, соответствующие перекрестку
		ed::vector<ed::vector<int32_t> > crossroadNodes;

		ed::RectQuadtree<lRoadSegment> segmentsTree;

		EDTERRAIN_API bool crossroadExists(int32_t index);
		EDTERRAIN_API bool roadExists(int32_t);

		EDTERRAIN_API int8_t getRoadType(const ed::string& road) const;

		EDTERRAIN_API lRoad* getRoadFromPointToPoint(const lPointOnRoad& startPoint, const lPointOnRoad& endPoint, double velocity, double error = 0.01, ICheckRoadSink* sink = NULL);
		
		EDTERRAIN_API int32_t addRoad(const lRoad& road = lRoad());
		EDTERRAIN_API int32_t addCrossroad(const lCrossroad& crossroad = lCrossroad());
		EDTERRAIN_API int32_t addBridge(const lCrossroad& bridge = lCrossroad(true)) {return this->addCrossroad(bridge);};

		EDTERRAIN_API int8_t addRoadType(const ed::string& name, const lRoadTrafficLanes& direct = lRoadTrafficLanes(), const lRoadTrafficLanes& back = lRoadTrafficLanes());

		EDTERRAIN_API bool findClosestPoint(const osg::Vec3d& point, lPointOnRoad& closestPoint, ICheckRoadSink* sink = NULL, double startDistance = 10000.0);
	
		EDTERRAIN_API osg::Vec3d getPoint(const lPointOnRoad& point) const;
		EDTERRAIN_API osg::Vec3d getControlVertex(const lPointOnRoad& point) const;

		EDTERRAIN_API void buildRoadGraph();

		virtual ed::string getType() {return "landscape4::lRoadNetwork";}

		EDTERRAIN_API bool save(const char* filename);
		EDTERRAIN_API bool save(const ed::string& filename);
		EDTERRAIN_API bool load(io::MmfStream& stream);
		EDTERRAIN_API bool load(const char* filename);
		EDTERRAIN_API bool load(const ed::string& filename);

	private:
		bool serialize(io::MmfStream& stream);
		
		void dump(const ed::vector<double>& weights, const ed::string& file);

		/**
		 * Добавить в граф вершину, которая соответсвует точке стыка дороги и перекрестка
		 * @param crossroad перекресток
		 * @param road дорога
		 */
		void addRoadGraphNode(int32_t crossroad, int32_t road);

		/**
		 * Получить вершину графа, которая соответсвует точке стыка дороги и перекрестка
		 * @param crossroad перекресток
		 * @param road дорога
		 * @returns -1 если такой ноды нет; и ее номер, если такая нода есть
		 */
		int32_t getRoadGraphNode(int32_t crossroad, int32_t road);

		/**
		 * Добавить в граф ребро (направленное и с весом)
		 * @param fromNode начало ребра
		 * @param toNode конец ребра
		 * @param length длина ребра
		 */
		void addRoadGraphEdge(int32_t fromNode, int32_t toNode, double length);

		/**
		 * Можно ли проехать от от одной вершины графа к другой
		 * @param fromNode стартовая вершина
		 * @param toNode конечная вершина
		 * @param roadType тип дороги, по которой ищем путь
		 * @param sink для определения возможности движения по дороге и перекрестку
		 */
		bool canMoveFromNodeToNode(int32_t fromNode, int32_t toNode, ICheckRoadSink* sink);
	};
}

template <typename S> S& operator >>(S& stream, roads4::lRoadNetwork::lRoadSegment& segment)
{
	uint32_t version = 1;
	stream >> version;

	if (version >= 1)
	{
		stream >> segment.road;
		stream >> segment.segment;
	}

	return stream;
}

template <typename S> S& operator >>(S& stream, roads4::lRoadNetwork::RoadGraphNode& node)
{
	uint32_t version = 1;
	stream >> version;

	if (version >= 1)
	{
		stream >> node.crossroad;
		stream >> node.road;
		stream >> node.neighbours;
	}

	return stream;
}

#endif