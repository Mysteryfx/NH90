#ifndef BOUNDING_BOX_OCTREE_H
#define BOUNDING_BOX_OCTREE_H

#include <osg/Vec3f>
#include <osg/Vec3d>
#include <osg/Polytope>
#include <osg/BoundingBox>

#include "io/STLSerializer.h"

// forward declarations
namespace ed { template <typename T> class BoundingBoxOctree; }
template <typename S, typename K> S& operator >> (S& stream, ed::BoundingBoxOctree<K>& tree);

namespace ed
{

template <typename T> class BoundingBoxOctree
{
public:

	struct ISink
	{
		virtual void onQuery(const T& t, const osg::BoundingBox& box) = 0;
	};

	BoundingBoxOctree();
	~BoundingBoxOctree();

	BoundingBoxOctree(const BoundingBoxOctree& copy);
	BoundingBoxOctree& operator =(const BoundingBoxOctree& obj);

	/**
	 * Вставить объект в дерево с четко заданным баундом
	 */
	void insert(const T& object, const osg::BoundingBox& bbox);

	/**
	 * Удвлить объект
	 */
	void remove(const T &object);

	/**
	 * Очистить дерево
	 */
	void clear();

	/**
	 * Выбрать объекты, попадающих во фрастум
	 */
	void query(const osg::Polytope& frustum, ed::vector<T>& queriedObjects, ed::vector<const osg::BoundingBox*>* queriedBoxes=NULL);
	void query(const osg::BoundingBox& frustum, ed::vector<T>& queriedObjects, ed::vector<const osg::BoundingBox*>* queriedBoxes=NULL);
	
	/**
	 * Выбрать объекты, попадающие во фрастум или в баундинг бокс
	 */
	void query(const osg::Polytope& frustum, ISink* sink);
	void query(const osg::BoundingBox& box, ISink* sink);

	/**
	 * Выбрать объекты, попадающие во фрастум или в баундинг бокс
	 * void f(const T& t, const osg::BoundingBox& box);
	 * queryLambda(box, [&](const T& t, const osg::BoundingBox& box)
	 */
	template <typename Function>
	void queryLambda(const osg::Polytope& frustum, const Function& f);
	template <typename Function>
	void queryLambda(const osg::BoundingBox& box, const Function& f);

	/**
	 * Высота дерева
	 */
	int height();

	// Подсчет числа объектов в выборке
	struct CounterSink : ISink
	{
		int count;
		CounterSink() : count(0) {}
		virtual void onQuery(const T& t, const osg::BoundingBox& box) { count++; }
	};

	// Сбор объектов в список
	struct CollectToArraySink : ISink
	{
		ed::vector<T>& queriedObjects;
		ed::vector<const osg::BoundingBox*>* queriedBoxes;
		CollectToArraySink(ed::vector<T>& queriedObjects, ed::vector<const osg::BoundingBox*>* _queriedBoxes) : queriedObjects(queriedObjects),queriedBoxes(_queriedBoxes) {};
		virtual void onQuery(const T& t, const osg::BoundingBox& box) { queriedObjects.push_back(t); if(queriedBoxes) queriedBoxes->push_back(&box);}
	};

	template<typename Function>
	struct LambdaSink : ISink
	{
		const Function& function;
		LambdaSink(const Function& f) : function(f) {}
		virtual void onQuery(const T& t, const osg::BoundingBox& box) { function(t, box); }
	};

	struct GpuQueryData
	{
		double size;
		osg::Vec3d center;
		ed::vector<int> childrens;
		ed::vector<int> isLeaf;
		ed::vector<int> objectIndices;
		ed::vector<int> startIndex;
		ed::vector<int> objectIndicesCount;
	};

	void getGpuQueryData(GpuQueryData& data)
	{
		insertAwaitingNodes();
		data = GpuQueryData();
		if (root != NULL)
		{
			data.center = root->center;
			data.size = root->size;
		}
		else
		{
			data.size = 0.0;
		}
		int nextIndex = 0;
		addToGpuQueryData(root, 0, nextIndex, data);
	}

private:


	struct Object
	{
		T object;
		osg::BoundingBox bbox; // Кэшируем баундинг бокс потому что вызов getBoundingBox() может быть тяжелым

		bool isValid;

		Object(const T& object = T(), const osg::BoundingBox& bbox = osg::BoundingBox()) : object(object), bbox(bbox), isValid(true) {}
	};

	struct Node
	{
		// Объекты
		const ed::vector<Object>& objects;
		ed::vector<int> objectIndicies;

		osg::Vec3d center;
		double size;
		osg::BoundingBox bbox;
		Node* childs[8];

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
		 * Получить все объекты дерева, попадающие во фрастум или бокс
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
	template <typename S, typename K> friend S& ::operator >> (S& stream, ed::BoundingBoxOctree<K>& tree);
	template <typename S> void serializeObjectList(S& stream);
	template <typename S> void serializeNode(S& stream, typename ed::BoundingBoxOctree<T>::Node*& node);

	void addToGpuQueryData(Node* node, int index, int& nextIndex, GpuQueryData& data);
};

//реализация

//BoundingBoxOctree methods
template <typename T> 
BoundingBoxOctree<T>::BoundingBoxOctree() : root(NULL), insertedNodes(0)
{
}

template <typename T> 
BoundingBoxOctree<T>::~BoundingBoxOctree()
{
	clear();
}

template <typename T> 
BoundingBoxOctree<T>::BoundingBoxOctree(const BoundingBoxOctree& copy)
{	
	this->root = NULL;

	for (auto it = copy.objects.begin(); it != copy.objects.end(); ++it)
	{
		this->insert(it->object, it->bbox);
	}

}

template <typename T> 
BoundingBoxOctree<T>& BoundingBoxOctree<T>::operator = (const BoundingBoxOctree& obj)
{
	this->clear();

	for (auto it = obj.objects.begin(); it != obj.objects.end(); ++it)
	{
		this->insert(it->object, it->bbox);
	}

	return *this;
}

template <typename T> 
void BoundingBoxOctree<T>::clear()
{
	delete root;
	root = NULL;
	objects.clear();
	insertedNodes = 0;
}

template <typename T> 
void BoundingBoxOctree<T>::insert(const T& object, const osg::BoundingBox& bbox)
{
	objects.push_back(Object(object, bbox));
}

template <typename T> 
void BoundingBoxOctree<T>::remove(const T& object)
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		intptr_t id = (intptr_t)((*it).object);
		if (id == (intptr_t)object)
		{
			(*it).object.isValid = false;
			break;
		}
	}
}

template <typename T> 
void BoundingBoxOctree<T>::insertAwaitingNodes()
{
	for (; insertedNodes < objects.size(); insertedNodes++)
	{
		// Создаем корень если его нет
		if (root == NULL)
		{
			osg::BoundingBox bbox;

			for (int i = insertedNodes; i < objects.size(); i++)
				bbox.expandBy(objects[i].bbox);

			root = new Node(objects, bbox.center(), bbox.radius() / sqrt(3.0));
		}

		// Баундинг бокс за пределами текущего корня
		osg::BoundingBox& bbox = objects[insertedNodes].bbox;
		while (!root->bbox.contains(bbox))
		{
			unsigned int cornerBitmask = 0;

			cornerBitmask |= root->center.x() < bbox.center().x() ? 1 : 0; 
			cornerBitmask |= root->center.y() < bbox.center().y() ? 2 : 0;
			cornerBitmask |= root->center.z() < bbox.center().z() ? 4 : 0;
			
			osg::Vec3d newRootCenter = root->bbox.corner(cornerBitmask);
			
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
void BoundingBoxOctree<T>::query(const osg::Polytope& frustum, ed::vector<T>& queriedObjects, ed::vector<const osg::BoundingBox*>* queriedBoxes)
{
	CollectToArraySink sink(queriedObjects, queriedBoxes);
	query(frustum, &sink);
}

template <typename T> 
void BoundingBoxOctree<T>::query(const osg::BoundingBox& box, ed::vector<T>& queriedObjects, ed::vector<const osg::BoundingBox*>* queriedBoxes)
{
	CollectToArraySink sink(queriedObjects, queriedBoxes);
	query(box, &sink);
}

template <typename T> 
void BoundingBoxOctree<T>::query(const osg::Polytope& frustum, ISink* sink)
{
	insertAwaitingNodes();

	if (sink == NULL)
		return;

	if (root != NULL)
		root->query(frustum, sink);
}

template <typename T> 
void BoundingBoxOctree<T>::query(const osg::BoundingBox& box, ISink* sink)
{
	insertAwaitingNodes();

	if (root != NULL)
		root->query(box, sink);
}

template <typename T>
template <typename Function>
void BoundingBoxOctree<T>::queryLambda(const osg::Polytope& frustum, const Function& f)
{
	insertAwaitingNodes();

	if (root != NULL)
	{
		LambdaSink<Function> sink(f);
		root->query(frustum, &sink);
	}
}

template <typename T>
template <typename Function>
void BoundingBoxOctree<T>::queryLambda(const osg::BoundingBox& box, const Function& f)
{
	insertAwaitingNodes();

	if (root != NULL)
	{
		LambdaSink<Function> sink(f);
		root->query(box, &sink);
	}
}


template <typename T> 
int BoundingBoxOctree<T>::height()
{
	return root != NULL ? root->height() : 0; 
}

//node methods
template <typename T>
BoundingBoxOctree<T>::Node::Node(const ed::vector<Object>& objects, const osg::Vec3d& center, double size) : objects(objects), center(center), size(size)
{
	for (int i = 0; i < 8; i++)
		childs[i] = NULL;

	bbox.set(center.x() - size, center.y() - size, center.z() - size, center.x() + size, center.y() + size, center.z() + size);

	objectIndicies.reserve(SUBDIVIDE_NODE_OBJECT_COUNT);
}

template <typename T>
BoundingBoxOctree<T>::Node::~Node()
{
	if (childs[0] != NULL)
	{
		for (int i = 0; i < 8; i++)
			delete childs[i];
	}
}

template <typename T>
void BoundingBoxOctree<T>::Node::subdivide()
{
	double hsize = size / 2.0;

	childs[0] = new Node(objects, osg::Vec3d(center.x() - hsize, center.y() - hsize, center.z() - hsize), hsize);
	childs[1] = new Node(objects, osg::Vec3d(center.x() + hsize, center.y() - hsize, center.z() - hsize), hsize);
	childs[2] = new Node(objects, osg::Vec3d(center.x() - hsize, center.y() + hsize, center.z() - hsize), hsize);
	childs[3] = new Node(objects, osg::Vec3d(center.x() + hsize, center.y() + hsize, center.z() - hsize), hsize);
	childs[4] = new Node(objects, osg::Vec3d(center.x() - hsize, center.y() - hsize, center.z() + hsize), hsize);
	childs[5] = new Node(objects, osg::Vec3d(center.x() + hsize, center.y() - hsize, center.z() + hsize), hsize);
	childs[6] = new Node(objects, osg::Vec3d(center.x() - hsize, center.y() + hsize, center.z() + hsize), hsize);
	childs[7] = new Node(objects, osg::Vec3d(center.x() + hsize, center.y() + hsize, center.z() + hsize), hsize);

	// По-возможности раскидываем объекты данной ноды по чайлдам, но некоторые объекты могут остаться в этой ноде
	int n = 0;
	for (size_t nObject = 0; nObject < objectIndicies.size(); nObject++)
	{
		int objectIndex = objectIndicies[nObject];
		const Object& object = objects[objectIndex];

		bool inserted = false;

		for (int i = 0; i < 8; i++)
		{
			if (childs[i]->bbox.contains(object.bbox))
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
void BoundingBoxOctree<T>::Node::insert(int objectIndex)
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
		for (int i = 0; i < 8; i++)
		{
			if (childs[i]->bbox.contains(object.bbox))
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
void BoundingBoxOctree<T>::Node::query(const osg::Polytope& frustum, ISink* sink)
{
	if (frustum.contains(bbox))
	{
		for (size_t nObject = 0; nObject < objectIndicies.size(); nObject++)
		{
			const Object& object = objects[objectIndicies[nObject]];

			if (frustum.contains(object.bbox) && object.isValid)
				sink->onQuery(object.object, object.bbox);
		}

		if (childs[0] != NULL)
		{
			for (int i = 0; i < 8; i++)
				childs[i]->query(frustum, sink);
		}
	}
}

template <typename T>
void BoundingBoxOctree<T>::Node::query(const osg::BoundingBox& box, ISink* sink)
{
	if (box.intersects(bbox))
	{
		for (size_t nObject = 0; nObject < objectIndicies.size(); nObject++)
		{
			const Object& object = objects[objectIndicies[nObject]];

			if (box.intersects(object.bbox) && object.isValid)
				sink->onQuery(object.object, object.bbox);
		}

		if (childs[0] != NULL)
		{
			for (int i = 0; i < 8; i++)
				childs[i]->query(box, sink);
		}
	}
}

template <typename T>
int BoundingBoxOctree<T>::Node::height()
{
	if (childs[0] != NULL)
	{
		int result = 0;

		for (int i = 0; i < 8; i++)
			result = std::max(result, childs[i]->height());

		return result + 1;
	}
	else
		return 1;
}

// Сериализация
template <typename T> template <typename S> void BoundingBoxOctree<T>::serializeObjectList(S& stream)
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
			this->objects = ed::vector<ed::BoundingBoxOctree<T>::Object>();
		}
	}

	for (auto it = this->objects.begin(); it != this->objects.end(); ++it)
	{
		uint32_t version = 0;
		if (version >= 0)
		{
			stream >> it->object;
			stream >> it->bbox;
			stream >> it->isValid;
		}
	}
}

template <typename T> template <typename S> void BoundingBoxOctree<T>::serializeNode(S& stream, typename ed::BoundingBoxOctree<T>::Node*& treeNode)
{
	if (treeNode == NULL)
		treeNode = new ed::BoundingBoxOctree<T>::Node(objects, osg::Vec3d(), 0.0);

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
			for (int i = 0; i < 8; i++)
				serializeNode(stream, treeNode->childs[i]);
		}
	}
}

template <typename T>
void BoundingBoxOctree<T>::addToGpuQueryData(typename BoundingBoxOctree::Node* node, int index,
											 int& nextIndex,
											 typename BoundingBoxOctree::GpuQueryData& data)
{
	if (node == NULL)
		return;

	for (size_t i = 0; i < 8; ++i)
		data.childrens.push_back(-1);
		
	if (index >= data.isLeaf.size())
	{
		data.isLeaf.resize(index + 1);
		data.startIndex.resize(index + 1);
		data.objectIndicesCount.resize(index + 1);
	}
	data.startIndex[index] = data.objectIndices.size();
	data.objectIndicesCount[index] = node->objectIndicies.size();
	for (size_t i = 0; i < node->objectIndicies.size(); ++i)
		data.objectIndices.push_back(node->objectIndicies[i]);

	nextIndex++;

	if (node->childs[0] == NULL)
	{
		data.isLeaf[index] = 1;
		return;
	}
	
	data.isLeaf[index] = 0;

	size_t startIndex = data.childrens.size() - 8;
	for (size_t i = 0; i < 8; ++i)
	{
		data.childrens[startIndex + i] = nextIndex;
		addToGpuQueryData(node->childs[i], nextIndex, nextIndex, data);
	}
}


} // namespace ed

template <typename S, typename K> S& operator >> (S& stream, ed::BoundingBoxOctree<K>& tree)
{
	uint32_t version = 0;
	stream >> version;

	if (stream.getMode() == S::READ)
		tree.clear();
	else
		tree.insertAwaitingNodes();

	if (version >= 0)
	{
		tree.serializeObjectList(stream);

		bool rootExists = tree.root != NULL;
		stream >> rootExists;
		
		if (rootExists)
			tree.serializeNode(stream, tree.root);
	}
	if (stream.getMode() == S::READ)
		tree.insertedNodes = tree.objects.size();

	return stream;
}

#endif
