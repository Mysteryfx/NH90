#ifndef __GenerativeRandomizer_h__
#define __GenerativeRandomizer_h__

#include "_config.h"

class GenerativeRandomizer
{
protected:
    unsigned int theSeed; //Seed value for generator. Must be [0x00000000,0xffffffff]
	unsigned int theSeed0;

public:
	virtual ~GenerativeRandomizer() {}

    virtual unsigned int getSeed()
	{
        return theSeed;
	}
	virtual unsigned int getSeed0()
	{
		return theSeed0;
	}
    virtual void setSeed(unsigned int seed)
	{
        theSeed = seed;
		theSeed0 = theSeed;
	}
	virtual void resetSeed()
	{
		theSeed = theSeed0;
	}
};
#endif//__GenerativeRandomizer_h__