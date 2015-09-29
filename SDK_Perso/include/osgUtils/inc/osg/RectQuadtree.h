#ifndef RECT_QUADTREE_H
#define RECT_QUADTREE_H

#include <osg/Vec2f>
#include <osg/Vec2d>
#include <osg/Rect>

#include "io/STLSerializer.h"

// forward declarations
namespace ed { template <typename T> class RectQuadtree; }
template <typename S, typename K> S& operator >> (S& stream, ed::RectQuadtree<K>& tree);

namespace ed
{

template <typename T> class RectQuadtree
{
public:

	struct ISink
	{
		virtual void onQuery(const T& t) = 0;
	};

	RectQuadtree();
	~RectQuadtree();

	RectQuadtree(const RectQuadtree& copy);
	RectQuadtree& operator = (const RectQuadtree& obj);

	/**
	 * Вставить объект в дерево с четко заданным баундом
	 */
	void insert(const T& object, const osg::Rect& rect);

	osg::Rect getRect() const;

	/**
	 * Очистить дерево
	 */
	void clear();

	/**
	 * Выбрать объекты, попадающие в рект
	 */
	void query(const osg::Rect& rect, ed::vector<T>& queriedObjects);

	/**
	 * Выбрать объекты, попадающие в рект
	 */
	void query(const osg::Rect& rect, ed::list<T>& queriedObjects);
	
	/**
	 * Выбрать объекты, попадающие в рект
	 */
	void query(const osg::Rect& rect, ISink* sink);
	
	/**
	 * Выбрать объекты используя лямбда-функции
	 * queryLambda( rect, [&](const T& t)
	 */
	template <typename Function> void queryLambda(const osg::Rect& rect, const Function& func);

	/**
	 * Высота дерева
	 */
	int height();

	int size() const { return objects.size();};
	/**
	 * Вставить в дерево ноды, ожидающие вставки
	 */
	void insertAwaitingNodes();

	// Подсчет числа объектов в выборке
	struct CounterSink : ISink
	{
		int count;
		CounterSink() : count(0) {}
		virtual void onQuery(const T& t) { count++; }
	};

	// Сбор объектов в список
	struct CollectToListSink : ISink
	{
		ed::list<T>& queriedObjects;
		CollectToListSink(ed::list<T>& queriedObjects) : queriedObjects(queriedObjects) {};
		virtual void onQuery(const T& t) { queriedObjects.push_back(t); }
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
		osg::Rect rect; // Кэшируем баундинг бокс потому что вызов getRect() может быть тяжелым

		Object(const T& object = T(), const osg::Rect& rect = osg::Rect()) : object(object), rect(rect) {}
	};

	struct Node
	{
		// Объекты
		const ed::vector<Object>& objects;
		ed::vector<int> objectIndicies;

		osg::Vec2d center;
		double size;
		osg::Rect rect;
		Node* childs[4];

		const static int SUBDIVIDE_NODE_OBJECT_COUNT = 50;
		
		Node(const ed::vector<Object>& objects, const osg::Vec2d& center, double size);
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
		 * Получить все объекты дерева, попадающие в рект
		 */
		template <class Function> void query(const osg::Rect& rect, const Function& func);

		/**
		 * высота поддерева этой ноды
		 */
		int height();
	};

	Node* root;

	// Все объекты дерева
	int insertedNodes;
	ed::vector<Object> objects;

	// Сериализация
	template <typename S, typename K> friend S& ::operator >> (S& stream, ed::RectQuadtree<K>& tree);
	template <typename S> void serializeObjectList(S& stream);
	template <typename S> void serializeNode(S& stream, typename ed::RectQuadtree<T>::Node*& node);
};

//реализация

//RectQuadtree methods
template <typename T> 
RectQuadtree<T>::RectQuadtree() : root(NULL), insertedNodes(0)
{
}

template <typename T> 
RectQuadtree<T>::~RectQuadtree()
{
	clear();
}

template <typename T> 
RectQuadtree<T>::RectQuadtree(const RectQuadtree& copy)
{	
	this->root = NULL;
	this->insertedNodes = 0;

	for (auto it = copy.objects.begin(); it != copy.objects.end(); ++it)
	{
		this->insert(it->object, it->rect);
	}

}

template <typename T> 
RectQuadtree<T>& RectQuadtree<T>::operator = (const RectQuadtree& obj)
{
	this->clear();

	for (auto it = obj.objects.begin(); it != obj.objects.end(); ++it)
	{
		this->insert(it->object, it->rect);
	}

	return *this;
}

template <typename T>
osg::Rect RectQuadtree<T>::getRect() const
{
	if (root == nullptr)
		return osg::Rect();

	return root->rect;
}

template <typename T> 
void RectQuadtree<T>::clear()
{
	delete root;
	root = NULL;
	objects.clear();
	insertedNodes = 0;
}

template <typename T> 
void RectQuadtree<T>::insert(const T& object, const osg::Rect& rect)
{
	objects.push_back(Object(object, rect));
}

template <typename T> 
void RectQuadtree<T>::insertAwaitingNodes()
{
	for (; insertedNodes < objects.size(); insertedNodes++)
	{
		// Создаем корень если его нет
		if (root == NULL)
		{
			osg::Rect rect;

			for (int i = insertedNodes; i < objects.size(); i++)
				rect.expandBy(objects[i].rect);

			root = new Node(objects, rect.center(), rect.radius() / sqrt(2.0));
		}

		// Баундинг бокс за пределами текущего корня
		osg::Rect& rect = objects[insertedNodes].rect;
		while (!root->rect.contains(rect))
		{
			unsigned int cornerBitmask = 0;

			cornerBitmask |= root->center.x() < rect.center().x() ? 1 : 0; 
			cornerBitmask |= root->center.y() < rect.center().y() ? 2 : 0;
			
			osg::Vec2d newRootCenter = root->rect.corner(cornerBitmask);
			
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
void RectQuadtree<T>::query(const osg::Rect& rect, ed::vector<T>& queriedObjects)
{
	CollectToArraySink sink(queriedObjects);
	query(rect, &sink);
}

template <typename T> 
void RectQuadtree<T>::query(const osg::Rect& rect, ed::list<T>& queriedObjects)
{
	CollectToListSink sink(queriedObjects);
	query(rect, &sink);
}

template <typename T> 
void RectQuadtree<T>::query(const osg::Rect& rect, ISink* sink)
{
	if (sink == NULL)
		return;
	queryLambda(rect, [&](const T& t){sink->onQuery(t);});
}

template <typename T>
template <typename Function>
void RectQuadtree<T>::queryLambda(const osg::Rect& rect, const Function& func)
{
	insertAwaitingNodes();
	if (root != NULL)
		root->query(rect, func);
}

template <typename T> 
int RectQuadtree<T>::height()
{
	return root != NULL ? root->height() : 0; 
}

//node methods
template <typename T>
RectQuadtree<T>::Node::Node(const ed::vector<Object>& objects, const osg::Vec2d& center, double size) : objects(objects), center(center), size(size)
{
	for (int i = 0; i < 4; i++)
		childs[i] = NULL;

	rect.set(center.x() - size, center.y() - size, center.x() + size, center.y() + size);

	objectIndicies.reserve(SUBDIVIDE_NODE_OBJECT_COUNT);
}

template <typename T>
RectQuadtree<T>::Node::~Node()
{
	if (childs[0] != NULL)
	{
		for (int i = 0; i < 4; i++)
			delete childs[i];
	}
}

template <typename T>
void RectQuadtree<T>::Node::subdivide()
{
	double hsize = size / 2.0;

	childs[0] = new Node(objects, osg::Vec2d(center.x() - hsize, center.y() - hsize), hsize);
	childs[1] = new Node(objects, osg::Vec2d(center.x() + hsize, center.y() - hsize), hsize);
	childs[2] = new Node(objects, osg::Vec2d(center.x() - hsize, center.y() + hsize), hsize);
	childs[3] = new Node(objects, osg::Vec2d(center.x() + hsize, center.y() + hsize), hsize);

	// По-возможности раскидываем объекты данной ноды по чайлдам, но некоторые объекты могут остаться в этой ноде
	int n = 0;
	for (size_t nObject = 0; nObject < objectIndicies.size(); nObject++)
	{
		int objectIndex = objectIndicies[nObject];
		const Object& object = objects[objectIndex];

		bool inserted = false;

		for (int i = 0; i < 4; i++)
		{
			if (childs[i]->rect.contains(object.rect))
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
void RectQuadtree<T>::Node::insert(int objectIndex)
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
			if (childs[i]->rect.contains(object.rect))
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
template <typename Function>
void RectQuadtree<T>::Node::query(const osg::Rect& rectangle, const Function& func)
{
	if (rectangle.intersects(rect))
	{
		for (size_t nObject = 0; nObject < objectIndicies.size(); nObject++)
		{
			const Object& object = objects[objectIndicies[nObject]];

			if (rectangle.intersects(object.rect))
				func(object.object);
		}

		if (childs[0] != NULL)
		{
			for (int i = 0; i < 4; i++)
				childs[i]->query(rectangle, func);
		}
	}
}

template <typename T>
int RectQuadtree<T>::Node::height()
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
template <typename T> template <typename S> void RectQuadtree<T>::serializeObjectList(S& stream)
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
			this->objects = ed::vector<ed::RectQuadtree<T>::Object>();
		}
	}

	for (auto it = this->objects.begin(); it != this->objects.end(); ++it)
	{
		uint32_t version = 0;
		if (version >= 0)
		{
			stream >> it->object;
			stream >> it->rect;
		}
	}
}

template <typename T> template <typename S> void RectQuadtree<T>::serializeNode(S& stream, typename ed::RectQuadtree<T>::Node*& treeNode)
{
	if (treeNode == NULL)
		treeNode = new ed::RectQuadtree<T>::Node(objects, osg::Vec2d(), 0.0);

	uint32_t version = 0;
	stream >> version;

	if (version >= 0)
	{
		stream >> treeNode->center;
		stream >> treeNode->size;
		stream >> treeNode->rect;
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

template <typename S, typename K> S& operator >> (S& stream, ed::RectQuadtree<K>& tree)
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