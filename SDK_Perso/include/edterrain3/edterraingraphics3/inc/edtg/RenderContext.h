#ifndef EDTG_RENDER_CONTEXT_H
#define EDTG_RENDER_CONTEXT_H

#include <osg/Vec3f>
#include <osg/Vec3d>
#include <osg/Matrixf>
#include <osg/Matrixd>
#include <osg/Polytope>
#include "rwrappers/renderUserAPI.h"
#include "renderSurface.h"
#include "osg/MatrixUtils.h"

#ifdef EDGE
#include "Graphics/Context.h"
#else
#include "Renderer/Renderer.h"
#endif

namespace edtg
{
struct TerrainGpuContext;

// Контекст рендера
// Хранит всяческие матрицы. Самостоятельно ничего множить не надо - обращаться к нему!!!
// Валиден в момент выполнения IMaterial::Render
struct RenderContext
{
protected:
	// тип рендера
	enSurfaceRenderType type;
	
	// Очередь
	enSurfaceRenderQueue queue;

	osg::Vec3d clipmapCamera;

	osg::Matrixf mTTF;
	osg::Matrixf mLandLightTTF;
	osg::Matrixf mW;
	osg::Matrixf mV;
	osg::Matrixf mP;
	osg::Vec3f vOrigin;			// перенес начала координат
	
	// производные
	osg::Matrixf mVinv;
	osg::Matrixf mVP;
	osg::Matrixf mWVP;
	osg::Matrixf mWV;
	// нормализованая матрица V
	osg::Matrixf mVnormal;
	osg::Matrixf mVnormalinv;
	// Vsun: view матрица для солнца
	osg::Matrixf mVsun;
	osg::Matrixf mVsuninv;

	// матрица проекции теней от облаков
	osg::Matrixf mCloudShadowsProj;
	render::Texture mCloudShadowsTexture;

	// матрица и текстура карты высот + текстура сплатмапа, отрендеренная с этой же ортокамеры
	osg::Matrixf heightmapMat;
	render::Texture heightmapTex;
	render::Texture splatmapTex;

	// солнце, луна и ambient
	GlobalLights globalLights;

	// Туман
	edcg::Fog fog;

#ifdef EDGE
	edge::Context* context;
#else
	Graphics::Context* context;
#endif

	void* objectdata;			// для пропихивания:
	// DistrictInstanceDynamic*
	// LightRenderData*
	// query
public:
	const osg::Matrixf& W();
	const osg::Matrixf& V();
	const osg::Matrixf& invV();
	const osg::Matrixf& Vnormal();
	const osg::Matrixf& invVnormal();
	const osg::Matrixf& P();
	const osg::Matrixf& VP();
	const osg::Matrixf& WV();
	const osg::Matrixf& WVP();
	const osg::Vec3f& Origin();
	const osg::Matrixf& landlightTTF();
	const osg::Matrixf& landlightWTTF();
	
	const osg::Matrixf& SunV();
	const osg::Matrixf& invSunV();

	const osg::Matrixf& cloudShadowsProj();
	render::Texture& cloudShadowsTexture();

	const osg::Matrixf& heightmapMatrix();
	render::Texture& heightmapTexture();
	render::Texture& splatmapTexture();

	const osg::Vec3d cameraDir();
	const osg::Vec3d cameraPos();
	const osg::Vec3d cameraPosWS();
	const osg::Vec3d clipmapCameraPos();
	const osg::Vec3d clipmapCameraPosWS();
	const osg::Polytope getCameraFrustum();

	const GlobalLights& getGlobalLights();
	const edcg::Fog& getFog();

	void* ObjectData();

	enSurfaceRenderType renderType() const;
	enSurfaceRenderQueue renderQueue() const;

	// setup
public:
	RenderContext();
	void setCamera(
		const osg::Vec3f& origin,
		const osg::Matrixf& view,
		const osg::Matrixf& proj,
		enSurfaceRenderType type
	);

	void setQueue(enSurfaceRenderQueue queue);
	void setW(const osg::Matrixf& W);
	void setWinOrigin(const osg::Matrixf& W);
	void setLandLightTTF(const osg::Matrixf& ttf);

	void setCloudShadows(const osg::Matrixf& cloudShadowsProj, const render::Texture& texture);
	void setHeightmap(const osg::Matrixf& heighmMapMat, const render::Texture& heightmapTex, const render::Texture& splatmapTex);

	void setObjectData(void*);
	void setClipmapCameraPos(const osg::Vec3d& cameraPos);

	void setGlobalLights(const GlobalLights& globalLights);
	void setFog(const edcg::Fog& fog);

#ifdef EDGE
	void setPipelineContext(edge::Context* context);
	edge::Context* getPipelineContext();
#else
	void setPipelineContext(Graphics::Context* context);
	Graphics::Context* getPipelineContext();
#endif

// TerrainGpuContext
public:
	render::ConstantBuffer gpuContextBuffer;

	// update data in TerrainGpuContext 
	void updateGpuContext(TerrainGpuContext& gpuContext);
	
	// for render and upload
	void setGpuContext(render::ConstantBuffer& gpuContextBuffer){this->gpuContextBuffer = gpuContextBuffer;};
	render::ConstantBuffer& getGpuContext(){return this->gpuContextBuffer;};
};

}

inline edtg::enSurfaceRenderType edtg::RenderContext::renderType() const 
{
	return this->type;
}

inline edtg::enSurfaceRenderQueue edtg::RenderContext::renderQueue() const 
{
	return this->queue;
}

inline edtg::RenderContext::RenderContext()
{
	objectdata = NULL;
	context    = NULL;
}

inline const osg::Matrixf& edtg::RenderContext::W()
{
	return mW;
}

inline const osg::Matrixf& edtg::RenderContext::V()
{
	return mV;
}

inline const osg::Matrixf& edtg::RenderContext::invV()
{
	return mVinv;
}

inline const osg::Matrixf& edtg::RenderContext::Vnormal()
{
	return mVnormal;
}

inline const osg::Matrixf& edtg::RenderContext::invVnormal()
{
	return mVnormalinv;
}

inline const osg::Matrixf& edtg::RenderContext::P()
{
	return mP;
}

inline const osg::Matrixf& edtg::RenderContext::VP()
{
	return mVP;
}

inline const osg::Matrixf& edtg::RenderContext::WV()
{
	mWV = mW;
	mWV *= mV;
	return mWV;
}

inline const osg::Matrixf& edtg::RenderContext::WVP()
{
	mWVP = mW;
	mWVP *= mVP;

	return mWVP;
}

inline const osg::Matrixf& edtg::RenderContext::landlightWTTF()
{
	osg::Matrixf E;
	E(1, 1) = 0.0f;

	osg::Matrixf pos = E * mW;
	mLandLightTTF = pos * mTTF;
	return mLandLightTTF;
}
inline const osg::Matrixf& edtg::RenderContext::SunV()
{
	return mVsun;
}
inline const osg::Matrixf& edtg::RenderContext::invSunV()
{
	return mVsuninv;
}

inline const osg::Matrixf& edtg::RenderContext::landlightTTF()
{
	return mTTF;
}

// Позиция ориджина в мировых координатах
inline const osg::Vec3f& edtg::RenderContext::Origin()
{
	return vOrigin;
}

// Позиция камеры в спейсе ориджина
inline const osg::Vec3d edtg::RenderContext::cameraPos()
{
	return mVinv.getAxis(3);
}

inline const osg::Vec3d edtg::RenderContext::cameraDir()
{
	return -mVinv.getAxis(2);
}

// Позиция камеры в мировых координатах
inline const osg::Vec3d edtg::RenderContext::cameraPosWS()
{
	return this->cameraPos() + this->Origin();
}

inline const osg::Vec3d edtg::RenderContext::clipmapCameraPos()
{
	return this->clipmapCamera;
}

inline const osg::Vec3d edtg::RenderContext::clipmapCameraPosWS()
{
	return this->clipmapCamera + this->Origin();
}

inline void edtg::RenderContext::setClipmapCameraPos(const osg::Vec3d& cameraPos)
{
	this->clipmapCamera = cameraPos;
}

inline const osg::Polytope edtg::RenderContext::getCameraFrustum()
{
	osg::Matrixd orig = osg::Matrixd::translate(-this->Origin());
	osg::Matrixd view = this->V();
	osg::Matrixd proj = this->P();

	osg::Polytope frustum; frustum.setToUnitFrustum(true, true);
	frustum.transformProvidingInverse(orig * view * proj);

	return frustum;
}

inline const osg::Matrixf& edtg::RenderContext::cloudShadowsProj()
{
	return mCloudShadowsProj;
}

inline render::Texture& edtg::RenderContext::cloudShadowsTexture()
{
	return mCloudShadowsTexture;
}

inline void edtg::RenderContext::setCloudShadows(const osg::Matrixf& cloudShadowsProj, const render::Texture& texture)
{
	this->mCloudShadowsProj = cloudShadowsProj;
	if (texture.isValid())
		this->mCloudShadowsTexture = texture;
	else
		if(!this->mCloudShadowsTexture.isValid()){
#ifndef EDGE
			this->mCloudShadowsTexture.open("DummyWhiteTexture");
#else
			this->mCloudShadowsTexture.open("white.png");
#endif
		}
}

inline const osg::Matrixf& edtg::RenderContext::heightmapMatrix()
{
	return heightmapMat;
}

inline render::Texture& edtg::RenderContext::heightmapTexture()
{
	return heightmapTex;
}

inline render::Texture& edtg::RenderContext::splatmapTexture()
{
	return splatmapTex;
}

inline void edtg::RenderContext::setHeightmap(const osg::Matrixf& heightmapMat, const render::Texture& heightmapTex, const render::Texture& splatmapTex)
{
	this->heightmapMat = heightmapMat;
	this->heightmapTex = heightmapTex;
	this->splatmapTex  = splatmapTex;
}

inline const edtg::GlobalLights& edtg::RenderContext::getGlobalLights()
{
	return globalLights;
}

inline const edcg::Fog& edtg::RenderContext::getFog()
{
	return fog;
}

inline void* edtg::RenderContext::ObjectData()
{
	return objectdata;
}

inline void edtg::RenderContext::setCamera(
	const osg::Vec3f& origin,
	const osg::Matrixf& view,
	const osg::Matrixf& proj,
	enSurfaceRenderType type
)
{
	this->vOrigin = origin;
	this->mV      = view;
	this->mP      = proj;
	this->type    = type;

	mVinv = osg::Matrixf::inverse(mV);
	mVP = mV * mP;

	// нормализовать V матрицу
	mVnormalinv = mVinv;
	double f;
	f = 1.0f / mVnormalinv.getAxis(0).length();
	mVnormalinv(0, 0) *= f;
	mVnormalinv(0, 1) *= f;
	mVnormalinv(0, 2) *= f;
	f = 1.0f / mVnormalinv.getAxis(1).length();
	mVnormalinv(1, 0) *= f;
	mVnormalinv(1, 1) *= f;
	mVnormalinv(1, 2) *= f;
	f = 1.0f / mVnormalinv.getAxis(2).length();
	mVnormalinv(2, 0) *= f;
	mVnormalinv(2, 1) *= f;
	mVnormalinv(2, 2) *= f;

	mVnormal = osg::Matrixf::inverse(mVnormalinv);
}

inline void edtg::RenderContext::setQueue(enSurfaceRenderQueue queue)
{
	this->queue = queue;
}

inline void edtg::RenderContext::setW(const osg::Matrixf& W)
{
	this->mW = W;
	this->mW(3,0) -= this->vOrigin.x();
	this->mW(3,1) -= this->vOrigin.y();
	this->mW(3,2) -= this->vOrigin.z();
}
inline void edtg::RenderContext::setWinOrigin(const osg::Matrixf& W)
{
	this->mW = W;
}

inline void edtg::RenderContext::setLandLightTTF(const osg::Matrixf& ttf)
{
	this->mTTF = ttf;
}

inline void edtg::RenderContext::setObjectData(void* objectdata)
{
	this->objectdata = objectdata;
}

inline void edtg::RenderContext::setGlobalLights(const GlobalLights& globalLights)
{
	this->globalLights = globalLights;

	// Vsun: view матрица для солнца
	osg::Vec3d z = globalLights.sunDirectionWS;
	osg::Vec3d x = osg::Vec3d(0.0, 1.0, 0.0) ^ z;
	osg::Vec3d y = x ^ z;
	x.normalize();
	y.normalize();
	z.normalize();
	mVsuninv = ed::makeFromBasis(x, y, z, this->cameraPos());
	mVsun = osg::Matrixf::inverse(mVsuninv);
}

inline void edtg::RenderContext::setFog(const edcg::Fog& fog)
{
	this->fog = fog;
}

#ifdef EDGE

inline void edtg::RenderContext::setPipelineContext(edge::Context* context)
{
	this->context = context;
}

inline edge::Context* edtg::RenderContext::getPipelineContext()
{
	return this->context;
}

#else

inline void edtg::RenderContext::setPipelineContext(Graphics::Context* context)
{
	this->context = context;
}

inline Graphics::Context* edtg::RenderContext::getPipelineContext()
{
	return this->context;
}

#endif

#endif
