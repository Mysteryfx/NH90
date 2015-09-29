#ifndef __RANDOM_H__
#define __RANDOM_H__

#include "_config.h"

#ifndef ED_CORE_EXPORT
#define ED_CORE_EXPORT EDSIM_DLL_IMPORT
#endif // ED_CORE_EXPORT

namespace ed
{

class ED_CORE_EXPORT Random
{
private:
#define RAND_N 624
	unsigned long mt[RAND_N]; /* the array for the state vector  */
	int mti; /* mti==RAND_N+1 means mt[RAND_N] is not initialized */

public:
	Random();
	Random(unsigned long int seed);
	Random(int keys[], int length);
	~Random();

public:
	/* generates a random number on [0,0xffffffff]-interval */
	unsigned long int genInt32();

	/* generates a random number on [0,0x7fffffff]-interval */
	long int genInt31();

	/* generates a random number on [0,1]-real-interval */
	double genReal();

	/* generate integer random number on [0, range) int interval */
	int genInt(int range);

	/* generate random double in range [0, max) */
	double genDouble(double maxv)
	{
		return genReal() * maxv;
	}

	/* generate random double in range [min, max) */
	double genDouble(double minv, double maxv)
	{
		return minv + genReal() * (maxv - minv);
	}

	/* generate int in range [min, max] */
	int genInt(int minv, int maxv)
	{
		return minv + genInt(maxv - minv + 1);
	}

private:
	void initLong(unsigned long int s);
};

}


#endif

