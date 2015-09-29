#ifndef __WARHEAD_STANDARD_H__
#define __WARHEAD_STANDARD_H__

#include "simulation_block_disc.h"

class wWarheadStandardBlockDescriptor;
struct Damage;
struct Warhead;
class  Registered;

class wWarheadStandardBlock : public wSimulationBlockDisc
{
	DECLARE_BLOCK(wWarheadStandardBlock, wSimulationBlockDisc, wWarheadStandardBlockDescriptor);
public:
	wWarheadStandardBlock(const wWarheadStandardBlockDescriptor* desc, wSimulationSystem* system);

	virtual double simulate(double time);
	double hit_time;
protected:
	bool is_about_to_explode() const;
	void explode_(bool);

	void calcDamageForObject(Registered * reg, 
							 Damage		 &damage,
							 Warhead     &warhead,
							 double		  cumulative = 0);

	void kineticHit();

protected:
	INPUT_WIRES(5, (
        // Позиция - локальная если object_id_ != 0
		WIRE(Math::Vec3d, pos_),
		WIRE(Math::Vec3d, vel_),
        WIRE(Math::Vec3d, normal_), // Только если object_id_ != 0, в мировой системе координат
		WIRE(int, object_id_),
		WIRE(int, objPartName_)
	))

	OUTPUT_WIRES(0, ())

	INPUT_PORT (bool, explode_port_);

	OUTPUT_PORT(bool	   , died_);
	OUTPUT_PORT(Math::Vec3d, warheadPosition_);
	OUTPUT_PORT(Math::Vec3d, rebound_);


	INPUT_PORT_CONST(int, my_id_); 
	INPUT_PORT_CONST(bool  , suppressExplosion_);
	INPUT_PORT_CONST(double, fuzeDelay_);

};


class wWarheadStandardBlockDescriptor : public wSimulationBlockDiscDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wWarheadStandardBlock);

public:
	wWarheadStandardBlock* create(Lua::Loader* runtime_cfg,wSimulationSystem* data);

public:
	DECL_PARAM(double, mass);
	DECL_PARAM(double, caliber);
	DECL_PARAM(double, piercing_mass);
	DECL_PARAM(double, expl_mass);

    // Коэффициенты: damage, effect, crater
    DECL_PARAM(double[2], obj_factors);          // попадание в объект
    DECL_PARAM(double,    cumulative_factor);    // кумулятивный эффект
    DECL_PARAM(double[3], concrete_factors);     // попадание в бетон (полоса, объекты)
    DECL_PARAM(double,    concrete_obj_factor);  // попадание в бетонный объект - дамаж самому объекту
    DECL_PARAM(double[3], other_factors);        // попадание в другие вещи
    DECL_PARAM(double,    cumulative_thickness); // толщина бронепробития для кумулятивного эффекта, м

	DECL_PARAM(bool, fantom);			// наносить ли повреждения
};


#endif
