#pragma once
#include "cPosition.h"

struct BC_InputData{
    BC_InputData(){};
    BC_InputData(const dPosition& myPos_, const cVector& myVel_, const cVector& targetPnt_, const cVector& targetVel_, const cVector& windVel_) :
        myPos(myPos_),
        myVel(myVel_),
        targetPnt(targetPnt_),
        targetVel(targetVel_),
        windVel(windVel_) {};

    dPosition myPos;
    cVector myVel;
    cVector targetPnt;
    cVector targetVel;
    cVector windVel;
};
