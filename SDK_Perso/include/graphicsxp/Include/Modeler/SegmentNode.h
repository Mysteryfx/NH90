#ifndef SegmentNode_H
#define SegmentNode_H

#include "ModelNode.h"

namespace Graphics
{

class TransformNode;

// Узел с характерным отрезком для проверки столкновений
class GRAPHICSXP_API SegmentNode : public ModelNode
{
protected:
	// ссылка на следующий сверху узел трансформации, для вычисление полной трансформации
	TransformNode *parent_transform;

	Vector3 a,b;

public:
	SegmentNode(ModelNode *_parent = 0);
	
	void SetA(const Vector3& a);
	void SetB(const Vector3& b);

	const Vector3& GetA();
	const Vector3& GetB();
	
	// проход по иерархии
	virtual bool Parse(ModelInstance *object, ModelParser *parser, const Position3& parent_pos, effectState *parent_effects);

	// сохранение/загрузка
	virtual void serialize(Serializer &serializer);

	// копирование модели
	virtual ModelNode *CloneEmpty();
	virtual void CopyFrom(ModelNode *source);
};

} // namespace Graphics

#endif // SegmentNode_H