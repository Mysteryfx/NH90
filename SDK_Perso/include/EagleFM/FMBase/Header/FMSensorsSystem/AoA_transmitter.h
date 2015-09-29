#pragma once

#include "Base.h"
#include "FMMathematics/math.h"

namespace EagleFM
{

class DynamicBody;

//	Модель щелевого датчика УА и УС. Основана на учете разницы давлений между дин.давлениями в разных векторах.
//	Используются три вектора: осевой и два симметрично развернутых относительно первого на заданный угол.
//	По умолчанию осевой вектор направлен по оси Х ЛА, другие развернуты на 20 град. вверх и вниз (на УА).
//	Для использования в расчете УС вызвать initAoS(double Angle).

class FMBASE_API AoA_transmitter
{
public:
	AoA_transmitter(DynamicBody*);

	void			setAxisVector(Vec3 Vn) { /*Vn0 = Vn;*/ } //Under construction (TODO: учесть в Vn1 и Vn2)
	void			initAoA(double Angle = 20*Deg_to_Rad);
	void			initAoS(double Angle = 20*Deg_to_Rad);

	void			setVind_l(Vec3 Vind) { Vind_l = Vind; }
	
	void			simulate(double dt);
	double			getAngle() const { return Angle; }


	Vec3			Pos_l;			

	Vec3			SurfacePoint_w;		//точка на поверхности под ЛА (для коррекции ветра)
	Vec3			SurfaceNormal_w;	//нормаль к точке на поверхности

private:
	DynamicBody		*pDBody;

	Vec3			Vn0;	//тройка направленных векторов: - осевой
	Vec3			Vn1;	//вниз (вправо)
	Vec3			Vn2;	//вверх (влево)

	double			Angle;	//расчитанный угол

	Vec3			Vind_l;	//вектор индуктивной скорости
};


}