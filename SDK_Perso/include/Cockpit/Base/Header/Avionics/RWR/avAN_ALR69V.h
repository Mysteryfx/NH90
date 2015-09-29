#pragma once

#include "Avionics/RWR/avRWR.h"
#include "WorldSound.h"
namespace cockpit
{

enum ALR69_DeviceModes
{
	ALR69_MODE_OFF,
	ALR69_MODE_MAIN,
	ALR69_MODE_BIT
};

typedef ed::map<unsigned int, ed::string>			ThreatsSymbolsMap;
typedef ed::unordered_map<ed::string  , ed::string> ThreatsSymbolsMapString;


class COCKPITBASE_API avAN_ALR69V : public avRWR
{
public:
	avAN_ALR69V();
	virtual ~avAN_ALR69V();
	
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void release();
	virtual void serialize(Serializer &serializer) {}
	virtual void update();
    virtual void post_initialize();
	//end of interface

	const Device_Mode& getDevMode() const {return DevMode;}
	const char* getThreatSymbol(const RWR_Emitter & emitter);

	void setSoundVolume(double snd) {SoundVolume = snd;}
    void setDevMode(const Device_Mode& DevModeIn);

protected:
	virtual void controlSounds();
	void checkSoundsVolumes();

	wModelTime SearchSoundOnTime[16];
	wModelTime LaunchSoundDelay;
	wModelTime startBitTime_;

	wModelTime EmitterSoundTime;
	
	typedef ed::map<wsType, int> LockModeCounter;
	LockModeCounter emittersInLockModeCounter;
	double SoundVolume, PrevSoundVolume;
	bool LaunchSoundIsOn, VolumeIsChanged;
	
	static const Device_Mode modeOFF;
	static const Device_Mode modeMAIN;
	static const Device_Mode modeBIT;

	static const char* ThreatSymbolUnknown;

	Device_Mode DevMode;

	ThreatsSymbolsMap		 ThreatsSymbols;
	ThreatsSymbolsMapString	 ThreatsSymbolsStrings;

	Sound::Source	bit_test_sound_;
};

}
