#pragma once

#include "Inter.h"
#include "cPosition.h"
#include "dPosition.h"
#include "wTime.h"
#include <osg/Vec3d>
#include "../../edObjects/Include/Registry/Registered.h"

class lLandObject;

enum TVScreenCondition;

// Система управления видами
class IView
{
public:
	virtual ~IView() = 0 {}

	virtual bool            GetRightMirrorOn() = 0;
	virtual bool            GetLeftMirrorOn() = 0;
	virtual bool            GetTempCockpitOn() = 0;
	virtual bool            GetCockpitHUDOnly() = 0;

	virtual bool			Init(void) = 0;						// Инициализация
	virtual void			Finish(void) = 0;					// Деинициализация
	virtual void			Start(void) = 0;					// Перед стартом имитации
	virtual void			Stop(void) = 0;						// После завершения имитации
	virtual void			NextFrame(wModelTime next) = 0;		// Вычисление параметров следующего кадра
	virtual void			CommandSet(int) = 0;				// Передача команд управления
	virtual void			CommandSet(int, float) = 0;			// Передача аналоговых команд управления
	virtual dPosition*		CameraPositionGet(void) = 0;		// Возвращает положение текущей камеры
	virtual dVector			CameraPointGet(void) = 0;			// Точка текущей камеры в double precision
	virtual dVector*		CameraLocalPointGet(void) = 0;		// Точка камеры в локальных координатах объекта
	virtual dVector*		CameraVelocityGet() = 0;			// Возвращает вектор скорости текущей камеры
	virtual dPosition*		CameraPositionGet(int num) = 0;		// Возвращает положение заданной камеры
	virtual int				CameraTypeGet(int num) = 0;			// Возвращает тип заданной камеры
	virtual int				CameraTypeGet(void) = 0;			// Возвращает тип текущей камеры
	virtual bool			InCockpit() = 0;
	virtual bool			InGroundUnitCockpit() = 0;
	virtual bool			IsParachuteCamera() const = 0;
	virtual void			Quake(dPosition, unsigned int) = 0;// Сотрясение камеры от взрыва
	virtual double			PilotVAngle(void) = 0;				// Возвращает угол поворота головы пилота в вертикальной плоскости
	virtual double			PilotHAngle(void) = 0;				// Возвращает угол поворота головы пилота в горизонтальной плоскости
	virtual double			GetViewAngle(void) = 0;				// Получить горизонтальный раствор текущей камеры
	virtual double			GetMaxViewAngle(void) = 0;			// Получить максимальный горизонтальный раствор текущей камеры
	virtual void			HelmetMove(double, double,double delta_time = 0) = 0;		// Управление головой при падлоке
	virtual void			SetPadlock(bool, unsigned int id = 0) = 0;				// Сообщение камере о падлоке
	virtual void			LaunchNotify(unsigned int idWho, unsigned int idWhat) = 0;
	virtual void			HumanCrashNotify(unsigned int idHuman) = 0;
	virtual void			HumanEjectNotify(bool value, unsigned int chairId = 0) = 0;
	virtual void			PilotInObjectNotify(bool value, unsigned int objId) = 0;
	virtual unsigned int	ObjectIDGet() = 0;	// Идентификатор текущего объекта мира															
	virtual void			CarrierLanding() = 0;	// Клевок головой
	virtual bool			Started() = 0;
	virtual void			AutoChange() = 0;
	virtual void			SwitchToPlane(unsigned int id) = 0;	// Переключить текущую камеру на данный объект
	virtual void			Shake(double dh, double dv, double dAOV = 0.0) = 0;	// Трясти камеру (в кабине)
	virtual double			GetCockpitHAngleOrigin() = 0;
	virtual double			GetCockpitVAngleOrigin() = 0;
	virtual double          GetHAngleMax() = 0;
	virtual double          GetVAngleMax() = 0;
	virtual double          GetVAngleMin() = 0;
	virtual double			GetDefaultExternalViewAngle(void) = 0;				// Получить умалчиваемый горизонтальный раствор текущей камеры
	virtual void            SetStartingModelTime(wModelTime t) = 0;
	virtual void            onCreate(lLandObject *) = 0;
	virtual void            ResetCockpitViews() = 0;
	virtual TVScreenCondition GetTVScreenCondition() = 0;
    virtual bool            IsNightVisionOn() const = 0;
	virtual float           GetNightVisionGain() const = 0;
	virtual void            ResetCamera() = 0;
	virtual void            SetCameraPosition(dPosition *) = 0;
	virtual dVector         GetCockpitLocalPoint(const ed::string & unit_type, unsigned char player_slot = 0) = 0;
	virtual void            SetCockpitLocalPoint(const dVector & new_pnt,const ed::string & unit_type, unsigned char player_slot = 0) = 0;
	virtual int				getDebugKey(int key) const = 0;
    // Движение камеры зависит от того, находимся ли мы в паузе или нет
    // Управление этими режимами осуществляется через команду iCommandViewCameraMoveMode
    virtual bool            isInPauseMode() const = 0;
    virtual void            OnHumanBorn(unsigned int  human,bool arcade = false) = 0;
	virtual	bool			externalViewsOn() const = 0;
	
	//FIXME Костыль для отображения бинокля на виде F10
	//Наводчик должен быть объектом мира а не камерой
	virtual Position3		getBinocularPos() const = 0;
	virtual bool			binocularObjExists() const = 0;
	virtual void			setBinocularPos(const Position3&) = 0; 
	virtual void			hideBinocularIcon() = 0;

	virtual void		    UI_wall_orient(Position3 & wall_object_position)=0;
	virtual void            addCameraObjectToIgnore(ObjectID) = 0;
    virtual void            removeCameraObjectFromIgnore(ObjectID) = 0;
    virtual ObjectID        getCurrentCameraObjectId() = 0;};

extern INTER_API IView *globalView;
