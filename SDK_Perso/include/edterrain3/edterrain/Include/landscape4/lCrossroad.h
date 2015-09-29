#ifndef LANDSCAPE4_LCROSSROAD_H
#define LANDSCAPE4_LCROSSROAD_H

#include "../landscape3_config.h"

#include "edTerrain.h"
#include "osg/Vec3d"
#include "osg/Matrixd"

#ifdef EDGE
	#include "Core/io/STLSerializer.h"
#else
	#include "io/STLSerializer.h"
#endif

namespace roads4
{
	struct lCrossroad
	{
		struct lRoadConnection
		{
			int32_t road1;
			int32_t road2;

			ed::vector<osg::Vec3d> vertices;

			double velocity;

			lRoadConnection() {};
			lRoadConnection(int32_t road1, int32_t road2, const ed::vector<osg::Vec3d>& vertices, double velocity)
				: road1(road1), road2(road2), vertices(vertices), velocity(velocity) {}

			void multiply(const osg::Matrixd& transform)
			{
				for (size_t i = 0; i < vertices.size(); i++)
					vertices[i] = vertices[i] * transform;
			}
		};

		ed::vector<int32_t> roads;
		ed::vector<bool> directions;

		bool bridge;

		ed::vector<lRoadConnection> roadConnections;

		EDTERRAIN_API lCrossroad(bool bridge = false) : bridge(bridge) {}
		EDTERRAIN_API lCrossroad(const ed::vector<int32_t>& roads, const ed::vector<bool>& directions, bool bridge = false) : roads(roads), directions(directions), bridge(bridge) {};

		EDTERRAIN_API int32_t addRoadConnection(int32_t inGoingRoad, int32_t outGoingRoad, const ed::vector<osg::Vec3d>& vertices, double velocity);
		
		EDTERRAIN_API void getPossibleOutGoingRoads(int32_t inGoingRoad, ed::vector<int32_t>& outGoingRoads);
		
		EDTERRAIN_API int32_t getRoadIndex(int32_t road);

		EDTERRAIN_API bool getRoadDirection(int32_t road);

		EDTERRAIN_API int32_t getRoadConnection(int32_t inGoingRoad, int32_t outGoingRoad, bool& reversed);
	};
}

template <class S> S& operator >>(S& stream, roads4::lCrossroad::lRoadConnection& roadConnection)
{
	uint32_t version = 0;
	stream >> version;

	if (version >= 0)
	{
		stream >> roadConnection.road1;
		stream >> roadConnection.road2;
		stream >> roadConnection.vertices;
		stream >> roadConnection.velocity;
	}

	return stream;
}

template <class S> S& operator >>(S& stream, roads4::lCrossroad& crossroad)
{
	uint32_t version = 0;
	stream >> version;

	if (version >= 0)
	{
		stream >> crossroad.roads;
		stream >> crossroad.directions;
		stream >> crossroad.roadConnections;
		stream >> crossroad.bridge;
	}

	return stream;
}

#endif