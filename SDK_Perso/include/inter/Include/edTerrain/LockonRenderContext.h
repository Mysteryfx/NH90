#ifndef inter_edterrain_lockon_render_context
#define inter_edterrain_lockon_render_context

#include "IRenderContext.h"

namespace render
{

class LockonRenderContext : public IRenderContext
{
private:
	void updateCamera(const Camera* camera, const osg::Vec3f origin, render::enShadingModel shadingModel){
		mCamera = *camera;
		mOrigin = origin;
		mShadingModel = shadingModel;
	}
public:
	LockonRenderContext(){}

	void update(const render::Camera* camera, const osg::Vec3f origin, render::enShadingModel shadingModel, render::ConstantBuffer sharedParams)
	{
		mCamera = *camera;
		mOrigin = origin;
		mShadingModel = shadingModel;
		mSharedParams = sharedParams;
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
	virtual render::Texture sharedTexture(SharedTextureHandle handle) const{ return render::Texture(); };
	// 
	virtual render::StructuredBuffer sharedStructuredBuffer(SharedStructuredBufferHandle handle) const{ return render::StructuredBuffer(); };
	// все возможные константы для общих hlsl процедур
	virtual render::ConstantBuffer sharedParams() const{ return mSharedParams;}

	// отладка
	virtual render::IDump* getDump() const { return nullptr;}

public:
	Camera mCamera;
	osg::Vec3f mOrigin;
	render::enShadingModel mShadingModel;
	render::ConstantBuffer mSharedParams;
};

};

#endif // inter_edterrain_lockon_render_context
