#pragma once

#include "WeaponsBase.h"
#include "IGun_carriage.h"
#include "cPosition.h"

#include "Randomizer/RandomObject.h"
#include "Guns/gun_model_proxy.h"

namespace Graphics { class ModelConnector; }

class IwoLA;
class IwoAIPilon;
class viObjectNode;
class viMovingObject;

class wGunCarriageBase : public wIGunCarriage, public wGunModelProxy
{
public:
	wGunCarriageBase(IwoLA* la, int8_t gun_index);
	virtual ~wGunCarriageBase();

	virtual Math::Vec3d getShellFirePosition();
	virtual Math::Vec3d getShellFireDirection();
	virtual Math::Vec3d getDirection();
	virtual Math::Vec3d getVelocity();

	virtual ObjectID getID() const;
	virtual wcCoalitionName getCoalition() const;
	virtual unsigned char getState() const;

	bool net_is_master() const;

	// Отдача
	virtual void acceptImpulse(double impulse, const Math::Vec3d & rot);
	// Эффект
	virtual void onGunStartBurst();
	virtual void onGunStopBurst();
	virtual void onGunShot();

	//инициализация
	void  setMuzzlePositionInitial(const Math::Vec3d & m_pos_) 
	{
		muzzlePosInitial_.x = m_pos_.x;
		muzzlePosInitial_.y = m_pos_.y;
		muzzlePosInitial_.z = m_pos_.z;
	}

	void  setEjectorPositionInitial(const Math::Vec3d & m_pos_) 
	{
		ejectorPosInitial_.x = m_pos_.x;
		ejectorPosInitial_.y = m_pos_.y;
		ejectorPosInitial_.z = m_pos_.z;
	}

	void  setEjectorDirInitial(const Math::Vec3d & m_dir_) 
	{
		ejectorDirInitial_.x = m_dir_.x;
		ejectorDirInitial_.y = m_dir_.y;
		ejectorDirInitial_.z = m_dir_.z;
	}

	void setBarrelCircularError(float barrel_circular_error)
	{
		barrel_circular_error_ = barrel_circular_error;
	}

	void    setBoresightAzimuth  (double a)  {   azimuth_boresight = a; }
	void    setBoresightElevation(double e)  { elevation_boresight = e; }

	void	setAzimuth(double a)		{ azimuth_ = a;   cachedTime_ = -1;}
	void	setElevation(double e)		{ elevation_ = e; cachedTime_ = -1;}

	double	getAzimuth() const          { return azimuth_;}
	double	getElevation() const        { return elevation_;}

	void	setAftGunMount(bool value)	{ aft_gun_mount_ = value; }
	bool	getAftGunMount() const		{ return aft_gun_mount_; }

	void	setConnectorName(const ed::string & name_) { connectorName_ = name_; ; };
	void	setEjectorConnectorName(const ed::string & name_) { ejectorConnectorName_ = name_;};

	const cPosition & getMuzzlePosition() 
	{
		cachePosition_();
		return cachedPos_;
	}

	const cPosition & getEjectorPosition()
	{
		cachePosition_();
		return cachedEjectorPos_;
	}

	const cVector & getEjectorDir() const
	{
		return ejectorDirInitial_;
	}

	virtual cPosition getMuzzleBoreSightPosition() const = 0;

	const cPosition & getMuzzlePositionLocal() 
	{
		cachePosition_();
		return cachedPosLocal_;
	}

	bool isHuman() const; 
	virtual void sync_gun_T() = 0;

	viMovingObject* master_object();
	IwoLA		* getAircraft() const { return la_;  };

protected:

	virtual void cachePosition_() = 0;
	virtual void checkConnector_();
	virtual viObjectNode* get_object() const = 0;

protected:
	cVector muzzlePosInitial_;
	cVector ejectorPosInitial_;
	cVector ejectorDirInitial_;

	double  azimuth_boresight;
	double  elevation_boresight;

	double  azimuth_;
	double  elevation_;
	bool    aft_gun_mount_;

	Graphics::ModelConnector* connector_;
	Graphics::ModelConnector* ejector_;

	IwoLA*		la_;

	int8_t		gunIndex_;
	int8_t		station_ind_; //0 - onboard gun

	cPosition   cachedPos_;
	cPosition   cachedPosLocal_;
	double      cachedTime_;
	cPosition   cachedEjectorPos_;

	ed::string connectorName_;
	ed::string ejectorConnectorName_;

	bool        connectorDirty_;
	float		barrel_circular_error_;

	static RandomObject randomEvenly;
};

// TODO: этот класс надо перенести во фрогфут
class WEAPONSBASE_API wLAGunCarriage : public wGunCarriageBase
{
public:

	wLAGunCarriage(IwoLA* la, int8_t gun_index);

	virtual cPosition getMuzzleBoreSightPosition() const;
	virtual void sync_gun_T() override;
protected:

	virtual void cachePosition_();
	virtual viObjectNode* get_object() const;
	virtual void setDrawArgument(int arg, float value);
};

class WEAPONSBASE_API wLAGunpodCarriage : public wGunCarriageBase
{
public:
	wLAGunpodCarriage(IwoLA* la, IwoAIPilon* pilon, int8_t gun_index );

	virtual cPosition getMuzzleBoreSightPosition() const;
	virtual ObjectID getID() const;
	virtual void sync_gun_T() override;
protected:

	virtual void cachePosition_();
	virtual viObjectNode* get_object() const;
	virtual void setDrawArgument(int arg, float value);

	IwoAIPilon* pilon_;
};