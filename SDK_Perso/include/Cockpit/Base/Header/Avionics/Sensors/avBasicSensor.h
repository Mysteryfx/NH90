#ifndef   _avBasicSensor_h
#define   _avBasicSensor_h

#include "wTime.h"
#include "IwHumanPLane.h"
#include "Avionics/Sensors/avTrackData.h"
#include "Avionics/avDevice.h"
#include "Avionics/avBasicElectric.h"
#include "Avionics/avBreakable.h"
#include "Avionics/avPlatform.h"

class viObject;
class viSearch;

#define IID_AVBASICSENSOR Common::identify<cockpit::avBasicSensor>()
 
namespace cockpit
{

	typedef ed::list<cockpit::avTrackData> avTrackDataList;
	class avBasicSensor;
	class avBasicSensor_BasicTimer;
	class avBasicSensor_SearchTimer;

	class COCKPITBASE_API avBasicSensor : public avPlatform, 
										  public avBasicElectric,
										  public avBreakable
	{
	public:// constructor/destructor
		avBasicSensor();
		virtual ~avBasicSensor(); 
		//functions
	public:
		virtual void search();
		virtual bool is_LockOn() const { return false;};
		virtual void set_external_designation(bool flag_in,bool success) {};
	protected:
		void	 start_search();
		void	 stop_search();
	private:
		//data
	public:
	protected:
		bool                       initialized;
		avBasicSensor_SearchTimer *p_search_timer;
	private:
	};

	//------------------------------------------//
	//             ACTIVITIES                   //
	//------------------------------------------//
	class COCKPITBASE_API avBasicSensor_SearchTimer : public avBasicTimer<avBasicSensor>
	{
        avBasicSensor_SearchTimer();
	public:
		avBasicSensor_SearchTimer(avBasicSensor* _parent,wModelTime _dtime);
	   ~avBasicSensor_SearchTimer();
		void NextEvent();
		void         SetTimeout(double t) { timeout = t;}
		wModelTime   GetTimeout()   const { return timeout;}
		wModelTime   GetStartTime() const { return start_time;}
        void         Start(double delay = 0);
	protected:
		bool           time_out();
		wModelTime     timeout;
		wModelTime     start_time;
	};
}

#endif  //_avBasicSensor_h 
