#pragma once
#include "edterrain/TerrainContextCamera.h"
#include "edterrain/IParseContext.h"

namespace render 
{

class TerrainParseContext : public IParseContext
{
public:
	TerrainParseContext();
	TerrainParseContext(const osg::Vec3f& origin, const osg::Vec3d& clipmapCenter, lua_State* mState = nullptr);
	// Добавить выборку
	SampleHandle addSample(const Camera* camera, enShadingModelMask shadingModelMask);
	//
	void setDump(render::IDump* dump);

	// updates parse context and clear all samples
	void update(const osg::Vec3f& origin, const osg::Vec3d& clipmapCenter, float modelTime);

	void setConfig(lua_State* mState);

	// Методы вызываемые из ITerrainGraphics
public:	
	// Настройки рендера
	virtual const lua_State* getOptions() const{return mState;}

	// origin
	virtual osg::Vec3d origin() const;

	// clipmap center
	virtual osg::Vec3d clipmapCenter(bool* ortho=nullptr) const;

	// model time
	virtual double time() const;

	// Массив выборок
	virtual int sampleCount() const;
	virtual SampleHandle sampleHandle(int index) const;

	// камера семпла, для выборок. 
	// используется Camera::projection(), Camera::polytope()
	// а для PR_PERSP еще Camera::nearPlane(), Camera::farPlane(), Camera::polytope(near, far)
	virtual const Camera* sampleCamera(const SampleHandle& sample) const;

	// шейдинг модели которые будут использованы для рендера семпла (enShadingModel BitMask)
	virtual uint32_t sampleShadingModels(const SampleHandle& sample) const;

	// отладка
	virtual render::IDump* getDump() const;
private:
	void setOrigin(const osg::Vec3f& origin);	

	struct ParseParams {
		const Camera* mCamera;
		enShadingModelMask mShadingModel;
	};
	std::vector<ParseParams> mSampleParams;
	lua_State* mState;
	osg::Vec3f mOrigin;
	osg::Vec3d mClipmapCenter;
	render::IDump* mDump;
	float mModelTime;
};

inline TerrainParseContext::TerrainParseContext() : 
	mState(nullptr), mDump(nullptr)
{

}

inline TerrainParseContext::TerrainParseContext(const osg::Vec3f& origin, const osg::Vec3d& clipmapCenter, lua_State* mState) :
	mState(mState), mOrigin(origin), mClipmapCenter(clipmapCenter), mDump(nullptr){}

inline void TerrainParseContext::update(const osg::Vec3f& origin, const osg::Vec3d& clipmapCenter, float modelTime)
{
	mSampleParams.clear();
	mOrigin = origin;
	mClipmapCenter = clipmapCenter;
	//mState = state;
	mModelTime = modelTime;
}

inline void TerrainParseContext::setConfig(lua_State* config)
{
	mState = config;
}

inline void TerrainParseContext::setDump(render::IDump* dump)
{
	this->mDump = dump;
}

inline osg::Vec3d TerrainParseContext::origin() const
{
	return mOrigin;
}
// clipmap center
inline osg::Vec3d TerrainParseContext::clipmapCenter(bool* ortho) const
{
	if( ortho)
		*ortho = false; // <-TODO
	return mClipmapCenter;
}

// model time
inline double TerrainParseContext::time() const
{
	return mModelTime;
}

inline int TerrainParseContext::sampleCount() const{
	return mSampleParams.size();
}

inline SampleHandle TerrainParseContext::sampleHandle(int index) const {
	return SampleHandle(index);
}

inline const Camera* TerrainParseContext::sampleCamera(const SampleHandle& sample) const{
	return mSampleParams[sample].mCamera;
}

inline uint32_t TerrainParseContext::sampleShadingModels(const SampleHandle& sample) const {
	return mSampleParams[sample].mShadingModel;
}

inline render::IDump* TerrainParseContext::getDump() const
{
	return mDump;
}

// inline SampleHandle TerrainParseContext::addSample(const Camera* camera, enShadingModelMask shadingModelMask)
// {
// 	// Один parseContext один origin
// 	assert(camera->origin() == mOrigin);
// 
// 	ParseParams params = { camera, shadingModelMask};
// 	mSampleParams.emplace_back(params);
// 	return SampleHandle(mSampleParams.size()-1);
// }

inline void TerrainParseContext::setOrigin(const osg::Vec3f& origin){
	mOrigin = origin;
}

inline SampleHandle TerrainParseContext::addSample(const Camera* camera, enShadingModelMask shadingModelMask)
{
	// Один parseContext один origin
	assert(camera->origin() == mOrigin);

	for (size_t i = 0; i < mSampleParams.size(); ++i){
		if (*mSampleParams[i].mCamera == *camera){
			mSampleParams[i].mShadingModel = enShadingModelMask(uint32_t(mSampleParams[i].mShadingModel) | uint32_t(shadingModelMask));
			return SampleHandle(i);
		}
	}
	ParseParams params = { camera, shadingModelMask };
	mSampleParams.emplace_back(params);
	return SampleHandle(mSampleParams.size() - 1);
}


}//namespace edterrain