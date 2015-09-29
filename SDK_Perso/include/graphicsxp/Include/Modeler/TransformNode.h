#ifndef TransformNode_H
#define TransformNode_H

#include "ModelNode.h"
#include "position.h"

namespace Graphics
{

class ModelInstance;

// Узел, имеющий трансформацию
class GRAPHICSXP_API TransformNode : public ModelNode
{
protected:
	// ссылка на следующий сверху узел трансформации, для вычисление полной трансформации
	TransformNode *parent_transform;

	// статическая трансформация
	Position3 static_transform;

public:
	TransformNode(ModelNode *_parent = 0);

	// найти следующий сверху узел трансформации
	TransformNode *UpdateParentTransform();

	// управление статической трансформацией
	void SetStaticTransform(const Position3& transform);
	const Position3& GetStaticTransform();

	// получить трансформацию от корня (заполняет cur_transform, не использовать при обходе)
	virtual const Position3& GetFullTransform(ModelInstance &object);

    virtual const Position3& GetFullTransform2(ModelInstance &object, const Position3& pos_);

	// создать трансформацию для текущего обхода модели
	// матрица создается из временного пула - на один кадр
	virtual Position3 *CreateCurTransform(ModelInstance &object, const Position3& parent_pos);

	// получить описанный бокс узла
	virtual void CalculateBox();

	// проход по иерархии
	// будет выполнена трансформация pos *= transform
	virtual bool Parse(ModelInstance *object, ModelParser *parser, const Position3& parent_pos, effectState *parent_effects);

	// сохранение/загрузка
	virtual void serialize(Serializer &serializer);

	// копирование модели
	virtual ModelNode *CloneEmpty();
	virtual void CopyFrom(ModelNode *source);
};

} // namespace Graphics

#endif // TransformNode_H