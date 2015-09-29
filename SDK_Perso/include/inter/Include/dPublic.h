// dPublic.h

#pragma once

// Return values of cSubSystem::Init()
typedef enum dInit_e 
{
	dInitOK,
	dInitContinue,
	dInitFail
} ;

// Enum for dDispatcher::mode and modeAfterStop
typedef enum dMode_e
{
    dModeInit,             // Initialization
    dModeUser,			   // User interface (idle loop)
	dModeNetUser,		   // Net User interface (idle loop)
    dModeStart,            // Start
    dModeWork,             // Playing (maybe along with recording AVI)
    dModeFinish            // Finish
} ;

// Enum for dDispatcher::sub_mode
typedef enum dSubMode_e
{
	dSubMode0  =  0,
	dSubMode1  =  1,
	dSubMode2  =  2,
	dSubMode3  =  3,
	dSubMode4  =  4,
	dSubMode5  =  5, // encyclopedia
	dSubMode6  =  6,
	dSubMode7  =  7,
	dSubMode8  =  8,
	dSubMode9  =  9,
	dSubMode10 = 10,
	dSubMode11 = 11,
	dSubMode12 = 12, // campaign
	dSubMode13 = 13,
	dSubMode14 = 14, // training
	dSubMode15 = 15, // new mission
	dSubMode16 = 16,
	dSubMode17 = 17,
	dSubMode18 = 18, // open mission
	dSubMode19 = 19, // start mission using buttons "Fly"
	dSubMode20 = 20,
	dSubMode21 = 21,
	dSubMode22 = 22, // FBP
	dSubMode23 = 23, // Pilot Log Book
	dSubMode24 = 24, // Options
	dSubMode33 = 33,
	dSubMode35 = 35,
	dSubMode36 = 36  // Play Demo
} ;
