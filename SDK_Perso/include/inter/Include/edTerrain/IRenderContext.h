#ifndef inter_edTerrain_IRenderContext_h__
#define inter_edTerrain_IRenderContext_h__

#include "rwrappers/renderUserAPI.h"
#include "TerrainContextCamera.h"
#include "ed/string.h"
#include "renderer/MetaContext.h"
#include "IDump.h"
//#include "edterrain/SharedParams.hlsl.h"

namespace render 
{
struct SharedParams;

//////////////////////////////////////////////////
// IRenderContext
//
// ������������ � ������ ������� �������. ��. ITerrainGraphics::render()
// 
class IRenderContext
{
public:
	// shading model
	virtual render::enShadingModel shadingModel() const=0;

	// origin
	virtual osg::Vec3d origin() const=0;

	// ������
	virtual const Camera& camera() const=0;
	// ������ �� ����. �����
	virtual const Camera& prevFrameCamera() const=0;

	// �� ���������� render::getRenderTarget()
	virtual render::Texture sharedTexture(SharedTextureHandle handle) const=0;
	// 
	virtual render::StructuredBuffer sharedStructuredBuffer(SharedStructuredBufferHandle handle) const=0;
	// ��� ��������� ��������� ��� ����� hlsl ��������
	virtual render::ConstantBuffer sharedParams() const=0;

	virtual const SharedParams* sharedParamsCpu() const { return nullptr; }
	// �������
	virtual render::IDump* getDump() const = 0;

	// from metacontext
	static inline IRenderContext* fromMeta(render::MetaContext& metaContext);
};

} //namespace render

#include "inl/IRenderContext.inl"

#endif