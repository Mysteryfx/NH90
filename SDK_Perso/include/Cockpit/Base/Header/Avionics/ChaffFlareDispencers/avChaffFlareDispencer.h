#pragma once

#include "Avionics\avDevice.h"
#include "Avionics\avBasicElectric.h"
#include "Avionics\avBreakable.h"
#include "Avionics\ChaffFlareDispencers\avChaffFlareContainer.h"

namespace cockpit
{

// program of dispensing
class COCKPITBASE_API avDispenseProgram
{
public:
	avDispenseProgram(bool isOpr = false);
	virtual ~avDispenseProgram() {}

	void Copy(const avDispenseProgram& p);

	bool getIsOperable() const {return isOperable;}
	unsigned getChaff()  const {return chaff;}
	unsigned getFlare()  const {return flare;}
	unsigned getCycle()  const {return cycle;}
	float    getIntv()   const {return intv;}

	void setChaff(unsigned n) {chaff = n;}
	void setFlare(unsigned n) {flare = n;}
	
	void setCycle(unsigned n);
	void setIntv(float n);

	void setParams(unsigned _chaff, unsigned _flare, float _intv, unsigned _cycle)
	{
		chaff = _chaff; flare = _flare; intv = _intv; cycle = _cycle; isOperable = true;
	}

private:

	static const float    MIN_INTV; 
	static const float    MAX_INTV;
	static const unsigned MIN_CYCLE = 1;
	static const unsigned MAX_CYCLE = 99;

	bool     isOperable; // is program operable
	unsigned chaff;      // the number of chaff bundles that will be released in the current program
	unsigned flare;      // the number of flares that will be released in the current program
	float    intv;       // the time between counter measure releases in the current program [0.25 sec step]
	unsigned cycle;      // the number of times the program will repeat itself [1..99]
};



typedef ed::map<char, avDispenseProgram*> DispensePrograms;


class COCKPITBASE_API avChaffFlareDispencer :	public avDevice,
												public avBreakable, 
												public avBasicElectric
{
public:
	avChaffFlareDispencer();
	virtual ~avChaffFlareDispencer();
	
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void serialize(Serializer &serializer) {}
	virtual void update();
	//end of interface
	
	void tryDispence(unsigned char Type, bool Continuous = false, bool Launching = false);
	void stopDispence();

	bool getIsDispensingFlag() const {return IsDispensingFlag;}
	bool getIsDispensingFlagChanged() const {return IsDispensingFlagChanged;}
	void setIsDispensingFlagChanged(bool FlagIn) {IsDispensingFlagChanged = FlagIn;}
	bool getIsContinuousDispence() const {return IsContinuousDispence;}

	virtual void setIsEmerFlaresJtsn(bool FlagIn) {IsEmerFlaresJtsn = FlagIn;}

	unsigned int getChaffCount();
	unsigned int getFlareCount();

	char               getCurrentProgramName() const {return CurrentProgram;}
	
	avDispenseProgram* getCurrentProgram() {return Programs[CurrentProgram];}
	void               setCurrentProgram(char letter) {CurrentProgram = letter;}

	void  incCurrentProgram();
	void  decCurrentProgram();


	unsigned int getFlareNumberPerCycle() const {return FlareCount;}
	unsigned int getChaffNumberPerCycle() const {return ChaffCount;}

protected:
	void processDrop(unsigned char Type, int Count = 2, char Param = -1);
	virtual bool checkIsDropPossible(unsigned char Type) {return true;}

	virtual void InitContainersPayload();
	virtual void DropSnar(unsigned char Type, int Count = 2, char Param = -1);

	char             CurrentProgram;   // current dispense program (A-Z)
    DispensePrograms Programs;         // set of programs

	ed::vector<avChaffFlareContainer> ChaffFlareContainer;

	int								   ContainerChaffCapacity;
	int								   ContainerFlareCapacity;
	int								   NumOfContainers;

private:
	bool IsDispensingFlag;
	bool IsDispensingFlagChanged;
	bool IsContinuousDispence;
	bool IsEmerFlaresJtsn;

	unsigned int CycleCounter; // counter of cycles
	unsigned int CycleMax;     // max number of cycles to perform
	wModelTime Interval;       // time interval between cycles
	wModelTime LastTimeOfDispensing;
	unsigned FlareCount, ChaffCount; // number of items per cycle

};

}
