#ifndef POINT_QUADTREE_H
#define POINT_QUADTREE_H

#include <osg/Vec3f>
#include <osg/Vec3d>
#include <osg/Polytope>
#include <osg/BoundingBox>

#include "io/STLSerializer.h"

// forward declarations
namespace ed { template <typename T> class PointQuadtree; }
template <typename S, typename K> S& operator >> (S& stream, ed::PointQuadtree<K>& tree);

namespace ed
{

template <typename T> class PointQuadtree
{
public:

	struct ISink
	{
		virtual void onQuery(const T& t) = 0;
	};

	PointQuadtree();
	~PointQuadtree();

	PointQuadtree(const PointQuadtree& copy);
	PointQuadtree& operator =(const PointQuadtree& obj);

	/**
	 * Вставить объект в дерево с четко заданной позицией
	 */
	void insert(const T& object, const osg::Vec3d& position);

	/**
	 * Очистить дерево
	 */
	void clear();

	/**
	 * Выбрать объекты, попадающих во фрастум
	 */
	void query(const osg::Polytope& frustum, ed::vector<T>& queriedObjects);
	
	/**
	 * Выбрать объекты, попадающие во фрастум или в баундинг бокс
	 */
	void query(const osg::Polytope& frustum, ISink* sink);
	void query(const osg::BoundingBox& box, ISink* sink);

	/**
	 * Высота дерева
	 */
	int height();

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
		virtual void onQuery(const T& t) { queriedObjects.push_back(t); }
	};

private:

	struct Object
	{
		T object;
		osg::Vec3d position; // Позиция объекта

		Object(const T& object = T(), const osg::Vec3d& position = osg::Vec3d()) : object(object), position(position) {}
	};

	struct Node
	{
		// Объекты
		const ed::vector<Object>& objects;
		ed::vector<int> objectIndicies;

		osg::Vec3d center;
		double size;
		osg::BoundingBox bbox;
		Node* childs[4];

		const static int SUBDIVIDE_NODE_OBJECT_COUNT = 50;
		
		Node(const ed::vector<Object>& objects, const osg::Vec3d& center, double size);
		~Node();

		/**
		 * Подразбиваем ноду, добавляя к ней 8 листьев
		 */
		void subdivide();

		/**
		 * Вставка объекта в ноду
		 */
		void insert(int objectIndex);

		/**
		 * Получить все объекты дерева, попадающих во фрастум и бокс
		 */
		void query(const osg::Polytope& frustum, ISink* sink);
		void query(const osg::BoundingBox& box, ISink* sink);

		/**
		 * высота поддерева этой ноды
		 */
		int height();
	};

	Node* root;

	// Все объекты дерева
	int insertedNodes;
	ed::vector<Object> objects;

	/**
	 * Вставить в дерево ноды, ожидающие вставки
	 */
	void insertAwaitingNodes();

	// Сериализация
	template <typename S, typename K> friend S& ::operator >> (S& stream, ed::PointQuadtree<K>& tree);
	template <typename S> void serializeObjectList(S& stream);
	template <typename S> void serializeNode(S& stream, typename ed::PointQuadtree<T>::Node*& node);
};

//реализация

//BoundingBoxOctree methods
template <typename T> 
PointQuadtree<T>::PointQuadtree() : root(NULL), insertedNodes(0)
{
}

template <typename T> 
PointQuadtree<T>::~PointQuadtree()
{
	clear();
}

template <typename T> 
PointQuadtree<T>::PointQuadtree(const PointQuadtree& copy)
{	
	this->root = NULL;

	for (auto it = copy.objects.begin(); it != copy.objects.end(); ++it)
	{
		this->insert(it->object, it->bbox);
	}
}

template <typename T> 
PointQuadtree<T>& PointQuadtree<T>::operator = (const PointQuadtree& obj)
{
	this->clear();

	for (auto it = obj.objects.begin(); it != obj.objects.end(); ++it)
	{
		this->insert(it->object, it->bbox);
	}

	return *this;
}

template <typename T> 
void PointQuadtree<T>::clear()
{
	delete root;
	root = NULL;
	objects.clear();
}

template <typename T> 
void PointQuadtree<T>::insert(const T& object, const osg::Vec3d& position)
{
	objects.push_back(Object(object, position));
}

template <typename T> 
void PointQuadtree<T>::insertAwaitingNodes()
{
	for (; insertedNodes < objects.size(); insertedNodes++)
	{
		// Создаем корень если его нет
		if (root == NULL)
		{
			osg::BoundingBox bbox;

			for (int i = insertedNodes; i < objects.size(); i++)
				bbox.expandBy(objects[i].position);

			root = new Node(objects, bbox.center(), bbox.radius() / sqrt(3.0));
		}

		// Баундинг бокс за пределами текущего корня
		osg::Vec3d& pt = objects[insertedNodes].position;
		while (!root->bbox.contains(pt))
		{
			unsigned int cornerBitmask = 0;

			cornerBitmask |= root->center.x() < pt.x() ? 1 : 0; 
			cornerBitmask |= root->center.z() < pt.z() ? 6 : 0;

			osg::Vec3d newRootCenter = root->bbox.corner(cornerBitmask);
			
			newRootCenter.y() = root->center.y();
			cornerBitmask &= 0x03;

			// Создаем новый более большой корень
			Node* newRoot = new Node(objects, newRootCenter, 2.0 * root->size);

			// Добавляем в нем листья и один из них заменяем на текущий корень
			newRoot->subdivide();
			delete newRoot->childs[cornerBitmask];
			newRoot->childs[cornerBitmask] = root;

			root = newRoot;
		}

		root->insert(insertedNodes);
	}
}

template <typename T> 
void PointQuadtree<T>::query(const osg::Polytope& frustum, ed::vector<T>& queriedObjects)
{
	CollectToArraySink sink(queriedObjects);
	query(frustum, &sink);
}

template <typename T> 
void PointQuadtree<T>::query(const osg::Polytope& frustum, ISink* sink)
{
	insertAwaitingNodes();

	if (root != NULL)
		root->query(frustum, sink);
}

template <typename T> 
void PointQuadtree<T>::query(const osg::BoundingBox& box, ISink* sink)
{
	insertAwaitingNodes();

	if (root != NULL)
		root->query(box, sink);
}

template <typename T> 
int PointQuadtree<T>::height()
{
	return root != NULL ? root->height() : 0; 
}

//node methods
template <typename T>
PointQuadtree<T>::Node::Node(const ed::vector<Object>& objects, const osg::Vec3d& center, double size) : objects(objects), center(center), size(size)
{
	for (int i = 0; i < 4; i++)
		childs[i] = NULL;

	bbox.set(center.x() - size, center.y() - size, center.z() - size, center.x() + size, center.y() + size, center.z() + size);

	objectIndicies.reserve(SUBDIVIDE_NODE_OBJECT_COUNT);
}

template <typename T>
PointQuadtree<T>::Node::~Node()
{
	if (childs[0] != NULL)
	{
		for (int i = 0; i < 4; i++)
			delete childs[i];
	}
}

template <typename T>
void PointQuadtree<T>::Node::subdivide()
{
	double hsize = size / 2.0;

	childs[0] = new Node(objects, osg::Vec3d(center.x() - hsize, center.y(), center.z() - hsize), hsize);
	childs[1] = new Node(objects, osg::Vec3d(center.x() + hsize, center.y(), center.z() - hsize), hsize);
	childs[2] = new Node(objects, osg::Vec3d(center.x() - hsize, center.y(), center.z() + hsize), hsize);
	childs[3] = new Node(objects, osg::Vec3d(center.x() + hsize, center.y(), center.z() + hsize), hsize);
	
	// По-возможности раскидываем объекты данной ноды по чайлдам, но некоторые объекты могут остаться в этой ноде
	int n = 0;
	for (size_t nObject = 0; nObject < objectIndicies.size(); nObject++)
	{
		int objectIndex = objectIndicies[nObject];
		const Object& object = objects[objectIndex];

		bool inserted = false;

		for (int i = 0; i < 4; i++)
		{
			if (childs[i]->bbox.contains(object.position))
			{
				childs[i]->insert(objectIndex);
				inserted = true;
				break;
			}
		}

		if (!inserted)
			objectIndicies[n++] = objectIndex;
	}

	objectIndicies.resize(n);
}

template <typename T>
void PointQuadtree<T>::Node::insert(int objectIndex)
{
	if (childs[0] == NULL) // Чайлдов либо нет ни одного, либо есть все
	{
		// Если это лист, то вставляем в него объект
		objectIndicies.push_back(objectIndex);

		// Если в листе собралось слишком много вершин, то подразбиваем его
		if (objectIndicies.size() == SUBDIVIDE_NODE_OBJECT_COUNT)
			subdivide();
	}
	else
	{
		const Object& object = objects[objectIndex];

		// Распихиваем по чайлдам
		for (int i = 0; i < 4; i++)
		{
			if (childs[i]->bbox.contains(object.position))
			{
				childs[i]->insert(objectIndex);
				return;
			}
		}

		// Если объект не попал ни в одного чайлда, то оставляем его в этой ноде 
		objectIndicies.push_back(objectIndex);
	}
}

template <typename T>
void PointQuadtree<T>::Node::query(const osg::Polytope& frustum, ISink* sink)
{
	if (frustum.contains(bbox))
	{
		for (size_t nObject = 0; nObject < objectIndicies.size(); nObject++)
		{
			const Object& object = objects[objectIndicies[nObject]];

			if (frustum.contains(object.position))
				sink->onQuery(object.object);
		}

		if (childs[0] != NULL)
		{
			for (int i = 0; i < 4; i++)
				childs[i]->query(frustum, sink);
		}
	}
}

template <typename T>
void PointQuadtree<T>::Node::query(const osg::BoundingBox& box, ISink* sink)
{
	if (box.intersects(bbox))
	{
		for (size_t nObject = 0; nObject < objectIndicies.size(); nObject++)
		{
			const Object& object = objects[objectIndicies[nObject]];

			if (box.contains(object.position))
				sink->onQuery(object.object);
		}

		if (childs[0] != NULL)
		{
			for (int i = 0; i < 4; i++)
				childs[i]->query(box, sink);
		}
	}
}

template <typename T>
int PointQuadtree<T>::Node::height()
{
	if (childs[0] != NULL)
	{
		int result = 0;

		for (int i = 0; i < 4; i++)
			result = std::max(result, childs[i]->height());

		return result + 1;
	}
	else
		return 1;
}

// Сериализация
template <typename T> template <typename S> void PointQuadtree<T>::serializeObjectList(S& stream)
{
	uint32_t size = (uint32_t)this->objects.size();
	stream >> size;

	if (stream.getMode() == S::READ)
	{
		if (size > 0)
		{
			this->objects.resize(size);
		}
		else
		{
			this->objects = ed::vector<ed::PointQuadtree<T>::Object>();
		}
	}

	for (auto it = this->objects.begin(); it != this->objects.end(); ++it)
	{
		uint32_t version = 0;
		if (version >= 0)
		{
			stream >> it->object;
			stream >> it->bbox;
		}
	}
}

template <typename T> template <typename S> void PointQuadtree<T>::serializeNode(S& stream, typename ed::PointQuadtree<T>::Node*& treeNode)
{
	if (treeNode == NULL)
		treeNode = new ed::PointQuadtree<T>::Node(objects, osg::Vec3d(), 0.0);

	uint32_t version = 0;
	stream >> version;

	if (version >= 0)
	{
		stream >> treeNode->center;
		stream >> treeNode->size;
		stream >> treeNode->bbox;
		stream >> treeNode->objectIndicies;
		
		// Сериализация чайлдов
		bool haveChilds = treeNode->childs[0] != NULL;
		stream >> haveChilds;

		if (haveChilds)
		{
			for (int i = 0; i < 4; i++)
				serializeNode(stream, treeNode->childs[i]);
		}
	}
}

}

template <typename S, typename K> S& operator >> (S& stream, ed::PointQuadtree<K>& tree)
{
	uint32_t version = 0;
	stream >> version;

	if (stream.getMode() == S::READ)
		tree.clear();

	if (version >= 0)
	{
		tree.serializeObjectList(stream);

		bool rootExists = tree.root != NULL;
		stream >> rootExists;
		
		if (rootExists)
			tree.serializeNode(stream, tree.root);
	}

	return stream;
}

#endif