#ifndef _RANDOM_H_
#define	_RANDOM_H_

class Random
{
public:
	Random() : seed(0) {}	// initialize the seed from the system clock

	// functor to return random number between 0 and 1 and update seed:
	float operator() ()
	{
		seed = (seed * A + C) % M;
		return (float)seed / (float)M;
	}

	inline void setSeed(unsigned int Iseed) { seed = Iseed; }

private:
	enum consts
	{
		A = 9301,
		C = 49297,
		M = 233280
	};

	//const unsigned int A, C, M;
	unsigned int seed;
};
  
//const unsigned int Random::A = 9301;
//const unsigned int Random::C = 49297;
//const unsigned int Random::M = 233280;

#endif
