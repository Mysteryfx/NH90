#ifndef HierarchyLayer_H
#define HierarchyLayer_H

#include "HierarchyLinearNode.h"

// пространственный узел в абстрактном дереве, умеет делиться по медиане
// рассчитывает на то, что дети у него - наследники HierarchyLinearNode
class EDOBJECTS_API HierarchyLayer : public HierarchyLinearNode
{
protected:
	bool accepting;

	// должна вернуть true если object должен попасть в nodes
	// если false => воткнуть в одного из детей
	virtual bool PreserveHere(HierarchyNode *object) {return accepting;}

public:
	HierarchyLayer();
	~HierarchyLayer();

	// установить флаг оставления объектов на этом уровне
	void SetAcceptance(bool accept);

	// посчитать боксы по иерархии
	virtual void CalculateBoxes();

	// разделить этот узел по медиане
	virtual bool Subdivide();

	// полностью построить под собой сбалансированное двоичное дерево
	virtual void Build();
};

#endif // HierarchyLayer_H
