#ifndef VolumeEffect_H
#define VolumeEffect_H

#include "Resourcer\Resource.h"
#include "position.h"
#include "DynamicParams.h"
#include "Resourcer\HeapVector.h"

namespace Graphics
{

class RenderObject;
class Light;
class Fog;

// Интерфейс объемных эффектов (в основном - источники света)
// Объект создается для каждого экземпляра эффекта
class VolumeEffect : public Resource
{
public:
	VolumeEffect()	{rtype = rtEffect;}
	virtual ~VolumeEffect(){}

	// получить объем эффекта (TODO: подключить абстрактные объемы)
	// получить максимальный объем (с произвольными динамическими параметрами)
	virtual float GetRadius() = 0;

	// обновить положение эффекта
	virtual void UpdatePos(const Position3& pos) = 0;
	virtual const Position3& GetPos() = 0;

	// получить ссылку на список динамических параметров
	virtual DParamList& GetParamList() = 0;

	// Применить к текущему состоянию для отрисовки объекта
	virtual void Apply(RenderObject *robject) = 0;
	
	// получить интерфейсы редактирования
	virtual Light*	QueryLightInterface()	{return 0;}

	// сколько требует textureStage
	virtual int TextureStageDemand() = 0;
};

// состояние системы по эффектам
typedef HeapVector<VolumeEffect*> effectState;

}

#endif // VolumeEffect_H
