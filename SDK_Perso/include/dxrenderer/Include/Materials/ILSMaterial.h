#ifndef _LOCKON_ILS_MATERIAL_H_
#define _LOCKON_ILS_MATERIAL_H_

#include "MaterialBase.h"

namespace Graphics {

/// Standart material (default)
class ILSMaterial: public MaterialBase {
protected:
	ed::vector<ed::string> _textures_to_load;
	render::handle_t hTech, hTechAdditive, hTechWithoutTexture;
	render::handle_t hTechA8, hTechAdditiveA8;

	render::handle_t _uPower;
	render::handle_t _uBlendColor;
	render::handle_t _uTex;

	render::Texture _whiteMask;

public:

	/// Constructor \ Destructor
	DXRENDERER_API ILSMaterial();
	DXRENDERER_API ILSMaterial(const ILSMaterial&);

	DXRENDERER_API virtual ~ILSMaterial();

	/// Material factory
	DXRENDERER_API static Resource *Create(ResourceType type, const ed::string& className);

	/// Draw material & geometry (geom in robject)
	DXRENDERER_API virtual void DrawGeometry(RenderObject *robject);

	/// Load material resources
	DXRENDERER_API virtual void LoadToStep(int step);
	DXRENDERER_API virtual void LoadWithDependents(int step);

	/// Free material resources
	DXRENDERER_API virtual void FreeToStep(int step);

	DXRENDERER_API virtual float GetLoadPriority()	{return 3;}

	DXRENDERER_API void SetRenderTexture( int index, const char *name );

};

}

#endif
