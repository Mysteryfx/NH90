//                    file wNetObject.h
#pragma once

#include "WorldGeneral.h"
#include "wTime.h"
#include "cPosition.h"
#include "Vector.h"
#include "Quaternion.h"

class MovingObject;

class WORLDGENERAL_API wNetObject
{
public:

	wNetObject();

	void init(wModelTime time, const cPosition& pos);

	void		setPos(wModelTime time, const cPosition& pos);
	void		getPos(wModelTime time, cPosition& outPos, cVector* outVel=0, cVector* outRot=0);
	dVector		getPos(wModelTime time);

	void serialize(Serializer &);

private:
	struct Update {
		wModelTime time;
		Vector3 pos;
		Quaternion rot;
	};
	Update  update1, update2; // fucking C++ disallows arrays with non-compiler generated contructors
	Update* update[2];

	int state;
	bool isStaying;
};
