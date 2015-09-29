#ifndef IAnimClip_H__
#define IAnimClip_H__

#include "AnimPose.h"

namespace model
{
class IModel;
struct ModelParams;

// Анимационный клип (секвенция)
class IAnimClip
{
protected:
	virtual ~IAnimClip() {}
public:
	// имя
	virtual const char* getName()const=0;

	// Длительность. В секундах
	virtual float getDuration()const=0;

	// Зацикленая?
	virtual bool isCycled()const=0;

	// Скорость постоянная?
	virtual bool isConstantVelocity()const=0;

	// Угловая скорость постоянна?
	virtual bool isAngularVelocityConstant()const=0;

	// motion extraction
	virtual osg::Vec3f getPosition(float time) const=0;
	virtual osg::Quatf getRotation(float time) const=0;

	// интерполяция
	virtual void processAnimation(
		float time, 
		const model::IModel* model, 
		model::AnimPose& animPose
		)const=0;
};
}

#endif
