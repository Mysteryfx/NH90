#ifndef __ReloadingStateSequentialy__
#define __ReloadingStateSequentialy__

#include "cReloadingStateOrdinary.h"

class cReloadingStateSequentialy : public cReloadingStateOrdinary
{
public:
    cReloadingStateSequentialy() {};
    ~cReloadingStateSequentialy() {};

    //control
    virtual bool                reloadBarrel(wBarrel* br);
    virtual void                start_reload_all_barrels();
    virtual void                startRearming();
    virtual void                interrupt();
    virtual void                activateStowage(wStowage* stowage_);

    //getters
    inline virtual int          getReloadingType() const { return rtSequentialy; };
    virtual const int           getState() const;
    virtual const wModelTime    getDelayToReady( int* reloadingState_ ) const;
    virtual const bool          needToBeRearmed() const;

    // handlers
    virtual void                maintainAfterTracking(const bool noMissilesTracking);
    virtual void                onReloadPosition();

    //MAIL
    virtual void                net_reloadStarted();
    virtual void                net_reloadFinished(wStowage* mailedStowage);

protected:
    virtual void                reload_all_barrels() {};
    void                        put_ammo_to_barrels(wModelTime& timeStep);
    virtual void                stowageActivationFinished(wModelTime& timeStep);
    void                        prepareBarrelsReloadTimes();
    void                        checkIfReloadingAvailable(wModelTime& timeStep);
    virtual const bool          checkStowageIfItNeedsToBeRearmed(const wStowage* st) const;

    //handlers
    virtual void                maintainRearming(wModelTime& timeStep);
    virtual void                reloadFinished(wModelTime& timestep);
    virtual void                finishRearming();

    ed::deque<wModelTime>       barrelsReloadTimes;
};

#endif
