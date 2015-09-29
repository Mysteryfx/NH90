#pragma once
#include "ObjectLight.h"
#include "EffectHolder.h"
#include "graphicEffect.h"
#include "WorldGeneral.h"

class MovingObject;

enum {
    CELL_DISABLE	= 0x01, // отсутствтует
    CELL_OFF		= 0x02, // выкл
    CELL_LEAKAGE	= 0x04, // утечка
    CELL_SMOKE		= 0x08, // дым
    CELL_FIRE		= 0x10, // пожар
    CELL_INVERSION	= 0x20, // инверс. след.
};

// Структура состояния ячейки(части) самолета (для обработки столкновения)
struct WORLDGENERAL_API ImpactCell
{
	int						damage;			//номер ячейки с пожаром, -1 по-умолчанию
	gePointer				pTail;			//указатель на хвост пожара
	EffectHolderPtr			pTailNew;
	wModelTime				time_start_fire;//время возгорания
	wModelTime				time_stop_fire; //время запланированного тушения
	cPosition				fire_pos;		//матрица пламени (в локальной системе объекта)
	cVector					fire_addVel;	//добавочный вектор скорости для пламени
	float					fire_power;		//мощность пламени
	unsigned char			status;         //состояние
	unsigned char			available_staus;//допустимое состояние данной ячейки
	bool					factor;         //фактор внешнего воздействия (true - внеш.)
	
	ImpactCell()
	{
		damage = -1;
		pTail	= 0;
		time_start_fire = 0;
		time_stop_fire = 0;
		fire_pos.ONE();
		fire_addVel = cVector(0.0f);
		fire_power = 1.0f;
		status = CELL_OFF;
		available_staus = CELL_OFF; 
		factor = false;
	}

	~ImpactCell()
	{
		off();
	}

	ImpactCell&		operator = (ImpactCell& v)	 
	{	damage			= v.damage; 
		pTail			= v.pTail; 
		time_start_fire	= v.time_start_fire;
		time_stop_fire	= v.time_stop_fire;
		fire_pos		= v.fire_pos; 
		fire_addVel     = v.fire_addVel;
		fire_power		= v.fire_power;
		status			= v.status;
		factor			= v.factor;
		return *this;
	}
	void off()
	{
		if(pTail)
		{
			pTail->undock();
			pTail = 0;
		}
		if (pTailNew)
		{
			pTailNew->stop();
			pTailNew = 0;
		}
		time_start_fire = 0;
		time_stop_fire = 0;
		status = CELL_OFF;
		factor = false;
	}

	void make_fire(MovingObject* obj, float scale, bool external_control = false);
	void make_leakage(MovingObject* obj, float scale, bool external_control = false);
	void make_blacktail(MovingObject* obj, float scale, bool external_control = false);
	static void make_test_fire(MovingObject* obj);

	friend Serializer& operator << (Serializer& serializer, ImpactCell& data)
	{
		return serializer << data.damage
//			<< data.pTail 			
//			<< (wtPointer)0 			
			<< data.time_start_fire
			<< data.time_stop_fire
			<< data.fire_pos.p
			<< data.fire_addVel
			<< data.fire_power
		    << data.status
			<< data.factor;
	}
};

