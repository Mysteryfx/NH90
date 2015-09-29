#ifndef Modeler_AnimHolder_H
#define Modeler_AnimHolder_H

#include "graphicsxp.h"
#include "Renderer\DynamicParams.h"

namespace Graphics
{

struct AnimHolderList;

// Holds phase animations of individual parameters in DynamicParam set
class AnimHolder
{
public:
	// place instead of 'start' to begin with current value
	// place instead of 'stop' to make animation infinite
	GRAPHICSXP_API const static double CURRENT;

	DParamList params;

	GRAPHICSXP_API AnimHolder(int initSize = 0, double startTime = 0.0);
	GRAPHICSXP_API ~AnimHolder();

	// setup new animation for argument 'arg'
	// from 'start' to 'stop' with 'speed' units per second
	void GRAPHICSXP_API Play(int arg, double start, double stop, double speed);

	// set argument to the value (discarding any animation on it)
	void GRAPHICSXP_API Set(int arg, double value);

	// advance all animations
	void GRAPHICSXP_API AdvanceTo(double t);

protected:
	double time_;
	AnimHolderList *first_;
};

} // namespace Graphics

#endif // Modeler_AnimHolder_H
