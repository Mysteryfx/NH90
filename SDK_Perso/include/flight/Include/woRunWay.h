// woRunWay.h

#pragma once

#include "Flight.h"
#include "wMovingObject.h"

#define IID_IwoRunWay Common::identify<woRunWay>() 

class FLIGHT_API woRunWay : public MovingObject 
{
public:

	woRunWay();
	~woRunWay();
	int				Class(void){ return woClass_RunWay; }
	const float		Course(void) { return psi; }
	void			setRunWay(cPosition& _pos, float _psi, float _len, wsType _Type);
    virtual void	serialize(Serializer &);

	bool			isRunWayOpen(void);
	virtual void	PostCollisionProcessing( wsCollisionData*, ObjectID);
	const float		Hrad(void) { return length;} //Возвращает длину дорожки
	void			setIDAirdrome(	unsigned int a_id) {	idAirdrome = a_id;}

private:

	unsigned int	idAirdrome;
	float			psi;
	float			length;
};

