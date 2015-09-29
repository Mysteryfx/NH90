#pragma once

#include "Avionics/avDevice.h"
#include "Avionics/avBasicElectric.h"
#include "Avionics/avBreakable.h"
#include "Utilities/MathDefs.h"
#include "wsType_Level4.h"

namespace cockpit {

enum IAM_STATUS
{
	IAM_STAT_NA,
	IAM_STAT_OFF,
	IAM_STAT_INIT,
	IAM_STAT_ALN_UNS,
	IAM_STAT_ALN_GRDY,
	IAM_STAT_ALN_RDY,
	IAM_STAT_RDY,
	IAM_STAT_ERASING,
	IAM_STAT_ERASED,
	IAM_STAT_FAIL,
	IAM_STAT_ALN_DEG,
	IAM_STAT_RETRY,
	IAM_STAT_ABORTED,
};

const unsigned IAM_TSTSTAT_NA		= 0x01;
const unsigned IAM_TSTSTAT_UN		= 0x02;
const unsigned IAM_TSTSTAT_IP		= 0x04;
const unsigned IAM_TSTSTAT_PASS		= 0x08;
const unsigned IAM_TSTSTAT_FAIL		= 0x10;

typedef struct 
{
	double      height;  
	double      open_time;
	Math::Vec3d des_wx;
	bool        fzu39;
	bool		has_wcmd;
} CBUparams ;

class COCKPITBASE_API eqIAM : public avBasicElectric,
							     public avBreakable
{
public:
	eqIAM(const wsType& type_ = wsType(0, 0, 0, 0), bool is_wcmd = false);
	virtual ~eqIAM() {}

	void initialize(bool Ready = false);
	void update(wModelTime delta = 0.0);
	
	void setElecPower(bool flag_in);
	void setElecPowerOnOff();

	const wsType& Type() const {return type;}
	
	void set_station(int p, int s = 0) 
	{
		pilon	 = p;
		subpilon = s;
	}

	int get_pilon() const { return pilon; }

	unsigned getStatus() const {return Status;}
	unsigned getPrevStatus() const {return PrevStatus;}
	float getAlignStatus() const {return AlignStatus;}
	
	unsigned getTestStatus() const {return TestStatus;}
	void forceTestStatus(unsigned StatusIn) {setTestStatus(StatusIn);}

	bool getINS_avail() const {return INS_avail;}
	void setINS_avail(bool FlagIn);
	void setEGI_FOM_degrated(bool FlagIn);
	void setTXA_quality(double TXA_quality);

	void setCBUparams(const Vec3d& des_wx, double drop_height, double drop_time, bool fzu39 , bool has_wcmd);

	void startTest();
	bool testIsAvail() const;
	
	void setPickleFlag(bool FlagIn);
	bool getPickleFlag() const {return IsPickled_;}

	void setGPSReadyFlag(bool FlagIn) {GPSReady_ = FlagIn;}
	bool getGPSReadyFlag() const {return GPSReady_;}
	
	bool IsReadyToFire() const;
	unsigned int launch(IwHumanPlane* airplane);
	bool isTrainingWeapon() const {return false;}

	static void InitConstants(wModelTime ConstInitTimeIn, wModelTime ConstPowerOffTimeIn,
		wModelTime ConstAlignTimeIn, wModelTime ConstTestTimeIn);

	void setTgtLinCoords(const cVector& VecIn) {TgtLinCoords = VecIn;}
	
	void setGPSReady(bool FlagIn); 
	bool getGPSReady() const {return GPSReady_;}

	void setRetryLaunchFlag();

protected:
	void setStatus(unsigned StatusIn);
	void setTestStatus(unsigned StatusIn);
	void define_current_status();

	static wModelTime ConstInitTime;
	static wModelTime ConstPowerOffTime;
	static wModelTime ConstAlignTime;
	static wModelTime ConstTestTime;

	wsType		type;
	int			pilon;
	int			subpilon;
	
	bool		IsPickled_;
	
	unsigned	Status;
	unsigned	PrevStatus;
	
	bool		INS_avail;
	float		AlignStatus;
	bool		GPSReady_;
	bool		EGI_FOM_Degrated_; //An EGI FOM greater than 2 will preclude JDAM's from reaching a GPS RDY state.
	bool		is_wcmd_;
	float		TXA_quality_;

	unsigned char TestStatus;
	unsigned char PrevTestStatus;

	CBUparams   Cbu_params;

	wModelTime	StartTime;
	wModelTime	PowerOffTime;
	wModelTime	AlignStartTime;
	wModelTime	TestStartTime;

	cVector     TgtLinCoords;

	bool        InRetry;
	wModelTime	RetryStartTime;

	bool		power_off_in_progress_;
};

}
