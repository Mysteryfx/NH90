#ifndef __GroundAiInit__
#define __GroundAiInit__

#include "Transport.h"
#include "wMovingObject.h"

#include "Registry/Registered.h"
#include <ed/map.h>
#include "Comm/wCommunicator.h"
#include "Comm/wTransiverOwner.h"


class wCommunicator;
struct VoiceCommNetState;
enum wModulation;
struct lua_State;

namespace AI {

void TRANSPORT_API groundAndNavalAiInit();

void registerTransportTaskStates();


class TRANSPORT_API VehicleCommunicator : public wCommunicator
{
public:
	VehicleCommunicator(wMessage::Object::Callsign callsign, woPointer pUnit, float frequency, wModulation modulation);
	virtual ~VehicleCommunicator();
	virtual void	setTransmitCommNetFlags(wCommNetFlags flags);
	virtual void	setReceiveCommNetFlags(wCommNetFlags flags);
	virtual void    finishTransmition(bool onTime);

    static float    getDefaultFrequency();
    static int      getDeafaultCallsign();

protected:
	virtual void startMessageTransmition(const wMessagePtr & message);
	virtual void processMessage(const wMessagePtr & message);
private:
    wTransiverOwner m_transiverOwner;
};


}

#endif __GroundAiInit__