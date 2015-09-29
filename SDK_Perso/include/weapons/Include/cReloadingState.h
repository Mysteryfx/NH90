#ifndef __ReloadingState__
#define __ReloadingState__

// ���������� ��������� ����� ����������� ��� �������
#include "wBarrel.h"
#include "wStowage.h"

class cLauncher;

enum ReloadingTypes {
    rtOrdinary = 1, // ����� �������� ����� ������������� ��������������, ���������� �� ������ �������
    rtSimultaneously = 2, // ���������������� ������������� ����������� ���� ������� (�� ����������� �������� �� ���������� ��������)
    rtSequentialy = 3 // ����������� ������� ���������� ��� ����������� ���� �������,
    // ���� ��� ������ �� ��� � ����������� ����������� ��������
    // ������ ����� ����� ������ �� 1 � �� ���������� �������.
    // ������: ����������� ��� �������� � ����; M2 Bradley (TOW, ������ = 1), ��� ��� (������ ����� 4)

};
enum InterruptFlagValues {
    ifvInterruptRearming = 1,
    ifvInterruptReloading = 1<<1
};

enum ReloadingState {
    rsTrackingMissile,
    rsNormal,
    rsReloading,
    rsRearming,
    rsNeedToBeMaintained,
};

class cReloadingState
{
    friend cLauncher;
public:
    cReloadingState(void):launcher(NULL),hi_rate(true),stowages(NULL),currentStowage(NULL), interruptFlag(0){};
    virtual ~cReloadingState(void){};

    //initialization
    virtual void                setLauncher(cLauncher* ln) { launcher = ln; };

    //control
    virtual bool                reloadBarrel(wBarrel* br) = 0;
    virtual void                start_reload_all_barrels() = 0;
    virtual void                startRearming() = 0;
    virtual void                interrupt() {interruptFlag = ifvInterruptRearming | ifvInterruptReloading; };
    virtual void                switch_to_high_rate(bool hirate_) { hi_rate = hirate_; };
    virtual void                setActiveStowage(wStowage* stowage_) = 0;
    virtual void                activateStowage(wStowage* stowage_) = 0;

    
    //getters
    virtual int                 getReloadingType() const = 0;
    virtual const wModelTime    getDelayToReady(int* reloadingState_) const = 0;
    virtual const int           getState() const = 0;
    virtual bool                is_reloading() const = 0;
    virtual const bool          needToBeRearmed() const = 0;

    //handlers
    virtual void                onReloadPosition() = 0;
    virtual void                maintainAfterTracking(const bool noMissilesTracking) = 0;
    
    //MAIL
    virtual void                net_reloadStarted() = 0;
    virtual void                net_reloadFinished(wStowage* mailedStowage) = 0;

protected:
    cLauncher*                  launcher;
    bool                        hi_rate;
    wStowage*                   currentStowage;
    ed::vector<wStowage*>*      stowages;
    int                         interruptFlag;
};

#endif