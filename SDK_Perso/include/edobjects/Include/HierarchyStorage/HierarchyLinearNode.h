#ifndef HierarchyLinearNode_H
#define HierarchyLinearNode_H

#include <ed/list.h>

#include "box.h"
#include "HierarchyNode.h"

class viSearch;

// вариант узла с полным перебором
class EDOBJECTS_API HierarchyLinearNode : public HierarchyNode
{
private:
	unsigned total;
	int searchID;

protected:
	// габариты (должны заполняться наследниками)
	Box box;

	// список детей
	typedef ed::list< Link<HierarchyNode> > nodeList;
	nodeList nodes;

	// должна вернуть true если object должен попасть в nodes
	// если false => воткнуть в одного из детей
	virtual bool PreserveHere(HierarchyNode *object) {return true;}

	// вернет true если уже тестировали этот поиск
	bool WasInSearch(viSearch *search);

public:
	HierarchyLinearNode();
	~HierarchyLinearNode();
	
	// добавление/удаление объектов
	virtual bool AddObject(HierarchyNode *object);
	virtual bool RemoveObject(HierarchyNode *object);
	virtual void DeleteObjects();

	// получение статистики
	virtual unsigned GetCount();
	
	// получить габариты узла
	virtual const Box& GetBox() const;
	
	// сбор объектов
	virtual bool GetObjects(viSearch_Implement *search, IntersectionType it);
};

#endif // HierarchyLinearNode_H