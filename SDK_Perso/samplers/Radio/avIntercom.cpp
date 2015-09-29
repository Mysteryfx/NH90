#include "stdafx.h"

#include "ccSound.h"
#include "Utilities/ccUtilities.h"

#include "Avionics/Radio/avIntercom.h"
#include "Avionics/Radio/avRadioGUI.h"
#include "Avionics/Radio/avReceiver.h"
#include "ccModuleManager.h"

#include "ICommandDialogsPanel.h"
#include "iCommand.h"
#include "IOptions.h"

#include "Comm/wHumanCommunicator.h"
#include "Comm/wMsgEnum.h"

using namespace cockpit;

REGISTER_DYNAMIC_FACTORY(cockpit::avIntercom, avIntercom);

avIntercom::avIntercom() : curCommunicator(NULL), modeVoIP(false)
{
	micSwitchTimer.setIntercom(this);
}

//avDevice interface
void avIntercom::initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name)
{
	avDevice::initialize(ID, Name, script_name);
	telephone.initialize(Name.c_str());
}

void avIntercom::post_initialize()
{
	mainRegulator.setGain(1.0f);
	mainSwitcher.setOnOff(true);	
#ifdef DEBUG_TEXT	
	mainSwitcher.name = "mainSwitcher";
	mainRegulator.name = "mainRegulator";
#endif
	mainRegulator >> mainSwitcher;
	telephone.post_initialize();
	ICommandDialogsPanel::Intercom intercom;
	intercom.id = this->ID();
	Lua::Config config(ensured_Lua());
	if(config.open("GUI"))
	{
		if(config.open("interphone"))
		{
			ICommandDialogsPanel::Interphone interphone;
			interphone.id = this->ID();
			loadInterphone(config, interphone);
			globalCommandDialogsPanel->loadInterphone(interphone);
			config.pop();
		}
		globalCommandDialogsPanel->loadIntercom(intercom);		
		config.pop();
	}
	else
	{
		globalCommandDialogsPanel->loadIntercom(intercom);
	}
	globalCommandDialogsPanel->toggleMenu(false);

	listen_command(this, iCommandToggleCommandMenu);
}

void avIntercom::release()
{
	telephone.release();
	avDevice::release();
}

void avIntercom::SetCommand(int command, float value)
{
	switch(command)
	{
		case iCommandToggleCommandMenu:
			if(globalCommandDialogsPanel->visible())
				globalCommandDialogsPanel->toggleMenu(false);
			else if(messageVoIP == NULL)
			{
				if(globalOptions->GetDifficultyOptions().easyComm)
				{
					globalCommandDialogsPanel->setCommunicatorId(ICommandDialogsPanel::COMMUNICATOR_AUTO);
					globalCommandDialogsPanel->toggleMenu();
				}
				else if(!aircraft().human_ptr->In_Air_())
				{
					globalCommandDialogsPanel->setCommunicatorId(ICommandDialogsPanel::COMMUNICATOR_VOID);
					globalCommandDialogsPanel->toggleMenu();
				}
			}
			break;
	}
}

void avIntercom::serialize(Serializer &serializer)
{

}

bool avIntercom::isOn() const
{
	return true;
}

bool avIntercom::isWireCommOn() const
{
	return true;
}

bool avIntercom::isWireCommTransmitOn() const
{
	return isWireCommOn();
}

void avIntercom::setVoIPMode(bool mode)
{
	if(modeVoIP != mode)
	{
		if(mode)
		{

		}
		else
		{
			if(messageVoIP != NULL)
			{
				c_get_communicator()->finishTransmition(false);
				messageVoIP = NULL;
				globalCommandDialogsPanel->setVoIP(false);
			}
		}
	}
	modeVoIP = mode;
}

float avIntercom::getNoiseSoundLevel() const
{
	if(receivers.size() == 1)
	{
		return (*receivers.begin())->getNoiseSoundLevel();
	}
	else
	{
		float noiseLevel = 0.0f;
		std::for_each(	receivers.begin(), receivers.end(),
						[&noiseLevel](avReceiver * receiver) -> bool
						{
							return noiseLevel += pow(receiver->getNoiseSoundLevel(), 2);
						});
		noiseLevel = sqrt(noiseLevel);
		return noiseLevel;
	}
}

float avIntercom::getSignalSoundLevel(ObjectID senderID) const
{
	if(receivers.size() == 1)
	{
		return (*receivers.begin())->getSignalSoundLevel(senderID);
	}
	else
	{
		float signalLevel = 0.0f;
		std::for_each(	receivers.begin(), receivers.end(),
						[&signalLevel, senderID](avReceiver * receiver) -> bool
						{
							return signalLevel += pow(receiver->getSignalSoundLevel(senderID), 2);
						});
		signalLevel = sqrt(signalLevel);
		return signalLevel;
	}
}

void avIntercom::updateState()
{
	mainSwitcher.setOnOff(isOn());
	if(isOn() && curCommunicator != NULL)
		curCommunicator->setAsCurrent();
	else
		avCommunicator::resetCurrent();
	updateWireCommState();
}

void avIntercom::updateWireCommState()
{
	telephone.setOnOff(isOn() && isWireCommOn());
	telephone.setMicrophoneOnOff(isWireCommTransmitOn());
}

void avIntercom::setCommunicator(avCommunicator * pCommunicator)
{
	curCommunicator = pCommunicator;
	updateState();
}

inline void avIntercom::resetCommunicator()
{
	setCommunicator(NULL);
}

void avIntercom::pushToTalk(bool pushed)
{
	if(modeVoIP)
	{
		if(pushed)
		{
			if(messageVoIP == NULL)
			{
				micSwitchTimer.start();
				globalCommandDialogsPanel->toggleMenu(false);
			}
		}
		else
		{
			if(messageVoIP != NULL)
			{
				c_get_communicator()->finishTransmition(false);
				messageVoIP = NULL;
				globalCommandDialogsPanel->setVoIP(false);
			}
			else if(micSwitchTimer.IsActive())
			{
				micSwitchTimer.abort();
				if(curCommunicator != NULL)
					globalCommandDialogsPanel->toggleMenu(true);
			}
		}
	}
	else
	{
		if(pushed)
			globalCommandDialogsPanel->toggleMenu();
	}
}

void avIntercom::l_register_functions(lua_State *Lua_s,int index)
{
	avDevice::l_register_functions(Lua_s, index);

	lua_pushstring(Lua_s, "set_voip_mode");					lua_pushcfunction(Lua_s, l_set_voip_mode);				lua_settable(Lua_s,index);
	lua_pushstring(Lua_s, "get_noise_level");				lua_pushcfunction(Lua_s, l_get_noise_level);			lua_settable(Lua_s,index);
	lua_pushstring(Lua_s, "get_signal_level");				lua_pushcfunction(Lua_s, l_get_signal_level);			lua_settable(Lua_s,index);
}

int	avIntercom::l_set_voip_mode(lua_State *Lua_s)
{
	avIntercom * device = l_get_device_from_meta<avIntercom>(Lua_s);
	if(device != NULL)
	{
		if(lua_isboolean(Lua_s, 2))
		{
			bool mode = lua_toboolean(Lua_s, 2);
			device->setVoIPMode(mode);
		}
	}
	return 0;
}

int	avIntercom::l_get_noise_level(lua_State *Lua_s)
{
	avIntercom * device = l_get_device_from_meta<avIntercom>(Lua_s);
	if(device != NULL)
	{
		lua_pushnumber(Lua_s, device->getNoiseSoundLevel());
		return 1;
	}
	else
		return 0;
}

int	avIntercom::l_get_signal_level(lua_State *Lua_s)
{
	avIntercom * device = l_get_device_from_meta<avIntercom>(Lua_s);
	if(device != NULL)
	{
		if(lua_isnumber(Lua_s, 2))
		{
			ObjectID senderID = lua_tonumber(Lua_s, 2);
			lua_pushnumber(Lua_s, device->getSignalSoundLevel(senderID));
			return 1;
		}
	}
	return 0;
}

void avIntercom::startVoIPTransmission_()
{
	assert(messageVoIP == NULL);
	messageVoIP = new wMessage(*c_get_communicator(), wMsgPlayerVoIP, wMessage::PLAY_MODE_UNLIMITED_DURATION);
	messageVoIP->type = wMessage::TYPE_NULL;
	c_get_communicator()->transmitMessageNow(messageVoIP);
	globalCommandDialogsPanel->setVoIP(true);
}

avIntercom::MicSwitchTimer::MicSwitchTimer() : intercom_(NULL)
{
}

void avIntercom::MicSwitchTimer::setIntercom(avIntercom * intercom)
{
	intercom_ = intercom;
}

void avIntercom::MicSwitchTimer::start()
{
	assert(!IsActive());
	wakeUpTime = wTime::GetModelTime() + 0.5;
	InsertToCalendar();
}

void avIntercom::MicSwitchTimer::abort()
{
	Cancel();
}

void avIntercom::MicSwitchTimer::NextEvent()
{
	intercom_->startVoIPTransmission_();
	Cancel();
}