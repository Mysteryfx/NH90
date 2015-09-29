#ifndef __FlowRotorModel2_h__
#define __FlowRotorModel2_h__

#include "Base.h"

#include "FMDynamics/DynamicBody.h" 
#include "FMAerodynamics/PlaneAD.h"
#include "HelicoptersElements/IFlowRotorModel.h"

namespace EagleFM
{

class FlowRotorModel2 : public IFlowRotorModel
{
//------------------- МЕТОДЫ -------------------------------
public:

	struct AtmoData
	{
		double	rho;
		double	tempK;
		double	press;
		double	a_s;
	};

	struct PropellerData
	{
		double	diameter;
		int		bladeNumber;
		double	rotDirection;
		double	bladeMass;
		double	flapRegCoeff;	
		double	sinFlapRegCoeff;	
		double	cosFlapRegCoeff;	
		double	mu_pc[5];  //mu_pc[4] - пороговое значение, остальные - к-ты полинома
		Vec3	pos_l;
		Matrix3 mr;
		double	k_loss;
		double	wlim;

		double	tas;
		double	tas_k;

		double	pitch;

		double	controlX;
		double	controlZ;

		double	flapping;

		double	w;
		double	aoa;
		double	sin_aoa;
		double	cos_aoa;

		double	thrust;
		double	moment;
	};

	struct BladeElementData
	{
		double	radius;
		double	length;
		double	chord;

		double	pitchLocal;

		//double	pitch;
		double	ringS;
		double	elementS;
		double	aoa;
		double	cl;
		double	cd;
		double	v;
		double	w;
		double	v_old[2];
		double	w_old[2];

		double	intFactor;

		double	v_slip;

		double	mach;

		double	thrust;
		double	drag;
		double	moment;

		double	azimuthCurr;

		double	momentumTheoryThrust;
		double	momentumTheoryMoment;


		//TEMP!
		double cya;
		double cxa;
	};


	FMBASE_API struct Init
	{
		Init()
		{
		}
	};


	FMBASE_API FlowRotorModel2(DynamicBody*, PlaneAD*); 
	FMBASE_API virtual ~FlowRotorModel2();

	FMBASE_API virtual void		init(const FlowRotorModelInit&);
	virtual void				initMachCorr(double *machCorrM, double *machCorrK, int machCorrSize) {}
	virtual void				initThrustIAS_Corr(double thrustCorrMainK, double thrustCorrK, double thrustCorrOffset, double thrustCorrPower) {}

	virtual void				setSegmentsNum(int) {}
	virtual void				setIF(double*) {}
	FMBASE_API virtual void		setControl(double generalPitch, double xPitch, double zPitch, double rotateSpeed);

	FMBASE_API const virtual  Vec3*	getInductiveVelocity(double azimuth, bool param = true) const;
	virtual double				getSegmentsNum() const { return elementsNum; }
	virtual double				getThrust() const { return prop.thrust; }
	virtual double				getMu() const { return mu; }
	virtual double				getRotorAoA() const { return prop.aoa; }
	FMBASE_API 	virtual double	getVindAvr() const { return aver_v; }




protected:
	FMBASE_API	virtual void	doSimulate();

	void						momentumTheory(int i);
	void						elementTheory(int i);


public:
	
	PropellerData*				getPropellerData()			{ return &prop; }

	double						getF1()	const { return f1; }
	double						getF2() const { return f2; }

//------------------- ПОЛЯ -------------------------------

protected:
//не хозяин
	DynamicBody			*pDBody;			//сам вертолет
	PlaneAD				*pFanAD;			//аэродинамика лопасти

//хозяин
	int					elementsNum;		//кол-во колец винта (элементов лопасти в МЭЛ)
	
	BladeElementData*	elements;
	PropellerData		prop;
	AtmoData			atmo;

	Vec3*				vind;

	//для расчета зависимости инд.скорости от азимута:
	double				aver_v;
	double				mu;
	double				lambda;
	double				f1;
	double				f2;
	double				azimuthV;
};

}

#endif