#ifndef  _AV_BASIC_TIMER_
#define  _AV_BASIC_TIMER_
#include "wTime.h"
namespace cockpit
{
    template <class T> 
    class avBasicTimer : public wTime
    {	
        avBasicTimer(){};
    public:
        avBasicTimer(T * _parent, wModelTime _dtime)
        {
            parent    = 0;
            dtime     = 0;
            to_cancel = false;
            if (_parent)
            {
                parent = _parent;
                dtime  = _dtime;
            }
        }
        ~avBasicTimer()
        {
            Cancel();
            parent = 0;
            dtime  = 0;
        }
        inline void          DoStop()	            { to_cancel = true;	}
        inline void 		 SetParent(T * _parent) { parent = _parent;}
		inline wModelTime    GetDeltaTime() const   { return dtime;}
		inline  void		 SetDeltaTime(const wModelTime dTime_) { dtime = dTime_;}
        virtual void         Start(double start_delay = 0)
        {
            if (!parent)
                return;
            to_cancel  = false;
            wakeUpTime = wTime::GetModelTime() + start_delay + dtime;
            if (!IsActive())
            {
                PutToSleep();
            }
        }
		inline void  Kill(bool immediate = false)
		{
			if (immediate)
				delete this;
			else
			{
				DoStop();	
				Cancel();
				SetParent(nullptr);		
				KillMe();
			}
		}
    protected:
        T		*      parent;
        wModelTime     dtime;
        bool           to_cancel;
    };
}
#endif //_AV_BASIC_TIMER_
