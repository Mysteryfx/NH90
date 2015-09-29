#pragma once
#include "TerrainContextCamera.h"
#include "IParseContext.h"

namespace render 
{

class ParseContextLockon : public IParseContext
{
public:
	ParseContextLockon(const osg::Vec3f& origin);
	// �������� �������
	SampleHandle addSample(const Camera* camera, enShadingModel shadingModel);

	// ������ ���������� �� ITerrainGraphics
public:	
	// ��������� �������
	virtual const lua_State* getOptions() const{return mState;}

	// origin
	virtual osg::Vec3d origin() const;

	// ������ �������
	virtual int sampleCount() const;
	virtual SampleHandle sampleHandle(int index) const;

	// ������ ������, ��� �������. 
	// ������������ Camera::projection(), Camera::polytope()
	// � ��� PR_PERSP ��� Camera::nearPlane(), Camera::farPlane(), Camera::polytope(near, far)
	virtual const Camera* sampleCamera(const SampleHandle& sample) const;

	// ������� ������ ������� ����� ������������ ��� ������� ������ (enShadingModel BitMask)
	virtual uint32_t sampleShadingModels(const SampleHandle& sample) const;

	// �������
	virtual render::IDump* getDump() const;
private:
	void setOrigin(const osg::Vec3f& origin);	

	struct ParseParams {
		const Camera* mCamera;
		enShadingModel mShadingModel;
	};
	std::vector<ParseParams> mSampleParams;
	lua_State* mState;
	osg::Vec3f mOrigin; 
};

inline ParseContextLockon::ParseContextLockon(const osg::Vec3f& origin):
	mState(nullptr), mOrigin(origin){}

inline osg::Vec3d ParseContextLockon::origin() const
{
	return mOrigin;
}

inline int ParseContextLockon::sampleCount() const{
	return mSampleParams.size();
}

inline SampleHandle ParseContextLockon::sampleHandle(int index) const {
	return SampleHandle(index);
}

inline const Camera* ParseContextLockon::sampleCamera(const SampleHandle& sample) const{
	return mSampleParams[sample].mCamera;
}

inline uint32_t ParseContextLockon::sampleShadingModels(const SampleHandle& sample) const {
	return mSampleParams[sample].mShadingModel;
}

inline render::IDump* ParseContextLockon::getDump() const
{
	return nullptr;
}

inline SampleHandle ParseContextLockon::addSample(const Camera* camera, enShadingModel shadingModel)
{
	// ���� parseContext ���� origin
	assert(camera->origin() == mOrigin);

	ParseParams params = {camera, shadingModel};
	mSampleParams.emplace_back(params);
	return SampleHandle(mSampleParams.size()-1);
}

inline void ParseContextLockon::setOrigin(const osg::Vec3f& origin){
	mOrigin = origin;
}

}//namespace edterrain