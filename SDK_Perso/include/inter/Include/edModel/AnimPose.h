#ifndef AnimPose_H__
#define AnimPose_H__

#include "ed/vector.h"
#include "osg/Quatf"
#include "osg/Vec3f"
#include "edModel/IModel.h"

namespace model
{
// AnimPose
// 
// usage:
// AnimPose ap1;
// clip1->processAnimation(T0, model, ap1);
// AnimPose ap2;
// clip2->processAnimation(T1, model, ap2);
// 
// бленд:
// ap = ap1*w + ap2*(1-w);
// model::ModelParams params;
// ap.updateModelParams(params);
// 
// добавление частичной анимации:
// AnimPose ap_part;
// clip3->processAnimation(T3, model, ap_part);
// ap += ap_part*0.5;
// 
struct AnimPose
{
public:
	const model::IModel* model;			// модель. Можно блендить позы только для одинаковых моделей
	ed::vector<osg::Quatf> quats;
	ed::vector<float> weights;

	static const unsigned int InvalidRootIndex = 0xffffffff;
	// root bone index
	unsigned int rootIndex;

	// root bone translation
	osg::Vec3f translation;
	
// операции для блендинга
public:
	inline AnimPose operator*(float w) const;
	inline AnimPose& operator+=(const AnimPose& arg);
	inline AnimPose operator+(const AnimPose& arg) const;

public:
	// заполнить параметры модели
	inline void updateModelParams(model::IModelParams& params) const;

	void blend(const AnimPose& a, const AnimPose& b, float w);
};
}

#include "inl/AnimPose.inl"
#endif
