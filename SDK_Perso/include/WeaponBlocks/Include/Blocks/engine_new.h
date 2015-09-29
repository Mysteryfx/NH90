#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "WeaponBlocks.h"

#include "simulation_block_disc.h"

const int cEngTail_Smoke	= 1;
const int cEngTail_Burning	= 2;

class wEngineDescriptor;

// Базовый класс для хвоста
class wAmmunition;

class wEngineTail;

// Описывает двигатель ракеты или бомбы
// А также управляет эффектами
// Двухступенчатый двигатель
class WEAPON_BLOCKS_API wEngine : public wSimulationBlockDisc
{
	DECLARE_BLOCK(wEngine, wSimulationBlock, wEngineDescriptor)

public:
	wEngine(const wEngineDescriptor* desc, wSimulationSystem* system);
	virtual ~wEngine();

	virtual void reset();

	void turnOn();

	virtual double simulate(double time);
	// TODO: это надо удалить
	void simulateDt(double time, double dt);
protected:
	// Включить
	void turnOn_(bool on);
	// Сменить состояние
	void switchState_(int state);
	// Заменить хвост
	void createTail_(double width);

protected:
	INPUT_WIRES(1, (
		WIRE(Math::Vec3d, vel_)
	))

	OUTPUT_WIRES(2, (
		WIRE(double, thrust_),		// Тяга, Н
		WIRE(double, fuel_)		// Остаток топлива, кг
	))

	INPUT_PORT(bool, on_);
	INPUT_PORT_OBJ_PTR(wAmmunition, parent_node_);

	int state_;						// 0 - выключен, 1 - буст, 2 - поддерживание, 3 - отработал
	double startTime_;				// Когда пустили


	const double * rail_passed;

	ed::vector<wEngineTail*> tails_;
	bool prevSndState;
};


// Параметры двигателя
class WEAPON_BLOCKS_API wEngineDescriptor : public wSimulationBlockDiscDescriptor
{
public:
	DECLARE_BLOCK_DESCRIPTOR(wEngine);

	virtual void serialize(wBlockSerializer& ser);

	virtual wEngine* create(Lua::Loader* runtime_cfg,wSimulationSystem* data);

	// Для прицела
	void setDt(double dt);
	void copyData(const wEngineDescriptor& desc); 

	unsigned int getColor() const; 

public:
	DECL_PARAM(double, dt);
	DECL_PARAM(double, impulse);			// Импульс
	DECL_PARAM(double, fuel_mass);			// Кол-во топлива, кг
	DECL_PARAM(double, boost_time);			// Разгонное время (может быть = 0)
	DECL_PARAM(double, work_time);			// Полное рабочее время 
	DECL_PARAM(double, boost_factor);		// (Тяга на разгоне)/(Поддерживающая тяга), если boost_time = 0 => factor=1
	DECL_PARAM(ed::vector<Math::Vec3d> , nozzle_position);		// Положение сопел (тяга по -x)
	DECL_PARAM(double, tail_width);			// ширина следа
	DECL_PARAM(Math::Vec3d, smoke_color);
	DECL_PARAM(double	  , smoke_transparency);

	double fuel_rate;			// расход топлива в поддерживающем режиме
	double thrust;				// тяга в поддерживающем режиме
};

#endif
