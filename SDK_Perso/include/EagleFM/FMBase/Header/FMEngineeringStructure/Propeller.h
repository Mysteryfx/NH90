#ifndef __Propeller_h__
#define __Propeller_h__

#include "Base.h"
#include "FMMathematics/math.h"
#include "Optimizable.h"

namespace EagleFM
{

namespace BaseOptions
{
	class PropellerOptions;
}


class DynamicBody;
class PlaneAD;

//Модель самолетного винта

class Propeller : public Optimizable
{
public:
	struct AtmoData
	{
		double	rho;
		double	tempK;
		double	press;
		double	a_s;
	};

	struct Transversal
	{
		Vec3	force_l;
		Vec3	moment_l;
		double	coeff;
	};

	
	//struct BladeData
	//{
	//	int		elementsNum;
	//	double	moi;
	//};

	struct BladeElementData
	{
		double		radius;
		double		length;
		double		chord;

		double		pitchLocal;
		//double	pitch;
		double		ringS;
		double		elementS;

		double		aoa;
		double		cl;
		double		cd;
		double		v;
		double		w;

		double		v_old;
		double		w_old;

		double		v_slip;

		double		mach;

		double		thrust;
		double		drag;
		double		moment;

		double		azimuthCurr;

		double		momentumTheoryThrust;
		double		momentumTheoryMoment;

		PlaneAD*	ad;
	};

	struct PropellerData
	{
        PropellerData()
        {
		    diameter = 0;			
		    bladeNumber = 0;		
		    rotDirection = 0;		
		    moi = 0;

		    tas = 0;				
		    k_tas = 0;				

		    pitch = 0;				

		    w = 0;					
		    elevation = 0;			
		    azimuth = 0;	

		    sin_el = 0;
		    cos_el = 0;

		    thrust = 0;				
		    moment = 0;	

			momentAbs = 0.0;

		    momentCorrected = 0;

		    pos_l.set(0);			//координаты центра в СК ЛА
		    dir_l.set(0);			//вектор направления в СК ЛА (придумать инициализацию!)
		    strafe_l.set(0);		//рассчитывается!
		    up_l.set(0);			//рассчитывается!

		    thrust_l.set(0);			
		    moment_l.set(0);	
		    thrustDir_l.set(0);
		    transversal.force_l.set(0);
            transversal.moment_l.set(0);
			transversal.coeff = 1.0;   
		}
		
		double		diameter;			
		int			bladeNumber;		
		double		rotDirection;	
		double		moi;
		Vec3		pos_l;			//координаты центра в СК ЛА
		Vec3		dir_l;			//вектор направления в СК ЛА (придумать инициализацию!)
		Vec3		strafe_l;		//рассчитывается!
		Vec3		up_l;			//рассчитывается!

		double		tas;				
		double		k_tas;				

		double		pitch;				

		double		w;					
		double		elevation;			
		double		azimuth;	

		double		sin_el;
		double		cos_el;

		double		thrust;				
		double		moment;				

		double		momentAbs;

		double		momentCorrected;	

		Vec3		thrust_l;			
		Vec3		moment_l;	
		Vec3		thrustDir_l;
		Transversal	transversal;		
	};

	struct Slipstream
	{
        Slipstream()
        {
           elevation = 0;
           average_v = 0;
           average_w = 0;
           transientCoeff = 0; //to describe slipstream shrinking/acceleration
           dir_l.set(0);
           up_l.set(0);
           strafe_l.set(0);
           skewStrafe_l.set(0);		//орт перелома для учета скоса потока
        }
		double	elevation;
		double	average_v;
		double	average_w;
		double	transientCoeff; //to describe slipstream shrinking/acceleration
		Vec3	dir_l;
		Vec3	up_l;
		Vec3	strafe_l;
		Vec3	skewStrafe_l;		//орт перелома для учета скоса потока
	};

	struct SlipstreamPoint //обдув в точке
	{
		SlipstreamPoint()
		{
			v_coeff = w_coeff = 1.0;
			skew_angle = 0; 
			v = 0;			
			w = 0;
			p_l.set(0);		
			v_l.set(0);
			trans_v_l.set(0);
			vel_l.set(0); 
			p_skew_l.set(0);
			dir_skew_l = Vec3(-1.0, 0.0, 0.0);
			p_grskew_l.set(0);
			dir_grskew_l = Vec3(-1.0, 0.0, 0.0);
		}
		Vec3	p_l;		//данные
		double	v_coeff;	//к-т ускорения/замедления продольной составляющий скорости струи
		double	w_coeff;	//к-т ускорения/замедления поперечной составляющий скорости струи
		double  skew_angle; //угол перелома, если 0 - не надо
		
		double	v;			//выход
		double	w;
		Vec3	v_l;
		Vec3	trans_v_l;
		Vec3	vel_l; //full v
		Vec3	p_skew_l;
		Vec3	p_grskew_l;
		Vec3	dir_skew_l;
		Vec3	dir_grskew_l;
	};


//------------------- МЕТОДЫ -------------------------------

	FMBASE_API Propeller(DynamicBody* db, PlaneAD** ad, double dt = 0.1);
	FMBASE_API Propeller(DynamicBody* db, PlaneAD** ad, BaseOptions::PropellerOptions& options, double dt = 0.1);
	FMBASE_API virtual ~Propeller();

	FMBASE_API void		setSkew_x0(double x0) { skew_x0 = x0; }
	FMBASE_API void		setSkewH0(double h0) { groundSkewH0 = h0; }

	FMBASE_API void		setUnderlyingSurface(const Vec3 &p_w, const Vec3 &n_w)	{ underlyingSurf_p_w = p_w; underlyingSurf_n_w = n_w; }

protected:

	void				initialize(PlaneAD** ad, BaseOptions::PropellerOptions& options);

	virtual void		doSimulate();	

	double				calcVperp(int i) const;
	void				momentumTheory(int i);
	void				elementTheory(int i);

public:

	FMBASE_API void				calcSlipstreamVelocity(SlipstreamPoint&) const;


	FMBASE_API PropellerData*	getPropellerData()		{ return &prop; }
	FMBASE_API Slipstream*		getSlipstreamAverage()	{ return &slipstreamAverage; }

	double FMBASE_API get_N() const; // Power Wt

//------------------- ПОЛЯ -------------------------------

protected:
//не хозяин
	DynamicBody*				dbody;				//физика ЛА

//хозяин
	int							elementsNum;		//кол-во элемнтов лопасти

	BladeElementData*			elements;
	PropellerData				prop;
	AtmoData					atmo;

	//параметры
	Slipstream					slipstreamAverage; 
	double						skew_x0;				//координата X (в СК ЛА) точки перелома струи (скоса)

	Vec3						underlyingSurf_p_w;	//точка на подситалающей поверхности
	Vec3						underlyingSurf_n_w;	//нормаль к точке на подстилающей поверхности
	
	Vec3						underlyingSurf_n_l;	
	double						underlyingH;		//расстояние от центра ЛА до земли

	//к-ты для расчета эффекта влияния земли на скос струи
	double						groundSkewH0;		//расстояние от точки перелома до земли, при котором поворот струи осуществляется до горизонтали
};

}

#endif