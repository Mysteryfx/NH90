#ifndef __HelicopterFM_h__
#define __HelicopterFM_h__
#include "AerodyneFM.h"
struct StaticObjectData;
typedef Math::Matrix3d Matrix3d;
namespace EagleFM
{

class FMBASE_API HelicopterFM : public AerodyneFM
{
public:
	HelicopterFM(double dt = 0.006);
	virtual ~HelicopterFM();

	virtual void				simulate(double dt);

	//доступ к частностям состояния (-1...1 или 0...1 или 0...Pi2):
	virtual	double				getCollectiveBladePitch() const;
	virtual	double				getCyclicXBladePitch() const;
	virtual	double				getCyclicZBladePitch() const;
	virtual	double				getRudderBladePitch() const;
	virtual double				getRotorPhase(char rotorID = 0) const;
	virtual double				getRotorRPM(char rotorID = 0) const;
	virtual double				getRotorIF(char rotorID = 0) const;
	virtual double				getBladeFlapping(int BladeNum) const;
	virtual double				getBladeHunting(int BladeNum) const;
	virtual double				getBladeDeflection(int BladeNum) const;
	virtual	double				getRotorControlState(int ControlNum) const;



	//DEBUG OUTPUT!!!!!!!!
	virtual double				getDebugParam(int key)	const { return 0; }
	virtual void                setCargoForce(Vec3 F, Vec3 pos_hook);

protected:
};

}

#endif