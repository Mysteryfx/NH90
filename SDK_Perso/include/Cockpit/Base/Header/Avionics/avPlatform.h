#pragma once

#include "dPosition.h"
#include "Utilities/ccUtilities.h"
#include "Avionics/avMotor.h"
#include "wMovingObject.h"

namespace Graphics
{
	class Camera;
};
namespace cockpit
{
	class COCKPITBASE_API avPlatform 
	{
	public:
	    avPlatform();
		~avPlatform();
		virtual double			  azimuth()   const;
		virtual double			  elevation() const;
		virtual void			  fill_camera(Graphics::Camera & camera) const;
		virtual PolarPosition     getPolarPosition() const;

		PolarPosition			  get_relative_position(const dVector & vec) const;
		const Graphics::Camera	& getCamera() const; 
        ////
		const dPosition			& getPlatformPosition()  const;
        dPosition                 getPlatformPositionInterpolated() const;
        ////
		const dPosition			& getPlatformInitialPosition() const;
		dPosition				  getPlatformInitialPositionInterpolated() const;
        ////
		const dPosition			& getPlatformInitialLocalPosition() const;
		virtual void			  setPlatformInitialLocalPosition(const dPosition & new_local_pos);
        ////
        dPosition                 getPlatformLocalPosition() const;

        virtual void			  respat_angles(double h_angle,double v_angle);

        virtual void              set_platform(avPlatform * platform);
        avPlatform *              get_platform()        { return root; };
        const avPlatform *        get_platform() const  { return root; };


		virtual void			  set_carrier		(woPointer new_carrier);
		const woPointer			& get_carrier() const {return carrier;}
		
		const dPosition			& get_carrier_position() const;



		avMotor					& get_h_axis(){return h_axis;};
		avMotor					& get_v_axis(){return v_axis;};
		const avMotor			& get_h_axis()const{return h_axis;};
		const avMotor			& get_v_axis()const{return v_axis;};

		wModelTime				  get_last_update_time () const  { return update_time;}
   		virtual void			  restart();
        void			          update_platform_position();
	protected:
        virtual    PolarPosition  interpolate_angles(const dPosition & init_currrent,double delta)  const;
		virtual void			  on_update_position(double delta);
		virtual void			  update_angles(double delta);
    private:
        avPlatform                *root;
   		woPointer                  carrier;
		avMotor                    h_axis;
		avMotor                    v_axis; 
		dPosition                  platform_pos;  // положение и ориентация датчика в мировой системе координат 
		dPosition                  platform_pos_initial;
		dPosition                  local_pos_initial;
		wModelTime                 update_time;
	};
}
