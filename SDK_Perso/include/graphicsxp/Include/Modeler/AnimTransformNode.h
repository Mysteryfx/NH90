#ifndef AnimTransformNode_H
#define AnimTransformNode_H

#include "TransformNode.h"
#include "Animation.h"

namespace Graphics
{

// Узел, определяющий анимированную трансформацию
class GRAPHICSXP_API AnimTransformNode : public TransformNode
{
protected:
	AnimationVector	*aMove;
	AnimationQuat	*aRotate;
	AnimationQuat	*aScaleOrient;
	AnimationVector	*aScale;

public:
	AnimTransformNode(ModelNode *_parent = 0);
	~AnimTransformNode();

	// операции с анимацией
	bool isMoveAnimated();
	bool isRotateAnimated();
	bool isScaleOrientAnimated();
	bool isScaleAnimated();

	// анимация будет автоматически создана по запросу
	AnimationVector*& GetMoveAnim();
	AnimationQuat*& GetRotateAnim();
	AnimationQuat*& GetScaleOrientAnim();
	AnimationVector*& GetScaleAnim();
	
	void DestroyMoveAnim();
	void DestroyRotateAnim();
	void DestroyScaleOrientAnim();
	void DestroyScaleAnim();
	
	// получить максимальный индекс динамического параметра, 
	// используемого данным узлом
	virtual int GetMaxParamIndex();

	// получить описанный бокс узла
	virtual void CalculateBox();
	
	// создать трансформацию для текущего обхода модели
	Position3 *CreateCurTransform(ModelInstance &object, const Position3& parent_pos);

	// сохранение/загрузка
	virtual void serialize(Serializer &serializer);

	// копирование модели
	virtual ModelNode *CloneEmpty();
	virtual void CopyFrom(ModelNode *source);
};

} // namespace Grpahics

#endif // AnimTransformNode_H