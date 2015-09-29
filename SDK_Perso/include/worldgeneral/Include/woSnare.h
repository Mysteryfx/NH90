//                    file woSnare.h
#pragma once

#include "WorldGeneral.h"
#include "wMovingObject.h"
#include "GraphicEffect.h"
#include "ObjectLight.h"
#include "Randomizer\RandomObject.h"
#include "wsType.h"
#include "wsType_Level4.h"
#include "wDetectable.h"

struct woSnareInitData {
    ObjectID    ID_;
    unsigned int type;
    wModelTime  Time0;
    float       scale;
    wModelTime  PosUpdateTime;
    cVector     pos;
    cVector     V;
};

class woSnare;
class woSnareMailManager;

class WORLDGENERAL_API woSnareActivity : public wTime
{
public:
	virtual woSnareActivity::~woSnareActivity();
	void NextEvent();
	woSnare *obj;
};

#define IID_IwoSnare Common::identify<woSnare>() 

class WORLDGENERAL_API woSnare : public MovingObject , wDetectable // Ловушки и аналогичные им
{ 
friend class woSnareActivity;
public:

    woSnare::woSnare();
	~woSnare();
	static RandomObject	randomOrderly;	// Собственный итератор в нормальной последовательности

    void			setSnare(cPosition&, cVector&, wsType, float sc=1.f);
    int				Class(void) { return woClass_Snare;}
	void			Status_Quo(wModelTime t);	// Cостояние в момент t 
	virtual	const float Hrad();

	cPosition&		Position(wModelTime t);
	
  	virtual void	serialize(Serializer &serializer); 
    Effects::ObjectLight*	get_pLight()	{	return pLight;}

	virtual void Parse(Graphics::ModelParser& parser, const Position3& pos,	Graphics::effectState* effects=0);

	float getRCS(float lambda, const cVector & vec) const
	{
		if (is_flare())
			return 0;
		else
			return 5.0; // ЭПР дипольного отражателя
	}

	float getTemperature(float cos_target_aspect_angle) const
	{
		if (is_flare())
			return 0.7 * T_Basis;
		else
			return 10.0;
	}

	float getTemperature(const cVector & vec) const
	{
		return getTemperature(1.0);
	}

	wDetectable * getDetectable() { return this; }
	
	inline bool	is_flare() const
	{
		return  obj_type.Level4() == wsType_Flare || is_signal_flare();
	}

    void netSetSnare(woSnareInitData& sd);
    void netGetCreate(woSnareInitData& sd) const;

private:
	inline bool	is_signal_flare() const
	{
		return	obj_type.Level4() == wsType_Flare_GREEN ||
				obj_type.Level4() == wsType_Flare_RED   ||
				obj_type.Level4() == wsType_Flare_WHITE ||
				obj_type.Level4() == wsType_Flare_YELLOW;
	}

	void			initialize();
	void			createSnareActivity();
	void			CreateLight(void);	// Создать источник света  
	
	float			scale;       // масштаб ловушки

	wModelTime      Time0;       // модельное время создания.
	cPosition		pos_time;
    wModelTime      life_time; // время горения звездочки
    wModelTime      smoke_dissipation_time;

       // Restore
	gePointer 		smokeTail;		// Дымовой хвост
	wtPointer		pSnarActivity; 
    Effects::ObjectLight*	pLight;

	//Параметры ветра
	cVector         vWind;				
};
