#ifndef __avIntercom__
#define __avIntercom__

#include "Avionics/avDevice.h"
#include "Avionics/avTelephone.h"
#include "Sound/Sounder.h"
#include "Comm/wMessage.h"

namespace cockpit {

class avReceiver;
class avCommunicator;

enum {
	INTERCOM_COMM_VOID = -1
};

class COCKPITBASE_API avIntercom : public avDevice
{
public:
	avIntercom();
	//avDevice interface
	virtual void initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name);
	virtual void post_initialize();
	virtual void release();
	virtual void SetCommand(int command, float value = 0);
	virtual void serialize(Serializer &serializer);
	//end of interface	
	virtual bool isOn() const;
	virtual bool isWireCommOn() const;
	virtual bool isWireCommTransmitOn() const;
	void setVoIPMode(bool mode);
	float getNoiseSoundLevel() const;
	float getSignalSoundLevel(ObjectID senderID) const;
protected:

	virtual bool		isCommunicatorAvailable(unsigned comm_id) { return isOn(); }
	virtual void		trySetCommunicator(unsigned comm_id) {}
	void		updateState();
	void		updateWireCommState();
	void		setCommunicator(avCommunicator * pCommunicator);
	void		resetCommunicator();
	void		pushToTalk(bool pushed);
	avCommunicator *	curCommunicator;
	avTelephone			telephone;
	Sounder::Switcher	mainSwitcher;
	Sounder::Regulator	mainRegulator;
	bool				modeVoIP;
	wMessagePtr			messageVoIP;
	typedef ed::list<avReceiver*> Receivers;
	Receivers			receivers;
protected:
	void		l_register_functions(lua_State *Lua_s,int index);
	static int	l_is_communicator_available(lua_State *Lua_s);
	static int	l_set_communicator(lua_State *Lua_s);
	static int	l_set_voip_mode(lua_State *Lua_s);
	static int	l_get_noise_level(lua_State *Lua_s);
	static int	l_get_signal_level(lua_State *Lua_s);
private:
	void startVoIPTransmission_();
	class MicSwitchTimer : public wTime
	{
	public:
		MicSwitchTimer();
		void setIntercom(avIntercom * intercom);
		void start();
		void abort();
	private:
		virtual void NextEvent();
		avIntercom * intercom_;
	} micSwitchTimer;
	friend class MicSwitchTimer;
};

}

#endif __avIntercom__