#ifndef inter_edterrain_lockon_render_context
#define inter_edterrain_lockon_render_context

#include "edTerrain/IRenderContext.h"

namespace render
{

class TerrainRenderContext : public IRenderContext
{
private:
	void updateCamera(const Camera* camera, const osg::Vec3f origin, render::enShadingModel shadingModel){
		mCamera = *camera;
		mOrigin = origin;
		mShadingModel = shadingModel;
	}
public: 

	TerrainRenderContext():mDump(nullptr){}

	void update(const render::Camera* camera, const osg::Vec3f origin, render::enShadingModel shadingModel, render::ConstantBuffer sharedParams, render::IDump* dump = nullptr)
	{
		mCamera = *camera;
		mOrigin = origin;
		mShadingModel = shadingModel;
		mSharedParams = sharedParams;
		mDump = dump;
		if( !defaultTexture.isValid())
			defaultTexture.open("white.png");
	}

	// shading model
	virtual render::enShadingModel shadingModel() const{ return mShadingModel; };

	// origin
	virtual osg::Vec3d origin() const{ return mOrigin; };

	// камера
	virtual const Camera& camera() const { return mCamera; }
	// камера на пред. кадре
	virtual const Camera& prevFrameCamera() const { return mCamera; }

	// не используем render::getRenderTarget()
	virtual render::Texture sharedTexture(SharedTextureHandle handle) const
	{ 
		return defaultTexture; 
	};
	// 
	virtual render::StructuredBuffer sharedStructuredBuffer(SharedStructuredBufferHandle handle) const{ return render::StructuredBuffer(); };
	// все возможные константы для общих hlsl процедур
	virtual render::ConstantBuffer sharedParams() const{ return mSharedParams;}

	// отладка
	virtual render::IDump* getDump() const { return mDump;}

public:
	Camera mCamera;
	osg::Vec3f mOrigin;
	render::enShadingModel mShadingModel;
	render::ConstantBuffer mSharedParams;
	render::Texture defaultTexture;
	render::IDump* mDump;
};

};

#endif // inter_edterrain_lockon_render_context
