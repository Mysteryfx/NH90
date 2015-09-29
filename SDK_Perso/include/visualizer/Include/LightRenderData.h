#ifndef EDTG_LIGHT_RENDER_DATA_H
#define EDTG_LIGHT_RENDER_DATA_H

#include <osg/Matrixf>

// передайется через RenderContext::objectdata
struct LightRenderData
{
	enum enLightType
	{
		ltAmbient = 0,
		ltDirectional = 1,
		ltOmni = 2,
		ltSpot = 3
	};

	enLightType type;

	// позиция
	osg::Matrixf m;

	// цвет
	osg::Vec3f diffuseColor;

	// Радиус пятна (ltOmni, ltSpot)
	float att0, att1, att2, radius;

	// cone angle (ltSpot)
	float fi, theta;
};

#endif