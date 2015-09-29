#ifndef DXMaterial_H
#define DXMaterial_H

#include "DXRendererAPI.h"
#include "vector.h"
#include "Renderer\Material.h"
#include "rwrappers\renderUserAPI.h"

namespace Graphics
{

#define MAX_TEXTURE_UNITS_PER_BATCH (16)

// интерфейс материалов DirectX
class DXMaterial : public Material
{
protected:

	render::Shader _shader;
public:
	// можно ли накладывать проективные источники света универсально
	DXRENDERER_API virtual bool AllowProjLights();

	//tango-lima-bravo
	// для универсального наложения проективных источников света - основная текстура или цвет
    //DXRENDERER_API virtual DXTexture * GetMainDiffuseMap();
    DXRENDERER_API virtual Vector3 *GetMainDiffuseColor();

	DXRENDERER_API virtual int GetLoadStepCount();

	DXRENDERER_API virtual float GetLoadPriority();

	/// Returns if material can render in IR mode.
	virtual bool CanRenderIR()const{return false;}

	/// Renders in IR mode.
	virtual void RenderIR(RenderObject *robject){}

	inline render::Shader &getShader() {return _shader;}
};

} // namespace Graphics

#endif // DXMaterial_H
