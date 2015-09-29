#ifndef MovingHandle_H
#define MovingHandle_H

#include "edObjects.h"
#include "position.h"
#include "Serializer/Serializer.h"

class viSearch;

// интерфейс для объекта в хранилище движущихся объектов
// реальные объекты должны быть унаследованы также от viObject
class EDOBJECTS_API MovingHandle
{
friend class QuadTreeNode;
private:
	QuadTreeNode *node;
	Vector3 p;
	float size;
	
public:
	MovingHandle(const Vector3& _p = Vector3());
	virtual ~MovingHandle();

	virtual float GetSize() = 0;
	virtual void ChangePos(const Position3& newpos);

	// поиск по дереву движущихся объектов снизу вверх
	bool SearchFromThisNode(viSearch *search);
	virtual void serialize(Serializer &);
};

#endif // MovingHandle_H