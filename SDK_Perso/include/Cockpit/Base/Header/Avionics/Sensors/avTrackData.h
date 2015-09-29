#ifndef  _avTrackData_h
#define  _avTrackData_h

#include "CockpitBase.h"
#include "Serializer\Serializer.h"
#include "dPosition.h"
#include "wsType.h"
#include "cAlgorithm.h"
#include "Utilities\ccUtilities.h"


class viObjectShape;
namespace cockpit
{

	class COCKPITBASE_API avTrackData
	{
		public:// construction/destruction
			 avTrackData(){ clear();};
			 ~avTrackData(){};
			 avTrackData(const avTrackData &Target);
			 avTrackData& operator=(const avTrackData &Target);

			 bool update(const dPosition & self);
		//functions
		public:
			void    clear();
			friend Serializer& operator << (Serializer& serializer, avTrackData& data);
			dVector get_point() const;
		protected:
		private:

		//data
		public:
			unsigned int world_id;  // ID цели в мире
			PolarPosition polar_p;   // углы визирования и дальность
			dPosition     world_p;   // позиция в мире
			dVector       velocity;  // скорость
			dVector       forces;    // перегрузки
			double        last_update_time;
			double		  track_time;// время сопровождения
			float         priority;  // опасность цели 
			wsType        type;      // тип цели в мире(если возможно)
			double        size;
			dVector		  local_point;
		protected:
		private:
	};

	struct COCKPITBASE_API avSensorLimits
	{
		avSensorLimits(const ed::string  &_name ="EMPTY",
					   limits_d       _hViewAngle     = limits_d(0,0),
					   limits_d       _vViewAngle     = limits_d(0,0),
					   limits_d       _hTrackAngle    = limits_d(0,0),
					   limits_d       _vTrackAngle    = limits_d(0,0),
					   limits_d			_hRotationAngle = limits_d(0,0),
					   limits_d 	   _vRotationAngle  = limits_d(0,0));
		avSensorLimits(const avSensorLimits &lim);
		ed::string  name;
		limits_d   hViewAngle;
		limits_d   vViewAngle;
		limits_d   hTrackAngle;
		limits_d   vTrackAngle;
		limits_d	hRotationAngle;
		limits_d 	vRotationAngle;
	};
	


    bool COCKPITBASE_API getUserBox(viObjectShape * obj,Box & box);

	typedef ed::map<ed::string,cockpit::avSensorLimits> avSensorLimitsMap;
}

#endif //_avTrackta_hDa
