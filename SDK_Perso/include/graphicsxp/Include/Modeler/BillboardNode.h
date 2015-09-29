#ifndef BillboardNode_H
#define BillboardNode_H

#include "TransformNode.h"

namespace Graphics
{

// способ поворота билборда
enum BillboardType
{
	btPoint,	// поворачивать на точку камеры
	btDirection,// поворачивать параллельно осям камеры
};

// оси поворота биллборда
enum BillboardAxis
{
	// не поворачивать
	baNone,
	
	// поворачивать вокруг одной своей оси
	baRotateX,
	baRotateY,
	baRotateZ,

	// совместить свою ось с осью камеры (на камеру)
	baAlongX,
	baAlongY,
	baAlongZ,

	// совместить все оси с осями камеры (на камеру)
	baComplete,
};

// Узел, поворачивающий объект на камеру
class GRAPHICSXP_API BillboardNode : public TransformNode
{
protected:
	BillboardType bbType;
	BillboardAxis bbAxis;

	// заполнить матрицу, к которой будем поворачивать
	virtual void CameraTransform(Position3& cam_trans, const Position3& my_pos);

public:
	BillboardNode(ModelNode *parent = 0);

	// параметры
	void SetBillboardType(BillboardType type);
	void SetBillboardAxis(BillboardAxis axis);
	BillboardType GetBillboardType();
	BillboardAxis GetBillboardAxis();

	// создать трансформацию для текущего обхода модели
	// матрица создается из временного пула - на один кадр
	virtual Position3 *CreateCurTransform(ModelInstance &object, const Position3& parent_pos);

	// получить описанный бокс узла
	virtual void CalculateBox();

	// проход по иерархии
	virtual bool Parse(ModelInstance *object, ModelParser *parser, const Position3& parent_pos, effectState *parent_effects);

	// сохранение/загрузка
	virtual void serialize(Serializer &serializer);

	// копирование модели
	virtual ModelNode *CloneEmpty();
	virtual void CopyFrom(ModelNode *source);
};

} // namespace Graphics

#endif // BillboardNode_H