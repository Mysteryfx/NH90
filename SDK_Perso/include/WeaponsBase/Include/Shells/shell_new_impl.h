#ifndef __SHELL_NEW_IMPL_H__
#define __SHELL_NEW_IMPL_H__

#include "Shells/shell_new.h"

#include "lPointOnSurface.h"

#include "ed/MemPool.h"
#include "Math/Vector.h"

#include "Randomizer/RandomObject.h"

#include "weaponsBase.h"
#include "Shells/shell_descriptors.h"

#include "IWorldNet.h"
#include "WorldMail.h"

#include "edTerrain/ITerrain.h"

struct ExplosionInfo;

template <bool NetMaster>
class WEAPONSBASE_API wShellImpl : public wShell
{
public:
	static void init_randoms(int offset);
	static void serialize_randoms(Serializer& s);

public:

	// initial_vel - дульная скорость снаряда (используется для гаубиц)
	wShellImpl(
		wShellGroup * owner,
		const wShellDescriptor& shellData, 
		const Math::Vec3d& launcher_pos, 
		const Math::Vec3d& launcher_dir, 
		const Math::Vec3d& launcher_vel,
		ObjectID launcher_id,
		const wShellCoefficients & coefficients,
		wcCoalitionName coalition,
		unsigned char state,
		double initial_vel = 0,
		ObjectID forced_id = 0);	
	virtual ~wShellImpl();

	//from MovingObject
	virtual int	Class(void) { return woClass_Shell;}
	virtual void destroy(bool immediately = false);

	// from ActivityGroupMember
	virtual eSimulationResult simulate(double dt, bool& exploded);

	virtual Math::Vec3d getVelocity() const { return vel_; }

	// from viObjectShape
	virtual cPosition& Position(wModelTime t);
	virtual cPosition& Position(); 
	virtual dVector GetPoint();

	virtual Graphics::Model		*GetModel();

	virtual const wShellDescriptor & getDesc () { return shellData_; }
	
protected:
	Math::Vec3d interpolate_(wModelTime time); 

	void getAtm_(Math::Vec3d& wind, double& rho, double& M); 
    void updateFM_(double dt, int steps,
                   Math::Vec3d& pos, 
                   Math::Vec3d& vel);

	eSimulationResult checkGroundCollision_(double dt);
	eSimulationResult checkWorldCollision_(double dt);

	// Запланировать рикошет через время dt (если dt близко к 0, то прямо щас его совершить)
	eSimulationResult scheduleRebound_(double dt, const Math::Vec3d& new_vel, double cxFactor);
	// То же самое для взрыва
	eSimulationResult scheduleExplosion_(double dt, double imp, double cumulative, const Math::Vec3d& p, int flags, ObjectID obj, bool pierced, float decayFactor = 1.0f);

	// По данным в explosionData_
	void explode_();
	// По данным в reboundData_
	void rebound_();

    // Обработать столкновение с объектом мира
    eSimulationResult processCollision_(double dt,
                                        ObjectID col_id, double col_dt,
                                        const Math::Vec3d& col_pnt,
                                        const Math::Vec3d& normal,
										const ed::string & partName);

    // Обработать рикошет
    eSimulationResult processRebound_(double dt, double colDt, 
                                      Math::Vec3d colPos, ObjectID colObj,  
                                      Math::Vec3d n, 
                                      const wShellDescriptor::ReboundCoefficients& reboundData);
	// Отклонить вектор n на угол alpha = N(0, sigma). Все плоскости равновероятны.
	// n должен быть нормализован
    Math::Vec3d deviateVector_(const Math::Vec3d& n, double sigma);

	// From wINamedAmmunitionDescriptor
	virtual const wINamedAmmunitionDescriptor* getNameDescriptor() const;
    virtual ObjectID getLauncherId() const;


	void tuneEffect(ExplosionInfo & info);

protected:
	enum eSpecialState
	{
		SS_NONE,
		SS_ABOUT_TO_REBOUND,
		SS_ABOUT_TO_EXPLODE
	};

protected:
	struct ExplosionData
	{
		MEM_DECL_CLASS_POOL(typename wShellImpl<NetMaster>::ExplosionData);

		Math::Vec3d pos;
		double effect, cumulative;
		ObjectID obj;
		int flags;
		bool pierced;
        float decayFactor;
	};

	struct ReboundData
	{
		MEM_DECL_CLASS_POOL(typename wShellImpl<NetMaster>::ReboundData);

		Math::Vec3d vel;
        double cxFactor;
	};

protected:

	Math::Vec3d pos_, vel_, prev_pos_;
    // Количество шагов, оставшихся до следующей грубой проверки столкновений со
    // статическими объектами.
    int stepsToStaticCollision_;
    // Количество шагов, в течение которых надо проверять столкновения со 
    // объектами.
    int detailedStaticCollisionSteps_;

	const wShellDescriptor& shellData_;
	double prev_time_, launch_time_;
    double modeloff_time_diff, lifeoff_time_diff; // для вариации жизни и погасания трассера

#ifndef USE_TERRAIN4
	landscape::lPointOnSurface surfacePoint_;
#else
	edterrain::PointOnSurface surfacePoint_;
#endif
	eSpecialState specialState_;
	bool rebounded_;
    double cxFactor_;

	ExplosionData* explosionData_;	// Здесь есть что-нибудь только когда specialState_ == SS_ABOUT_TO_EXPLODE
	ReboundData*   reboundData_;	// Здесь есть что-нибудь только когда specialState_ == SS_ABOUT_TO_REBOUND

	cPosition position_;

    ObjectID launcherId_;

    bool shapeOff() const 
    {
		double t = wTime::GetModelTime() - launch_time_;
		return t < shellData_.tracer_on ||
			   t + modeloff_time_diff > shellData_.tracer_off;
    }
    bool smokeOff() const 
    {
		double t = wTime::GetModelTime() - launch_time_;
		return t > shellData_.smoke_tail_life_time;
    }

protected:
	static RandomObject rndNormal_;
	static RandomObject rndUniform_;
};


#endif
