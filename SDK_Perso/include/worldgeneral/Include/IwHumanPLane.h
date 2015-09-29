#pragma once

#include "WorldGeneral.h"
#include "Renderer/Camera.h"
#include "cLinear.h"
#include "dPosition.h"
#include "IwoLA.h"
#include "Renderer\DynamicParams.h"

class  IcClickable;
class  IwHumanHead;
class  IwHumanSensor;
class  IwHumanPayload;
struct Waypoint;
class  wTargetZonesList;
class  wDatalinkCommunicator;
class  IwHumanLWS;
class  IwHumanRWR;
class  IwHumanRoute;
class  IwHumanPlane_Autopilot;
class wCommunicator;
class Parameter;
class smCamera;

namespace intelliVIBE
{
	struct LockOn;
}
//struct  intelliVIBE::LockOn;

enum TVScreenCondition
	 {
	    TV_Blank = 0,
		TV_TV = 1,
		TV_IR = 2,
		TV_RADAR_AG =3, //изображение радиолокационной картины земли (для Су-39)
		MAP_ALT = 4, // карта местности c линиями уровня
		MAP_CLEAR = 5,//карта местности без линий уровня
		INDICATION_ONLY = 6, //только индикация без земли и мира,
        TV_IR_AS_OPTICAL,
		FROZEN_PICTURE
	};

enum wsMailType 
{
	wsMailNULL = 0,	            // 0 
	wsMailRadarModeScan,		// Облучение радаром в режиме Scan (поиск)
	wsMailRadarModeTWS,			// Облучение радаром в режиме Track While Scan (сопровождение во время поиска)
	wsMailRadarModeLock,		// Облучение радаром в режиме Lock (захват и наведение)
	wsMailRadarModeCR,			// Облучение радаром в режиме radio-command ракетой
    wsMailLaserRangeFinder,     // Облучение лазером в режиме дальномера.
    wsMailLaserIllumination,     // Облучение лазером в режиме подсветки
	wsMailLaserIlluminationPattern,
};

struct wGroundCrewCommand;

struct WORLDGENERAL_API NightVisionParams
{
    NightVisionParams():gain(0),goggles_type(0) {}
    float    gain;
    unsigned goggles_type;
};

struct WORLDGENERAL_API MirrorRenderParams
{
	MirrorRenderParams()
	{
		angle		= 45.0;
		aspect		= 3.0;
		near_clip   = 0.4f;
		middle_clip = 100.0f;
		far_clip    = 80000.0f;
	}
	dPosition pos;
	double    angle;
	double    aspect;

	float	  near_clip;
	float     middle_clip;
	float     far_clip;
};



class WORLDGENERAL_API IwHumanPlane 
{
public:
	virtual ~IwHumanPlane() = 0 {}
	virtual void  SetCockpitArgument(int index, float value) = 0;
	virtual float GetCockpitArgument(int index) = 0;
	virtual bool  GetFlagCockpit() = 0;
    virtual void  DrawCockpitUpdate() = 0;
	virtual void  DrawCockpit() = 0;
	virtual void  ParseCockpitForRender(Graphics::ModelParser &parser) = 0;
    virtual void  DrawScreenSpace() = 0;
    virtual bool  HaveArcade() = 0;
	virtual void  GetOveloadEffectValue(float & overload,float & distortion) = 0;
	virtual void  GetOxygenStarvEffectValue(float & effect1,float & effect2) = 0;
	virtual bool  onRenderingForegroudClipRegion(smCamera * cam) {	return false;	};

    virtual bool  GetNightVision(NightVisionParams & out_params)=0;

	virtual Graphics::Camera  GetTVCamera(int n) = 0;
	virtual dVector			  GetTVCameraPoint(int n) = 0;
	virtual TVScreenCondition GetTVScreenCondition(int n) = 0;
	virtual void              GetTVScreenParams(int n,Graphics::DParamList & args) = 0;
	virtual int               GetTVCount() = 0;
    virtual bool              GetTVRenderAlways(int n) const = 0;
	virtual const MirrorRenderParams * GetMirrorsCamera() = 0;

	virtual void  SetCommand(int) = 0;
	virtual void  SetCommand(int, float) = 0;
	virtual void  FarewellMessage() = 0;
	virtual float RudLeft() = 0;
	virtual float RudRight() = 0;
	virtual void			Hand_factor (float *FactorX, float *FactorY) = 0;
	virtual E2::Vector		Hand_neutral (void) = 0;
	virtual void			Hand_Friction_X (float *amp, float *freq) = 0;
	virtual void			Hand_Friction_Y (float *amp, float *freq) = 0;
	virtual bool IsLocalUser()		const = 0;
	virtual bool IsUncontrolled()	const = 0;
	virtual bool PilotIsDead()		const = 0;
	virtual void  RenderDevice(int n)=0;

	virtual Waypoint *          MissionRoute()=0;   // Указатель на маршрут
	virtual unsigned int        MissionDep_Adr_ID()=0; // ID аэродрома вылета
	virtual unsigned int        MissionArr_Adr_ID()=0; // ID аэродрома прибытия
	
    virtual IwHumanHead*             getHead()=0;
	virtual IwHumanSensor*           getSensorInterface()= 0;
	virtual IwHumanPayload*		     getPayloadInterface() = 0;
	virtual void					 setDatalinkInterface(wDatalinkCommunicator * datalinkCommunicatorIn)= 0;
	virtual IwHumanLWS*			     getLWSInterface() = 0;
	virtual IwHumanRWR*			     getRWRInterface() = 0;
	virtual IwoLA*                   getInterfaceToWoLA() = 0;
	virtual IwHumanRoute*            getRouteInterface() = 0;
	virtual IwHumanPlane_Autopilot * getAutopilotInterface() = 0;
	virtual wCommunicator *			 getCommunicator() = 0;
    virtual void					 update_padlock() = 0;
	// комплекс обороны - пассивные средства РЭБ
	virtual void Create_Chaff(int count = 1, char param = -1) = 0;
	virtual void Create_Flare(int count = 2, char param = -1) = 0;

	virtual void Create_Chaff_Human(int count = 1, char param = -1) = 0;
	virtual void Create_Flare_Human(int count = 2, char param = -1) = 0;

	// для определения самолет это или вертолет
	virtual bool Is_Helicopter() = 0;

	//задержанные команды
	virtual bool onGroundCrewCommand(const wGroundCrewCommand & command, Parameter * parameters, int stage = 0) = 0;

	virtual void  intelliVIBE_support_control(intelliVIBE::LockOn & ass_kicker) = 0;
	
	virtual void  setAirBrake(bool Pos) = 0;

	virtual void  setEjectionArmed(bool armed)   {}
	virtual void  setExtendLadderReleased(bool released) {}

	virtual void  setDamagedFlag(bool damaged)	 {}

	virtual void  raiseFailureEvent(const ed::string& s_failureID,
								    const ed::string& s_failureDisplayName)= 0;


	virtual void Kill_Recover() = 0;
};

extern WORLDGENERAL_API IwHumanPlane *globalHumanPlane;
