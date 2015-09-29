//Модель Yo-Yo для расчета вектора индуктивной скорости в области винта соосной схемы.

#ifndef __FlowCoaxialRotorModel_h__
#define __FlowCoaxialRotorModel_h__

#include "FlowRotorModel.h"

namespace EagleFM
{

class FlowCoaxialRotorModel : public FlowRotorModel
{
// ----------------------------- МЕТОДЫ ---------------------------------------
public:
	FlowCoaxialRotorModel(DynamicBody*, AerodynamicBody*); 
	virtual ~FlowCoaxialRotorModel();

	void					setK_Swirling(double K)		{ K_Swirling = K; }
	void					setK_UpForDnRing(double K)	{ K_UpForDnRing = K; }
	void					setUpRotorTwist(double T0, double T1 ) { UpRotorTwist0 = T0; UpRotorTwist1 = T1; }
	void					setDnRotorTwist(double T0, double T1 ) { DnRotorTwist0 = T0; DnRotorTwist1 = T1; }

	virtual void			setSegmentsNum(int); //разбить винт на N колец
	virtual void			setIF(double*); //массив IF лопастей

	void					setDiffPitch(double DPitch) { DiffPitch = DPitch; }

private:
	virtual void			doSimulate();
public:

	const virtual  Vec3*	getInductiveVelocity(double Azimuth, bool UpRotor) const;

private:
// ----------------------------- ПОЛЯ ---------------------------------------

	double				DiffPitch;

	Vec3				UpControlAxis;
	double				UpRotorAoA;
	double				sinUpRAoA;
	double				cosUpRAoA;

	Vec3				DnControlAxis;
	double				DnRotorAoA;
	double				sinDnRAoA;
	double				cosDnRAoA;

	double				*UpRingIF;			//средний Integrity Factor на кольце (массив по кольцам) ВВ

	double				*DnRingIF;			//средний Integrity Factor на кольце (массив по кольцам) НВ

//	Настроечные параметры
	double				UpRotorTwist0;
	double				UpRotorTwist1;

	double				DnRotorTwist0;
	double				DnRotorTwist1;

	double				K_Swirling;			//коэф-т закрутки индуктивного потока на нижнем винте
	double				K_UpForDnRing;		//коэф-т влияния верхнего винта на изолированную область нижнего в зависимости от количества колец


	//Понадобится в случае, если геометрия верхнего и нижнего винтов будет отличаться:
	/*double				*UpRingRmin;		//массив радиусов колец винта	
	double				*UpRingRmid;		
	double				*UpRingRmax;		
	
	double				*DnRingRmin;		//массив радиусов колец винта	
	double				*DnRingRmid;		
	double				*DnRingRmax;		
	*/

	//Vec3				*UpVind;			//ради чего вся это модель (массив скоростей по кольцам)
	//Vec3				*DnVind;			//ради чего вся это модель (массив скоростей по кольцам)


};

}


#endif

