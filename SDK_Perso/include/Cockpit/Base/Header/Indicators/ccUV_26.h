#ifndef _ccUV_26_h
#define _ccUV_26_h

#include "Indicators\ccIndicator.h"

namespace cockpit { 

class ccUV_26 : public ccIndicator
{
public:
	ccUV_26();
	virtual ~ccUV_26();
	
	virtual void create_elements_controllers_map();
	virtual void initialize(avDevice*, unsigned char, const ed::string&);
	static void  digits_control(Element*, ccDrawable*, const DParamList&);
};

} 

#endif // _ccUV_26_h
