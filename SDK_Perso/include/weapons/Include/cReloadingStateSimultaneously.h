#ifndef __ReloadingStateSimultaneously__
#define __ReloadingStateSimultaneously__

#include "cReloadingStateOrdinary.h"

class cReloadingStateSimultaneously : public cReloadingStateOrdinary
{
public:
    cReloadingStateSimultaneously() {};

    //control
    virtual bool                reloadBarrel(wBarrel* br);
    virtual void                start_reload_all_barrels();
    virtual void                activateStowage(wStowage* stowage_);

    //getters
    inline virtual int          getReloadingType() const { return rtSimultaneously; };
    virtual const wModelTime    getDelayToReady( int* reloadingState_ ) const;
    virtual const int           getState() const;
    virtual const bool          needToBeRearmed() const;

    // handlers
    virtual void                maintainAfterTracking(const bool noMissilesTracking);
    virtual void                onReloadPosition();
    virtual void                reloadFinished(wModelTime& timestep);
    virtual void                stowageActivationFinished(wModelTime& timeStep);

    //MAIL
    virtual void                net_reloadStarted();
    virtual void                net_reloadFinished(wStowage* mailedStowage);

protected:
    virtual void                reload_all_barrels();
};

#endif
