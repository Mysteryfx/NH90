#ifndef __wRadioAntenna__
#define __wRadioAntenna__

#include "wRadio.h"

#include "cLinear.h"

namespace Lua
{
	class Config;
}

namespace Radio
{
	class Antenna;
}

//Radio antenna has radiation pattern used to calculate the signal strength in the receiver.
//Both receivers and transmitters can have antennas.

class WRADIO_API wRadioAntenna
{
public:
	wRadioAntenna();
	~wRadioAntenna();
	void load(Lua::Config & config);
	bool empty() const;
	double getPolarization() const; //Returns polarization
	double getSqrG(const cVector & direction) const; //Returns sqrt(G), where G - directive gain
	operator Radio::Antenna * () const { return antenna_; }
private:
	wRadioAntenna(const wRadioAntenna & radioAntenna);
	wRadioAntenna & operator = (const wRadioAntenna & radioAntenna);
	Radio::Antenna * antenna_;
};



#endif