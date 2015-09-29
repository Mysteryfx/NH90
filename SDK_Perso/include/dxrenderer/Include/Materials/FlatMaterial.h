#ifndef _LOCKON_FLAT_MATERIAL_H_
#define _LOCKON_FLAT_MATERIAL_H_

#include "MaterialBase.h"

namespace Graphics {

/// FlatMaterial material (default)
class FlatMaterial: public MaterialBase {
protected:

	render::handle_t _uMVP;

	render::handle_t _uDiffuseMap;
	render::handle_t _uOpacity;
	render::handle_t _uTextured;

	render::handle_t _uMatDiffuse;

	render::handle_t _opaque;
	render::handle_t _opaque_z;
	render::handle_t _transparent;
	render::handle_t _transparent_z;

	render::Texture _whiteMask;
public:

	/// Constructor \ Destructor
	DXRENDERER_API FlatMaterial();

	/// Material factory
	DXRENDERER_API static Resource *Create(ResourceType type, const ed::string& className);

	/// Draw material & geometry (geom in robject)
	DXRENDERER_API virtual void DrawGeometry(RenderObject *robject);

	/// Load material resources
	DXRENDERER_API virtual void LoadToStep(int step);
	DXRENDERER_API virtual void LoadWithDependents(int step);

	/// Free material resources
	DXRENDERER_API virtual void FreeToStep(int step);

	/// Compatible
	DXRENDERER_API virtual MaterialKey GetKey() { return 0; }
    DXRENDERER_API virtual MaterialCaps GetMaterialCaps() { return mcDiffuseMap; }
    DXRENDERER_API virtual bool IsTransparent();
	DXRENDERER_API virtual bool GetTwoSided();
};

}

#endif
