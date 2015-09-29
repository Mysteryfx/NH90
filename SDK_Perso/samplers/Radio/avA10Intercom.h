#ifndef __avA10CIntercom__
#define __avA10CIntercom__

#include "Avionics/avBasicElectric.h"
#include "Avionics/avBreakable.h"
#include "Base/Header/Avionics/Radio/avIntercom.h"
#include "FMElectricSystems/Items/Consumer.h"

#include "ICommandsCockpitDevice.h"

namespace cockpit {

namespace A10Common { 
	class avTACAN_AN_ARN118;
	class avILS_AN_ARN108;
	class avElectricInterface;
}; 

namespace A10C {

class avUHF_ARC_164;
class avVHF_ARC_186;
class avToneGenerator;
class avVMU;
class avSidewinder_Interface;

enum avIntercom_commands 
{
	INTERCOM_CMD_INT	=  iCommandCockpitDevice_Action_1,
	INTERCOM_CMD_INT_VOLUME,
	INTERCOM_CMD_FM,
	INTERCOM_CMD_FM_VOLUME,
	INTERCOM_CMD_VHF,
	INTERCOM_CMD_VHF_VOLUME,
	INTERCOM_CMD_UHF,
	INTERCOM_CMD_UHF_VOLUME,
	INTERCOM_CMD_AIM,
	INTERCOM_CMD_AIM_VOLUME,
	INTERCOM_CMD_IFF,
	INTERCOM_CMD_IFF_VOLUME,
	INTERCOM_CMD_ILS,
	INTERCOM_CMD_ILS_VOLUME,
	INTERCOM_CMD_TCN,
	INTERCOM_CMD_TCN_VOLUME,
	INTERCOM_CMD_HM,
	INTERCOM_CMD_VOLUME,
	INTERCOM_CMD_MODE,
	INTERCOM_CMD_CALL
};

enum avIntercom_mode
{
	INTERCOM_INT_MODE = 0,
	INTERCOM_FM_MODE,
	INTERCOM_VHF_MODE,
	INTERCOM_HF_MODE,
	INTERCOM_FULL_CCW_MODE
};

enum avIntercom_source
{
	INTERCOM_SOURCE_INT,
	INTERCOM_SOURCE_VHF_FM,
	INTERCOM_SOURCE_VHF_AM,
	INTERCOM_SOURCE_UHF,
	INTERCOM_SOURCE_AIM,
	INTERCOM_SOURCE_IFF,
	INTERCOM_SOURCE_ILS,
	INTERCOM_SOURCE_TACAN,
	INTERCOM_SOURCE_VMU,
	INTERCOM_SOURCE_MAX
};

enum avIntercom_communicator
{
	INTERCOM_COMM_VOID = cockpit::INTERCOM_COMM_VOID,
	INTERCOM_COMM_INT,
	INTERCOM_COMM_VHF_FM,
	INTERCOM_COMM_VHF_AM,
	INTERCOM_COMM_UHF,
	INTERCOM_COMM_MAX
};

class avIntercom : 	public cockpit::avIntercom,
					public avBreakable, 
					public avBasicElectric
{
public:
	avIntercom();
	//avDevice interface
	virtual void initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name);
	virtual void post_initialize();
	virtual void release();
	virtual void SetCommand(int, float value = 0);
	virtual void serialize(Serializer & serializer);
	virtual void update();
	//end of interface
	virtual	bool isOn() const;
	virtual bool isWireCommOn() const;
	virtual bool isWireCommTransmitOn() const;
	void		enableMicro(int position);
	void		disableMicro();

	void		l_register_functions(lua_State *Lua_s,int index);
	static int	l_is_communicator_available(lua_State *Lua_s);
	static int	l_set_communicator(lua_State *Lua_s);

protected:
	void		setCommunicator(avIntercom_communicator communicatorNum);
	void		setCommunicatorToCommandMenuAndPTT_(avIntercom_communicator communicatorNum);
	
	avIntercom_mode			m_intecomMode;
	avIntercom_communicator m_currentTransmitterNum; //selected communicator to talk, determined by Mic-Switch

private:
	void onElecPowerChange(const lwl::CSignalInfo* pInfo);

	DECLARE_DEV_LINKPTR(A10Common::avElectricInterface, elec_system);
	DECLARE_DEV_LINKPTR(avUHF_ARC_164,					UHF_Radio);
	DECLARE_DEV_LINKPTR(A10Common::avTACAN_AN_ARN118,	TACAN);
	DECLARE_DEV_LINKPTR(A10Common::avILS_AN_ARN108,		ILS);
	DECLARE_DEV_LINKPTR(avToneGenerator,				ToneGenerator);
	DECLARE_DEV_LINKPTR(avVHF_ARC_186,					VHF_AM_Radio);
	DECLARE_DEV_LINKPTR(avVHF_ARC_186,					VHF_FM_Radio);
	DECLARE_DEV_LINKPTR(avVMU,							VMU);
	DECLARE_DEV_LINKPTR(avSidewinder_Interface,			AIM9);

	bool				m_bHM_enable;
	bool				m_bINT_enable;
	bool                m_TCN_switcher;
	bool				m_bCommPanelModeHM;
	bool				m_bMicSwitchFront;
	Sounder::Switcher	sourceSwitcher[INTERCOM_SOURCE_MAX];
	Sounder::Regulator	sourceRegulator[INTERCOM_SOURCE_MAX];
	avCommunicator*		communicator[INTERCOM_COMM_MAX];
	unsigned char		communicatorDeviceID[INTERCOM_COMM_MAX];

	EagleFM::Elec::Consumer ElecConDevice;
};

} }

#endif __avA10CIntercom__