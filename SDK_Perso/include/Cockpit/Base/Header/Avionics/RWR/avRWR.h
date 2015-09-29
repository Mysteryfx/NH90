#pragma once

#include "Avionics/avLuaDevice.h"
#include "Avionics/avBasicElectric.h"
#include "IwHumanRWR.h"
#include "Avionics/avMovingMapPoint.h"
#include "Avionics/Sensors/avSensorEye.h"

#include "WorldSound.h"
#include "ccSound.h"
#include <ed/SounderAPI.h>

#include <ed/set.h>

namespace cockpit
{

	class RWR_Emitter;
    class avRWR_Threat_symbol;

	typedef ed::vector<avSensorEye>             RWR_eye_vector;

	typedef Mem::Ptr<avRWR_Threat_symbol> symbol_ptr;
	typedef ed::list<symbol_ptr>		  symbol_ptr_list;

	// description of threat symbol on RWR
	class avRWR_Threat_symbol : public avMovingMapPoint
	{
		friend class avRWR;

	public:
		avRWR_Threat_symbol() {}
		avRWR_Threat_symbol(int n);

		void setValidity(bool v) {is_valid = v;}
		bool getValidity() const {return is_valid;}

		void setLocked(bool v) {is_locked = v;}
		bool getLocked() const {return is_locked;}

		void setPriority(double pr) {priority = pr;}

		void setThreatId(int k) {threat_id = k;}
		int  getThreatId() const {return threat_id;}

		double getAzimuth() const {return azimuth;}

	protected:
		// calculate symbol position on RWR display
		void update_rwr_position();
		void updateInfo(int k, const RWR_Emitter& em);
	private:

		int    threat_id;  // ID = 1..16
		double azimuth;
		double strength;
		bool   is_valid;
		bool   is_locked;
		double priority;
	};




	class rwr_nearest_checker : public SimpleOctTree_check_object ,
		public std::less<symbol_ptr>
	{
	public:
		rwr_nearest_checker(const dVector & basis_,bool tag = false):basis(basis_)
		{

		};
		virtual bool valid(SimpleOctTreeLoad *obj)
		{
			symbol_ptr          inserted = static_cast<avRWR_Threat_symbol*>(obj);
			dVector d = basis - inserted->getPoint();

			if (!inserted->getValidity())
				return false;

			//Vector3 curr = (symbol_ptr->getPosition()).p;

			double l = d.length2();

            if(l < 64.f) //81
			  result.insert(where(inserted),inserted);
			return true;
		};
		virtual bool operator()(const symbol_ptr & _Left , const symbol_ptr &  _Right) const
		{
			//Vector3 l_d = basis - _Left ->getPoint();
			//Vector3 r_d = basis - _Right->getPoint();
			//return l_d.lengthSq() < r_d.lengthSq();
			return true;
		};
		symbol_ptr_list  result;
	protected:
		symbol_ptr_list::iterator where(const symbol_ptr & inserted)
		{
			return std::lower_bound<symbol_ptr_list::iterator,
				symbol_ptr,
				rwr_nearest_checker>
				(result.begin(),
				result.end(),
				inserted,
				*this);
		}
		dVector  basis;
	};




enum RWR_SIGNALS_TYPES
{
	RWR_SIGNAL_UNKNOWN,
	RWR_SIGNAL_SEARCH,
	RWR_SIGNAL_LOCK,
	RWR_SIGNAL_LAUNCH
};

enum RWR_PLARFORM_TYPES
{
	RWR_PLARFORM_UNKNOWN,
	RWR_PLARFORM_AIR,
	RWR_PLARFORM_GROUND,
	RWR_PLARFORM_NAVY,
	RWR_PLARFORM_WEAPON
};

class RWR_Emitter
{
public:
	RWR_Emitter();
	void reset();

	float Priority;				// приоритет цели
	float SignalStreight;		// мощность сигнала. Диапазон от 1 до 0.

	cVector Direction;
	float Azimuth;				// азимут
	float Elevation;			// угол места

	wModelTime BirthTime;		// время когда облучатель появился на индикаторе
	wModelTime LaunchTime;		// время когда облучатель пустил ракету

	ObjectID client;			// идентификтор объекта
	woPointer pFrom;			// указатель на объект
	ObjectID missile_id;		// идентификтор наводимой источником ракеты

	// типы и режимы работы радара облучателя
	unsigned int RadarMode, RadarType;

	// типы облучателя
	wsType			EmitterType;
	ed::string		EmitterTypeStr;

	// Детализация информации об облучателе
	RWR_SIGNALS_TYPES	SignalType;
	RWR_PLARFORM_TYPES	PlatformType;

	bool NewEmitter; // новый
	bool Spike;		 // пришел сигнал от источника ???
	int WarningCounter;

	// пусковые установки с радарами пока
	// ничего не присылают и контроллируются только ракетами
	// поэтому их удаляем только по условию срыва
	// наведения ракеты
	bool DoNotControlEmissionTime;
	bool IsActiveMissile;
	bool IsValid;
	bool isLocked; // true = не обновляется в режиме TgtSeparate
};

typedef ed::vector<RWR_Emitter> EmittersStorage;
typedef ed::set<symbol_ptr> GroupOfSymbols;

// map for sounds from different radars (key = wsType.Level4)
typedef ed::map<unsigned int, Sound::Source*> SearchSoundMap;

class COCKPITBASE_API avRWR :   public avLuaDevice,
								public avBasicElectric
{
public:
	avRWR();
	virtual ~avRWR();

	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void update();
	//end of interface

	float getBrightness() const {return Brightness;}
	void setBrightness(float BrightnessIn) {Brightness = BrightnessIn;}
	void changeBrightness();

	const EmittersStorage& GetEmitters() {return emitters;}
	int GetMainEmitterPlace() const {return MainEmitterPlace;}
	ObjectID GetNewestEmitter() const {return NewestEmitter;}

	bool getLaunchEventIsActive() const {return LaunchEventIsActive;}
	void setLaunchEventIsActive(bool _in){LaunchEventIsActive = _in;}
	bool getLockEventIsActive()   const {return LockEventIsActive;}
	bool getPriorityMode()        const {return PriorityMode;}
	bool getSeparateMode()        const {return SeparateMode;}

	void initialize_storage();

	SimpleOctTree* getSymbolsStorage() const {return symbols_storage;}

    void search(int i, int gr_number);
	void dismissTgtSeparartion();

	woPointer getLaunchingSource() const {return LaunchingSource;}
	woPointer getLockingSource() const {return LockingSource;}
	unsigned int getLockingType() const {return LockingType;}


#ifdef _ED_HIDDEN_DEBUG 
	void external_parse(Graphics::ModelParser& parser,const Position3& pos,Graphics::effectState* effects);
#endif

	void ClearEmitters();

	const bool isDegraded()const;

protected:
	RWR_eye_vector eyes;

	virtual void checkLaunchEvent();
	void checkLockEvent();

	void ResizeStorage(int size){emitters.resize(size);};

	EmittersStorage	emitters;
	bool			SoundsOn;
	bool			LaunchEventIsActive;
	bool			LockEventIsActive;
	bool            PriorityMode;
	bool            SeparateMode;     // updating in A10C section
	bool            IsTargetSeparated;

    SimpleOctTree *symbols_storage;
	ed::set<int> groups[8]; // группы пересекающихся символов
	char cross[16][16];   // матрица пересечений символов для выявления групп
	ed::set<int> search_set; // множество символов, имеющих пересечение с другими символами

	void calculateGroups();
	void separateGroups();
    bool isPositionEmpty(const dPosition& pos, int exclude);


	void tryInsertEmitter(const RWR_event& event);
	bool getEmitterPos(woPointer pFrom, cPosition& currPlane_pos,
		float& Azimuth, float& Elevation, cVector& world_direction);
	void selectMainEmitter();
	float getSignalStreigth(woPointer pFrom, const cPosition& currPlane_pos,
		int L1, int L2, float radarDetectionDist) const;
	float calc_priority(unsigned int RadarMode, unsigned int RadarType_, float signalStreight) const;
	void setEmitterTypes(woPointer pFrom, RWR_Emitter& TempEmitter);
	bool checkEmitter(ObjectID fromID, ObjectID missile_id);
	void updateEmitters();
	int findNewPlace();
	int findWithLowestPriority(float& LowestPriority);
	int getEmittersSize() const {return EmittersSize;}
	void checkEmittersSize();
	void SetEmitterIsValid(int EmitterPos);
	void SetEmitterIsNotValid(int EmitterPos);

	float Brightness;

	int				EmittersSize;

	ObjectID		NewestEmitter;
//	float			MaxElevation;
	RWR_SIGNALS_TYPES LockLaunch;

	RWR_Emitter*	MainEmitter;  // главный облучатель
	int				MainEmitterPlace;
	unsigned short  MaxThreats; // максимальное количество облучателей

	wModelTime		EmitterLiveTime;

	woPointer       LaunchingSource;
	woPointer       LockingSource;
	unsigned int    LockingType;

	// old sound
	wsType defaultWsType;

	Sound::SourceParams SndParams;
	struct {
		SearchSoundMap search_new;     // acquisition sounds
		SearchSoundMap lock_new;       // locking sounds
		Sound::Source  launch_warning;
		Sound::Source  threat_new;
	} sound;

	// new sound
	ed::Sounder sounder;

	double RWR_detection_coeff;
};

}
