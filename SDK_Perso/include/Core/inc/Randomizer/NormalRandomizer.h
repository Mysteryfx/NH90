#ifndef __NormalRandomizer_h__
#define __NormalRandomizer_h__

#include "Randomizer.h"
#include "RandomizerManager.h"

template <class ValueType = float> class NormalRandomizer : public Randomizer<ValueType>
{
	ValueType theOldValue;
	bool useOldValue;
public:

    NormalRandomizer()
		: useOldValue(false)
	{
		setSeed(globalRandomizerManager->nextGenerativeSeed());
		globalRandomizerManager->add(this);
	}
	~NormalRandomizer()
	{
		globalRandomizerManager->remove(this);
	}

	//тут только чистый алгоритм, как сделать грязный см. UniformRandomizer
	ValueType getRandom()
	{
		if (useOldValue)
		{
			useOldValue =false;
			return theOldValue;
		}
		float random1, random2, modul;
		do
		{
			theSeed =(theSeed * 4096 + 150889) % 714025;
			random1 =(float)theSeed / (float)714025;
			random1 = random1 *2 - 1;
			theSeed =(theSeed * 4096 + 150889) % 714025;
			random2 =(float)theSeed / (float)714025;
			random2 = random2 *2 - 1;
			modul = random1 * random1 + random2 * random2;
		}
		while (modul >= 1);
		random1 = (random1 * sqrt(-2 * log(modul)/modul));
		random2 = (random2 * sqrt(-2 * log(modul)/modul));
		useOldValue = true;
		theOldValue = random2;
		return random1;
	}
};
#endif//__NormalRandomizer_h__