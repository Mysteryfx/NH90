#pragma once

#include "WorldGeneral.h"
#include "wMovingObject.h"
#include "wsTarget.h"

enum wModulation;

class ListTargetManager;

class WORLDGENERAL_API IWing
{
public:
	virtual ~IWing() = 0;
	virtual unsigned int	   getWingmanID(int num) const = 0; // выдать ведомого 
	virtual unsigned int	   getLeaderID(int num) const = 0; // выдать лидера	
	virtual unsigned int	   getAI_ID	(int num) const	 = 0; // выдать ID num члена звена 
	virtual const WingTargets& getWingTargets()	const = 0;
	virtual bool			   isCommAvailable() const = 0;
	virtual float			   get_frequency() const = 0;
	virtual wModulation		   get_modulation() const = 0;
	virtual unsigned int	   getDep_Adr_ID(void) const = 0;   // МИ аэродрома вылета       
	virtual unsigned int	   getArr_Adr_ID(void) const = 0;   // МИ аэродрома прибытия
	virtual unsigned int       getGrp_ID(void) const = 0;

	virtual const ListTargetManager*  getTargetManager() const = 0;
};