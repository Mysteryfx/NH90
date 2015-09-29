#ifndef __DynamicBody_h__
#define __DynamicBody_h__

#include "Base.h"
#include "FMMathematics/math.h"
#include "Optimizable.h"

// TO DO :
// 1) réaliser la fonction de calcul de la vitesse et l'accélération au point
// 2) faire une occasion rare de mettre à niveau les forces

namespace EagleFM
{

// Structure pour la description de l'état dynamique
struct FMBASE_API DynamicState
{
	DynamicState()
	{
		pos = Vec3(0.0);
		yaw = pitch = roll = 0.0;
		V = Vec3(0.0); 
		W = Vec3(0.0);
		A = Vec3(0.0);
		mass = 1.0;
		rotInert = Vec3(1.0, 1.0, 1.0); 
		rotInert_xy = 0.0;
		mass_pos = Vec3(0.0);
		GyroEffectFlag = false;

		_yaw = _pitch = _roll = 0.0;
		or = Matrix3(1.0);
	}
	Matrix3 getOrientation()
	{
		if(_yaw != yaw || _pitch != pitch || _roll != roll)
		{   //Mise à jour de la matrice
			or = calcMatrixByYawPitchRoll(yaw, pitch, roll); 
			_yaw = yaw;
			_pitch = pitch;
			_roll = roll;
		}
		return or;
	}

	Vec3		pos;
	float		yaw;
	float		pitch;
	float		roll;
	Vec3		A;
	Vec3		V;
	Vec3		W;
	double		mass;
	Vec3		rotInert; 
	double		rotInert_xy;
	Vec3		mass_pos; 
	bool		GyroEffectFlag;

	//Cache:
private:
	float		_yaw;
	float		_pitch;
	float		_roll;
	Matrix3		or;
};

// _w - World UK
// _l - SC locale

class FMBASE_API DynamicBody : public Optimizable
{
//--------------------------	CARACTÉRISTIQUES 	-----------------------------------
public:
	//	Création / Initialisation
	DynamicBody(double dt = 0.006);
	DynamicBody(const DynamicState&, double dt = 0.006);

	void			init(const DynamicState&);

	void			setGyroEffect(bool b)	{ GyroEffectFlag = b; }
	void			changeMass__MassPos_l__MOI_l(double AddMass, const Vec3& AddMassPos_l, const Vec3& AddMOI_l); // Modifiez les valeurs de masse corporelle, le centre de masse coordonnées de l'organisme dans la SSC et principaux moments d'inertie


	//	CONTRÔLE EXTERNE
	void			addForce_w(const Vec3& F_w);				//L'application d'une force externe par rapport au centre de masse dans la CMC
	void			addForce_l(const Vec3& F_l);				//L'application d'une force externe par rapport au centre de masse dans le LCS
	void			addForce_w(const Vec3& F_w, const Vec3& F_pos_w);	//L'application d'une force externe au point dans le SCG
	void			addForce_l(const Vec3& F_l, const Vec3& F_pos_l);	//L'application d'une force extérieure au point de LCS
	void			addForce_wl(const Vec3& F_w, const Vec3& F_pos_l);	//L'application d'une force externe de la CMC pour le point de LCS
	void			addForce_lw(const Vec3& F_l, const Vec3& F_pos_w);	//L'application d'une force externe dans la LCS au point dans WCS
	void			addMoment_w(const Vec3& M_w);				//L'application d'un couple externe dans MSC
	void			addMoment_l(const Vec3& M_l);				//L'application d'un couple externe dans LCS
	
	void			resetForce()	{ F_l = Vec3(0.0); }
	void			resetMoment()	{ M_l = Vec3(0.0); }

	// Le contrôle direct
	void			setQuaternion(const Rot3& q)		{ checkValid(const_cast<Math::Vec3f&>(q.v)); checkValid(const_cast<float&>(q.w)); setQuaternion_w(q); }
	void			setPosition(const Vec3& P)			{ checkValid(const_cast<Vec3&>(P)); pos_w = P; }
	void			setVelocity(const Vec3& V)			{ checkValid(const_cast<Vec3&>(V));  setVelocity_w(V); }
	void			setRotateSpeed(const Vec3& W)		{ checkValid(const_cast<Vec3&>(W));  setRotateSpeed_w(W);}
	void			setAcceleration(const Vec3& A)		{ checkValid(const_cast<Vec3&>(A));  setAcceleration_w(A); } 
	void			setRotAcceleration(const Vec3& AW)  { checkValid(const_cast<Vec3&>(AW)); setRotateAcceleration_w(AW); } 

	//	SIMULATION
private:
	virtual void	doSimulate();
public:

	//  CALCUL 
	Vec3			calcSpeedInPoint_w(const Vec3& p_w)			const;	//вектор линейной скорости в МСК в заданной точке в МСК
	Vec3			calcSpeedInPoint_l(const Vec3& p_l)			const;	//вектор линейной скорости в МСК в заданной точке в МСК
	Vec3			calcSpeedInPoint_lw(const Vec3& p_w)		const;	//вектор линейной скорости в ЛСК в заданной точке в МСК
	Vec3			calcSpeedInPoint_wl(const Vec3& p_l)		const;	//вектор линейной скорости в МСК в заданной точке в ЛСК
	Vec3			calcAccelerationInPoint_w(const Vec3& p_w)	const;	//вектор линейного ускорения в МСК в заданной точке в МСК
	Vec3			calcAccelerationInPoint_l(const Vec3& p_l)	const;	//вектор линейного ускорения в ЛСК в заданной точке в ЛСК
	Vec3			calcAccelerationInPoint_lw(const Vec3& p_w)	const;	//вектор линейного ускорения в ЛСК в заданной точке в МСК
	Vec3			calcAccelerationInPoint_wl(const Vec3& p_l)	const;	//вектор линейного ускорения в МСК в заданной точке в ЛСК


	//	ДОСТУП
	const Vec3	&	getPosition()		const		{ return pos_w; }
	Vec3			getTimePosition()	const;
	Matrix3			getOrientation();//				{ return qtn_w; }
	Matrix3			getOrientationT();//			{ return getOrientation().transposed(); }
	const Rot3	&	getQuaternion()		const		{ return qtn_w; }

	const Vec3	&	getVelocity_w()		const		{ return V_w; }
	// Это бы лучше обозвать AngularRate...
	const Vec3	&	getRotateSpeed_w()	const		{ return W_w; }

	Vec3			getVelocity_l();//				{ return V_w*getOrientation().transposed(); }
	Vec3			getRotateSpeed_l();//			{ return W_w*getOrientation().transposed(); }

	const Vec3	&	getAcceleration_w()	const		{ return a_w; }
	const Vec3	&	getRotateAcceleration_w() const	{ return dw_w; }

	Vec3			getAcceleration_l(); 	
	Vec3			getRotateAcceleration_l();

	const Vec3	&	getForce_l()		const		{ return F_l; }
	const Vec3	&	getMoment_l()		const		{ return M_l; }

	double			getMass()			const		{ return mass; }	
	const Vec3	&	getMassPos()		const		{ return mass_pos_l; }
	const Vec3	&	getMomentOfInertia()const		{ return rotInert_l; }
	double			getCentrifugalMOI()	const		{ return rotInert_xy; }

	DynamicState	getDynamicState()	const;

	//  остальные методы доступа при необходимости можно написать

protected:
	//	ДЛЯ КЭША:
	void			setQuaternion_w(const Rot3&);
	void			setVelocity_w(const Vec3&);
	void			setRotateSpeed_w(const Vec3&);
	void			setAcceleration_w(const Vec3&);
	void			setRotateAcceleration_w(const Vec3&);



//-----------------------	ПОЛЯ	--------------------------------------------

	//        СВОЙСТВА ТЕЛА

	//позиция и ориентация тела (кватернион) в МСК
	Rot3		qtn_w;	//изменять только через setQuaternion(Rot3) !!!
	Vec3		pos_w;

	//кинематика: линейная и угловая скорости в МСК
	Vec3		V_w;	//изменять только через setVelocity_w(Vec3)!!!
	Vec3		W_w;	//изменять только через setRotateSpeed_w(Vec3)!!!

	//свойства материи: масса и моменты инерции (в ЛСК)
	double		mass;
	Vec3		rotInert_l; 
	double		rotInert_xy;	//компонента для расширения момента инерции (центробежный момент инерции)

	Vec3		mass_pos_l; //позиция центра масс в ЛСК

	
	//		ДИНАМИКА   (РАСЧЕТ ПРОИЗВОДИТСЯ В ЛСК)
	
	Vec3		F_l;  // вектор внешней силы в ЛСК
	Vec3		M_l;  // вектор внешнеего момента в ЛСК
	Vec3		a_w;  // вектор линейного ускорения в МСК
	Vec3		dw_w; // вектор уголового ускорения в МСК

    bool		GyroEffectFlag; //флаг учета гироскопиго эффекта
	
	
	//		КЭШ:	
	Matrix3		or;
	Matrix3		orT;
	Vec3		V_l;
	Vec3		W_l;
	Vec3		a_l;
	Vec3		dw_l;
	bool		or_ready;
	bool		orT_ready;
	bool		V_l_ready;
	bool		W_l_ready;
	bool		a_l_ready;
	bool		dw_l_ready;
	

	// TODO:
	/*
	1) для разного шага интегрирования продумать механизм обновления сил и моментов. Возможно придется setForce_w(m_Force); setMoment_w(m_Moment) (AD например)
	*/

	double time; //*****

    friend class AerodyneFM;
};

} //EagleFM

#endif