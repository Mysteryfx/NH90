#ifndef _LOCKON_LASERBEAM_MATERIAL_H_
#define _LOCKON_LASERBEAM_MATERIAL_H_

#include "MaterialBase.h"

namespace Graphics {

/// Standart material (default)
class LaserBeamMaterial: public MaterialBase {
protected:

	render::Texture _mask;

	render::handle_t _standart;
	render::handle_t _no_mask;

	render::handle_t _uIsOn;
	render::handle_t _uColor;
	render::handle_t _uMaskMap; 

	static bool _useMask;
public:

	/// Constructor \ Destructor
	DXRENDERER_API LaserBeamMaterial();
	DXRENDERER_API LaserBeamMaterial(const LaserBeamMaterial&);

	DXRENDERER_API virtual ~LaserBeamMaterial();

	/// Material factory
	DXRENDERER_API static Resource *Create(ResourceType type, const ed::string& className);

	/// Draw material & geometry (geom in robject)
	DXRENDERER_API virtual void DrawGeometry(RenderObject *robject);

	/// Load material resources
	DXRENDERER_API virtual void LoadToStep(int step);
	DXRENDERER_API virtual void LoadWithDependents(int step);

	/// Free material resources
	DXRENDERER_API virtual void FreeToStep(int step);

	DXRENDERER_API static void setUseMask(bool value);
};

}

#endif

