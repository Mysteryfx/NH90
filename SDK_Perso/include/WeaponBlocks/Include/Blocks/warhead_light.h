#ifndef __BLOCK_RAMP_H__
#define __BLOCK_RAMP_H__

#include "simulation_block_disc.h"

#ifndef NO_REAL_LOCKON
	#include "graphicEffect.h"
	#include "ObjectLight.h"
#endif


class wWarheadLightDescriptor;
class wAmmunition;

class wWarheadLight : public wSimulationBlockDisc
{
	DECLARE_BLOCK(wWarheadLight, wSimulationBlockDisc, wWarheadLightDescriptor);
public:
	wWarheadLight(const wWarheadLightDescriptor* desc, wSimulationSystem* system);
	~wWarheadLight();

	virtual double simulate(double time);

protected:
	INPUT_WIRES(1, (
        WIRE(Math::Vec3d, vel_)
//		WIRE(type, name),
	))

	OUTPUT_WIRES(1, (
		WIRE(double, lightArg_)
	))

	INPUT_PORT_OBJ_PTR(wAmmunition, parent_node_);

#ifndef NO_REAL_LOCKON
	Effects::ObjectLight* light_;
	gePointer smoke_;
    bool isInitialized_;
#endif
};

class wWarheadLightDescriptor : public wSimulationBlockDiscDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wWarheadLight)

public:
	virtual wWarheadLight* create(Lua::Loader* runtime_cfg,wSimulationSystem* data);

public:
//	DECL_PARAM(type, name);
    DECL_PARAM(double, dt);
	DECL_PARAM(double, start_time);             // Когда загорается (секунды)
	DECL_PARAM(double, duration);               // Сколько горит (секунды)
	DECL_PARAM(Math::Vec3d,  light_position);   // Позиция относительно родителя (метры)
	DECL_PARAM(Math::Vec3d,  light_color);      // Цвет (RGB, 0.0-1.0)
    DECL_PARAM(double,       light_attenuation); // Затухание
	DECL_PARAM(Math::Vec3d,  smoke_position);   // Позиция начала дыма (метры)
	DECL_PARAM(Math::Vec3d,  smoke_color);      // Цвет дыма (RGB, 0.0-1.0)
    DECL_PARAM(double,       smoke_transparency); // Прозрачность дыма (0.0-1.0), 0 - прозрачный
	DECL_PARAM(double,		 smoke_width);      // Толщина дыма
};

#endif
