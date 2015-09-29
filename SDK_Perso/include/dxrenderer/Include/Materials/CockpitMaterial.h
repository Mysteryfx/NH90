#ifndef _LOCKON_COKPIT_MATERIAL_H_
#define _LOCKON_COKPIT_MATERIAL_H_

#include "MaterialBase.h"

namespace Graphics {

/// Standart material (default)
class CockpitMaterial: public MaterialBase {
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

	render::handle_t _opaque, _opaque_tex;
	render::handle_t _opaque_z,_opaque_z_tex;
	render::handle_t _transparent,_transparent_tex;
	render::handle_t _ambient_tex, _omni_tex;

	render::Texture _whiteMask;
	render::Texture _envMap;
	render::Texture _ambientMap;

	bool haveUV;
	virtual bool setLightTech(int type, render::handle_t &tech);

public:

	/// Constructor \ Destructor
	DXRENDERER_API CockpitMaterial();
	DXRENDERER_API CockpitMaterial(const CockpitMaterial&);

	DXRENDERER_API virtual ~CockpitMaterial();

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
