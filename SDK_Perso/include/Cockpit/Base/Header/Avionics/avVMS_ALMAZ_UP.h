#ifndef _avVMS_ALMAZ_UP_h
#define _avVMS_ALMAZ_UP_h

#include "avVMS.h"

namespace cockpit
{
	
class COCKPITBASE_API avVMS_ALMAZ_UP : public avVMS
{
protected:
	virtual void SetCommand(int, float value = 0);
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);

protected:
	virtual void repeat() {}
};

}

#endif // _avVMS_ALMAZ_UP_h
