#pragma once

#include "cLinear.h"
#include "cPosition.h"
#include "wTime.h"

#include "Modeler/ModelConnector.h"

class woCar; // Почему так то?
class InfantryAnimator; 

class HumanPlayable
{
public:
	HumanPlayable(woCar* _host, InfantryAnimator* infAnim);
	~HumanPlayable();

	void update(double deltaTime);

	void setMoveDirection(float mD);
	void setTurnDirection(float tD);
	void setStrafeDirection(float sD);

	float getMoveDirection();
	float getTurnDirection();
	float getStrafeDirection();

	void setLastMoveDir(float lstMvDr);
	void setLastStrafeDir(float lstStrfDr);

	float getLastMoveDir();
	float getLastStrafeDir();
protected:
	
private:
	cVector updateMoveParams(double deltaTime);
	float getCenterOfMassHeight(); // Примерное нахождение центра массы, ну или центра бойца.

	dPosition getCameraViewPointDbg();

	woCar* host;

	double horizontalView;
	double verticalView;

	float moveDirection;
	float turnDirection;
	float strafeDirection;

	float lastMoveDir;
	float lastStrafeDir;

	cPosition currentPosition;
	dVector currentPositionPoint;
	cVector currentVectorVelocity;

	dVector oldPositionPoint;

	bool impact;

	Graphics::ModelConnector* centerMassConnector;
	float centerOfMassHeight;

	Graphics::ModelConnector* cameraConnector;

	InfantryAnimator* iA;
};