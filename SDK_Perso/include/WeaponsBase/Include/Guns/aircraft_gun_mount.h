#pragma once

#include "Guns/aircraft_gun_carriage.h"
#include "Guns/Effects/Gun_Effect.h"

#include "cPosition.h"

namespace Mail { class Stream; }
namespace Lua  { class Loader; }
namespace weapons_gun {
	class wGun; 
	class wGunDescriptor; 
}

class wAmmoSupply;
class wAmmoSupplyDescriptor;
class wAircraftGunMountDescriptor;
class wGunCarriageBase;

class IwoLA;
class IwoAIPilon;
// Мы вынуждены завести этот класс, поскольку woLA не грузит свое описание из базы данных
class WEAPONSBASE_API wAircraftGunMount
{
	friend class weapons_gun::SimulateEffect;
	friend class weapons_gun::VisualEffect;

public:

	wAircraftGunMount(IwoLA* la,
		int8_t gun_id, 
		const wAircraftGunMountDescriptor* desc,
		Lua::Loader* cfg);

	wAircraftGunMount(IwoLA* la,
                      IwoAIPilon* pilon,
					  int8_t gun_id, 
                      const wAircraftGunMountDescriptor* desc,
                      Lua::Loader* cfg);

	~wAircraftGunMount();

	void beginBurst(Mail::Stream& stream);
	void endBurst(Mail::Stream& stream);

	void sync_T(Mail::Stream& stream); /*synchronizes gun temperature*/

	bool isFiring() const;
	bool isActive() const;

	bool isEmpty() const;
	int getAmmoRoundsCount() const;
	int getAmmoRoundsInitialCount() const;
	bool isReadyToFire() const;

    cPosition        getMuzzlePositionBoreSight() const	{ return carriage_->getMuzzleBoreSightPosition(); }
    const cPosition& getMuzzlePosition() const			{ return carriage_->getMuzzlePosition(); } /*in global coords*/
    const cPosition& getMuzzlePositionLocal() const		{ return carriage_->getMuzzlePositionLocal(); }
	const cPosition& getEjectorPosition() const			{ return carriage_->getEjectorPosition(); }
	const cVector&   getEjectorDir() const				{ return carriage_->getEjectorDir(); }

	double getAmmoSupplyMass() const;
	Math::Vec3d getAmmoSupplyPosition() const;

	weapons_gun::wGun* getGun() const { return gun_; }
	wAmmoSupply* getAmmoSupply() { return supply_; }
	wGunCarriageBase* getCarriage() const { return carriage_; }

	void resetAmmo(float gun_load_factor = 1.0f);

    void discharge();

	unsigned char getDropCartridge() const { return drop_cartridge_; }

	unsigned  getAmmoType() const;
	void      setAmmoType(unsigned t); 

    double    getAzimuth()   const { return carriage_->getAzimuth(); }
    double    getElevation() const { return carriage_->getElevation(); }

    void      setAzimuth(double a)   { carriage_->setAzimuth(a); }
    void      setElevation(double e) { carriage_->setElevation(e); }

    bool      getAftGunMount() const { return carriage_->getAftGunMount(); }
	bool      netIsMaster() const { return carriage_->net_is_master(); }
	bool	  sync_T_active() const { return sync_T_active_; }

	int		  getBurstLength() const;

	double    getEffectiveFireDistance() const;

	void	  setFailure(int failure, bool failure_status, float azimuth_deviation /*in degr*/, float elevation_deviation /*in degr*/);

	bool	  getFailure() const { return failure_; }
	
	void	  updateEffectsArgs(double dt);

	void 	dismount(bool value)
	{
		dismounted_ = value;
	}

	bool	dismounted() const { return dismounted_; }

	const ed::string & getName() const;
	const ed::string & getShortName() const;
	const ed::string & getDisplayName() const;

	void simulate(double dt);

	const wAircraftGunMountDescriptor* get_descriptor() const { return desc_; } 

protected:

	void initialize(Lua::Loader* cfg);

	weapons_gun::wGun*			gun_;
	wAmmoSupply*	supply_;
	wGunCarriageBase* carriage_;

	unsigned char drop_cartridge_;
	bool failure_;
	bool dismounted_;

	const wAircraftGunMountDescriptor* desc_;

	bool sync_T_active_; //net fantom temperature sync flag

	ed::vector<weapons_gun::SimulateEffectPtr> sim_effects_;
	ed::vector<weapons_gun::VisualEffectPtr> vis_effects_;
};

