#ifndef __IwaSun__
#define __IwaSun__

#include "Inter.h"

class INTER_API IwaSun
{
public:
	virtual float Get_Sun_Zenit(void) = 0;
};

extern INTER_API IwaSun * globalSun;

#endif __IwaSun__