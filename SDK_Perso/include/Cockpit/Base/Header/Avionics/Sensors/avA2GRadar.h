#ifndef __AVA2GRADAR_H__
#define __AVA2GRADAR_H__

#include "CockpitBase.h"
#include "Avionics/Sensors/avBasicSensor.h"

namespace cockpit
{
   typedef struct _radar_resolution_element
       {
	     float distance_rsolution ; // разрешение в метрах по дальности
         float azimut_rsolution   ; // разрешение в метрах по азимуту
	   }
	   RadarResolutionElement ;
   

   class  COCKPITBASE_API avA2GRadar : public avBasicSensor
    {
      public:
        avA2GRadar() ;
	    ~avA2GRadar() ;
	    bool initialize();
	    void deinitialize();
		void search();
        
		// расчитывает элемент разрешения который содержит точку поверхности GroundPoint
	    // номер канала дальности и номер доплеровского фильтра в которые попадает сигнал цели расположенной в GroundPoint
		void get_resolution_element ( dPosition GroundPoint , dPosition el , int* distance_chanal , int* dopler_chanal ) ; 
     
	protected:
        double time_resolution ; // разрешение по времени селектора дальности
        double freq_resolution ; // разрешение по частоте доплеровских фильтрах
		int   dopler_chanals  ; // число доплеровских фильтров 
		int   distance_chanals ; // число каналов дальности в селекторе дальности
		double pulse_frequency ; // частота повторения зондирующих импульсов
	    double amount  ;  // скважность импульсов
	};
}

#endif /*__AVA2GRADAR_H__*/