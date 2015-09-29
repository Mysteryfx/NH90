#ifndef __ReloadingStateOrdinary__
#define __ReloadingStateOrdinary__

#include "cReloadingState.h"
#include "wActivity.h"

class cReloadingStateOrdinary : public cReloadingState
{
public:
    cReloadingStateOrdinary();
    virtual ~cReloadingStateOrdinary();

    //initialization
    virtual void                setLauncher(cLauncher* ln);

    //control
    virtual bool                reloadBarrel(wBarrel* br);
    virtual void                start_reload_all_barrels();
    virtual void                startRearming();
    virtual void                interrupt();
    virtual void                switch_to_high_rate(bool hirate_);
    virtual void                setActiveStowage(wStowage* stowage_);
    virtual void                activateStowage(wStowage* stowage_);

    //getters
    inline virtual int          getReloadingType() const { return rtOrdinary; };
    /*  rsNormal, rsTrackingMissile, rsReloading, rsRearming, rsNeedToBeMaintained */
    virtual const wModelTime    getDelayToReady(int* reloadingState_ = NULL) const; 
    virtual const int           getState() const;
    virtual bool                is_reloading() const;
    virtual const bool          needToBeRearmed() const;

    //handlers
    virtual void                onReloadPosition() {};   
    virtual void                maintainAfterTracking(const bool noMissilesTracking);
    void                        finishRearming();

    //MAIL
    virtual void                net_reloadStarted() {};
    virtual void                net_reloadFinished(wStowage* mailedStowage);

protected:
    virtual void                reload_all_barrels(wModelTime reloadFinishTime_);
    virtual const wModelTime    prepareBarrelReloadDelay() const;
    virtual const bool          checkStowageIfItNeedsToBeRearmed(const wStowage* st) const;

    //handlers
    virtual void                stowageActivationFinished(wModelTime& timeStep);
    void                        maintainRearming(wModelTime& timeStep);

    typedef ed::deque<wBarrel*> BarrelsToLoad;
    BarrelsToLoad   barrelsToLoad;
    wModelTime      reloadFinishTime;
    wStowage*       currentRearmingStowage;
    wModelTime      rearmFinishTime;
    wtPointer       rearming_activity;
    wtPointer       current_activity;

};
#endif
