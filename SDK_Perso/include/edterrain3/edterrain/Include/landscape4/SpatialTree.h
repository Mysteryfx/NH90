#ifndef SPATIAL_TREE_H
#define SPATIAL_TREE_H

#include <queue>
#include <osg/Polytope>
#include <osg/BoundingBox>

#ifdef EDGE
//	#include "Core/io/STLSerializer.h"
	#include "Core/io/mmfStream.h"
#else
//	#include "io/STLSerializer.h"
	#include "io/mmfStream.h"
#endif

namespace landscape4
{

template <typename T> class SpatialTree
{
public:

	struct ISink
	{
		virtual void onQuery(const T& object, const osg::BoundingBox& bbox) = 0;
	};

	// т.к. push_back() у io::mmfvector нет, то при insert() сначала все складываем в
	// ed::vector и потом уже при вызове build() копируем все в io::mmfvector
	ed::vector<T> awaitingObjects;
	ed::vector<osg::BoundingBox> awaitingBoxes;

	// Временные ноды, используются во время построения дерева
	class TempNode
	{
	public:

		const static int SUBDIVIDE_NODE_OBJECT_COUNT = 50;

		const ed::vector<T>& objects;
		const ed::vector<osg::BoundingBox>& boxes;

		// Два бокса этой ноды
		osg::BoundingBox bbox;
		osg::BoundingBox expandedBbox;
		
		// Объекты, лежащие в этой ноде
		ed::vector<uint32_t> indicies;

		// Чайлды
		TempNode* childs[4];

		/**
		 * Конструктор ноды дерева
		 */
		TempNode(const ed::vector<T>& objects, const ed::vector<osg::BoundingBox>& boxes, const osg::Vec3d& center, double size, double ysize);

		/**
		 * Деструктор ноды дерева. Удаляет чайлдов
		 */
		~TempNode();

		/**
		 * Количество нод в поддереве, корнем которого является эта нода
		 */
		int nodeCount();

		/**
		 * Вставка в ноду объекта с индексом objectIndex, при необходимости подразбивает ноду
		 */
		void insert(uint32_t objectIndex);

		/**
		 * Подразбить ноду
		 */
		void subdivide();
	};

	// Корневая нода дерева
	TempNode* root;

	class Node
	{
	public:

		osg::BoundingBox expandedBbox;

		// объекты
		int32_t objectsOffset;
		int32_t objectsCount;

		// Индексы чайлдов
		int32_t childs[4];
	};

	io::mmfvector<T> objects;
	io::mmfvector<osg::BoundingBox> boxes;
	io::mmfvector<Node> nodes;

public:

	SpatialTree();
	~SpatialTree();

	/**
	 * Вставить объект в дерево
	 */
	void insert(const T& object, const osg::BoundingBox& bbox);

	/**
	 * Построить дерево. после этого добавлять новые объекты в него нельзя,
	 * только сериализовать и впоследствии делать выборки.
	 */ 
	void build();

	/**
	 * Пустое дерево
	 */ 
	bool empty() const;

	/**
	 * Выбрать объекты дерева, пересекающие фрастум, баундинг бокс, отрезок, или 2д точку (x, z)
	 */
	void query(const osg::Polytope& frustum, ISink* sink) const;
	void query(const osg::BoundingBox& box, ISink* sink) const;
	void query(const osg::Vec3d& a, const osg::Vec3d& b, ISink* sink) const;
	void query(const osg::Vec2d& p, ISink* sink) const;

	// хелперы
	void query(const osg::Polytope& frustum, ed::vector<T>& queriedObjects);
	void query(const osg::BoundingBox& frustum, ed::vector<T>& queriedObjects);
	void query(const osg::Vec3d& a, const osg::Vec3d& b, ed::vector<T>& queriedObjects);

	/**
	 * Сериализация дерева. Через метод, чтоб не тащить все в паблик для <<
	 */
	template <typename S> void serialize(S& stream);

public:
	// Подсчет числа объектов в выборке
	struct CounterSink : ISink
	{
		int count;
		CounterSink() : count(0) {}
		virtual void onQuery(const T& t) { count++; }
	};

	// Сбор объектов в список
	struct CollectToArraySink : ISink
	{
		ed::vector<T>& queriedObjects;
		CollectToArraySink(ed::vector<T>& queriedObjects) : queriedObjects(queriedObjects) {};
		virtual void onQuery(const T& t, const osg::BoundingBox& bbox) { queriedObjects.push_back(t); }
	};

protected:
	void queryRecursive(const osg::Polytope& frustum, ISink* sink, int nodeIndex) const;
	void queryRecursive(const osg::BoundingBox& box, ISink* sink, int nodeIndex) const;
	void queryRecursive(const osg::Vec3d& a, const osg::Vec3d& b, ISink* sink, int nodeIndex) const;
	void queryRecursive(const osg::Vec2d& p, ISink* sink, int nodeIndex) const;
};

// implementation

template <typename T> SpatialTree<T>::SpatialTree() : root(NULL)
{
	//...
}

template <typename T> SpatialTree<T>::~SpatialTree()
{
	//...
}

/**
 * Вставить объект в дерево
 */
template <typename T> void SpatialTree<T>::insert(const T& object, const osg::BoundingBox& bbox)
{
	awaitingObjects.push_back(object);
	awaitingBoxes.push_back(bbox);
}

/**
 * Построить дерево. после этого добавлять новые объекты в него нельзя,
 * только сериализовать и впоследствии делать выборки.
 */ 
template <typename T> void SpatialTree<T>::build()
{
	// Баундом корневой ноды будет общий баунд всех объектов в дереве
	osg::BoundingBox rootNodeBox;
	for (size_t i = 0; i < awaitingBoxes.size(); i++)
		rootNodeBox.expandBy(awaitingBoxes[i]);

	// В OXZ боксы должны быть квадратными
	double size  = std::max(rootNodeBox.xMax() - rootNodeBox.xMin(), rootNodeBox.zMax() - rootNodeBox.zMin()) / 2.0;
	double ysize = (rootNodeBox.yMax() - rootNodeBox.yMin()) / 2.0;

	// Сначала создаем временное дерево "на поинтерах", а потом сохраняем его в mmf вектора
	root = new TempNode(awaitingObjects, awaitingBoxes, rootNodeBox.center(), size, ysize);

	// Добавляем элементы во временное дерево
	for (size_t i = 0; i < awaitingBoxes.size(); i++)
		root->insert(i);

	// Ресайзим mmf вектора
	nodes.resize(root->nodeCount());
	objects.resize(awaitingObjects.size());
	boxes.resize(awaitingBoxes.size());

	// Сгружаем данные в mmf вектора
	int nodeIndex = 0, objectIndex = 0;

	std::queue<TempNode*> nodeq; nodeq.push(root);
	while (!nodeq.empty())
	{
		TempNode* node = nodeq.front(); nodeq.pop();
		
		// Сохраняем ноду дерева
		nodes[nodeIndex].expandedBbox = node->expandedBbox;
		
		for (int i = 0; i < 4; i++)
		{
			if (node->childs[0] != NULL)
			{
				nodeq.push(node->childs[i]);
				nodes[nodeIndex].childs[i] = nodeIndex + nodeq.size();
			}
			else
			{
				nodes[nodeIndex].childs[i] = -1;
			}
		}

		// Сохраняем объекты, лежащие в этой ноде
		nodes[nodeIndex].objectsOffset = objectIndex;

		for (size_t i = 0; i < node->indicies.size(); i++)
		{
			objects[objectIndex] = awaitingObjects[ node->indicies[i] ];
			boxes[objectIndex]   = awaitingBoxes[ node->indicies[i] ];

			objectIndex++;
		}

		nodes[nodeIndex].objectsCount = objectIndex - nodes[nodeIndex].objectsOffset;
		nodeIndex++;
	}

	// Освобождаем память
	awaitingObjects.clear();
	awaitingBoxes.clear();

	delete root;
	root = NULL;
}

/**
 * Пустое дерево
 */ 
template <typename T> bool SpatialTree<T>::empty() const 
{
	return this->objects.empty();
}

/**
 * Конструктор ноды дерева
 */
template <typename T> SpatialTree<T>::TempNode::TempNode(const ed::vector<T>& objects, const ed::vector<osg::BoundingBox>& boxes, const osg::Vec3d& center, double size, double ysize)
	: objects(objects), boxes(boxes), bbox(bbox) 
{
	childs[0] = childs[1] = childs[2] = childs[3] = NULL;

	// Рассчитываем баунды
	bbox.set(center.x() - size, center.y() - ysize, center.z() - size,
		     center.x() + size, center.y() + ysize, center.z() + size);

	size *= 1.1;

	expandedBbox.set(center.x() - size, center.y() - ysize, center.z() - size,
		             center.x() + size, center.y() + ysize, center.z() + size);
}

/**
 * Деструктор ноды дерева. Удаляет чайлдов
 */
template <typename T> SpatialTree<T>::TempNode::~TempNode()
{
	if (childs[0] != NULL)
	{
		for (int i = 0; i < 4; i++)
			delete childs[i];
	}
}

/**
 * Количество нод в поддереве, корнем которого является эта нода
 */
template <typename T> int SpatialTree<T>::TempNode::nodeCount()
{
	int result = 1;

	if (childs[0] != NULL)
	{
		for (int i = 0; i < 4; i++)
			result += childs[i]->nodeCount();
	}

	return result;
}


/**
 * Вставка в ноду объекта с индексом objectIndex, при необходимости подразбивает ноду
 */
template <typename T> void SpatialTree<T>::TempNode::insert(uint32_t objectIndex)
{
	if (childs[0] == NULL) // Чайлдов либо нет ни одного, либо есть все
	{
		// Если это лист, то вставляем в него объект
		indicies.push_back(objectIndex);

		// Если в листе собралось слишком много вершин, то подразбиваем его
		if (indicies.size() == SUBDIVIDE_NODE_OBJECT_COUNT)
			subdivide();
	}
	else
	{
		// Распихиваем по чайлдам
		for (int i = 0; i < 4; i++)
		{
			if (childs[i]->expandedBbox.contains(boxes[objectIndex]))
			{
				childs[i]->insert(objectIndex);
				return;
			}
		}

		// Если объект не попал ни в одного чайлда, то оставляем его в этой ноде
		indicies.push_back(objectIndex);
	}
}

/**
 * Подразбить ноду
 */
template <typename T> void SpatialTree<T>::TempNode::subdivide()
{
	osg::Vec3d center = bbox.center();
	double hsize = (bbox.xMax() - bbox.xMin()) / 4.0;
	double ysize = (bbox.yMax() - bbox.yMin()) / 4.0;

	childs[0] = new TempNode(objects, boxes, osg::Vec3d(center.x() - hsize, center.y(), center.z() - hsize), hsize, ysize);
	childs[1] = new TempNode(objects, boxes, osg::Vec3d(center.x() + hsize, center.y(), center.z() - hsize), hsize, ysize);
	childs[2] = new TempNode(objects, boxes, osg::Vec3d(center.x() - hsize, center.y(), center.z() + hsize), hsize, ysize);
	childs[3] = new TempNode(objects, boxes, osg::Vec3d(center.x() + hsize, center.y(), center.z() + hsize), hsize, ysize);
	
	// По-возможности раскидываем объекты данной ноды по чайлдам, но некоторые объекты могут остаться в этой ноде
	int newIndexCount = 0;
	for (size_t n = 0; n < indicies.size(); n++)
	{
		uint32_t objectIndex = indicies[n];

		bool inserted = false;

		for (int i = 0; i < 4; i++)
		{
			if (childs[i]->expandedBbox.contains(boxes[objectIndex]))
			{
				childs[i]->insert(objectIndex);
				inserted = true;
				break;
			}
		}

		if (!inserted)
			indicies[newIndexCount++] = objectIndex;
	}

	indicies.resize(newIndexCount);
}

/**
 * Выбрать объекты дерева, пересекающеися со фрастумом
 */
template <typename T> void SpatialTree<T>::query(const osg::Polytope& frustum, ISink* sink) const
{
	if (nodes.size() == 0)
		return;

	queryRecursive(frustum, sink, 0);
}

template <typename T> void SpatialTree<T>::queryRecursive(const osg::Polytope& frustum, ISink* sink, int nodeIndex) const
{
	const Node& node = nodes[nodeIndex]; 

	// Проверяем попадает ли нода во фрастум
	if (frustum.contains(node.expandedBbox))
	{
		// Проверям все объекты этой ноды на попадание в фрастум
		for (int i = 0; i < node.objectsCount; i++)
		{
			if (frustum.contains(boxes[node.objectsOffset + i]))
				sink->onQuery(objects[node.objectsOffset + i], boxes[node.objectsOffset + i]);
		}

		// Добавляем чайлду в очередь рассмотрения если они есть
		if (node.childs[0] != -1)
		{
			for (int i = 0; i < 4; i++)
				queryRecursive(frustum, sink, node.childs[i]);
		}
	}
}

/**
 * Выбрать объекты дерева, пересекающеися с боксом
 */
template <typename T> void SpatialTree<T>::query(const osg::BoundingBox& box, ISink* sink) const
{
	if (nodes.size() == 0)
		return;

	queryRecursive(box, sink, 0);
}

template <typename T> void SpatialTree<T>::queryRecursive(const osg::BoundingBox& box, ISink* sink, int nodeIndex) const
{
	const Node& node = nodes[nodeIndex];

	// Проверяем попадает ли нода во бокс
	if (box.intersects(node.expandedBbox))
	{
		// Проверям все объекты этой ноды на попадание в бокс
		for (int i = 0; i < node.objectsCount; i++)
		{
			if (box.intersects(boxes[node.objectsOffset + i]))
				sink->onQuery(objects[node.objectsOffset + i], boxes[node.objectsOffset + i]);
		}

		// Добавляем чайлду в очередь рассмотрения если они есть
		if (node.childs[0] != -1)
		{
			for (int i = 0; i < 4; i++)
				queryRecursive(box, sink, node.childs[i]);
		}
	}
}

/**
 * Выбрать объекты дерева, пересекающие отрезок
 */
template <typename T> void SpatialTree<T>::query(const osg::Vec3d& a, const osg::Vec3d& b, ISink* sink) const
{
	if (nodes.size() == 0)
		return;

	queryRecursive(a, b, sink, 0);
}

template <typename T> void SpatialTree<T>::queryRecursive(const osg::Vec3d& a, const osg::Vec3d& b, ISink* sink, int nodeIndex) const
{
	const Node& node = nodes[nodeIndex];

	// Проверяем пересекает ли отрезок бокс ноды
	if (ed::boxSegmentIntersect(node.expandedBbox, a, b))
	{
		// Проверям все объекты этой ноды на пересечение с отрезком
		for (int i = 0; i < node.objectsCount; i++)
		{
			if (ed::boxSegmentIntersect(boxes[node.objectsOffset + i], a, b))
				sink->onQuery(objects[node.objectsOffset + i], boxes[node.objectsOffset + i]);
		}

		// Добавляем чайлду в очередь рассмотрения если они есть
		if (node.childs[0] != -1)
		{
			for (int i = 0; i < 4; i++)
				queryRecursive(a, b, sink, node.childs[i]);
		}
	}
}

/**
 * Выбрать объекты дерева, пересекающие 2D точку (x, z). y игнорируется
 */
template <typename T> void SpatialTree<T>::query(const osg::Vec2d& p, ISink* sink) const
{
	if (nodes.size() == 0)
		return;

	queryRecursive(p, sink, 0);
}

template <typename T> void SpatialTree<T>::queryRecursive(const osg::Vec2d& p, ISink* sink, int nodeIndex) const
{
	const Node& node = nodes[nodeIndex];

	// Проверяем пересекает ли отрезок бокс ноды
	if (node.expandedBbox.xMin() <= p.x() && p.x() <= node.expandedBbox.xMax() &&
		node.expandedBbox.zMin() <= p.y() && p.y() <= node.expandedBbox.zMax())
	{
		// Проверям все объекты этой ноды на пересечение с отрезком
		for (int i = 0; i < node.objectsCount; i++)
		{
			const osg::BoundingBox& nodeBox = boxes[node.objectsOffset + i];
			if (nodeBox.xMin() <= p.x() && p.x() <= nodeBox.xMax() &&
				nodeBox.zMin() <= p.y() && p.y() <= nodeBox.zMax())
			{
				sink->onQuery(objects[node.objectsOffset + i], nodeBox);
			}
		}

		// Добавляем чайлду в очередь рассмотрения если они есть
		if (node.childs[0] != -1)
		{
			for (int i = 0; i < 4; i++)
				queryRecursive(p, sink, node.childs[i]);
		}
	}
}

template <typename T> void SpatialTree<T>::query(const osg::Polytope& frustum, ed::vector<T>& queriedObjects)
{
	CollectToArraySink sink(queriedObjects);
	query(frustum, &sink);
}

template <typename T> void SpatialTree<T>::query(const osg::BoundingBox& box, ed::vector<T>& queriedObjects)
{
	CollectToArraySink sink(queriedObjects);
	query(box, &sink);
}

template <typename T> void SpatialTree<T>::query(const osg::Vec3d& a, const osg::Vec3d& b, ed::vector<T>& queriedObjects)
{
	CollectToArraySink sink(queriedObjects);
	query(a, b, &sink);
}

}

/*/
template <typename T> template <typename S> void SpatialTree<T>::serialize(S& stream)
{
	uint32_t version = 0;
	stream >> version;

	if (version >= 0)
	{
		stream >> objects;
		stream >> boxes;
		stream >> nodes;
	}
}

template <typename S, typename T> S& operator >> (S& stream, landscape4::SpatialTree<T>& tree)
{
	tree.serialize<S>(stream);
	return stream;
}
/*/

#endif
