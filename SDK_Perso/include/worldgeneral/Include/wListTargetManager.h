//                    file wListTargetManager.h

#ifndef __wListTargetManager_h__
#define __wListTargetManager_h__

#include "WorldGeneral.h"
#include "wMovingObject.h"
#include "Common.h"
#include "wListener.h"

typedef ed::vector<ObjectID> ListTargets;

class WORLDGENERAL_API ListTargetManager
{
 public:

	ListTargetManager();
    virtual ~ListTargetManager();

    virtual void	initializeTargetManager();
    virtual void	uninitializeTargetManager();
    virtual void	addTarget(unsigned int);
    virtual void	removeTarget(unsigned int);
	virtual bool	checkPresenceObjectInList(unsigned int);
	virtual void	serialize(Serializer &);
	const ListTargets	&getListTargets() const {return listTargets;}

 protected:
  
	ListTargets		listTargets;
};

typedef char coalition_t;

class WORLDGENERAL_API CoalitionTargetManager: public wListener
{
public:

    CoalitionTargetManager(coalition_t coalition_);
    virtual ~CoalitionTargetManager();

    void                scan();
    void                onCreate(Registered *);
    void                onDestroy(Registered *);
    coalition_t         coalition()const{return my_coalition;};
    const ed::list<ObjectID>& get_listTargets()const{return listTargets;};
    const ed::list<ObjectID>& get_listHumanTargets()const{return listHumanTargets;};

private:
    bool                check_target(MovingObject* theObject_);
    bool                check_human(MovingObject* theObject_);

    coalition_t         my_coalition;
    ed::list<ObjectID> listTargets;
    ed::list<ObjectID> listHumanTargets; // объекты управляемые живыми игроками, без фильтра по коалиции!
};

extern WORLDGENERAL_API CoalitionTargetManager* globalCoalitionTargetManagerRed;
extern WORLDGENERAL_API CoalitionTargetManager* globalCoalitionTargetManagerBlue;

#endif




