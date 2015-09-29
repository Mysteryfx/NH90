#pragma once

#include "Avionics\ChaffFlareDispencers\avChaffFlareDispencer.h"

namespace cockpit
{
	
class COCKPITBASE_API avAN_ALE_40V : public avChaffFlareDispencer
{
public:
	avAN_ALE_40V() {}
	virtual ~avAN_ALE_40V();
	
	//avDevice interface
	virtual void release();
	virtual void serialize(Serializer &serializer) {}
	//end of interface

protected:

	virtual void InitContainersPayload();
	virtual void DropSnar(unsigned char Type, int Count = 2, char Param = -1);

private:
	
};

}
