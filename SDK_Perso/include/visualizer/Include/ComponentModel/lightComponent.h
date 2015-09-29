#pragma once
#include "entity.h"

namespace Graphics
{
	class VolumeEffect;
}

class Position3;

class LightComponent: public EntityComponent
{
public:
	Graphics::VolumeEffect* light;
	Position3	*pos;
	osg::Vec3f	color;
	float		amount;
	float		phi, theta;

	LightComponent();
	virtual bool init();
	
	Graphics::VolumeEffect *getLight() {return light;}
};

