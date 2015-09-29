#pragma once

#include "Terrain.h"
#include "Common/Link.h"
#include "wTime.h"
#include "LandObjects/lLandObject.h"


#define IID_lMine Common::identify<lMine>()

// Мина
// взрывается при приближении объектов
class TERRAIN_API lMine : public lLandObject, public LinkHost
{
public:
    class Activity : public wTime
    {
        Link<lMine> self;
        wModelTime dt;
        float r;


    public:
        struct ImpactData
        {
            Registered * obj1;
            Vector3      my_pos;
        };

        Activity(lMine *_self, wModelTime _dt, float _r);

        void NextEvent();
    };

    static lLandObject* CreateObject();

	lMine();

	// если объект получил достаточно повреждений, чтобы деградировать
	virtual void Degrade(bool silent);
};
