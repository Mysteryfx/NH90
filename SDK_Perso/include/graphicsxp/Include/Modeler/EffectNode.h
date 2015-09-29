#ifndef EffectNode_H
#define EffectNode_H

#include "graphicsxp.h"
#include "TransformNode.h"

namespace Graphics
{

class VolumeEffect;

// классы объектов, на которые воздействует данный эффект (формируют маску)
enum EffectTarget
{
	etNone		=0,	// ни на кого
	etSelf		=1,	// на свою модель
	etObjects	=2,	// на другие модели
	etLand		=4,	// на поверхность земли (воды)
};

// узел с эффектом
class GRAPHICSXP_API EffectNode : public TransformNode
{
protected:
	ed::string effectClass;

	typedef ed::vector<BYTE> untyped;
	untyped data;

	EffectTarget targets;

public:
	EffectNode(ModelNode *_parent = 0);

	// создать экземпляр объемного эффекта
	// передается префикс имени (постфикс - имя узла)
	VolumeEffect *CreateEffect(const ed::string& prefix);

	// обновить информацию в экземпляре (если возвращает false - не действует)
	bool UpdateEffect(VolumeEffect *effect, ModelInstance &object);

	// типы объектов, на которые воздействует данный эффект
	void SetEffectTarget(EffectTarget target);
	EffectTarget GetEffectTarget();

	// класс эффекта
	void SetEffectClass(const ed::string& effectClass);
	const ed::string& GetEffectClass();

	// сохранение/загрузка
	virtual void serialize(Serializer &serializer);
};

} // namespace Graphics

#endif // EffectNode_H