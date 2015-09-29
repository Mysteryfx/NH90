#ifndef __UnsignedLongRandomizer_h__
#define __UnsignedLongRandomizer_h__

#include "Randomizer.h"

class UnsignedLongRandomizer : public Randomizer <unsigned int>
{
public:
    UnsignedLongRandomizer(unsigned int seed = 0)
		:Randomizer<unsigned int>(seed)
	{
	}

	unsigned int getRandom()
	{
		/* это быстрый но грязный алгоритм (закладывается на формат uint'a)
		return (theSeed = theSeed * 1664525L + 1013904223L);
		*///этот чистый но медленнее и длинна выборки меньше
		return (theSeed = (theSeed * 4096 + 150889) % 714025);
	}

};

#endif//__UnsignedLongRandomizer_h__