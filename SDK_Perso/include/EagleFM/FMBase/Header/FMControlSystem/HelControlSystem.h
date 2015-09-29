#ifndef __HelControlSystem_h__
#define __HelControlSystem_h__

#include "Base.h"

#include "FMControlSystem/ControlSystem.h"
#include "FMControlSystem/ControlCollective.h"


namespace EagleFM
{

class FMBASE_API HelControlSystem : public ControlSystem
{
public:
	HelControlSystem();
	virtual ~HelControlSystem() {}

    virtual void        setCollectiveDir(int Dir) {} //задать движение РОШ
	virtual void        setCollectiveStopper(bool value) {} //стоппер РОШ

	//	доступ
	Collective*			getCollective() { return &collective; }
	Collective			getCollective() const { return collective; }

protected:
	//Хозяин
	Collective			collective;
};


}

#endif