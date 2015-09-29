#ifndef __System_h__
#define __System_h__

#include "_config.h"
/**
 * The System class represents methods for operating every-frame processes.
 * It supports limited functionality which depend on process real time.
 * @author Alexander Matveev
 */
class ED_CORE_EXTERN System 
{
protected:
    bool                initializedStatus;
public:
    System();
    virtual ~System();
    /** The initialize method initializes the system and returns true if succeded */
	virtual bool        initialize();
    /** The uninitialize method uninitializes the system and returns true if succeded */
	virtual bool        uninitialize();
    /** The reinitialize method reinitializes the system and returns true if succeded */
	virtual bool        reinitialize();
    /** The initialized method returns current initialized status */
    virtual bool	    initialized();
    /** The process method is called every frame and realize unlimited algorithm. */
    virtual bool        process();
    /** The process method is called every frame and realize limited algorithm. */
    virtual bool        process(unsigned int requestedTime);
};

#define INITIALIZE() initializedStatus = true; return true;
#define UNINITIALIZE() initializedStatus = false; return true;
#define CHECK_UNINITIALIZED() if (initialized()) return false;
#define CHECK_INITIALIZED() if (!initialized()) return false;

#define CHECK() if (!initialized()) return;

#endif
