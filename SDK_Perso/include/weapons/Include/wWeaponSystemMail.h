#ifndef __wWeaponSystemMail_h__
#define __wWeaponSystemMail_h__


class wWeaponSystem;
class cLauncher;

#include "wStowage.h"

struct FireMsgData
{
	FireMsgData()
		: barrelId(0), launcherId(0),
		missileID(0), targetID(0)
	{}

	FireMsgData(cLauncher::LauncherID launcherId, 
		wBarrel::BarrelID barrelId,	
		ObjectID missileID, ObjectID targetID)
		:barrelId(barrelId), launcherId(launcherId), 
		missileID(missileID), targetID(targetID)
	{}

	FireMsgData(cLauncher::LauncherID launcherId, 
		wBarrel::BarrelID barrelId)
		:barrelId(barrelId), launcherId(launcherId),
		missileID(0), targetID(0)
	{}

	wBarrel::BarrelID barrelId;
	cLauncher::LauncherID launcherId;
	//Только для ракет
	ObjectID missileID;
	ObjectID targetID;
};

class wWeaponSystemMail {
public:
		wWeaponSystemMail();
		~wWeaponSystemMail();

    static void assignTargetEx(cLauncher* turr, wsTarget tgt);
	static void releaseTarget(cLauncher* turr);
    static void trackingEmissionTurnOn(wWeaponSystem* sys);
	static void fireWeapon(cLauncher* tur, const FireMsgData& fmd);
    static void fireWeaponStart(cLauncher* turr);
    static void fireWeaponStop(cLauncher* tur, int actualAmmoCapacityInActiveStowage);
    static void rotateToReloadPosition(cLauncher* turr);
    static void reloadingStart(cLauncher* turr);
    static void reloadingMaintain(cLauncher* turr, std::queue<int>& loadedBarrels_);
    static void reloadingFinish(cLauncher* tur, int activeStowageID, int actualAmmoCap);

}; // class wWeaponSystemMail


#endif /* __wWeaponSystemMail_h__ */
