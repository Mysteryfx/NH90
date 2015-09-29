#include "stdafx.h"

#include "Avionics/Radio/avReceiver.h"

#include "Avionics/avDevice.h"
#include "IwoLA.h"
#include "ccSound.h"

#include "Comm/wMessage.h"
#include "Comm/wTransiver.h"
#include "Physic/wRadioReceiver.h"
#include "Physic/wRadioTransmitter.h"

#include "Lua/Config.h"

#include "mathDebug.h"
#include "Utilities/ccUtilities.h"

using namespace cockpit;

//-=avReceiver::AGR=-

avReceiver::AGC::AGC()
{
}

void avReceiver::AGC::init(	double signalInRatioMinDb, double signalInRatioMaxDb,
							double signalInMin,						
							double signalOutRatioDevDb)
{
	signalInRatioMin_ = pow(10.0, signalInRatioMinDb / 20.0);
	signalInRatioMax_ = pow(10.0, signalInRatioMaxDb / 20.0);
	signalInMin_ = signalInMin;
	signalOutRatioMin_ = pow(10.0, -signalOutRatioDevDb / 20.0);
	signalOutRatioMax_ = 1.0;
	K_ = signalOutRatioDevDb / (signalInRatioMaxDb - signalInRatioMinDb);
	K2_ = pow(10.0, signalOutRatioDevDb / 20.0);
}

void avReceiver::AGC::init(Lua::Config & config, double signalInMin)
{
	double signalInRatioMinDb;
	config.get("input_signal_linear_zone",	&signalInRatioMinDb,	10.0);
	double signalInRatioMaxDb;
	config.get("input_signal_deviation",	&signalInRatioMaxDb,	50.0);
	double signalOutRatioDevDb;
	config.get("output_signal_deviation",	&signalOutRatioDevDb,	5.0);	
	double T;
	config.get("regulation_time",			&T,						0.25);
	init(signalInRatioMinDb, signalInRatioMaxDb, signalInMin, signalOutRatioDevDb);
}

double avReceiver::AGC::getGain(double signalIn) const
{
	CheckValid(signalIn);
	const double signalInRatio = signalIn / signalInMin_;
	if(signalInRatio > signalInRatioMax_)
		return signalOutRatioMax_;
	else if(signalInRatio > signalInRatioMin_)
		return pow(signalInRatio / signalInRatioMin_, K_) / K2_ * signalOutRatioMax_;
	else
		return signalInRatio * signalOutRatioMin_;
}

//-=avReceiver::DetectorAM=-
double  avReceiver::DetectorAM::operator()(double signal, double noise, double innerNoise)
{
	return signal / (signal + noise);
}

double avReceiver::DetectorFM::operator()(double signal, double noise, double innerNoise)
{
	double SNR = signal / noise;
	if(SNR < 0.05)
	{
		return 0.0;
	}
	else
	{
		double SNR_in_DB = 10.0 * log(SNR) / log(10.0);
		double noise_to_inner_noise = noise / innerNoise;
		double SNR_lim_in_DB = 10.0 * log(1.0 * noise_to_inner_noise) / log(10.0); //начало спада шума
		double SNR_max_in_DB = 10.0 * log(10.0 * noise_to_inner_noise) / log(10.0); //шума уже нет						
		//детектор
		float noiseVolume = 0.0;
		if(SNR_in_DB < SNR_max_in_DB)
		{
			noiseVolume = 1.0;
			if(SNR_in_DB > SNR_lim_in_DB)
			{
				noiseVolume = (1.0 - (SNR_in_DB - SNR_lim_in_DB) / (SNR_max_in_DB - SNR_lim_in_DB));
			}
		}						
		return 1.0 - noiseVolume;
	}	
}

//-=avReceiver::Squelch=-

avReceiver::Squelch::Squelch() : on_(false), LFFopened_(false)
{
}

void avReceiver::Squelch::setOnOff(bool on)
{
	on_ = on;
}

double avReceiver::Squelch::operator()(float signal, float noise)
{
	LFFopened_ = !on_ || (noise - signal < 0.1f);
	return LFFopened_ ? 1.0 : 0.0;
}

//-=avReceiver::RadioDeviceUser=-

avReceiver::RadioDeviceCallback::RadioDeviceCallback(avReceiver * pReceiver_) : pReceiver(pReceiver_)
{
}

void avReceiver::RadioDeviceCallback::operator()(wRadioDevice * pRadioDevice, wRadioDevice::Event event)
{
	if(event == wRadioDevice::EVENT_UPDATE)
	{
		pReceiver->update();
	}
}

avReceiver::MessageHandler::MessageHandler(avReceiver * pReceiver_) : pReceiver(pReceiver_)
{
}

void avReceiver::MessageHandler::updateMessage(const wMessagePtr & message)
{
	pReceiver->updateSignalsPower();
	pReceiver->updateMessage(message);
}

void avReceiver::MessageHandler::finishMessage(const wMessagePtr & message, bool onTime)
{
	if(!onTime)
		pReceiver->messagesSource.stop(message);
}

inline bool checkMessageSignal(const wMessagePtr & message, wRadioSignals signals)
{
	return message->pRadioTransmitter != NULL && ((message->pRadioTransmitter->getSignals() & signals) != SIGNAL_NULL);
}

float avReceiver::MessageHandler::getMessageSNR(const wMessagePtr & message) const
{
	const float minGain = 0.05f;
	float deviceResultGain = pReceiver->switcher.getResultGain();
	if(deviceResultGain < minGain)
	{
		return 0.0f;
	}
	pReceiver->updateSignalsPower();
	if(message->pRadioTransmitter != NULL)
	{
		if(checkMessageSignal(message, getSignalByModulation(pReceiver->getModulation())))
		{
			float messageGain = pReceiver->signalHF > pReceiver->pRadioReceiver->getNoiseLevel() ? float(pReceiver->pRadioReceiver->getE(message->pRadioTransmitter) / pReceiver->signalHF * pReceiver->signalLF) : 0.0f;
			float otherMessagesGain = pReceiver->signalLF - messageGain;
			float noiseGain = pReceiver->noiseLF;
			return deviceResultGain * messageGain / (deviceResultGain * (otherMessagesGain + noiseGain) + minGain);
		}
		else
			return 0.0f;
	}
	else
		return 1.0f;
	
}

//-=avReceiver::State=-

avReceiver::State::State()
{
	name			= 0;
	isOn			= false;
	muted			= false;
	volume			= 0.0f;
	frequency		= 0.0f;
	modulation		= MODULATION_VOID;
	innerNoise		= 0.0f;
}

int avReceiver::State::saveInState(lua_State* L) const
{
	lua_newtable(L);
	int index = lua_gettop(L);

	lua_pushstring(L, "name");
	lua_pushnumber(L, name);
	lua_settable  (L, index);

	lua_pushstring(L, "isOn");
	lua_pushboolean(L, isOn ? 1 : 0);
	lua_settable  (L, index);

	lua_pushstring(L, "soundIsOn");
	lua_pushboolean(L, muted ? 1 : 0);
	lua_settable  (L, index);

	lua_pushstring(L, "volume");
	lua_pushnumber(L, volume);
	lua_settable  (L, index);

	lua_pushstring(L, "modulation");
	if(modulation == MODULATION_AM)
	{
		lua_pushstring(L, "AM");
	}
	else //if(modulation == MODULATION_FM)
	{
		lua_pushstring(L, "FM");
	}
	lua_settable  (L, index);

	lua_pushstring(L, "innerNoise");
	lua_pushnumber(L, innerNoise);
	lua_settable  (L, index);

	lua_pushstring(L, "pos_x");
	lua_pushnumber(L, pos.x);
	lua_settable  (L, index);

	lua_pushstring(L, "pos_y");
	lua_pushnumber(L, pos.y);
	lua_settable  (L, index);

	lua_pushstring(L, "pos_z");
	lua_pushnumber(L, pos.z);
	lua_settable  (L, index);

	return index;
}

//-=avReceiver=-

avReceiver::Extractors avReceiver::extractors;

avReceiver::avReceiver() : pRadioReceiver(0), ownRadioReceiver(false), modulation(MODULATION_AM), messagesSource(4)
{
	detectors[MODULATION_AM] = &detectorAM;
	detectors[MODULATION_FM] = &detectorFM;
	radioDeviceCallback.reset(new RadioDeviceCallback(this));
	messageHandler.reset(new MessageHandler(this));
}

avReceiver::~avReceiver()
{
	assert(pRadioReceiver == NULL);
}

void avReceiver::initialize(const char * name_, Lua::Config & config, wRadioReceiver * pRadioReceiver_)
{
	assert(pRadioReceiver == NULL);
	lastUpdateTime = 0.0;
	antennaIsDamaged = false;
	signalHF = noiseHF = 0.0;

	blanked = false;
	signalLF = noiseLF = 0.0;

	if(pRadioReceiver_ != NULL)
	{
		setReceiver(pRadioReceiver_);
		ownRadioReceiver = false;
	}
	else
	{
		float innerNoise;
		config.get("innerNoise",		&innerNoise,		0.0f);
		float frequencyAccuracy;
		config.get("frequency_accuracy",&frequencyAccuracy,	0.0f);
		float bandWidth;
		config.get("band_width",		&bandWidth,			0.0f);

		pRadioReceiver_ = new wRadioReceiver();
		pRadioReceiver_->setName(name_);
		pRadioReceiver_->setBody(aircraft().human_ptr);	
		pRadioReceiver_->setNoiseLevel(innerNoise);
		pRadioReceiver_->setFrequencyAccuracy(frequencyAccuracy);
		pRadioReceiver_->setBandWidth(bandWidth);
		setReceiver(pRadioReceiver_);
		ownRadioReceiver = true;
	}

	if(config.open("agr"))
	{
		agc.init(config, pRadioReceiver->getNoiseLevel() * 0.1);
		config.pop();
	}
	else
		agc.init(10.0, 50.0, pRadioReceiver->getNoiseLevel() * 0.1, 5.0);
#ifdef DEBUG_TEXT
	for(int s = 0; s < STATIC_SOURCE_MAX; s++)
	{
		staticSources[s].name = name_;
	}
	messagesSource.name = name_;
#endif
}

void avReceiver::postInitialize()
{
	c_get_communicator()->getTransiverStorage().addReceiver(pRadioReceiver);
	postInitialize_();
}

void avReceiver::update()
{
	pRadioReceiver->update();
	updateSignalsPower();
	updateMessages();	
}

void avReceiver::release()
{
	c_get_communicator()->removeMessageHandler(messageHandler.get());
	if(pRadioReceiver != NULL)
	{
		c_get_communicator()->getTransiverStorage().removeReceiver(pRadioReceiver);
		if(ownRadioReceiver)
			delete pRadioReceiver;
		pRadioReceiver = NULL;
	}
}

void avReceiver::damageAntenna(bool value)
{
	antennaIsDamaged = value;
	pRadioReceiver->setOnOff(switcher.isOn() && !antennaIsDamaged);
}

void avReceiver::setSquelchOnOff(bool on)
{
	squelch.setOnOff(on);
}

void avReceiver::setVolume(float volume_)
{
	volume.setGain(volume_);
}

void avReceiver::mute(bool on)
{
	muter.setOnOff(!on);
}

void avReceiver::setModulation(wModulation modulation_)
{
	modulation = modulation_;
}

void avReceiver::setFrequency(float frequency)
{
	if(pRadioReceiver)
	{
		pRadioReceiver->setFrequency(frequency);
	}
}

void avReceiver::setBlanked(bool blanked_)
{
	blanked = blanked_;
}

bool avReceiver::isOn() const
{
	return pRadioReceiver->isOn();
}

float avReceiver::getVolume() const
{
	return volume.getGain();
}

wModulation avReceiver::getModulation() const
{
	return modulation;
}

float avReceiver::getFrequency() const
{
	return pRadioReceiver ? pRadioReceiver->getFrequency() : 0.0f;
}

bool avReceiver::getMuteIsOn() const
{
	return !muter.isOn();
}

bool avReceiver::isSignalReceived() const //пойман хотя бы один передатчик
{
	return pRadioReceiver != NULL ? (pRadioReceiver->isOn() && !pRadioReceiver->isBlanked() && !pRadioReceiver->getTransmitters().empty()) : false;
}

float avReceiver::getNoiseSoundLevel() const
{
	float noiseLevel = 0.0f;
	for(unsigned int s = 0; s < STATIC_SOURCE_MAX; ++s)
	{
		noiseLevel += pow(staticSources[s].getResultGain(), 2);
	}
	return sqrt(noiseLevel);
}

float avReceiver::getSignalSoundLevel(ObjectID senderID) const
{
	const Sounder::MessagesSource::Element * element = std::find_if(messagesSource.begin(),
																	messagesSource.end(),
																	[senderID](const Sounder::MessagesSource::Element & element) -> bool
																	{
																		return	element.message != NULL &&
																				element.message->sender.pUnit->ID() == senderID;
																	} );
	if(element != messagesSource.end())
		return element->messageSource.getResultGain();
	else
		return 0.0f;
}

void avReceiver::setReceiver(wRadioReceiver* pRadioReceiverIn) 
{
	assert(pRadioReceiverIn != NULL);
	pRadioReceiver	= pRadioReceiverIn;
}

void avReceiver::setOnOff(bool on)
{
	switcher.setOnOff(on);
	pRadioReceiver->setOnOff(switcher.isOn() && !antennaIsDamaged);
}

//void avReceiver::update_volumes(double delta_t)
//{
//	if(!get_device_status())
//	{
//		clear_volumes();
//		return;
//	}
//
//	wModelTime current_time = wTime::GetModelTime();
//
//	if(!pRadioReceiver)
//	{
//		noise_volume = 0.0f;
//		LFF_open = true;
//		signals_info.clear();
//		wHumanCommunicator * human_communicator = c_get_communicator();
//		if(human_communicator)
//		{
//			//не радио-приемник
//			const wReceivingMessages & receiving_messages = human_communicator->get_receiving_messages();
//			for(wReceivingMessages::const_iterator itRM = receiving_messages.begin();
//				itRM != receiving_messages.end();
//				itRM ++)
//			{
//				SignalInfo signal_info(0, SIGNAL_TYPE_VOICE, 1.0f, itRM->first.get());
//				signals_info.push_back(signal_info);
//			}
//		}
//		return;
//	}
//	
//	//изменилась ли ситуация в эфире
//	bool changed = changesArePresent;	
//	changesArePresent = false;
//	if(!changed)
//	{
//		if(first_update_volumes)
//		{
//			changed = true;
//			first_update_volumes = false;
//		}
//		else
//		{	
//			if(pRadioReceiver->lastUpdateTime > last_update_time)
//			{
//				changed = true;
//				last_update_time = current_time;
//			}	
//		}
//	}
//	
//	if(	(getSoundIsOn() || first_update_volumes) &&
//		changed) 
//	{
//		const wRadioReceiver::Transmitters & transmitters = pRadioReceiver->getTransmitters();		
//		double Eni = pRadioReceiver->getSensitivity() * 0.1f; //ср. уровень внутр. шумов
//		double Pni = Eni * Eni;
//		//float frequency = tr_receiver->getFrequency();
//		//мощности принимаемых некогерентных сигналов
//		double noise_power = (Eni * Eni * Eni * Eni);
//		double audible_signal_power = 0.0;
//		int audible_signal_sources_qty = 0;
//		if(!antennaIsDamaged)
//		{			
//			for(wRadioReceiver::Transmitters::const_iterator itT = transmitters.begin();
//				itT != transmitters.end();
//				itT++)
//			{
//				wRadioTransmitter * tr_transmitter = itT->first;
//				double U = itT->second;
//				double signalPower = itT->second * itT->second;
//				double signalPower2 = signalPower * signalPower;
//				sum_power += signalPower2;
//				wRadioDevice::Callback * pUser = tr_transmitter->getUser();
//				if(	pUser == NULL &&
//					pUser->getModulation() > SIGNAL_MODULATION_VOID)
//				{
//					audible_signal_power += signalPower2;
//				}
//				else
//				{
//					noise_power += signalPower2;
//				}
//			}
//		}
//		sum_power				= sqrt(audible_signal_power + noise_power);
//		audible_signal_power	= sqrt(audible_signal_power);
//		noise_power				= sqrt(noise_power);
//		
//		double SNR =  audible_signal_power / noise_power;
//		//громкости
//		noise_volume = 0.0f;
//		interference_volume = 0.0f;
//		interference_frequency = 0.0f;
//		signals_info.clear();		
//		float signal_volume = 1.0f;
//		//громкости шума
//		if(modulation == MODULATION_AM)
//		{		
//			noise_volume = noise_power / sum_power;
//			if(SNR > 1.0f)
//			{					
//				signal_volume = 1.0f - noise_volume;
//			}
//			else
//			{
//				signal_volume = 0.0f;
//			}
//		}
//		else if(modulation == SIGNAL_VOICE_FM)
//		{
//			if(SNR < 0.05)
//			{
//				noise_volume = 1.0f;
//			}
//			else
//			{
//				double SNR_in_DB = 10.0 * log(SNR) / log(10.0);
//				double noise_to_inner_noise = noise_power / Pni;
//				double SNR_lim_in_DB = 10.0 * log10(1.0 * noise_to_inner_noise); //начало спада шума
//				double SNR_max_in_DB = 10.0 * log10(10.0 * noise_to_inner_noise); //шума уже нет						
//				//детектор
//				noise_volume = 0.0f;
//				if(SNR_in_DB < SNR_max_in_DB)
//				{
//					noise_volume = 1.0f;
//					if(SNR_in_DB > SNR_lim_in_DB)
//					{
//						noise_volume = (1.0f - (SNR_in_DB - SNR_lim_in_DB) / (SNR_max_in_DB - SNR_lim_in_DB));
//					}
//				}						
//			}
//			signal_volume = 1.0f - noise_volume;
//		}
//		
//		bool noise_over_signal = (noise_volume - signal_volume > 0.1f) && squelch;
//		//шумодав
//		if(squelch)
//		{
//			LFF_open = !noise_over_signal;
//		}
//		else
//		{
//			LFF_open = true;
//		}
//
//		if(!LFF_open)
//		{
//			return; //продолжать незачем: ничего слышно не будет		
//		}
//		
//		//громкость сигналов
//		if(!antennaIsDamaged)
//		{
//			double max_signal_power = 0.0;
//			wRadioTransmitter *max_signal_source = 0;
//			int over_noise_signals_qty = 0;			
//			
//			for(wRadioReceiver::Transmitters::const_iterator itT = transmitters.begin();
//				itT != transmitters.end();
//				itT++)
//			{
//				wRadioTransmitter *tr_transmitter = itT->first;
//				if(!tr_transmitter) continue;
//				float volume = 0.0f;
//				if(modulation == MODULATION_AM)
//				{
//					wRadioDevice::User * pUser = tr_transmitter->getUser();
//					if(pUser == NULL)
//					{
//						continue;
//					}
//					unsigned char transmitter_modulation = pUser->getModulation();
//					if(transmitter_modulation == MODULATION_AM)
//					{
//						double signalPower = itT->second * itT->second;
//						double signalPower2 = signalPower * signalPower;
//						volume = sum_power > 0.0f ? signalPower2 / audible_signal_power : 0.0f;
//					}
//					else
//					{
//						volume = 0.0f;
//					}
//					volume *= signal_volume;
//					SignalInfo signal_info = build_signal_info(tr_transmitter, volume);
//					if(signal_info.type != SIGNAL_TYPE_VOID)
//					{
//						signals_info.push_back(signal_info);
//					}
//				}
//				//
//				if(itT->second > max_signal_power)
//				{
//					max_signal_power = itT->second;
//					max_signal_source = tr_transmitter;
//				}
//				if(itT->second > noise_power)
//				{
//					over_noise_signals_qty++;
//				}
//			}
//			//взаимовлияние сигналов и сопутствующие эффекты
//			if(	max_signal_source &&
//				over_noise_signals_qty > 0)
//			{
//				//при FM слышен один самый мощный
//				if(modulation == SIGNAL_VOICE_FM)
//				{
//					float volume = max_signal_source->getUser()->getModulation() == SIGNAL_VOICE_FM ? 1.0f : 0.1f;
//					volume *= signal_volume;
//					SignalInfo signal_info = build_signal_info(max_signal_source, volume);
//					signals_info.push_back(signal_info);
//				}
//				//второй по силе источник сигнала
//				if(over_noise_signals_qty > 1)
//				{
//					double max_signal_power2 = 0.0f;
//					wRadioTransmitter * max_signal_source2 = 0;
//
//					for(wRadioReceiver::Transmitters::const_iterator itT = transmitters.begin();
//						itT != transmitters.end();
//						itT++)
//					{					
//						wRadioTransmitter *tr_transmitter = itT->first;
//						if(!tr_transmitter) continue;
//						unsigned char transmitter_modulation = tr_transmitter->getUser()->getModulation();
//						if(	transmitter_modulation != MODULATION_AM &&
//							transmitter_modulation != SIGNAL_VOICE_FM)
//						{
//							continue;
//						}
//						if(max_signal_source == tr_transmitter) 
//						{
//							continue;
//						}
//						if(itT->second < noise_power)
//						{
//							continue;
//						}
//						if(max_signal_power2 < itT->second)
//						{
//							max_signal_power2 = itT->second;
//							max_signal_source2 = tr_transmitter;
//						}
//					}		
//					if(max_signal_source2)
//					{
//						if(modulation == SIGNAL_VOICE_FM)
//						{
//							if(max_signal_power > 10.0 * max_signal_power2)
//							{
//								interference_volume =  0.0f;
//							}
//							else
//							{
//								interference_volume =  1.0f;
//								signals_info.clear();
//							}
//						}
//						else if(modulation == MODULATION_AM)
//						{
//							interference_volume =	fabs(1 - (max_signal_power - max_signal_power2) / max_signal_power) / 0.9f *
//														max_signal_power / audible_signal_power;
//						}
//						//частота тона биений(АМ) / помех интермодуляции(ЧМ)
//						interference_frequency = fabs(max_signal_source2->getFrequency() - max_signal_source->getFrequency());
//					}
//				}
//			}
//		}
//	}
//	//АРУ, работает постоянно
//	volume_factor = 1.0f;
//	double Pni = pRadioReceiver->getSensitivity() * 0.1f;
//	double P_in_min = Pni;
//	double P_in = sum_power;
//	double P_out = P_in * K;
//	double P_out_max = exp(log(10.0) * agr.signal_out_deviation / 10.0) * P_in_min;
//	K = agr.process(P_in, P_in_min, K, delta_t, VOLTAGE);
//	//if(modulation == MODULATION_AM)
//	{			
//		volume_factor = __min(P_out / P_out_max, 1.0f);	
//	}
//}
//
//void avReceiver::clear_volumes()
//{
//	noise_volume = 0.0f;
//	interference_volume = 0.0f;
//	interference_frequency = 0.0f;
////	morze_beep_volume = 0.0f;
////	telecode_volume = 0.0f;	
//	//speech_volume = 0.0f;
//	signals_info.clear();
//}

void avReceiver::postInitialize_()
{
	Sound::Host& headphonesHost = ccSound::instance().getHost("HEADPHONES");
	staticSources[STATIC_SOURCE_STATIC]			.create(&headphonesHost, "Aircrafts/Cockpits/Static");
	staticSources[STATIC_SOURCE_ATMOSPHERIC]	.create(&headphonesHost, "Aircrafts/Cockpits/Atmospheric");
	staticSources[STATIC_SOURCE_BEAT_TONE]		.create(&headphonesHost, "Aircrafts/Cockpits/BeatTone");
	staticSources[STATIC_SOURCE_INTERMODULATION].create(&headphonesHost, "Aircrafts/Cockpits/FMIntermodulation");
	messagesSource.create(&headphonesHost);

	switcher.setOnOff(false);	
	muter.setOnOff(true);	
	volume.setGain(1.0f);
#ifdef DEBUG_TEXT
	messagesSource.name = pRadioReceiver->getName() + " soundSource";
	switcher.name = pRadioReceiver->getName() + " switcher";
	muter.name = pRadioReceiver->getName() + " muter";
	volume.name = pRadioReceiver->getName() + " volume";
#endif
	switcher >> muter >> volume;
	for(int s = 0; s < STATIC_SOURCE_MAX; s++)
	{
		staticSources[s] >> switcher;
	}
	messagesSource >> switcher;
	c_get_communicator()->addMessageHandler(messageHandler.get());
}

void avReceiver::addStaticSoundSource(Sounder::Source& src)
{
   src >> switcher;
}

void avReceiver::updateSignalsPower()
{
	//АРУ
	double summ2HF	= pRadioReceiver->getSumE2();
	double summ		= sqrt(summ2HF);
	double agrK		= agc.getGain(summ);
	//ВЧ
	noiseHF			= pRadioReceiver->getNoiseLevel();
	signalHF		= sqrt(std::max(0.0, summ2HF - noiseHF * noiseHF));
	//НЧ
	//Детектирование
	if(pRadioReceiver->isActive())
	{
		signalLF		= (*detectors[modulation])(signalHF, noiseHF, noiseHF);
		noiseLF			= 1.0 - signalLF;
	}
	else
		signalLF = noiseLF = 0.0;
	signalLF *= agrK;
	noiseLF *= agrK;
	//НЧ. Шумодав
	double squelchK = squelch(signalLF, noiseLF);
	signalLF		*= squelchK;
	noiseLF			*= squelchK;
	lastUpdateTime = wTime::GetModelTime();
}

void avReceiver::updateMessages()
{
	//Громкость отдельных сообщений
	const wReceivingMessages & receivingMessages = c_get_communicator()->getReceivingMessages();
	for(wReceivingMessages::const_iterator itRM = receivingMessages.begin();
		itRM != receivingMessages.end();
		itRM++)
	{
		wMessage * pMessage = itRM->first.get();
		updateMessage(pMessage);
	}
	//Громкость шума
	staticSources[STATIC_SOURCE_STATIC].update(float(noiseLF));
	float frequency = getFrequency();
	float atmosphericGain =  frequency < 2000000.0f ? 1.0f : std::max(0.0f, (3000000.0f - frequency) / 1000000.0f);
	staticSources[STATIC_SOURCE_ATMOSPHERIC].update(float(noiseLF) * atmosphericGain);
}

void avReceiver::updateMessage(const wMessagePtr & message)
{
	if(message->pRadioTransmitter != NULL)
	{
		if(checkMessageSignal(message, getSignalByModulation(getModulation())))
		{
			float messageGain = signalHF > pRadioReceiver->getNoiseLevel() ? float(pRadioReceiver->getE(message->pRadioTransmitter) / signalHF * signalLF) : 0.0f;
			messagesSource.update(message.get(), messageGain);
		}
		else
			messagesSource.update(message.get(), 0.0f);
	}
}

void avReceiver::l_register_functions(lua_State *Lua_s,int index)
{
	lua_pushstring(Lua_s, "l_get_state");		lua_pushcfunction(Lua_s,l_get_state);		lua_settable  (Lua_s,index);
}

avReceiver* avReceiver::l_get_ptr(lua_State * L_state)
{
	lua_getfield(L_state,1,"link");
	void * ptr = lua_touserdata(L_state,-1);
	lua_pop(L_state, 1);
	Extractors::iterator itE = extractors.find(ptr);
	assert(itE != extractors.end());
	Extractor pExtractor = extractors[ptr];
	avReceiver * pReceiver = (*pExtractor)(L_state);
	return pReceiver;
}

int	avReceiver::l_get_state(lua_State *Lua_s)
{
	avReceiver * pReceiver = l_get_ptr(Lua_s);
	avReceiver::State state;
	if(pReceiver != NULL)
	{
		pReceiver->getState(state);
	}
	state.saveInState(Lua_s);
	return 1;
}

void avReceiver::getState(avReceiver::State & state)
{
	state.name		= 0;
	state.isOn		= pRadioReceiver->isOn();
	state.muted		= switcher.isOn() && muter.isOn();
	state.volume	= volume.getGain();
	if(pRadioReceiver != NULL)
	{
		state.frequency		= pRadioReceiver->getFrequency();
		state.modulation	= modulation;
		state.innerNoise	= pRadioReceiver->getNoiseLevel();
		state.pos			= aircraft().human_ptr->Position().p;
	}	
}

//-avCommunicator=-

avCommunicator::State::State() : avReceiver::State()
{
	power = 0.0f;
}

int avCommunicator::State::saveInState(lua_State* L) const
{
	int index = avReceiver::State::saveInState(L);
	
	lua_pushstring(L, "power");
	lua_pushnumber(L, power);
	lua_settable  (L, index);

	return index;
}

avCommunicator::avCommunicator() : avReceiver(), pRadioTransmitter(0),
	ownRadioTransmitter(false), transiver(NULL)
{

}

avCommunicator::~avCommunicator()
{
	assert(pRadioTransmitter == NULL);
}

void avCommunicator::initialize(const char * name_, Lua::Config & config, wRadioReceiver * pRadioReceiver_, wRadioTransmitter * pRadioTransmitter_)
{
	avReceiver::initialize(name_, config, pRadioReceiver_);

	assert(pRadioTransmitter == NULL);
	if(pRadioTransmitter_ != NULL)
	{
		pRadioTransmitter = pRadioTransmitter_;
		ownRadioTransmitter = false;
	}
	else
	{
		double power;
		bool hasPower = config.get("power", &power, 0.0);	
		float frequencyAccuracy;
		config.get("frequency_accuracy",&frequencyAccuracy, 0.0f);
		float bandWidth;
		config.get("band_width",		&bandWidth, 0.0f);

		pRadioTransmitter = new wRadioTransmitter();
		pRadioTransmitter->setName(name_);
		pRadioTransmitter->setBody(aircraft().human_ptr);
		if(hasPower)
			pRadioTransmitter->setPower(power);
		pRadioTransmitter->setFrequencyAccuracy(frequencyAccuracy);
		pRadioTransmitter->setBandWidth(bandWidth);
		ownRadioTransmitter = true;
	}
	assert(pRadioTransmitter != NULL && pRadioReceiver != NULL);
	transiver.reset(new wTransiver(*pRadioTransmitter, *pRadioReceiver, false));
	setOnOff(false);
}

void avCommunicator::postInitialize()
{
	avReceiver::postInitialize_();
	c_get_communicator()->getTransiverStorage().addTransiver(transiver.get());
}

void avCommunicator::release()
{
	wHumanCommunicator * humanCommunicator = c_get_communicator();
	if(transiver.get() != NULL)
	{
		if(	humanCommunicator->isTransmitting() &&
			humanCommunicator->getCurrentMessage()->pRadioTransmitter == transiver->getTransmitter())
		{
			humanCommunicator->finishTransmition(false);
		}
		if(humanCommunicator->getTransiverStorage().getCurrentTransiver() == transiver.get())
			sendNetMessage(false);
		humanCommunicator->getTransiverStorage().removeTransiver(transiver.get());
	}
	assert(pRadioTransmitter != NULL);
	if(pRadioTransmitter != NULL)
	{
		if(ownRadioTransmitter)
			delete pRadioTransmitter;
		pRadioTransmitter = NULL;
	}
	avReceiver::release();
}

void avCommunicator::setAsCurrent()
{
	c_get_communicator()->getTransiverStorage().setCurrentTransiver(transiver.get());
	sendNetMessage(true);
}

void avCommunicator::resetCurrent()
{
	c_get_communicator()->getTransiverStorage().setCurrentTransiver(NULL);	
	IwoLA * la = dynamic_cast<IwoLA*>(aircraft().human_ptr.Obj());
	la->sendVoiceCommTransmitter(NULL);
}

void avCommunicator::setAsTransiverForTunningFromOutside()
{
	c_get_communicator()->getTransiverStorage().setTransiverForTunningFromOutside(transiver.get());
}

void avCommunicator::setOnOff(bool on)
{
	setReceiverOnOff(on);
	setTransmitterOnOff(on);
}

void avCommunicator::setReceiverOnOff(bool on)
{
	avReceiver::setOnOff(on);
}

void avCommunicator::setTransmitterOnOff(bool on)
{
	transiver->setTransmitterOnOff(on);
}

void avCommunicator::setFrequency(float frequency)
{
	if(pRadioReceiver)
	{
		pRadioReceiver->setFrequency(frequency);
	}
	if(pRadioTransmitter)
	{
		pRadioTransmitter->setFrequency(frequency);
	}
}

void avCommunicator::setModulation(wModulation modulation_)
{
	avReceiver::setModulation(modulation_);
	transiver->setModulation(modulation_);
}

void avCommunicator::getState(avCommunicator::State & state)
{
	avReceiver::getState(state);
	if(pRadioTransmitter != NULL)
	{
		state.power = pRadioTransmitter->getPower();
	}
}

bool avCommunicator::isReceiverOn() const
{
	return avReceiver::isOn();
}

bool avCommunicator::isTransmitterOn() const
{
	return transiver->isTransmitterOn();
}

bool avCommunicator::isOn() const
{
	return isReceiverOn() && isTransmitterOn();
}

void avCommunicator::updateMessage(const wMessagePtr & message)
{
	if(message->pRadioTransmitter != NULL)
	{
		wRadioTransmitter * pSenderRadioTransmitter = message->pRadioTransmitter;
		if(pSenderRadioTransmitter == pRadioTransmitter)
		{
			messagesSource.update(message.get(), switcher.getResultGain()); //self listening
			return;
		}
	}
	avReceiver::updateMessage(message);
}

void avCommunicator::sendNetMessage(bool on)
{
	IwoLA * la = dynamic_cast<IwoLA*>(aircraft().human_ptr.Obj());
	la->sendVoiceCommTransmitter(on && isTransmitterOn() ? getTransiver() : NULL);
}

void avCommunicator::l_register_functions(lua_State *Lua_s,int index)
{
	lua_pushstring(Lua_s, "l_get_state");		lua_pushcfunction(Lua_s,l_get_state);		lua_settable  (Lua_s,index);
}

int	avCommunicator::l_get_state(lua_State *Lua_s)
{
	avCommunicator * pCommunicator = static_cast<avCommunicator*>(l_get_ptr(Lua_s));
    avCommunicator::State state;
	if(pCommunicator != NULL)
	{
		pCommunicator->getState(state);
	}
	state.saveInState(Lua_s);
	return 1;
}
