#pragma once
#include "Flight.h"
#include "viObjectFlatShape.h"
#include "ClipVolumes/ClipVolumeBox.h"

// интерфейс объекта, создающего локальное возмущение атмосферы
class FLIGHT_API AtmosphericEffect : public viObjectFlatShape
{
public:
	~AtmosphericEffect();

	// дурки, возвращают дурь
	virtual cPosition &Position(wModelTime t);
	virtual const gBox &LocalBox(wModelTime t);

	virtual void getAirState(const dVector& p, cVector &v) = 0;

	ClipVolume *FlatShape();

	// в наследниках необходимо также перекрыть
	//virtual IntersectionType TestVolume(ClipVolume *volume);
	//virtual void serialize(Serializer &serializer);

protected:
	ClipVolumeBox box;
};
