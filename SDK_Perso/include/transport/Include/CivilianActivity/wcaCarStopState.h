#pragma once
#include "CivilianActivity/wcaCarState.h"

class wcaCarStopState: public wcaCarState
{
public:
	wcaCarStopState(wcaCar* host, const cPosition& _stopPos, double pathPar);
	virtual ~wcaCarStopState();

	cPosition &Position(wModelTime t);

	float ScalarVelocity() const;
	const cVector &VectorVelocity() const;
	const float Omy();
	double GetPathPar(wModelTime t) const;

	IntersectionType TestVolume(ClipVolume *volume);
	void Parse(Graphics::ModelParser& parser, const Position3& pos, Graphics::effectState* effects);
private:
	double _pathPar;
};