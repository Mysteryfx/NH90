#ifndef __VICLOUDOBJECT_H__
#define __VICLOUDOBJECT_H__

//#include "CumulusCloud.h"
//#include "gObject.h"
#include "viEffect.h"
//#include "CloudMaterial.h"
#include "Renderer/Material.h"
#include "Modeler\gGeometry.h"
#include "Osg/Vec2i"
#include "Osg/Vec3d"
#include "Atmosphere/Clouds.h"

namespace Graphics {
	class Geometry;
}

namespace Weather
{
	class CumulusCloud;
}


class viCloudObject : public viEffect
{
protected:
	// Позиция, тип и радиус
	Position3 position3;
	DWORD cloudtypeindex;
	float radius;
	// Плотность осадков
	float precipitation_power;

	// Старые
	cPosition pos;
	gBox gbox;

	DWORD cloud;

    Graphics::Material *material;
	Graphics::Geometry *geometry;

    Weather::CloudId atmoCloud;

public:	
    viCloudObject( int type, const osg::Vec3d& pos, float radius, float precipitation_power=0,
        Weather::CloudId id = Weather::CloudId(0,0,0xFFFF));
	~viCloudObject();

    const DWORD getCloudId() const;
    
	// Плотность осадков
	float GetPrecipitation();
	// Радиус
	float GetRadius();

	// Позиция
	virtual const Position3& GetPosition();
	// Убрать облако из отрисовки
	void KillCloud();

    void modify(const osg::Vec3d& newPos, double newRadius);
    void changeAtmoCloudId(const Weather::CloudId& id){atmoCloud = id;}

// viEffect
public:	
	// Ф-ции viObject
	virtual cPosition	&Position(wModelTime t);
	virtual const gBox	&LocalBox(wModelTime t);

	// парсинг
	virtual void Parse(Graphics::ModelParser& parser, const Position3& pos, Graphics::effectState* effects=0);
};


#endif
