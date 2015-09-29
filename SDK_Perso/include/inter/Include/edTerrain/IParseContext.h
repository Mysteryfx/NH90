#ifndef inter_edTerrain_IParseContext_h__
#define inter_edTerrain_IParseContext_h__

#include "stdint.h"
#include "ITerrainGraphicsConstants.h"
#include "IDump.h"

#ifdef EDGE
namespace edge
{
	class IRenderable;
	class Context;	// ������ ��� ������� ���!!!
}
#endif
namespace render 
{
	class Camera;
}
struct lua_State;

namespace render 
{

// ParseContext
// 1. �� �������������� � passIndex 
// 2. ��������� ��� �������
// 4. SampleHandle - ���� �������
// 5. Sampl� ���������������: 
//      �������, 
//      ������� ������� ������� ������� ����� ������������ ��� �������
class IParseContext
{
// ������ ���������� �� ITerrainGraphics
public:
	// ��������� �������
	virtual const lua_State* getOptions() const = 0;

	// model time (for procedural animations)
	virtual double time() const = 0;

	// origin
	virtual osg::Vec3d origin() const = 0 ;

	// clipmap center
	virtual osg::Vec3d clipmapCenter(bool* ortho=nullptr) const = 0 ;

	// ������ �������
	virtual int sampleCount() const = 0;
	virtual SampleHandle sampleHandle(int index) const = 0;
	
	// ������ ������, ��� �������. 
	// ������������ Camera::projection(), Camera::polytope()
	// � ��� PR_PERSP ��� Camera::nearPlane(), Camera::farPlane(), Camera::polytope(near, far)
	virtual const Camera* sampleCamera(const SampleHandle& sample) const = 0;

	// ������� ������ ������� ����� ������������ ��� ������� ������ (enShadingModel BitMask)
	virtual uint32_t sampleShadingModels(const SampleHandle& sample) const = 0;

	// �������
	virtual render::IDump* getDump() const =0;

#ifdef EDGE
	virtual edge::Context* getEdgeContext()=0;	// ������ ��� ������� ���!!!
	virtual void addRenderable(SampleHandle sample, edge::IRenderable* renderable, int sortingHint){};
#endif
};

}

#endif
