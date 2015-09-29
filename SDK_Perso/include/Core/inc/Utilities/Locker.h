#ifndef __Locker__
#define __Locker__

//Resource locker with requests counter. 
//If at least one requesting system need resource locked the resource must be locked.
template<class Locker>
class CountLocker
{
public:
	CountLocker(Locker & lockerIn) : locker_(lockerIn), counter_(0) {;}
	inline void lock()
	{
		if(counter_ == 0)
			locker_.lock();
		counter_++;
	}
	inline void unlock()
	{
		assert(counter_ > 0);
		counter_--;
		if(counter_== 0)
			locker_.unlock();
	}
private:
	unsigned int 	counter_;
	Locker & 		locker_;
};

//For systems those doesn't care good enough about resource management 
//and may try to lock resource several times with no unlocking or unlock unlocked system.
template <class Locker>
class LockSwitcher
{
public:
	LockSwitcher(Locker & lockerIn) : locker_(lockerIn), locked_(false) {;}
	inline void lock()
	{
		if(!locked_)
			locker_.lock();
		locked_ = true;
	}
	inline void unlock()
	{
		if(locked_)
			locker_.unlock();
		locked_ = false;
	}
private:
	bool			locked_;
	Locker &		locker_;
};

#endif