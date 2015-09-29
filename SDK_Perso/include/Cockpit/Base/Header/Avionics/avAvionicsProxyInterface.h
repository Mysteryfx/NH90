#ifndef	__av_Weqapons_IAvionics_Proxy__
#define __av_Weqapons_IAvionics_Proxy__

#include "CockpitBase.h"
/************************************************************************/
/*    Provides Access to the navigation data from cockpits devices       */
/************************************************************************/
namespace cockpit
{
	class COCKPITBASE_API IAvionicsDataProxy
	{
	public:
		IAvionicsDataProxy()
			:default_proxy_(NULL)
		{
			default_proxy_ = IAvionicsDataProxy::proxy_;
			IAvionicsDataProxy::proxy_ = this;
		}

		~IAvionicsDataProxy()
		{
			IAvionicsDataProxy::proxy_ = default_proxy_;
		}

		virtual bool   is_valid()  const = 0;
		virtual double get_baro_altitude() const = 0;
		virtual double get_radar_altitude() const = 0;
		virtual double get_roll() const = 0;
		virtual double get_pitch() const = 0;
		virtual double get_rate_of_roll() const = 0;
		virtual double get_rate_of_yaw() const = 0;
		virtual double get_rate_of_pitch() const = 0;
		virtual double get_tas() const = 0;

		//methods with check
		double get_baro_altitude_s() const
		{
			return  valid_ ? get_baro_altitude() : 0.0;
		}

		double get_radar_altitude_s() const
		{
			return  valid_ ? get_radar_altitude() : 0.0;
		};

		double get_roll_s() const
		{
			return  valid_ ? get_roll() : 0.0;
		};

		double get_pitch_s() const
		{
			return  valid_ ? get_pitch() : 0.0;
		};

		double get_rate_of_roll_s() const
		{
			return  valid_ ? get_rate_of_roll() : 0.0;
		}

		double get_rate_of_yaw_s() const
		{
			return  valid_ ? get_rate_of_yaw() : 0.0;
		}

		double get_rate_of_pitch_s() const
		{
			return  valid_ ? get_rate_of_pitch() : 0.0;
		}

		double get_tas_s() const
		{
			return  valid_ ? get_tas() : 0.0;
		}

	static IAvionicsDataProxy* proxy_;

	protected:
		IAvionicsDataProxy* default_proxy_;
		bool valid_;
	};
}
#endif