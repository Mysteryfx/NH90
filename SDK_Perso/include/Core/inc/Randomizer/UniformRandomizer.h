#ifndef __UniformRandomizer_h__
#define __UniformRandomizer_h__

#include "Randomizer.h"
#include "RandomizerManager.h"

template <class ValueType = float> class UniformRandomizer : public Randomizer<ValueType>
{
protected:
    ValueType theMin; //Lower boundary value for generator
    ValueType theMax; //Upper boundary value for generator
public:
	UniformRandomizer(ValueType min = 0, ValueType max = 1)
	{
		assert(min <= max);
		theMin = min;
        theMax = max;
		setSeed(globalRandomizerManager->nextGenerativeSeed());
		globalRandomizerManager->add(this);
	}
	~UniformRandomizer()
	{
		globalRandomizerManager->remove(this);
	}
	ValueType getRandom()
	{
		/*  это быстрый но грязный алгоритм (закладывается на формат float'a)
			static unsigned int iran;
			unsigned int temp;
			float fran;
			 static unsigned int jflone=0x3f800000;
			 static unsigned int jflmsk=0x007fffff;
			iran=1664525L*iran+1013904223L;
			temp=jflone|(jflmsk&iran);
			fran=(*(float *)&temp)-1.F;
			return fran;
		*/
		// этот чистый но медленный
		theSeed =(theSeed * 4096 + 150889) % 714025;
		return (theMin + (theMax - theMin)*(float)theSeed / (float)714025);
	}

    void setMin(ValueType min)
	{
		assert(min <= theMax);
        theMin = min;
	}
    void setMax(ValueType max)
	{
		assert(theMin <= max);
        theMax = max;
	}
    ValueType getMin()
	{
		return theMin;
	}
    ValueType getMax() 
	{ 
		return theMax;
	}
    ValueType getM() //Gets mean value
	{ 
		return (theMax + theMin) / 2;
	} 
    ValueType getD() //Gets standard deviation value
	{
		return (theMax - theMin) / 2;
	}
};
#endif//__UniformRandomizer_h__