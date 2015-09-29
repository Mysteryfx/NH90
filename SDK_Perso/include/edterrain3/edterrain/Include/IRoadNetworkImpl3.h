#ifndef ED_IRoadNetworkImpl3_H
#define ED_IRoadNetworkImpl3_H

#include "edTerrain/IRoadNetwork.h"
#include "Paths/lPath.h"


class IRoadNetworkImpl3 : public edterrain::IRoadNetwork
{
public:
	IRoadNetworkImpl3(path::PathNetwork* pathNetwork);

	virtual void release();

	virtual int roadsCount();
	virtual edterrain::IRoute* getRoad(int i);

	virtual int crossroadsCount();
	virtual edterrain::ICrossroad* getCrossroad(int i);

	virtual int chainCount();
	virtual ed::vector<int> getChain(int i);
	virtual ed::string getChainName(int chain, int i);

	virtual bool getPoint(const edterrain::PointOnRoad& pointOnRoad, osg::Vec3d& point);

	// Ближайшая точка
	bool findClosestPoint(const osg::Vec3d& point, edterrain::PointOnRoad& closestPoint,
		edterrain::ICheckRoadSink* sink = NULL, double startDistance = 10000.0);

	// Найти путь
	edterrain::IRoute* createRoute(const edterrain::PointOnRoad& startPoint, const edterrain::PointOnRoad& endPoint,
		edterrain::ICheckRoadSink* sink = nullptr);


	// дорога по индексу, включая построенные пути
	NM_ROAD::Road* getRoad3(int index);
	int getRoadIndex3(NM_ROAD::Road* road);

	void releasePath(int index);

	// НАДО ПРОВЕРИТЬ
	void convertToPointOnRoad(const path::PointOnPath& pointOnPath, edterrain::PointOnRoad& pointOnRoad);
	void convertToPointOnRoad(const roads3::PointOnRoad& pointOnRoad3, edterrain::PointOnRoad& pointOnRoad);
	bool convertToPointOnRoad3(const edterrain::PointOnRoad& pointOnRoad, roads3::PointOnRoad& pointOnRoad3);
	// NOT implemented
	void convertToPointOnPath(const edterrain::PointOnRoad& pointOnRoad, path::PointOnPath& pointOnPath);

private:
	path::PathNetwork *pathNetwork;

	/*
	 * Храним здесь же построенные пути.
	 */
	int nextPathIndex; // индекс для следующего пути
	ed::map<int, NM_ROAD::Road*> pathes;
	ed::map<NM_ROAD::Road*, int> pathesIndecies; // обратная связь
};


#endif // ED_IRoadNetworkImpl3_H
