#ifndef __ControlSurfaceBody_h__
#define __ControlSurfaceBody_h__


#include "FMEngineeringStructure/ADElement.h"

namespace EagleFM
{

//Динамика управляещей поверхности для расчета шарнирных моментов

class FMBASE_API ControlSurfaceBody : public ADElement
{

public:
			// МЕТОДЫ

	ControlSurfaceBody(DynamicBody *DBody, AerodynamicBody *ADBody);
	virtual ~ControlSurfaceBody() {}

	void			initAxisType(Vec3 BasisP_l, Vec3 SelfP_l_l, double Mass, Vec3 MassP_l_l, 
					Vec3 Hin_l_l = Vec3(0.0, 0.0, 1.0)) 
						{ TrackType = false; BasisPos_l = BasisP_l; SelfPos_l_l = SelfP_l_l;  selfDState.mass = Mass; 
							selfDState.mass_pos = MassP_l_l; Hinge_l_l = Hin_l_l; }

	void			initTrackType(Vec3 BasisP_l, Vec3 SelfP_l_l, double Mass, Vec3 MassP_l_l, 
					Vec3 TrackD_l = Vec3(1.0, 0.0, 0.0), Vec3 Hin_l_l = Vec3(0.0, 0.0, 1.0)) 
						{ TrackType = true; BasisPos_l = BasisP_l; SelfPos_l_l = SelfP_l_l;  selfDState.mass = Mass; 
							TrackDir_l = TrackD_l; selfDState.mass_pos = MassP_l_l; Hinge_l_l = Hin_l_l; }

	void			setBasisAngles(float Yaw, float Pitch, float Roll);
	void			setSelfAngle(float Angle);
	void			setOmega(double Om) { Omega = Om; }
	
	void			calcDynamic();

	double			getAxisMoment()			const { return AxisMoment; }
	double			getTrackForce()			const { return TrackForce; }

			// ПОЛЯ
			
protected:
	//	не хозяин:
	DynamicBody		*pDBody;				


	//	хозяин:
	bool			TrackType;				// true - направляющий путь - расчет силы, false - ось вращения - расчет момента

	double			AxisMoment;				// скаляр момента на оси вращения
	double			TrackForce;				// скаляр силы вдоль направляющего пути

	Matrix3			BasisMtr;				// СК базиса (центра оси вращения)
	Matrix3			SelfMtr;

	Vec3			BasisPos_l;
	Vec3			SelfPos_l_l;

	Vec3			MassPos_l;

	Vec3			Hinge_l_l;				// ось вращения
	Vec3			TrackDir_l;				// вектор направляющего пути
	double			TrackAngleK;			// к-т поворота направляющего пути с поворотом элемента

	double			Omega;

	//КЭШ
	float			BasisYaw;
	float			BasisPitch;
	float			BasisRoll;
	float			Angle;
	bool			MtrChanged;
};

}

#endif