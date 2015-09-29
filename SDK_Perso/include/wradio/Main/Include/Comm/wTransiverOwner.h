#ifndef __wTransiverOwner__
#define __wTransiverOwner__

#include "Comm/wTransiver.h"
#include "Physic/wRadioAntenna.h"
#include "Physic/wRadioTransmitter.h"
#include "Physic/wRadioReceiver.h"

//Transiver stores radio transmitter and radio receiver within itself
class WRADIO_API wTransiverOwner : public wTransiver
{
public:
	wTransiverOwner(bool fullDuplexIn = false) : wTransiver(__radioTransmitter_,
															__radioReceiver_,
															fullDuplexIn)
	{
		setAntenna(&__radioAntenna_);
	}
private:
	wRadioAntenna 		__radioAntenna_;
	wRadioTransmitter 	__radioTransmitter_;
	wRadioReceiver 		__radioReceiver_;
};

#endif __wTransiverOwner__