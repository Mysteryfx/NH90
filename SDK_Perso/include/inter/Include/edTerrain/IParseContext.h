#ifndef inter_edTerrain_IParseContext_h__
#define inter_edTerrain_IParseContext_h__

#include "stdint.h"
#include "ITerrainGraphicsConstants.h"
#include "IDump.h"

#ifdef EDGE
namespace edge
{
	class IRenderable;
	class Context;	// только для рендера едм!!!
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
// 1. не заморачиваться с passIndex 
// 2. описывает ВСЕ выборки
// 4. SampleHandle - одна выборка
// 5. Samplе характеризуется: 
//      камерой, 
//      набором шейдинг моделей которые будут использованы для рендера
class IParseContext
{
// Методы вызываемые из ITerrainGraphics
public:
	// Настройки рендера
	virtual const lua_State* getOptions() const = 0;

	// model time (for procedural animations)
	virtual double time() const = 0;

	// origin
	virtual osg::Vec3d origin() const = 0 ;

	// clipmap center
	virtual osg::Vec3d clipmapCenter(bool* ortho=nullptr) const = 0 ;

	// Массив выборок
	virtual int sampleCount() const = 0;
	virtual SampleHandle sampleHandle(int index) const = 0;
	
	// камера семпла, для выборок. 
	// используется Camera::projection(), Camera::polytope()
	// а для PR_PERSP еще Camera::nearPlane(), Camera::farPlane(), Camera::polytope(near, far)
	virtual const Camera* sampleCamera(const SampleHandle& sample) const = 0;

	// шейдинг модели которые будут использованы для рендера семпла (enShadingModel BitMask)
	virtual uint32_t sampleShadingModels(const SampleHandle& sample) const = 0;

	// отладка
	virtual render::IDump* getDump() const =0;

#ifdef EDGE
	virtual edge::Context* getEdgeContext()=0;	// только для рендера едм!!!
	virtual void addRenderable(SampleHandle sample, edge::IRenderable* renderable, int sortingHint){};
#endif
};

}

#endif
