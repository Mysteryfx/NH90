#ifdef INPUT_LUA_BINDING
  #undef _ACTIONS_H_
#endif

#ifndef _ACTIONS_H_
#define _ACTIONS_H_

#include "iCommand.h"

#ifndef INPUT_LUA_BINDING
  #undef REGISTER_COMMAND
  #define REGISTER_COMMAND(cmd) cmd,
  #undef REGISTER_COMMAND_EX
  #define REGISTER_COMMAND_EX(cmd, value) cmd = value,
#endif

#ifndef INPUT_LUA_BINDING
enum iCommandKey
{
#endif

    REGISTER_COMMAND_EX(iCommandKeyNull, 2000)
    REGISTER_COMMAND(iCommandPlanePitch)
    REGISTER_COMMAND(iCommandPlaneRoll)
    REGISTER_COMMAND(iCommandPlaneRudder)
    REGISTER_COMMAND(iCommandPlaneThrustCommon)
    REGISTER_COMMAND(iCommandPlaneThrustLeft)
    REGISTER_COMMAND(iCommandPlaneThrustRight)
    REGISTER_COMMAND(iCommandPlaneViewHorizontal)
    REGISTER_COMMAND(iCommandPlaneViewVertical)
    REGISTER_COMMAND(iCommandPlaneZoomView)
    REGISTER_COMMAND(iCommandViewHorizontalAbs) // =  2010
    REGISTER_COMMAND(iCommandViewVerticalAbs)
    REGISTER_COMMAND(iCommandViewZoomAbs)
    REGISTER_COMMAND(iCommandPlanePitchDelta)
    REGISTER_COMMAND(iCommandPlaneRollDelta)
    REGISTER_COMMAND(iCommandPlaneRudderDelta)
    REGISTER_COMMAND(iCommandPlaneThrustCommonDelta)
    REGISTER_COMMAND(iCommandPlaneThrustLeftDelta)
    REGISTER_COMMAND(iCommandPlaneThrustRightDelta)
	REGISTER_COMMAND(iCommandPlaneTrimPitch)
	REGISTER_COMMAND(iCommandPlaneTrimRoll) // == 2020
	REGISTER_COMMAND(iCommandPlaneTrimRudder)
	REGISTER_COMMAND(iCommandPlaneTrimPitchAbs)
	REGISTER_COMMAND(iCommandPlaneTrimRollAbs)
	REGISTER_COMMAND(iCommandPlaneTrimRudderAbs)
	REGISTER_COMMAND(iCommandPlaneRadarHorizontal)
	REGISTER_COMMAND(iCommandPlaneRadarVertical)
	REGISTER_COMMAND(iCommandPlaneRadarHorizontalAbs)
	REGISTER_COMMAND(iCommandPlaneRadarVerticalAbs)
	REGISTER_COMMAND(iCommandPlaneMFDZoom)
	REGISTER_COMMAND(iCommandPlaneMFDZoomAbs) // = 2030
	REGISTER_COMMAND(iCommandPlaneSelecterHorizontal)
	REGISTER_COMMAND(iCommandPlaneSelecterVertical)
	REGISTER_COMMAND(iCommandPlaneSelecterHorizontalAbs)
	REGISTER_COMMAND(iCommandPlaneSelecterVerticalAbs)
	REGISTER_COMMAND(iCommandPlaneFeedbackTrimmer)	// Сохранение состояния Feedback для треков (Inside Action)
	REGISTER_COMMAND(iCommandCockpitCursorHorizontal)	// Курсор движется относительно кабины для управления устройствами
	REGISTER_COMMAND(iCommandCockpitCursorVertical)
	REGISTER_COMMAND(iCommandCockpitCursorHorizontalAbs)
	REGISTER_COMMAND(iCommandCockpitCursorVerticalAbs)
	REGISTER_COMMAND(iCommandPlaneBase_Distance) //2040    // Галетник база - дальность (база цели )
	REGISTER_COMMAND(iCommandPlaneBase_DistanceAbs)
/* OBSOLETE */	REGISTER_COMMAND(iCommandViewAWACSMapMouseMove)
	REGISTER_COMMAND(iCommandViewCameraAltitude)		// Корректировка высоты камеры (для сходимости треков)
	REGISTER_COMMAND(iCommandViewAWACSObjectIdLow)
	REGISTER_COMMAND(iCommandViewAWACSObjectIdHigh)
	REGISTER_COMMAND(iCommandViewRoll)
	REGISTER_COMMAND(iCommandViewRollAbs)
	REGISTER_COMMAND(iCommandViewHorTrans)
	REGISTER_COMMAND(iCommandViewHorTransAbs)
	REGISTER_COMMAND(iCommandViewVertTrans) //2050
	REGISTER_COMMAND(iCommandViewVertTransAbs)
	REGISTER_COMMAND(iCommandViewLongitudeTrans)
	REGISTER_COMMAND(iCommandViewLongitudeTransAbs)
	REGISTER_COMMAND(iCommandViewAWACSDistanceToolDistance)
	REGISTER_COMMAND(iCommandViewAWACSDistanceToolAngle)
	REGISTER_COMMAND(iCommandViewAWACSMouseMoveX)
	REGISTER_COMMAND(iCommandViewAWACSMouseMoveY)
	REGISTER_COMMAND(iCommandViewAWACSLastZoomX1)
	REGISTER_COMMAND(iCommandViewAWACSLastZoomX2)
	REGISTER_COMMAND(iCommandViewAWACSLastZoomY1) //2060
	REGISTER_COMMAND(iCommandViewAWACSLastZoomY2)
	REGISTER_COMMAND(iCommandViewAWACSZoom100)
	REGISTER_COMMAND(iCommandViewAWACSZoomIn)
	REGISTER_COMMAND(iCommandViewAWACSZoomOut)
	REGISTER_COMMAND(iCommandViewAWACSDistance)
	REGISTER_COMMAND(iCommandViewAWACSLabels)
	REGISTER_COMMAND(iCommandViewAWACSDetectionAreas)
	REGISTER_COMMAND(iCommandViewAWACSDangerousAreas)
	REGISTER_COMMAND(iCommandViewAWACSCenterOnPlane)
	REGISTER_COMMAND(iCommandViewAWACSDistanceX1) //2070
	REGISTER_COMMAND(iCommandViewAWACSDistanceY1)
	REGISTER_COMMAND(iCommandViewAWACSDistanceX2)
	REGISTER_COMMAND(iCommandViewAWACSDistanceY2)
	REGISTER_COMMAND(iCommandCockpitVirtualAxis)   // виртуальная ось для управления элементами в кабине типа ось 
	REGISTER_COMMAND(iCommandCockpitVirtualAxisAbs) // виртуальная ось для управления элементами в кабине типа ось
	REGISTER_COMMAND(iCommandViewAWACSTargetPointerX)
	REGISTER_COMMAND(iCommandViewAWACSTargetPointerY)
	REGISTER_COMMAND(iCommandViewAWACSApplyTargetPointer)
	REGISTER_COMMAND(iCommandViewAWACSInfoTargetButton) //2080
	REGISTER_COMMAND(iCommandViewAWACSInfoCloseButton)
	REGISTER_COMMAND(iCommandViewAWACSTargetFireButton)
	REGISTER_COMMAND(iCommandViewAWACSTargetRemoveButton)
	REGISTER_COMMAND(iCommandViewAWACSTargetCloseButton)
	REGISTER_COMMAND(iCommandViewAWACSTargetStartTime)
	REGISTER_COMMAND(iCommandViewAWACSTargetUnits)
	REGISTER_COMMAND(iCommandViewAWACSTargetRounds)

	REGISTER_COMMAND(iCommandPlaneCollective) //Ручка ШАГ-ГАЗ

	//оси для динамических настроек 
	REGISTER_COMMAND(iCommandDebugAxis1)
	REGISTER_COMMAND(iCommandDebugAxis2)
	REGISTER_COMMAND(iCommandDebugAxis3)
	REGISTER_COMMAND(iCommandDebugAxis4)
	REGISTER_COMMAND(iCommandDebugAxis5)
	REGISTER_COMMAND(iCommandDebugAxis6)
	REGISTER_COMMAND(iCommandDebugAxis7)
	REGISTER_COMMAND(iCommandDebugAxis8)
	REGISTER_COMMAND(iCommandDebugAxis9)
	REGISTER_COMMAND(iCommandDebugAxis10)
	//ENGINE CONTROL ANALOG AXIS
	REGISTER_COMMAND(iCommandHelicopterLeftEngineBrake_ByAxis)
	REGISTER_COMMAND(iCommandHelicopterRightEngineBrake_ByAxis)
	REGISTER_COMMAND(iCommandHelicopterRotorEngineBrake_ByAxis)

	REGISTER_COMMAND(iCommandWheelBrake) //ось колесного тормоза
	
	REGISTER_COMMAND(iCommandViewAWACSSetColumnPath)
	REGISTER_COMMAND(iCommandViewAWACSCancelColumnPath)
	REGISTER_COMMAND(iCommandViewAWACSInfoPathButton)
	REGISTER_COMMAND(iCommandViewAWACSSetPathCloseButton)
	REGISTER_COMMAND(iCommandViewAWACSUpdateWOInfoLow)
	REGISTER_COMMAND(iCommandViewAWACSUpdateWOInfoHigh)

	REGISTER_COMMAND(iCommandViewAWACSVehControlCloseButton)
	REGISTER_COMMAND(iCommandViewAWACSSetColumnSpeed)
	REGISTER_COMMAND(iCommandViewAWACSChangeColumnState)
	REGISTER_COMMAND(iCommandViewAWACSChangeColumnForm)

	REGISTER_COMMAND(iCommandLeftWheelBrake) //ось тормоза левого колеса
	REGISTER_COMMAND(iCommandRightWheelBrake) //ось тормоза правого колеса

	REGISTER_COMMAND(iCommandViewAWACSTargetSetButton)
	REGISTER_COMMAND(iCommandRoleCoalitionUpdated)

	//Команды приходящие от диалога карты в ф10, нужны для воспроизведения трэков
	REGISTER_COMMAND(iCommandViewAWACSMapEventType)
	REGISTER_COMMAND(iCommandViewAWACSMapEventX)
	REGISTER_COMMAND(iCommandViewAWACSMapEventY)
	REGISTER_COMMAND(iCommandViewAWACSMapEventFlags)

	//Команды приходящие от клавиатуры в ф10, нужны для воспроизведения трэков
	REGISTER_COMMAND(iCommandViewAWACSKeyState)
	REGISTER_COMMAND(iCommandViewAWACSKeyKey)
	REGISTER_COMMAND(iCommandViewAWACSKeyData)

	//Команды приходящие от тулбара в ф10, нужны для воспроизведения трэков
	REGISTER_COMMAND(iCommandViewAWACSShowPathes)
	REGISTER_COMMAND(iCommandViewAWACSShowTargets)

	//Комманды от диалогов ф10
	REGISTER_COMMAND(iCommandViewAWACSRoadPriorChanged)
	REGISTER_COMMAND(iCommandViewAWACSBridgesPriorChanged)
	REGISTER_COMMAND(iCommandViewAWACSFireEnabledChanged)
	REGISTER_COMMAND(iCommandViewAWACSTargetXChanged)
	REGISTER_COMMAND(iCommandViewAWACSTargetZChanged)
	

    REGISTER_COMMAND(iCommand_TDC_AbsolutePositionHorizontal)
    REGISTER_COMMAND(iCommand_TDC_AbsolutePositionVertical)

    REGISTER_COMMAND(iCommandAdjustViewAngleToComfortValue)

	REGISTER_COMMAND(iCommandViewAWACSChangeROE)
	REGISTER_COMMAND(iCommandViewAWACSChangeAlarmState)
	REGISTER_COMMAND(iCommandViewAWACSTargetChangeRadius)
	REGISTER_COMMAND(iCommandViewAWACSTargetRadiusSet)

	REGISTER_COMMAND(iCommandViewLaserCodeChanged)
	REGISTER_COMMAND(iCommandViewAWACSSetColumnAltitude)
	REGISTER_COMMAND(iCommandViewAWACSShowHealths)
	REGISTER_COMMAND(iCommandViewAWACSInfoAddTargetButton)
	REGISTER_COMMAND(iCommandViewAWACSInfoDelTargetButton)
	
	REGISTER_COMMAND(iServiceInformAboutUserHAngle)
	REGISTER_COMMAND(iServiceInformAboutUserVAngle)

	REGISTER_COMMAND(iOculusRiftYaw)
	REGISTER_COMMAND(iOculusRiftPitch)
	REGISTER_COMMAND(iOculusRiftRoll)
	REGISTER_COMMAND(iOculusRiftHeadPosX)
	REGISTER_COMMAND(iOculusRiftHeadPosY)
	REGISTER_COMMAND(iOculusRiftHeadPosZ)


	//-insert here
	REGISTER_COMMAND(iCommandRoleSet)
	REGISTER_COMMAND(iCommandCoalitionSet)

    REGISTER_COMMAND(iCommandKeyMaximum)
//----------------------------------------
	REGISTER_COMMAND_EX(iCommandCockpitDeviceCommandsStart, 3000) //начальная точка команд кокпита (зарезервировано до iCommandCockpitDeviceCommandsEnd)
	REGISTER_COMMAND_EX(iCommandCockpitDeviceCommandsEnd, 3999) //конечная точка  команд кокпита (вставлять после !!!!)
//----------------------------------------
	REGISTER_COMMAND_EX(iCommandRepKeyBegin, 4000)		// Начальная точка для радио команд 

#ifndef INPUT_LUA_BINDING
};
#endif

#endif

#ifdef INPUT_LUA_BINDING
  #define _ACTIONS_H_
#endif