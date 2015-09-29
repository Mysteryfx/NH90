//Рандомайзер

#ifndef __Randomizer_h__
#define __Randomizer_h__

#include "Base.h"
#include <math.h>
#include <stdint.h>

namespace EagleFM
{

class FMBASE_API Randomizer
{
public:
	Randomizer(); 
	void		init(uint32_t x);
	double		get() {	x = (uint32_t)fmod(double(a*x + c), (double)m); return x/(double)m; } 

private:
	uint32_t	x;
    
	uint32_t	a;
	uint32_t	c;
	uint32_t	m;



	static uint32_t	xLast; //значение х у последнего объекта после прокрутки в конструкторе (для начальной инициализации)
};

}

#endif