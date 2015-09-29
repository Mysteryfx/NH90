#pragma once
#include "Indicators/ccIndicator.h"
namespace cockpit {
	class COCKPITBASE_API ccBaseASP_3 : public ccIndicator
	{
		float geometry_rotation_initial;
	public:
		ccBaseASP_3();
		virtual ~ccBaseASP_3();

		//ccIndicator interface
		virtual void initialize(avDevice*, unsigned char, const ed::string&);
		virtual void create_elements_controllers_map();
		virtual void update(unsigned char draw_level);
		virtual void update();
		virtual	void control();
		//

		static void ASP_3N_rotateSpanMask(Element*, ccDrawable*, const DParamList&);
		static void ASP_3N_rotateRangeMask(Element*, ccDrawable*, const DParamList&);

		static void ASP_3N_gyro_sight_pos(Element*, ccDrawable*, const DParamList&);
		static void ASP_3N_draw_sights(Element*, ccDrawable*, const DParamList&);

		static void ASP_3N_movableSightLightness(Element*, ccDrawable*, const DParamList&);
		
		static void ASP_3N_dbgSightCircle(Element*, ccDrawable*, const DParamList&);
		static void ASP_3N_rotateMirror  (Element*, ccDrawable*, const DParamList&);

	protected:
		float ScaleModifier;

	private:

	protected:
		enum ASP_3N_SIGHTS
		{
			FIXED_CROSS = 0,
			FIXED_RING,
			GYRO_SIGHT
		};
	};

}
