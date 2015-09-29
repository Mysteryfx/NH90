#ifndef	__av_Weqapons_Avionics_Proxy__
#define __av_Weqapons_Avionics_Proxy__

/************************************************************************/
/*    Provides Access to the navigation data from cockpits devices       */
/*	  Default Proxy - access to sensors                                 */
/************************************************************************/
#include "Avionics/avAvionicsProxyInterface.h"

class IwHumanSensor;

namespace cockpit
{
	class COCKPITBASE_API avAvionicsDataProxyDefault : public IAvionicsDataProxy
	{
	public:
		avAvionicsDataProxyDefault();
		virtual ~avAvionicsDataProxyDefault();

		virtual bool   is_valid() const;
		virtual double get_baro_altitude() const;
		virtual double get_radar_altitude() const;
		virtual double get_roll() const;
		virtual double get_pitch() const;
		virtual double get_rate_of_roll() const;
		virtual double get_rate_of_yaw() const;
		virtual double get_rate_of_pitch() const;
		virtual double get_tas() const;

	private:
		IwHumanSensor * sensors_; 
	};
}
#endif