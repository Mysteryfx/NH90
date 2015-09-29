#pragma once
#include <math.h>
#include "World.h"
#include "Common/cPointerTemplate.h"
#include "Common.h"
#include "Serializer/Serializable.h"
#include "Killer.h"

#include <functional>

struct lua_State;

class RandomObject;

typedef double wModelTime;	// Тип модельного времени (измеряется в секундах)
class wTime;
typedef cPointerTemplate<wTime>	wtPointer;
#define IID_IwTimeNotifier identify<wTimeNotifier>()
class WORLD_API wTimeNotifier : public Common::Unknown<Serializable>
{
private:
	wModelTime wakeUpTime;	// Модельное время пробуждения
	unsigned int num;		// Номер в порядке создания уведомлений
	static unsigned int lastNum;	// Последний присвоенный num
	wtPointer activity;		// Указатель на деятельность
public:
	wTimeNotifier();
	virtual ~wTimeNotifier();
	void serialize(Serializer &);
	friend class wTime;
	friend struct PtrGreater;
};
// Функция сравнения уведомлений по времени. Если времена одинаковые, то сравниваются
// порядковые номера, дабы избежать неоднозначности внутри priority_queue.
struct PtrGreater
{
	typedef wTimeNotifier* PwTimeNotifier;
	inline bool operator () (const PwTimeNotifier& left, const PwTimeNotifier& right) const
	{
		return (left->wakeUpTime == right->wakeUpTime) ? (left->num > right->num)
			: (left->wakeUpTime > right->wakeUpTime);
	}
};
// Это класс календаря-будильника - упорядоченная по времени очередь деятельностей
typedef std::priority_queue<wTimeNotifier *, ed::vector<wTimeNotifier *>, PtrGreater > wCalendar;

struct TimeAndDuration
{
	wModelTime modelTime;
	double duration;		// реальная продолжительность в миллисекундах
};
// Класс объекта-деятельности, живущего в модельном времени (он же - будильник в своих
// статических переменных и функциях)
// ВНИМАНИЕ! Не убивайте деятельности с помощью delete, а вызывайте у них
// функцию KillMe()!
#define IID_IwTime identify<wTime>()

// Changes made by Eugene 25.03.2002
// Reason : symbol Unknown is ambiguous when winioctl.h file (Platform SDK) is insight.
class WORLD_API wTime : public Suicide, public LinkHost, public Common::Unknown<Serializable>
{
public:
	class IStepCallback
	{
	public:
		virtual void onStep(wModelTime time) = 0;
	};

private:
	static wCalendar calendar;		// Собственно календарь
	// Стек заказов на самоубийства
	static ed::vector<Suicide *> killVector;
	static wTime *currentActivity;
	static ed::string logMode;
	static RandomObject _phase_shift;

	wTimeNotifier	*notifier;		// Указатель текущего уведомления в календаре
	bool	ToBeKilled;				// Должна застрелиться после выполнения NextEvent()

	static ed::vector<IStepCallback*> prestepCallbacks_;
	static ed::vector<IStepCallback*> poststepCallbacks_;

protected:
	static wModelTime modelTime;	// Текущее модельное время в мире
	static wModelTime modelTime0;	// Абсолютное модельное время старта имитации
	static wModelTime stepMaxModelTime;	// Модельное время завершения шага имитации
	static bool bTraceOn;			// Трассировка включена = true
    static bool bActivitiesTraceOn;			// Трассировка активностей включена = true
	static bool bTraceDuration;		// Трассировка продолжительности действий включена = true
	static double traceDurationMin;	// Порог чувствительности трассировки продолжительности
	static wModelTime tTraceBegin;	// Начало интервала времени трассировки
	static wModelTime tTraceEnd;	// Конец интервала времени трассировки
	static wModelTime tConstStep;	// Постоянный шаг квантователя (1.0/FPS)
	static bool bTimeTraceOn;		// Признак трассировки событий в файл TimeTrace.log
	static bool bTraceFrames;
	static ed::vector<ed::string> traceNames;	// Массив строковых имен трассируемых деятельностей
	static ed::vector<ed::string> excludeNames; // Массив имен деятельностей, исключаемых из трассировки
	static bool bCommandsTraceDiscreteOn;	// Признак трассировки входных команд
	static bool bCommandsTraceAnalogOn;	// Признак трассировки аналоговых команд
	static bool bObjectsTraceOn;	// Признак трассировки состояния объектов
	static bool bRecordTrackByDefault;	// Fly == Record Track
	static bool bRecordModelTimeAccelerationToTrack;

	static wModelTime tObjectsTraceBegin;	// Начало интервала времени трассировки объектов
	static wModelTime tObjectsTraceEnd;		// Начало интервала времени трассировки объектов
	static wModelTime tObjectsTraceTime;	// текущее время трассировки объектов (с заданным шагом)
	static wModelTime tObjectsTraceStep;	// шаг времени трассировки объектов
	static ed::list<ed::string> tObjectsTraceExclude;	// Массив строковых имен классов объектов (MovingObject), исключаемых из трассировки

#ifdef _ED_HIDDEN_DEBUG 
	static wModelTime dtCheckMemory;
	static wModelTime tCheckMemory;
	static bool bCheckMemoryAtEveryTimeStep;
#endif _ED_HIDDEN_DEBUG 
	static bool CheckTraceName(wTime *a);

	// получить степень сдвига фазы (для разброса деятельностей)
	// возвращает равномерно-распределённое случайное число [0, 1)
	static float GetPhaseShift();
    static wTimeNotifier* GetTopAction();
    static void DoAction(wTimeNotifier *tn);

public:
	static wModelTime GetModelTime(void);	// Модельное время в секундах
	static wModelTime GetModelTime0(void);
	static wModelTime GetAbsoluteModelTime(void);
	static void SetModelTime0(wModelTime t);
	static void SetModelTime(wModelTime t);
	static wModelTime GetNextEventTime();
	static void DoActionsUntil(wModelTime maxModelTime);	// Продолжатель действий в модельном времени
	static void Break();	// Прерыватель текущего шага изнутри цикла DoActionsUntil
	static void Clear(void);		// Очистить модельный таймер
	// Функция-киллер, принимающие заказы на самоубийства без ожидания выполнения NextEvent()
	static void KillMe(Suicide *p);
	// Функции управления трассировкой
	static void TraceOn();
	static void TraceOff();
	static void TraceDurationOn();
	static void TraceDurationOff();
	static void SetTraceInterval(wModelTime tBegin, wModelTime tEnd);
	static bool InsideTraceInterval();
	static bool TraceIsOn();
    static bool ActivitiesTraceOn();
	static bool TraceDurationIsOn();
	static void TraceFramesOn();
	static void TraceFramesOff();
	static bool TraceFramesIsOn();
	static void ParseConfig();
    static void Start(const ed::string &mode);
	static void Stop();

    // all object tracing goes through this
    static void Trace(const char *fmt, ...);

	static bool CommandsTraceDiscreteIsOn();
	static bool CommandsTraceAnalogIsOn();
	static bool ObjectsTraceIsOn();
	static wModelTime ObjectsTraceTime();
	static bool IsObjectClassTrace(const ed::string& name);
	static bool RecordModelTimeAccelerationIsOn();
	static wModelTime GetConstTimeStep();
	// Функция сериализации
	static void serializeStatic(Serializer& s);
	static std::priority_queue<wModelTime, ed::vector<wModelTime>, std::greater<wModelTime> > stopTimes;
	static wModelTime roundUp(wModelTime time);

	static void addPrestepCallback(IStepCallback* callback);
	static void removePrestepCallback(IStepCallback* callback);
	static void addPoststepCallback(IStepCallback* callback);
	static void removePoststepCallback(IStepCallback* callback);

	wModelTime	wakeUpTime;			// Модельное время очередного действия
	ed::string stringName;			// Текстовое имя для опознания в трассировках
	//идентификатор активности для профайлера(один на одинаковые активности разных объектов)
	int	 profId;

	wTime();
	wTime(wModelTime t);
	wTime(const wTime &a);
	virtual ~wTime();
	void PutToSleep(void);			// Запланировать деятельность в календаре
	void InsertToCalendar(void);	// Синоним PutToSleep()
	void Cancel(void);				// Выключить запланированную деятельность
	void RemoveFromCalendar(void);	// Синоним Cancel()
	void KillMe(void);	// Поручить будильнику уничтожить данную деятельность сразу после
						// выполнения ее NextEvent()
	bool IsActive(void) const;	// Деятельность запланирована или выполняется. Если она не активна
							// то будильник сам ее не сможет активизировать, это должна сделать
							// какая-то другая активная деятельность.
	bool IsCurrent(void) const;	// Деятельность выполняется в данный момент модельного времени
	// Выполнить очередное действие (событие).
	// Для перепланирования деятельности на следующий момент модельного времени
	// достаточно перед выходом из NextEvent() соответственно увеличить wakeUpTime.
	// Чтобы перевести деятельность в пассивное состояние по собственной инициативе,
	// достаточно просто выйти из NextEvent(), не увеличивая wakeUpTime. Такая деятельность
	// не значится в календаре, однако и не умирает, она еще может быть активизирована
	// снаружи функцией InsertToCalendar().
	// Если же мы хотим еще и умереть после выполнения NextEvent(), то перед выходом
	// из NextEvent() нужно вызвать KillMe(). Если этого не сделать, то пассивные ненужные
	// деятельности будут засорять память.
	virtual void NextEvent();	// Выполнить очередное действие (событие) - вызывается будильником
	virtual void Profile(TimeAndDuration &td);	// Вызвать обработку профилирования
	virtual void MissedEvent();
	virtual void serialize(Serializer& s);
};

// Производные триггеры должны переопределять только функции Condition() и Action(),
// функция NextEvent() у всех триггеров одинаковая.
class WORLD_API TriggerActivity : public wTime
{
protected:
	wModelTime period;	// Период проверки условия
public:
	TriggerActivity();
	TriggerActivity(wModelTime t, wModelTime dt);
	TriggerActivity(const TriggerActivity &a);
	virtual ~TriggerActivity();
	void SetPeriod(wModelTime dt);
	virtual bool Condition();
	virtual void Action();
	virtual void NextEvent();
};

