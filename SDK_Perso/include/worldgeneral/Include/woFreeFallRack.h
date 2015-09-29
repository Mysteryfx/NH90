//                    file woFreeFallRack.h
#pragma once

#include "WorldGeneral.h"
#include "wMovingObject.h"
#include "wTime.h"
#include "woFreeFall.h"

#define IID_IwoFreeFallRack Common::identify<woFreeFallRack>()
#define ARG_MAX 30

class gShapeObject;
class IwoAIPilon;

class WORLDGENERAL_API woFreeFallRack : public woFreeFall  // Свободно падающая подвеска с ракетами.
{ 
public:
	static RandomObject	randomEvenly;	// Собственный итератор в нормальной последовательности
    static woFreeFallRack *   Create(unsigned long id,
                                     const cPosition & pos,
                                     IwoAIPilon * pilon);
	woFreeFallRack();
	virtual void	serialize(Serializer &serializer); 
	//реализация viObject
	virtual gShapeObject*		Shape(wModelTime t);
	virtual Graphics::Model	   *GetModel();
	virtual cPosition&			Position(wModelTime t);
	virtual bool				UseChildren(wModelTime t, viArgumentPurpose purpose);
	//////////////////////////////////////////////////////////////////////////
protected:
	bool			ChildFlag;	// true - подвеска на балке.
	cPosition		pos_local;
};	
