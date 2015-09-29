#pragma once

enum CarFormationType
{
	fNotDefined = 0,
	fColumn = 1,
	fRow,
	fWedge,
	fVee,
	fDiamond,
	fEchelonR,
	fEchelonL,
	fFree
};

enum LandGroupStateType
{
	gtNotDefined = 0,
	gtFollowPath = 1,
	gtArriveEndPos,
	gtArriveCustomForm,
	gtDisperse,
	gtStop,
	gtWait,
};

enum VehicleStateType
{
	vsNotDefined = 0,
	vsFollower = 1,
	vsLeader,
	vsArriveVector,
	vsArrivePosition,
	vsStop,
	vsAgony,
	vsWander
};
