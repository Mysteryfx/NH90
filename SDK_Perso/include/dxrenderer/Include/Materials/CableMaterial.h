#ifndef _LOCKON_CABLE_MATERIAL_H_
#define _LOCKON_CABLE_MATERIAL_H_

#include "MaterialBase.h"
#include "FogHelper.h"

namespace Graphics {

/// Cable material (default)
class CableMaterial: public MaterialBase {
protected:

	render::handle_t _uMatShadow;
	render::handle_t _uLightView;
	render::handle_t _uLightUVDepth;
	render::handle_t _uShadowMap;

	render::handle_t _uDiffuseMap;
	render::handle_t _uEnvMap;

	render::handle_t _uMatView;
	render::handle_t _uMatProj;
	render::handle_t _uMatWorld;

	render::handle_t _uSunDir;
	render::handle_t _uSunDiffuse;
	render::handle_t _uSunAmbient;
	render::handle_t _uSunSpec;

	render::handle_t _uCameraPos;
	render::handle_t _uLighted;
	render::handle_t _uTwoSided;
	render::handle_t _uOpacity;
	render::handle_t _uGloss;

	render::handle_t _uMatSpec;
	render::handle_t _uMatEmissive;
	render::handle_t _uMatDiffuse;
	render::handle_t _uMatAmbient;

	render::handle_t _opaque;
	render::handle_t _opaqueMelt;
	render::handle_t _transparent;

	render::Texture _whiteMask;
	render::Texture _envMap;
	render::Texture _ambientMap;

	render::FogHelper _fogHelper;

	virtual void FillFlags();
public:

	/// Constructor \ Destructor
	DXRENDERER_API CableMaterial();
	DXRENDERER_API CableMaterial(const CableMaterial&);

	DXRENDERER_API virtual ~CableMaterial();

	/// Material factory
	DXRENDERER_API static Resource *Create(ResourceType type, const ed::string& className);

	/// Draw material & geometry (geom in robject)
	DXRENDERER_API virtual void DrawGeometry(RenderObject *robject);

	/// Load material resources
	DXRENDERER_API virtual void LoadToStep(int step);
	DXRENDERER_API virtual void LoadWithDependents(int step);

	/// Free material resources
	DXRENDERER_API virtual void FreeToStep(int step);
};

}

#endif
