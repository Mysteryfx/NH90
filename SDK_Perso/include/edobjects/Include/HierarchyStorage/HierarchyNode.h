#ifndef HierarchyNode_H
#define HierarchyNode_H

#include "Common/Link.h"
#include "ClipVolumes/IntersectionType.h"
#include "edObjects.h"

class viSearch_Implement;

// узел в абстрактном иерархическом дереве
// клиенты могут создать его наследников со своей семантикой
class EDOBJECTS_API HierarchyNode : public LinkHost
{
public:
	virtual ~HierarchyNode() {};
	
	// добавление/удаление объектов
	virtual bool AddObject(HierarchyNode *object) = 0;
	virtual bool RemoveObject(HierarchyNode *object) = 0;

	// получение статистики
	virtual unsigned GetCount() = 0;
	
	// сбор объектов
	virtual bool GetObjects(viSearch_Implement *search, IntersectionType it) = 0;
};

#endif // HierarchyNode_H
