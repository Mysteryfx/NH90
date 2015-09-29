#pragma once
#include "entity.h"

class Position3;

class CameraComponent: public EntityComponent
{
public:
	enum ProjectionType 
	{
		PT_PERSPECTIVE,
		PT_ORTHOGONAL,
	};

	ProjectionType projType;
	Position3	*pos;
	float		nearClip, farClip;
	float		hFov, aspect;

	CameraComponent();
	virtual bool init();
};

