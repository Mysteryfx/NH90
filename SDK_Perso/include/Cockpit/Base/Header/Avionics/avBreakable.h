#ifndef AVBREAKABLE_H
#define AVBREAKABLE_H

#include "CockpitBase.h"
#include "Utilities/avBasicTimer.h"
#include "Randomizer/RandomObject.h"

namespace EagleFM
{
	class ProbabilityChecker;
}

namespace cockpit
{


struct avDamageData
{
	avDamageData() {clear();}
	void clear()
	{
		Failure_ID = 0;
		Integrity = 1.0f;
		Integrity_Treshold = 0.5f;
		Slope_param = 1.0f;
		now = true;
		mean_TTF_coeff = 10.0f;
		work_time_to_fail_enable	= false;
		work_time_to_fail_probability	= 0.5;
		work_time_to_fail			= 3600.0*50;
		failureName.clear();
		failureEditorName.clear();
		isBroken	= false;
	}
	unsigned Failure_ID;
	float	Integrity;
	float	Integrity_Treshold;
	float	Slope_param;
	bool	now;
	float	mean_TTF_coeff; //mean time to failure coefficient on integrity dependense
	bool	work_time_to_fail_enable;		//включена ли возможность отказа по наработке
	double	work_time_to_fail_probability;	//Вероятность отказа за время наработки
	double	work_time_to_fail;				//время наработки на отказ
	ed::string	failureName;
	ed::string	failureEditorName;
	bool	isBroken;
};

typedef ed::vector<avDamageData> avDamageDataVector;

typedef ed::map<int, avDamageDataVector> DamageTable;

class avBreakable;

class COCKPITBASE_API avBreakable_WorkTimeFailureTimer : public wTime
{	
	avBreakable_WorkTimeFailureTimer(){};
public:
	avBreakable_WorkTimeFailureTimer(wModelTime _dtime)
	{
		dtime     = _dtime;
		to_cancel = false;
	}
	~avBreakable_WorkTimeFailureTimer()
	{
		Cancel();
		dtime  = 0;
	}
	inline void          DoStop()	            { to_cancel = true;	}
	inline wModelTime    GetDeltaTime() const   { return dtime;}
	inline void			 SetDeltaTime(const wModelTime dTime_) { dtime = dTime_;}
	inline void Start(double start_delay = 0)
	{
		to_cancel  = false;
		wakeUpTime = wTime::GetModelTime() + start_delay + dtime;
		if (!IsActive())
		{
			PutToSleep();
		}
	}
	virtual void NextEvent();
protected:
	wModelTime     dtime;
	bool           to_cancel;
};

class COCKPITBASE_API avBreakable_BasicTimer : public avBasicTimer<avBreakable>
{	
    avBreakable_BasicTimer();
public:
	avBreakable_BasicTimer(avBreakable* _parent, wModelTime _dtime, const ed::string& failureNameIn, const ed::string& failureEditorNameIn, unsigned Failure_ID_ = 0);
	virtual void NextEvent();
private:
	unsigned Failure_ID;
	ed::string	failureName;
	ed::string	failureEditorName;
};

// интерфейс для отказов
class COCKPITBASE_API avBreakable
{
	bool failure;
	bool ready;         // время подготовки // ???	
	bool registered_as_breakable;
public:
	avBreakable() : failure(false),ready(false),registered_as_breakable(false) {}
   ~avBreakable();

	virtual void setflag_failure(bool flag_in) {failure = flag_in;};
	virtual void setflag_ready	(bool flag_in) {  ready = flag_in;};
		
	virtual bool getflag_failure() const {return failure;};
	virtual bool getflag_ready()   const {return ready;};
	
	void process_damage(unsigned element, double integrity_factor); //ОБЩАЯ ОБРАБОТКА ПОВРЕЖДЕНИЯ	

	static void		start_work_time_failures();
	static void		release_work_time_failures();

    void            on_planned_failure(const ed::string & failure_id_,double probability_, double start_, double interval_);

	virtual void repair();

	friend class avBreakable_BasicTimer;	
	friend class avBreakable_WorkTimeFailureTimer;	
protected:
	void load_from_state(lua_State * L);
	virtual void check_damage(unsigned element, double integrity_factor, avDamageData & damage_data); //ВЫНОСИТ ПРИГОВОР
	virtual bool set_damage(unsigned Failure_ID, const bool can_be_rejected = false); //ПРИВОДИТ В ИСПОЛНЕНИЕ

	void register_as_breakable();
	void unregister_as_breakable();

	void  set_failure(unsigned Failure_ID);
private:
	static void createHumanFailureEvent(const ed::string& s_failureID, const ed::string& s_failureDisplayName);
	static void	process_work_time_failures();
	static avBreakable_WorkTimeFailureTimer	*p_work_time_failures_timer;
	static EagleFM::ProbabilityChecker*	p_work_time_failures_checker;

	void on_timer_event(unsigned Failure_ID,
		const ed::string& s_failureID, const ed::string& s_failureDisplayName);

	void add_failure_timer(const unsigned &Failure_ID,
		const double &time_to_failure,
		const ed::string& failureName,
		const ed::string& failureEditorName);

	DamageTable damages;
	ed::map<unsigned, avBreakable_BasicTimer*> timers;

	static RandomObject				random_evenly;
public:
	static ed::list<avBreakable*> damage_capable_list;
};

}

#endif //AVBREAKABLE_H
