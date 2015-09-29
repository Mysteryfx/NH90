#ifndef UserBoxNode_H
#define UserBoxNode_H

#include "ModelNode.h"

namespace Graphics
{

// Узел с заданным дезайнерами боксом
class GRAPHICSXP_API UserBoxNode : public ModelNode
{
protected:
	Box user_box;

public:
	UserBoxNode(ModelNode *_parent = 0);

	void SetUserBox(const Box& box);
	const Box& GetUserBox();
	
	// сохранение/загрузка
	virtual void serialize(Serializer &serializer);

	// копирование модели
	virtual ModelNode *CloneEmpty();
	virtual void CopyFrom(ModelNode *source);
};

} // namespace Graphics

#endif // UserBoxNode_H