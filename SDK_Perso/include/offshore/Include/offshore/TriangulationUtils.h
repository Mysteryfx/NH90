#ifndef TRIANGULATIONUTILS_H
#define TRIANGULATIONUTILS_H

#include <osg/Vec3d>
#include <ed/vector.h>
#include <ed/set.h>
#include "scene.h"

namespace offshore
{
	//================================================================================
	// constrainedDelaunayTriangulation
	//    Триангуляция Делоне с ограничениями.
	//
	//    vertices    - входные вершины
	//    constraints - "жёсткие" рёбра
	//    output      - выходная триангуляци
	//    errors      - ошибки
	//================================================================================
	OFFSHORE_API bool constrainedDelaunayTriangulation(
			ed::vector<osg::Vec3d> &vertices, ed::vector<offshore::Edge32> &constraints,
			ed::vector<offshore::Polygon> &output,
			ed::vector<offshore::Notification>* notifications = NULL);

	OFFSHORE_API bool constrainedDelaunayTriangulation(
			ed::vector<osg::Vec3d> &vertices, ed::vector<offshore::Edge32> &constraints,
			ed::vector<offshore::Face> &output,
			ed::vector<offshore::Notification>* notifications = NULL);

	//================================================================================
	// constrainedDelaunayTriangulationMerge
	//    Триангуляция Делоне с ограничениями. Merge версия предварительно мержит точки.
	//
	//    vertices    - входные вершины
	//    constraints - "жёсткие" рёбра
	//    output      - выходная триангуляци
	//    threshold   - расстояние с которого начинать мержить точки
	//    errors      - ошибки
	//================================================================================
	OFFSHORE_API bool constrainedDelaunayTriangulationMerge(
			const ed::vector<osg::Vec3d> &vertices,
			const ed::vector<offshore::Edge32> &constraints, 
			ed::vector<offshore::Face> &output,
			double threshold2d, double threshold3d=0,
			ed::vector<offshore::Notification>* notifications=0, bool ignoreErrors = false,
			ed::set<int>* bordervertex=0);

	OFFSHORE_API bool constrainedDelaunayTriangulationMerge(
			const ed::vector<osg::Vec3d> &vertices,
			const ed::vector<offshore::Edge32> &constraints,
			ed::vector<offshore::Polygon> &output,
			double threshold2d, double threshold3d=0,
			ed::vector<offshore::Notification>* notifications=0, bool ignoreErrors = false,
			ed::set<int>* bordervertex=0);

	/**
	 * Триангуляция полигона
	 * @param vertices вершины
	 * @param openVertices индексы внешних вершин полигона в обходе по часовой стрелке
	 * @param output триангуляция
	 * @param nots сообщения об ошибках
	 * @return true если триангуляция удалась, false в противном случае
	 */
	OFFSHORE_API bool triangulatePolygon(
		ed::vector<osg::Vec3d>& vertices, 
		ed::vector<int>& openVertices, 
		ed::vector<offshore::Polygon>& output,
		ed::vector<offshore::Notification>* nots = nullptr
		);

	OFFSHORE_API bool triangulatePolygon(
		ed::vector<osg::Vec3d>& vertices, 
		ed::vector<int>& openVertices, 
		ed::vector<offshore::Face>& output,
		ed::vector<offshore::Notification>* nots = nullptr
		);

}

#endif