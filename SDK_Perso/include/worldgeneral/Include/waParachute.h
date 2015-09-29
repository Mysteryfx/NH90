//                    file waParachute.h
#ifndef __waParachute_h__
#define __waParachute_h__

#include "WorldGeneral.h"
#include "wMovingObject.h"

class WORLDGENERAL_API waParachute;
class WORLDGENERAL_API waParachuteLife : public wTime
{
public:
	waParachuteLife(waParachute *p, wModelTime dt);
	virtual ~waParachuteLife();
	virtual void NextEvent();

	waParachute *par;
};

#define IID_IwaParachute Common::identify<waParachute>()

void  WORLDGENERAL_API PutWheelTrack(const cPosition& pos_);


class WORLDGENERAL_API waParachute : public MovingObject // Сброшенный парашют
{ // Неподвижные миражи, живущие ограниченное время. 
public:

	static void Create(const cPosition& pos_, wsType F_Type);

	waParachute();
	virtual ~waParachute(void);

	cPosition&		Position(wModelTime t)	{	return pos;}

	void			setParachute(const cPosition& pos_, wsType Type);
	virtual void	serialize(Serializer &serializer); 

	wtPointer		parLife;

private:

    void            Init(void);
	
}; 

#endif