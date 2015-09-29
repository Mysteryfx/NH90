#pragma once

#include "wListTargetManager.h"

class wCarEventualTargets :  public ListTargetManager
{
public:
	wCarEventualTargets( unsigned char myCountry = 0 );
	virtual ~wCarEventualTargets(void);

	virtual void	initializeCarEventualTargets();
	virtual void	addCarEventualTargets(Registered*);
	virtual void	removeCarEventualTargets(Registered*);
	ListTargets	&	getListTargets(void){return listTargets;}
	virtual void	setCountry(unsigned char c);
	virtual void	serialize(Serializer &serializer);

protected:

	virtual bool  inspectObject(MovingObject *);
	virtual bool  inspectObject(lLandObject *);
	unsigned char friendCountry;  // номер своей коалиции    
};

