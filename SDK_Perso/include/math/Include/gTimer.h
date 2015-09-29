//	gTimer.h ******************************************************************

#pragma once

#include "MathExport.h"

class MATH_API	gTimer
{
public:	
    gTimer(void);
	~gTimer(void){}

    void	clear();
    void	start();
    void	stop();
    double	result();

private:
    long long sumTicks_, startTick_;
    double factor_;
};

