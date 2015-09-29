#ifndef __Direction_h__
#define __Direction_h__

#include "Base.h"
#include "FMMathematics/CoreMath.h"

namespace EagleFM
{
	// Вычисление углов азимута и места по заданному 3D вектору
	void FMBASE_API calcAzimuthAndElevationByVector3D(Vec3 Vector3D, double *Azimuth, double *Elevation);

	// Вычисление 3D вектора по заданным углам азимута и места
	Vec3 FMBASE_API calcVector3DByAzimuthAndElevation(double Azimuth, double Elevation, double VectorLength = 1.0);

	// Вычисление углов актаки и скольжения по заданному 3D вектору
	void FMBASE_API calcAnglesOfAttackAndSlipByVector3D(Vec3 Vector3D, double *AngleOfAttack, double *AngleOfSlip);

	// Вычисление 3D вектора по заданным углам актаки и скольжения
	Vec3 FMBASE_API calcVector3DByAnglesOfAttackAndSlip(double AngleOfAttack, double AngleOfSlip, double VectorLength = 1.0);

	// Вычисление пространственных углов азимута и атаки по заданному 3D вектору
	void FMBASE_API calcSolidAzimuthAndSolidAngleOfAttackByVector3D(Vec3 Vector3D, double *SolidAzimuth, double *SolidAngleOfAttack);

	// Вычисление 3D вектора по заданным пространственным углам азимута и актаки
	Vec3 FMBASE_API calcVector3DBySolidAzimuthAndSolidAngleOfAttack(double SolidAzimuth, double SolidAngleOfAttack, double VectorLength = 1.0);
}

#endif