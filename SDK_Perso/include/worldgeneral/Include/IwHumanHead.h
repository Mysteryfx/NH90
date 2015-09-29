#pragma once

#include "WorldGeneral.h"
#include "cLinear.h"

namespace Lua
{
	class Config;
}

class WORLDGENERAL_API IwHumanHead
{
public:
	virtual ~IwHumanHead() = 0;
	virtual void    setControlledAngles     (double _hAngle,double _vAngle,double _roll_angle)=0;//задать углы которыми летчик управляет осознанно
	virtual void    setShakeAngles	        (double _hAngle,double _vAngle,double _roll_angle)=0;//задать углы от влияния внешних факторов 

	virtual void    setControlledViewAngle  (double _viewAngle)=0;
	virtual void    setShakeViewAngle	    (double _viewAngle)=0;
	
	virtual void    setControlledShift       (double X,double Y,double Z)=0;
	virtual void    setShakeShift            (double X,double Y,double Z)=0;
    virtual void    setSeatAdjustment        (double X,double Y,double Z)=0;
	virtual void    setSeatAdjustmentAngles  (double hAngle,double vAngle)=0;
	virtual void    setHeadAdjustment        (double X,double Y,double Z)=0;
	

	virtual void	setControlled_hAngle(double angle)=0;
	virtual void	setControlled_vAngle(double angle)=0;
	virtual void	setControlled_rollAngle(double angle)=0;


	virtual double  getControlled_hAngle()    const =0;
	virtual double  getControlled_vAngle()    const =0;
	virtual double  getControlled_rollAngle() const  = 0;

	virtual double  getShake_hAngle() const =0;
	virtual double  getShake_vAngle() const =0;
	virtual double  getShake_rollAngle() = 0;

	virtual double  getControlledViewAngle  () const =0;
	virtual double  getShakeViewAngle	    () const =0;
	
	virtual const  dVector &  getControlledShift      () const =0;
	virtual const  dVector &  getShakeShift           ()       =0;
    virtual const  dVector &  getSeatAdjustment        () const =0;
	virtual const  dVector &  getSeatPoint        () const =0;
	virtual const  dVector &  getHeadAdjustment        () const =0;
	virtual const  double  &  getAddSeatvAngle        () const =0;
	virtual const  double  &  getAddSeathAngle        () const =0;

	virtual void			  update_limits(dVector & lim_min,dVector & lim_max) = 0;

    dVector         getShift() { return -getSeatPoint()		 +
										 getSeatAdjustment()  + 
										 getHeadAdjustment()  +
                                         getControlledShift() + 
                                         getShakeShift(); }

    double          get_hAngle() const { return getControlled_hAngle() + getShake_hAngle(); }
    double          get_vAngle() const { return getControlled_vAngle() + getShake_vAngle(); }
    double          get_rollAngle() { return getControlled_rollAngle() + getShake_rollAngle(); }

	virtual void    headMovingOnOff() = 0;
	virtual void    change_seat(unsigned char seat = 0)  = 0;
	virtual unsigned int    get_seat_num()  = 0;


	virtual  void	configure(Lua::Config & config) = 0;


	virtual  void	informAboutChangingAngles(bool value) = 0;


	virtual  void   acceptBulletImpulse(const dVector& pos, const dVector& imp) = 0;

};