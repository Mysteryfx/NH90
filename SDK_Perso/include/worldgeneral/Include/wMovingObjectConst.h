#pragma once

// Object classes identificators
const unsigned __int32	woClass_Object			= 1<< 0;	//1000;
const unsigned __int32	woClass_AirObject		= 1<< 1;	//1001;
const unsigned __int32	woClass_Plane			= 1<< 2;	//1002;
const unsigned __int32	woClass_HumanPlane		= 1<< 3;	//1003;
const unsigned __int32	woClass_AIPlane			= 1<< 4;	//1004;
const unsigned __int32	woClass_Shell			= 1<< 5;	//1005;
const unsigned __int32	woClass_Rocket			= 1<< 6;	//1006;
const unsigned __int32	woClass_Ship			= 1<< 7;	//1007;
const unsigned __int32	woClass_Snare			= 1<< 8;    //1008;
const unsigned __int32  woClass_Airdrome		= 1<< 9;	//1009;
const unsigned __int32  woClass_Bomb			= 1<<10;	//1010;
const unsigned __int32  woClass_Heliport		= 1<<11;	//1011;

const unsigned __int32  woClass_NURS			= 1<<13;	//1013;
const unsigned __int32  woClass_FreeFall		= 1<<14;	//1014;
const unsigned __int32  woClass_Vehicle			= 1<<15;	//1015;
const unsigned __int32  woClass_Point			= 1<<17;	//1017;
const unsigned __int32  woClass_Chair			= 1<<18;	//1018;


const unsigned __int32	woClass_RunWay			= 1<<28;
const unsigned __int32	woClass_AIHelicopter    = 1<<29;
const unsigned __int32	woClass_HumanHel	    = 1<<30;

const unsigned __int32	W_CLASS_AIRCRAFT = woClass_HumanPlane | woClass_HumanHel | woClass_AIPlane | woClass_AIHelicopter;

const unsigned __int32	W_CLASS_UNIT = W_CLASS_AIRCRAFT | woClass_Ship | woClass_Vehicle;

const unsigned __int32	W_CLASS_NURS	= woClass_NURS;

const unsigned __int32	W_CLASS_WEAPON	= woClass_Shell | woClass_Rocket | woClass_Bomb;

const unsigned __int32	W_CLASS_SAVE =
	woClass_Shell	|	woClass_Rocket	|	woClass_Point	|	woClass_Snare	|
	woClass_Airdrome|	woClass_Bomb	|	woClass_NURS	|	woClass_FreeFall;

const unsigned int wInvalidId = 0xFFFFFFFF;

const unsigned __int32	W_CLASS_MISSION_TARGETS	= W_CLASS_UNIT;

const unsigned __int32	W_CLASS_TARGETS	= W_CLASS_MISSION_TARGETS | woClass_Rocket;

const unsigned __int32	W_CLASS_HUMAN = woClass_HumanPlane | woClass_HumanHel;

const unsigned __int32	W_CLASS_AI_UNIT = woClass_AIPlane | woClass_AIHelicopter | woClass_Ship | woClass_Vehicle;

const unsigned __int32	W_CLASS_AIRBASE = woClass_Airdrome | woClass_Heliport | woClass_Ship;
