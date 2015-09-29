#ifndef __RandomizerManager_h__
#define __RandomizerManager_h__

#include <ed/list.h>

#include "_config.h"

class GenerativeRandomizer;
class UnsignedLongRandomizer;
typedef ed::list<GenerativeRandomizer*> Randomizers;

class RandomizerManager
{
    Randomizers* theRandomizers;
	UnsignedLongRandomizer*	theGenerativeRandomizer;
public:
	ED_CORE_EXTERN RandomizerManager();
	ED_CORE_EXTERN ~RandomizerManager();

	ED_CORE_EXTERN void setGenerativeSeed(unsigned int seed);
	ED_CORE_EXTERN unsigned int nextGenerativeSeed();

	ED_CORE_EXTERN void add(GenerativeRandomizer* randomizer);
	ED_CORE_EXTERN void remove(GenerativeRandomizer* randomizer);
};

extern ED_CORE_EXTERN RandomizerManager *globalRandomizerManager;

#endif//__RandomizerManager_h__