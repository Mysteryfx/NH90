#pragma once

#include <ed/list.h>
#include "Terrain.h"
#include "GraphicEffect.h"
#include "ParticleApi.h"
#include "Random.h"

class lStandartStructure;

// Пожар на наземном объекте
// горит и наносит дополнительный вред своему объекту
class TERRAIN_API lObjectFire : public wTime
{
protected:
	lStandartStructure *object;
    EffectHolderPtr fire;
	int level;
	static ed::Random rand;

public:
	// создать пожар на obj.
	// p - точка пожара в мировых координатах
	// level - начальный размер пожара
	lObjectFire(lStandartStructure *obj, const cPoint& p, float level, const char* effect,float effect_scale = -1);
	~lObjectFire();

	virtual void NextEvent();
};