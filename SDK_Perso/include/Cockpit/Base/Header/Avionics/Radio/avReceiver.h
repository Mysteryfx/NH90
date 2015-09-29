#ifndef _avReceiver_h
#define _avReceiver_h

#include "CockpitBase.h"
#include "Sound/Sounder.h"
#include "Physic/wRadioDevice.h"
#include "Comm/wHumanCommunicator.h"
#include <ed/map.h>

class wRadioReceiver;
class wRadioTransmitter;

namespace Lua
{
	class Config;
}

namespace cockpit
{

#define AV_RECEIVER_LUA_EXPORT_BY_SINGLE_TABLE

/*avReceiver - радиоприёмник ОЗВУЧИВАЕМЫХ(!) сигналов. Выполняет Функции АРУ, шумоподавления, ручной
регулировки громкости и приглушения, ввод в действие разных звуковых эффектов типа шумов, биений и.т.д.*/

class COCKPITBASE_API avReceiver
{
protected:
	class AGC
	{
	public:
		AGC();
		void	init(double	signalInRatioMinDb, double signalInRatioMaxDb, double signalInMin, double signalOutRatioDevDb);
		void	init(Lua::Config & config, double signalInMin);
		double	getGain(double signalIn) const;
	private:
		//parameters
		double	signalInRatioMin_;
		double	signalInRatioMax_;
		double	signalInMin_;
		double	signalOutRatioMin_;
		double	signalOutRatioMax_;
		double	K_, K2_;
	};
	class Detector
	{
	public:
		virtual double operator()(double signal, double noise, double innerNoise) = 0;
	};
	class DetectorAM : public Detector
	{
	public:
		virtual double operator()(double signal, double noise, double innerNoise);
	};
	class DetectorFM : public Detector
	{
	public:
		virtual double operator()(double signal, double noise, double innerNoise);
	};
	class Squelch
	{
	public:
		Squelch();
		void	setOnOff(bool on);
		bool	isOn() const { return on_; }
		double	operator()(float signal, float noise);
	private:
		bool	on_;
		bool	LFFopened_;
	};
private:
	class RadioDeviceCallback : public wRadioDevice::Callback
	{
	public:
		RadioDeviceCallback(avReceiver * pReceiver_);
		virtual void	operator()(wRadioDevice * pRadioDevice, wRadioDevice::Event event);
	private:
		avReceiver *	pReceiver;
	};

	class MessageHandler : public wHumanCommunicator::ReceivingMessageHandler
	{
	public:
		MessageHandler(avReceiver * pReceiver_);
		virtual void	updateMessage(const wMessagePtr & message);
		virtual void	finishMessage(const wMessagePtr & message, bool onTime);
		virtual float	getMessageSNR(const wRadioTransmitter * pTransmitter) const;
	private:
		avReceiver *	pReceiver;
	};
	friend class MessageHandler;

	enum {	STATIC_SOURCE_STATIC,
			STATIC_SOURCE_ATMOSPHERIC,
			STATIC_SOURCE_BEAT_TONE,
			STATIC_SOURCE_INTERMODULATION,
			STATIC_SOURCE_MAX };

public:
	typedef avReceiver* (*Extractor)(lua_State * L);
	typedef ed::map<void*, Extractor> Extractors;
	struct COCKPITBASE_API State
	{
	public:
		State();
		int saveInState(lua_State* L) const;
	public:
		unsigned char	name;
		bool			isOn;
		bool			muted;
		float			volume;
		float			frequency;
		wModulation		modulation;
		float			innerNoise;
		cPoint			pos;
	};
public:
	avReceiver();
	~avReceiver();	

	void			initialize(const char * name_, Lua::Config & config, wRadioReceiver * pRadioReceiver_ = NULL);
	void			postInitialize();
	void			update();
	void			release();	

	//initialization
	void			setReceiver(wRadioReceiver* tr_receiver_);
	inline wRadioReceiver* getReceiver() const { return pRadioReceiver; }
	inline Sounder::Element & getSoundElement() { return volume; }

	//settings
	void			setOnOff(bool on);
	void			setSquelchOnOff(bool on);
	void			setVolume(float volume_);
	void			mute(bool on);
	void			setModulation(wModulation modulation_);
	void			setFrequency(float frequency);
	void			setBlanked(bool blanked_);
	void			damageAntenna(bool value);
	void            addStaticSoundSource(Sounder::Source& src);

	bool			isOn() const;
	bool			getSquelchIsOn() const {return squelch.isOn();}
	float			getVolume() const;
	bool			getMuteIsOn() const;
	wModulation		getModulation() const;
	float			getFrequency() const;

	//status
	virtual bool	isSignalReceived() const;
	inline float	getSignalLF() const { return signalLF; }
	float			getNoiseSoundLevel() const;
	float			getSignalSoundLevel(ObjectID senderID) const;
	void			getState(State & state);
	
	void			l_register_functions(lua_State *Lua_s,int index);
	static avReceiver* l_get_ptr(lua_State * L_state);
	static			Extractors extractors;
protected:
	void			postInitialize_();
	void			updateSignalsPower();
	void			updateMessages();
	virtual void	updateMessage(const wMessagePtr & message);
	static int		l_get_state(lua_State *Lua_s);

	Sounder::Source		staticSources[STATIC_SOURCE_MAX];
	Sounder::MessagesSource	messagesSource;
	Sounder::Switcher	switcher;
	Sounder::Switcher	muter;
	Sounder::Regulator	volume;
	std::auto_ptr<RadioDeviceCallback>	radioDeviceCallback;
	std::auto_ptr<MessageHandler>		messageHandler;
	wRadioReceiver*		pRadioReceiver;
	bool				ownRadioReceiver;
	wModelTime			lastUpdateTime;
	//АРУ
	AGC					agc;
	//ВЧ-часть
	bool				antennaIsDamaged;
	double				signalHF, noiseHF;
	//НЧ-часть
	double				signalLF, noiseLF;
	Detector*			detectors[MODULATION_FM + 1];
	DetectorAM			detectorAM;
	DetectorFM			detectorFM;	
	Squelch				squelch;
	wModulation			modulation;
	bool				blanked;
};

class COCKPITBASE_API avCommunicator :  public avReceiver 
{
public:
	struct COCKPITBASE_API State : public avReceiver::State
	{
	public:
		State();
		int saveInState(lua_State* L) const;
	public:
		float			power;
	};
public:
	avCommunicator();
	~avCommunicator();

	void		initialize(const char * name_, Lua::Config & config, wRadioReceiver * pRadioReceiver_ = NULL, wRadioTransmitter * pRadioTransmitter_ = NULL);
	void		postInitialize();
	void		release();

	void		setAsCurrent();
	static void	resetCurrent();
	void		setAsTransiverForTunningFromOutside();
	
	inline wTransiver * getTransiver() { return transiver.get(); }
	inline wRadioTransmitter * getTransmitter() const { return pRadioTransmitter; }
	
	//settings
	void		setOnOff(bool on);
	void		setReceiverOnOff(bool on);
	void		setTransmitterOnOff(bool on);
	void		setFrequency(float frequency);
	void		setModulation(wModulation modulation_);
	
	//status	

	bool		isReceiverOn() const;
	bool		isTransmitterOn() const;
	bool		isOn() const;

	void		getState(State & state);
	void		l_register_functions(lua_State *Lua_s,int index);
protected:
	virtual void updateMessage(const wMessagePtr & message);
		
	void		sendNetMessage(bool on);

	static int	l_get_state(lua_State *Lua_s);

protected:
	wRadioTransmitter *			pRadioTransmitter;
	std::auto_ptr<wTransiver>	transiver;
	bool						ownRadioTransmitter;
};

}

#endif // _avReceiver_h
