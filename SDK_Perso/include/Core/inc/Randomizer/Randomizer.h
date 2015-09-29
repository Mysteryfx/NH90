#ifndef __Randomizer_h__
#define __Randomizer_h__

#include <math.h>
#include "GenerativeRandomizer.h"

template <class ValueType = float> class Randomizer : public GenerativeRandomizer
{
public:
    Randomizer(unsigned int seed = 0)
	{
        setSeed(seed);
    }
    virtual ~Randomizer() {}

    virtual ValueType getRandom() = 0; //Generates next random value of the sequence associated with this object.
};

#endif//__Randomizer_h__

