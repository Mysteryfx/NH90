#ifndef __Optimizable_h__
#define __Optimizable_h__

#include "Base.h"

// Inherits peuvent choisir leur propre terrain simulé indépendamment
// La démarche globale, qui est appelé simulation (double).
// Modélisation doit être décrit dans une fonction virtuelle doSimulate ().
class FMBASE_API Optimizable
{
public:
	Optimizable(double _dt = 0.02) : dt(_dt), callTime(0.0) {}
	virtual ~Optimizable() {}

	void	setModelingTime(double _dt) { dt = _dt; }

	void	simulate(double dTime)
	{
		callTime += dTime;
		while(callTime >= dt)
		{
			doSimulate();
			callTime -= dt;
		}
	}

	double	getModelingTime()	const { return dt; }
	double	getRemainderTime()	const { return callTime; }

protected:
	virtual void doSimulate() = 0;
	double dt; //étape de l'intégration interne
	
private:
	double callTime; //surcharge variable
};

#endif