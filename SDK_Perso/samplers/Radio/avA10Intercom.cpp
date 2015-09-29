#include "stdafx.h"

#include "ccSound.h"
#include "Avionics/Radio/avA10Intercom.h"
#include "ccModuleManager.h"
#include "Utilities/ccUtilities.h"

#include "Avionics/Radio/avUHF_ARC_164.h"
#include "Avionics/Radio/avVHF_ARC_186.h"
#include "A10Common/Header/Avionics/Radio/avTACAN_AN_ARN118.h"
#include "A10Common/Header/Avionics/Radio/avILS_AN_ARN108.h"
#include "Avionics/avToneGenerator.h"
#include "Avionics/avVMU.h"
#include "Avionics/avSidewinder_Interface.h"
#include "A10Common/Header/Avionics/avElectricInterface.h"
#include "Avionics/avHOTAS.h"

#include "ICommandDialogsPanel.h"
#include "IOptions.h"

using namespace cockpit::A10C;

REGISTER_DYNAMIC_FACTORY(cockpit::A10C::avIntercom, avIntercom);

avIntercom::avIntercom() : ElecConDevice(EagleFM::Elec::DC_26_VOLT), m_bMicSwitchFront(false)
{
	
}

//avDevice interface
void avIntercom::initialize(unsigned char ID, const ed::string & name, const ed::string & scriptName)
{
	cockpit::avIntercom::initialize(ID, name, scriptName);
	m_intecomMode = INTERCOM_INT_MODE;
	m_currentTransmitterNum = INTERCOM_COMM_INT;
	m_bHM_enable = true;
	m_bINT_enable = true;
	m_bCommPanelModeHM = false;

	listen_command_range(this,iCommandPlaneIntercomINTDecrease,iCommandPlaneIntercomHF);
}

void avIntercom::post_initialize()
{
	cockpit::avIntercom::post_initialize();

	elec_system->getFMElecSystem()->getCircuitBreaker(A10::ECB_INTERCOM) >> ElecConDevice;
	
	ElecConDevice.m_signalPowerOn +=
		lwl::delegate(this, &avIntercom::onElecPowerChange);
	ElecConDevice.m_signalPowerOff +=
		lwl::delegate(this, &avIntercom::onElecPowerChange);

	communicator[INTERCOM_COMM_INT]		= NULL;
	communicator[INTERCOM_COMM_VHF_FM]	= &VHF_FM_Radio->getCommunicator();
	communicator[INTERCOM_COMM_VHF_AM]	= &VHF_AM_Radio->getCommunicator();
	communicator[INTERCOM_COMM_UHF]		= &UHF_Radio->getCommunicator();

	communicatorDeviceID[INTERCOM_COMM_INT]		= this->ID();
	communicatorDeviceID[INTERCOM_COMM_VHF_FM]	= VHF_FM_Radio->ID();
	communicatorDeviceID[INTERCOM_COMM_VHF_AM]	= VHF_AM_Radio->ID();
	communicatorDeviceID[INTERCOM_COMM_UHF]		= UHF_Radio->ID();
	
	for (int s = 0; s < INTERCOM_SOURCE_MAX; s++)
		sourceRegulator[s] >> sourceSwitcher[s] >> mainRegulator;

	telephone.getSoundElement() >> sourceRegulator[INTERCOM_SOURCE_INT];
	VHF_FM_Radio->getSoundElement() >> sourceRegulator[INTERCOM_SOURCE_VHF_FM];
	VHF_AM_Radio->getSoundElement() >> sourceRegulator[INTERCOM_SOURCE_VHF_AM];
	UHF_Radio->getSoundElement() >> sourceRegulator[INTERCOM_SOURCE_UHF];
	ILS->getLocalizerSoundElement() >> sourceRegulator[INTERCOM_SOURCE_ILS];
	ILS->getMarkerBeaconsSoundElement() >> sourceRegulator[INTERCOM_SOURCE_ILS];
	TACAN->getSoundElement() >> sourceRegulator[INTERCOM_SOURCE_TACAN];
	VMU->getSoundElement() >> sourceRegulator[INTERCOM_SOURCE_VMU];
	AIM9->getSoundElement() >> sourceRegulator[INTERCOM_SOURCE_AIM];
	
	ToneGenerator->getSoundElement(flag_TONE_AUTOPILOT_ENGAGE) >> mainRegulator;

	performClickableAction(INTERCOM_CMD_INT, 1.0f);
	performClickableAction(INTERCOM_CMD_INT_VOLUME, 1.0f);
	performClickableAction(INTERCOM_CMD_FM, 1.0f);
	performClickableAction(INTERCOM_CMD_FM_VOLUME, 1.0f);
	performClickableAction(INTERCOM_CMD_VHF, 1.0f);
	performClickableAction(INTERCOM_CMD_VHF_VOLUME, 1.0f);
	performClickableAction(INTERCOM_CMD_UHF, 1.0f);
	performClickableAction(INTERCOM_CMD_UHF_VOLUME, 1.0f);
	performClickableAction(INTERCOM_CMD_AIM, 1.0f);
	performClickableAction(INTERCOM_CMD_AIM_VOLUME, 1.0f);
	performClickableAction(INTERCOM_CMD_IFF, 1.0f);
	performClickableAction(INTERCOM_CMD_IFF_VOLUME, 1.0f);
	performClickableAction(INTERCOM_CMD_ILS, 1.0f);
	performClickableAction(INTERCOM_CMD_ILS_VOLUME, 1.0f);
	performClickableAction(INTERCOM_CMD_TCN, 1.0f);
	performClickableAction(INTERCOM_CMD_TCN_VOLUME, 1.0f);
	performClickableAction(INTERCOM_CMD_HM, 1.0f);
	performClickableAction(INTERCOM_CMD_VOLUME, 1.0f);
	performClickableAction(INTERCOM_CMD_MODE, float(INTERCOM_VHF_MODE) * 0.1f);

	listen_command(this, iCommandToggleCommandMenu);

	sourceSwitcher[INTERCOM_SOURCE_VMU].setOnOff(true);
	sourceRegulator[INTERCOM_SOURCE_VMU].setGain(1.0f);

	receivers.push_back(&VHF_FM_Radio->getCommunicator());
	receivers.push_back(&VHF_AM_Radio->getCommunicator());
	receivers.push_back(&UHF_Radio->getCommunicator());
}

void avIntercom::release()
{
	cockpit::avIntercom::release();
}

void avIntercom::SetCommand(int command, float value)
{
	switch (command)
	{
		case iCommandPlaneIntercomINTPress:
			switcher_value_iterate(INTERCOM_CMD_INT);
			break;
		case INTERCOM_CMD_INT:
			sourceSwitcher[INTERCOM_SOURCE_INT].setOnOff(value > 0.5);
			updateWireCommState();			
			break;
		case iCommandPlaneIntercomFMPress:
			switcher_value_iterate(INTERCOM_CMD_FM);
			break;
		case INTERCOM_CMD_FM:
			sourceSwitcher[INTERCOM_SOURCE_VHF_FM].setOnOff(value > 0.5);
			break;
		case iCommandPlaneIntercomVHFPress:
			switcher_value_iterate(INTERCOM_CMD_VHF);
			break;
		case INTERCOM_CMD_VHF:
			sourceSwitcher[INTERCOM_SOURCE_VHF_AM].setOnOff(value > 0.5);
			break;
		case iCommandPlaneIntercomUHFPress:
			switcher_value_iterate(INTERCOM_CMD_UHF);
			break;
		case INTERCOM_CMD_UHF:
			sourceSwitcher[INTERCOM_SOURCE_UHF].setOnOff(value > 0.5);
			break;
		case iCommandPlaneIntercomAIMPress:
			switcher_value_iterate(INTERCOM_CMD_AIM);
			break;
		case INTERCOM_CMD_AIM:
			sourceSwitcher[INTERCOM_SOURCE_AIM].setOnOff(value > 0.5);
			break;
		case iCommandPlaneIntercomIFFPress:
			switcher_value_iterate(INTERCOM_CMD_IFF);
			break;
		case INTERCOM_CMD_IFF:
			sourceSwitcher[INTERCOM_SOURCE_IFF].setOnOff(value > 0.5);
			break;
		case iCommandPlaneIntercomILSPress:
			switcher_value_iterate(INTERCOM_CMD_ILS);
			break;
		case INTERCOM_CMD_ILS:
			sourceSwitcher[INTERCOM_SOURCE_ILS].setOnOff(value > 0.5);
			break;
		case iCommandPlaneIntercomTCNPress:
			switcher_value_iterate(INTERCOM_CMD_TCN);
			break;
		case INTERCOM_CMD_TCN:
			if(m_intecomMode != INTERCOM_FULL_CCW_MODE)
			 sourceSwitcher[INTERCOM_SOURCE_TACAN].setOnOff(value > 0.5);

			m_TCN_switcher = value > 0.5;
			break;
		case iCommandPlaneIntercomHM:
			switcher_value_iterate(INTERCOM_CMD_HM);
			break;
		case INTERCOM_CMD_HM:
			m_bHM_enable = value > 0.5;
			m_currentTransmitterNum = INTERCOM_COMM_VOID;
			if(	!m_bHM_enable &&
				m_bCommPanelModeHM &&
				globalCommandDialogsPanel->visible())
			{
				globalCommandDialogsPanel->setCommunicatorId(ICommandDialogsPanel::COMMUNICATOR_VOID);				
				globalCommandDialogsPanel->toggleMenu(false);
			}
			updateWireCommState();			
			break;
		case iCommandPlaneIntercomVolumeDecrease:
			axis_value_iterate(INTERCOM_CMD_VOLUME,-1);return;
			break;
		case iCommandPlaneIntercomVolumeIncrease:
			axis_value_iterate(INTERCOM_CMD_VOLUME,1);return;
			break;
		case INTERCOM_CMD_VOLUME:
			mainRegulator.setGain(value);
			break;
		case iCommandPlaneIntercomINT:
			performClickableAction(INTERCOM_CMD_MODE,0.0f);
			break;
		case iCommandPlaneIntercomFM:
			performClickableAction(INTERCOM_CMD_MODE,0.1f);
			break;
		case iCommandPlaneIntercomVHF:
			performClickableAction(INTERCOM_CMD_MODE,0.2f);
			break;
		case iCommandPlaneIntercomHF:
			performClickableAction(INTERCOM_CMD_MODE,0.3f);
			break;
		case INTERCOM_CMD_MODE:
			{
				// if we change INTERCOM_FULL_CCW_MODE we should set TACAN according its switcher
				if (m_intecomMode == INTERCOM_FULL_CCW_MODE)
					sourceSwitcher[INTERCOM_SOURCE_TACAN].setOnOff(m_TCN_switcher);

				if (m_intecomMode == INTERCOM_VHF_MODE && m_currentTransmitterNum == INTERCOM_COMM_VHF_AM)
					cockpit::avIntercom::resetCommunicator();

				m_intecomMode = avIntercom_mode(int(cockpit::round(value * 10.0)));
				updateWireCommState();
				if (m_intecomMode == INTERCOM_VHF_MODE && m_bMicSwitchFront)
					setCommunicator(INTERCOM_COMM_VHF_AM);
				else if(m_intecomMode == INTERCOM_FULL_CCW_MODE)
					// always enable TACAN
					 sourceSwitcher[INTERCOM_SOURCE_TACAN].setOnOff(true);
				else if(m_intecomMode == INTERCOM_INT_MODE && m_bMicSwitchFront)
					setCommunicator(INTERCOM_COMM_INT);
				else
					setCommunicator(INTERCOM_COMM_VOID);

				if(	globalCommandDialogsPanel->visible() &&
					globalCommandDialogsPanel->getCommunicatorId() != ICommandDialogsPanel::COMMUNICATOR_AUTO)
				{
					globalCommandDialogsPanel->setCommunicatorId(m_currentTransmitterNum > INTERCOM_COMM_VOID ? communicatorDeviceID[m_currentTransmitterNum] : ICommandDialogsPanel::COMMUNICATOR_VOID);
				}
			}
			break;
		case INTERCOM_CMD_CALL:
			break;
		case iCommandPlaneIntercomINTDecrease:
			axis_value_iterate(INTERCOM_CMD_INT_VOLUME,-1);return;
			break;
		case iCommandPlaneIntercomINTIncrease:
			axis_value_iterate(INTERCOM_CMD_INT_VOLUME,1);return;
			break;
		case INTERCOM_CMD_INT_VOLUME:
			sourceRegulator[INTERCOM_SOURCE_INT].setGain(value);
			break;

		case iCommandPlaneIntercomFMDecrease:
			axis_value_iterate(INTERCOM_CMD_FM_VOLUME,-1);return;
			break;
		case iCommandPlaneIntercomFMIncrease:
			axis_value_iterate(INTERCOM_CMD_FM_VOLUME,1);return;
			break;
		case INTERCOM_CMD_FM_VOLUME:
			sourceRegulator[INTERCOM_SOURCE_VHF_FM].setGain(value);
			break;

		case iCommandPlaneIntercomVHFDecrease:
			axis_value_iterate(INTERCOM_CMD_VHF_VOLUME,-1);return;
			break;
		case iCommandPlaneIntercomVHFIncrease:
			axis_value_iterate(INTERCOM_CMD_VHF_VOLUME,1);return;
			break;
		case INTERCOM_CMD_VHF_VOLUME:
			sourceRegulator[INTERCOM_SOURCE_VHF_AM].setGain(value);
			break;

		case iCommandPlaneIntercomUHFDecrease:
			axis_value_iterate(INTERCOM_CMD_UHF_VOLUME,-1);return;
			break;
		case iCommandPlaneIntercomUHFIncrease:
			axis_value_iterate(INTERCOM_CMD_UHF_VOLUME,1);return;
			break;
		case INTERCOM_CMD_UHF_VOLUME:
			sourceRegulator[INTERCOM_SOURCE_UHF].setGain(value);
			break;

		case iCommandPlaneIntercomAIMDecrease:
			axis_value_iterate(INTERCOM_CMD_AIM_VOLUME,-1);return;
			break;
		case iCommandPlaneIntercomAIMIncrease:
			axis_value_iterate(INTERCOM_CMD_AIM_VOLUME,1);return;
			break;
		case INTERCOM_CMD_AIM_VOLUME:
			sourceRegulator[INTERCOM_SOURCE_AIM].setGain(value);
			break;

		case iCommandPlaneIntercomIFFDecrease:
			axis_value_iterate(INTERCOM_CMD_IFF_VOLUME,-1);return;
			break;
		case iCommandPlaneIntercomIFFIncrease:
			axis_value_iterate(INTERCOM_CMD_IFF_VOLUME,1);return;
			break;
		case INTERCOM_CMD_IFF_VOLUME:
			sourceRegulator[INTERCOM_SOURCE_IFF].setGain(value);
			break;

		case iCommandPlaneIntercomILSDecrease:
			axis_value_iterate(INTERCOM_CMD_ILS_VOLUME,-1);return;
			break;
		case iCommandPlaneIntercomILSIncrease:
			axis_value_iterate(INTERCOM_CMD_ILS_VOLUME,1);return;
			break;
		case INTERCOM_CMD_ILS_VOLUME:
			sourceRegulator[INTERCOM_SOURCE_ILS].setGain(value);
			break;

		case iCommandPlaneIntercomTCNDecrease:
			axis_value_iterate(INTERCOM_CMD_TCN_VOLUME,-1);return;
			break;
		case iCommandPlaneIntercomTCNIncrease:
			axis_value_iterate(INTERCOM_CMD_TCN_VOLUME,1);return;
			break;
		case INTERCOM_CMD_TCN_VOLUME:
			sourceRegulator[INTERCOM_SOURCE_TACAN].setGain(value);
			break;
		case iCommandToggleCommandMenu:
			if(messageVoIP != NULL)
				break;
			//This is the way to toggle menu with no selected radio
			if(globalCommandDialogsPanel->visible())
				globalCommandDialogsPanel->toggleMenu();
			else if(globalOptions->GetDifficultyOptions().easyComm)
			{
				globalCommandDialogsPanel->setCommunicatorId(ICommandDialogsPanel::COMMUNICATOR_AUTO);
				globalCommandDialogsPanel->toggleMenu();
				m_bCommPanelModeHM = false;
			}
			else if(m_bHM_enable)
			{
				setCommunicator(INTERCOM_COMM_VOID);
				globalCommandDialogsPanel->setCommunicatorId(communicatorDeviceID[INTERCOM_COMM_INT]);
				globalCommandDialogsPanel->toggleMenu();
				m_bCommPanelModeHM = true;
			}
			else if(!aircraft().human_ptr->In_Air_())
			{
				globalCommandDialogsPanel->setCommunicatorId(ICommandDialogsPanel::COMMUNICATOR_VOID);
				globalCommandDialogsPanel->toggleMenu();
				m_bCommPanelModeHM = false;
			}
			break;
		default:
			break;
	}
}

void avIntercom::serialize(Serializer &serializer)
{

}

void avIntercom::update()
{
 	cockpit::avIntercom::update();
}

bool avIntercom::isOn() const
{
	return ElecConDevice.isPowered();
}

bool avIntercom::isWireCommOn() const
{
	return sourceSwitcher[INTERCOM_SOURCE_INT].isOn() || m_intecomMode == INTERCOM_SOURCE_INT;
}

bool avIntercom::isWireCommTransmitOn() const
{
	return (m_bHM_enable && sourceSwitcher[INTERCOM_SOURCE_INT].isOn() && m_currentTransmitterNum == INTERCOM_COMM_VOID) || (m_intecomMode == INTERCOM_SOURCE_INT && m_currentTransmitterNum == INTERCOM_COMM_INT);
}

void avIntercom::enableMicro(int position)
{
	switch(position)
	{
		case SWITCH_DOWN:
			setCommunicatorToCommandMenuAndPTT_(INTERCOM_COMM_UHF);
			m_bMicSwitchFront = false;
			break;
		case SWITCH_LEFT: //aft
			setCommunicatorToCommandMenuAndPTT_(INTERCOM_COMM_VHF_FM);
			m_bMicSwitchFront = false;
			break;
		case SWITCH_RIGHT: //front
			if (m_intecomMode ==  INTERCOM_VHF_MODE)
			{
				setCommunicatorToCommandMenuAndPTT_(INTERCOM_COMM_VHF_AM);
			}
			else if (m_intecomMode ==  INTERCOM_INT_MODE)
			{
				setCommunicatorToCommandMenuAndPTT_(INTERCOM_COMM_INT);
			}
			else
			{
				setCommunicatorToCommandMenuAndPTT_(INTERCOM_COMM_VOID);
			}
			m_bMicSwitchFront = true;
			break;
		default:
			m_bMicSwitchFront = false;
			break;
	}
	m_bCommPanelModeHM = false;
}

void avIntercom::disableMicro()
{
	pushToTalk(false);
//	setCommunicator_(INTERCOM_COMM_VOID);

// 	if ((m_intecomMode == INTERCOM_INT_MODE || m_bINT_enable) && m_bHM_enable)
// 	{
// 		//TO DO: enable INT transmitter
// 		m_currentTransmitterNum = INTERCOM_COMM_INT;
// 		setCommunicator_(m_currentTransmitterId);
// 	}
// 	else
// 	{
// 		m_currentTransmitterNum = INTERCOM_COMM_MAX;
// 		//TO DO: disable  transmission
// 		cockpit::avIntercom::resetCommunicator_();
// 	}
}

void avIntercom::l_register_functions(lua_State *Lua_s,int index)
{
	cockpit::avIntercom::l_register_functions(Lua_s, index);
	lua_pushstring(Lua_s, "is_communicator_available");	lua_pushcfunction(Lua_s, l_is_communicator_available);	lua_settable  (Lua_s, index);
	lua_pushstring(Lua_s, "set_communicator");			lua_pushcfunction(Lua_s, l_set_communicator);			lua_settable  (Lua_s, index);
}

int	avIntercom::l_is_communicator_available(lua_State *Lua_s)
{
	avIntercom  *device = l_get_device_from_meta<avIntercom>(Lua_s);
	if(!device->isOn())
		return false;
	unsigned char communicatorId = lua_isnumber(Lua_s, 2) ? (unsigned char)(int)(lua_tonumber(Lua_s, 2)) : 0;
	avIntercom_communicator communicatorNum = (avIntercom_communicator)(std::find(device->communicatorDeviceID, device->communicatorDeviceID + INTERCOM_COMM_MAX, communicatorId) - device->communicatorDeviceID);
	switch(communicatorNum)
	{
		case INTERCOM_COMM_INT:
			lua_pushboolean(Lua_s, device->m_intecomMode == INTERCOM_INT_MODE || (device->m_bHM_enable && device->sourceSwitcher[INTERCOM_SOURCE_INT].isOn()));
			break;
		case INTERCOM_COMM_VHF_AM:
			lua_pushboolean(Lua_s, device->m_intecomMode == INTERCOM_VHF_MODE);
			break;
		default:
			lua_pushboolean(Lua_s, true);
			break;
	}
	return 1;
}

int	avIntercom::l_set_communicator(lua_State *Lua_s)
{
	avIntercom  *device = l_get_device_from_meta<avIntercom>(Lua_s);
	unsigned char communicatorId = lua_isnumber(Lua_s, 2) ? (unsigned char)(int)(lua_tonumber(Lua_s, 2)) : 0;
	avIntercom_communicator communicatorNum = (avIntercom_communicator)(std::find(device->communicatorDeviceID, device->communicatorDeviceID + INTERCOM_COMM_MAX, communicatorId) - device->communicatorDeviceID);
	switch(communicatorNum)
	{
		case INTERCOM_COMM_INT:
			if(device->m_intecomMode != INTERCOM_INT_MODE || (device->m_bHM_enable && device->sourceSwitcher[INTERCOM_SOURCE_INT].isOn()))
				return 0;
			break;
		case INTERCOM_COMM_VHF_AM:
			if(device->m_intecomMode != INTERCOM_COMM_VHF_AM)
				return 0;
			break;
	}
	device->setCommunicator(communicatorNum);
	return 0;
}

void avIntercom::setCommunicator(avIntercom_communicator communicatorNum)
{
	m_currentTransmitterNum = communicatorNum;
	if (communicatorNum > INTERCOM_COMM_INT)
		cockpit::avIntercom::setCommunicator(communicator[communicatorNum]);
	else
		cockpit::avIntercom::resetCommunicator();
}

void avIntercom::setCommunicatorToCommandMenuAndPTT_(avIntercom_communicator communicatorNum)
{
	setCommunicator(communicatorNum);
	const unsigned char communicatorID = communicatorNum > INTERCOM_COMM_VOID ? communicatorDeviceID[communicatorNum] : ICommandDialogsPanel::COMMUNICATOR_VOID;
	if(globalCommandDialogsPanel->getCommunicatorId() != communicatorID)
	{
		globalCommandDialogsPanel->toggleMenu(false);
		globalCommandDialogsPanel->setCommunicatorId(communicatorID);
	}
	pushToTalk(true);
}

// TODO: to implement
void avIntercom::onElecPowerChange(const lwl::CSignalInfo* pInfo)
{
	updateState();	
}

