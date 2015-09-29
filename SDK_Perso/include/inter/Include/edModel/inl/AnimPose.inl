namespace model
{

inline
AnimPose AnimPose::operator*(float w) const
{
	AnimPose res = *this;
	unsigned int bcnt = weights.size();

	for (unsigned int i = 0; i < bcnt; ++i)
	{
		res.weights[i] *= w;
	}

	return res;
}

inline
AnimPose& AnimPose::operator+=(const AnimPose& arg)
{
	unsigned int bcnt = quats.size();

	float w = 0.0f;

	for (unsigned int i = 0; i < bcnt; ++i)
	{
		quats[i].slerp(weights[i], quats[i], arg.quats[i]);
		weights[i] = weights[i] + arg.weights[i];
	}

	for (unsigned int j = 0; j < 3; ++j)
	{
		translation._v[j] = translation._v[j] + (arg.translation._v[j] - translation._v[j]) * w;
	}

	return *this;
}

inline
AnimPose AnimPose::operator+(const AnimPose& arg) const
{
	AnimPose res = *this;
	res += arg;
	return res;
}

inline
void AnimPose::blend(const AnimPose& a, const AnimPose& b, float w)
{
	model = a.model;

	unsigned int bcnt = a.quats.size();

	quats.resize(bcnt);
	weights.resize(bcnt);

	for (unsigned int i = 0; i < bcnt; ++i)
	{
		osg::Quatf& q = quats[i];
		q.slerp(w, a.quats[i], b.quats[i]);

		// ???
		weights[i] = 1.0f;
	}

	translation = a.translation + b.translation - a.translation * w;

	rootIndex = a.rootIndex;
}

// заполнить параметры модели
inline
void AnimPose::updateModelParams(model::IModelParams& params) const
{
	params.init(*model);
	if( params.getBones().empty())
		return;

	unsigned int bcnt = params.getBones().size();
	for (unsigned int i = 0; i < bcnt; ++i)
	{
		osg::Matrixd& transform = params.getBones()[i];
		
		// 
		osg::Quatd qd = transform.getRotate();
		osg::Quatf qf;
		for (unsigned int j = 0; j < 4; ++j)
		{
			qf._v[j] = qd._v[j];
		}

		float w = weights[i];
		const osg::Quatf& c = quats[i];

		osg::Quatf q;
		q.slerp(w, qf, c);
		transform.setRotate(q);
	}

	if (rootIndex != InvalidRootIndex)
	{
		if( rootIndex<params.getBones().size())
			params.getBones()[rootIndex].setTrans(translation);
	}
}

}
