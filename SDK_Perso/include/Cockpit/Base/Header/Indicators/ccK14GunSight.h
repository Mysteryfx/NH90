#pragma once

#include "Indicators/ccIndicator.h"

namespace cockpit {

	class COCKPITBASE_API ccK14GunSight : public ccIndicator
	{
	public:
		ccK14GunSight();
		virtual ~ccK14GunSight();

		//ccIndicator interface
		virtual void initialize(avDevice*, unsigned char, const ed::string&);
		virtual void create_elements_controllers_map();
		virtual void release();
		virtual void post_initialize();
		virtual void update(unsigned char draw_level);
		virtual	void control();
		//

		static void K14_rotateSpanMask(Element*, ccDrawable*, const DParamList&);
		static void K14_rotateRangeMask(Element*, ccDrawable*, const DParamList&);

		static void K14_gyro_sight_pos(Element*, ccDrawable*, const DParamList&);
		static void K14_draw_sights(Element*, ccDrawable*, const DParamList&);

		static void K14_fixedSightLightness(Element*, ccDrawable*, const DParamList&);
		static void K14_movableSightLightness(Element*, ccDrawable*, const DParamList&);
		
		static void K14_dbgSightCircle(Element*, ccDrawable*, const DParamList&);

	protected:

		Device_Mode LastMode;
		float ScaleModifier;

	private:

	protected:
		enum K14_SIGHTS
		{
			FIXED_CROSS = 0,
			FIXED_RING,
			GYRO_SIGHT
		};
	};

}