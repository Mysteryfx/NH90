#ifndef __woSamTemplates_h__
#define __woSamTemplates_h__

#include "wTime.h"

template <class T>
class wActivity : public wTime
{
public:

	typedef void (T::*woHandler)(wModelTime&);

	wActivity(T* theHost, wModelTime timeStep0, woHandler function_)
	{
		init(theHost, timeStep0, 0., function_);
	}

	wActivity(T* theHost, wModelTime timeStep0, wModelTime time0, woHandler function_)
	{
		init(theHost, timeStep0, time0, function_);
	}

	wActivity(T* theHost, wModelTime timeStep0, woHandler function_, const ed::string &name)
	{
		stringName = name;
		init(theHost, timeStep0, 0, function_);
	}

	virtual ~wActivity()
	{
		host = 0;
	}

	virtual void NextEvent();

    T*				host;

protected:

	virtual bool	checkHost();

	woHandler		function;
	wModelTime		TimeStep;

private:
	void init(T* theHost, wModelTime timeStep0, wModelTime time0, woHandler function_);
};

template <class T>
void wActivity<T>::NextEvent()
{
	if(host != 0)
	{
		(host->*function)(TimeStep);
		if(TimeStep > 0.0)
		{
			wakeUpTime += TimeStep;
			return;
		}
	}
	KillMe();
}

template <class T>
void wActivity<T>::init(T* theHost, wModelTime timeStep0, wModelTime time0, woHandler function_)
{
	host = theHost;
	TimeStep = timeStep0;
	function = function_;
	wModelTime phase = timeStep0 > 0 ? timeStep0 * GetPhaseShift() : 0;
	wakeUpTime = time0 == 0. ? (wTime::GetModelTime()  + phase) : time0;
	InsertToCalendar();
}

template <class T>
inline bool wActivity<T>::checkHost()
{
	return host != 0;
}

#endif
