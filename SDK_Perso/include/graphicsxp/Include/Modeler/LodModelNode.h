#ifndef LodModelNode_H
#define LodModelNode_H

#include "graphicsxp.h"
#include "ModelNode.h"

#include <ed/map.h>

namespace Graphics
{

// Узел, из которого расходятся различные уровни детализации
class GRAPHICSXP_API LodModelNode : public ModelNode
{
protected:

	// коэффициент умножения дальностей переключения уровней
	static float multSq;
    static float adder;

	// таблица, на каких расстояниях использовать соответствующий уровень детализации
	typedef  ed::map<float, int>  distanceTable;
	distanceTable distances;

public:
	LodModelNode(ModelNode *_parent = 0);

	// глобальный коэффициент дальности переключения LOD
	static void __cdecl SetDistanceMultipliers(float mult, float adder);
    static void __cdecl SetDistanceMultipliers2(float mult, float adder);
	static float GetDistanceMultiplier();
	/// Returns square of distance multiplier.
	static float GetDistanceMultiplier2();
    static float GetDistanceAdder();

	// таблица уровней детализации
	float GetLodDistance(int i);
	void SetLodDistance(int i, float dist);
	int GetLodLevel(float distSq);

	int GetLodsCount() {return (int)distances.size();}

	// проход по иерархии
	// будет продолжен проход уровня детализации, для которого (cam.p - pos.p).length() < dist
	virtual bool Parse(ModelInstance *object, ModelParser *parser, const Position3& parent_pos, effectState *parent_effects);

	// сохранение/загрузка
	virtual void serialize(Serializer &serializer);
	
	// копирование модели
	virtual ModelNode *CloneEmpty();
	virtual void CopyFrom(ModelNode *source);
};

} // namespace Graphics

#endif // LodModelNode_H