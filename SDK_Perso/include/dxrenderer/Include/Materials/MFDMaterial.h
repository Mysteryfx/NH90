#ifndef _LOCKON_MFD_MATERIAL_H_
#define _LOCKON_MFD_MATERIAL_H_

#include "MaterialBase.h"

namespace Graphics {

/// Standart material (default)
class MFDMaterial: public MaterialBase {
protected:

	render::handle_t _colored;
	render::handle_t _BW_b_negative;
	render::handle_t _BW_b_positive;

	render::handle_t _uPower;
	render::handle_t _uBlendColor;
	render::handle_t _u_pc0;
	render::handle_t _u_pc1;
	render::handle_t _uB;
	render::handle_t _uC;
	render::handle_t _uMFDMap1;
	render::handle_t _uMFDMap2;

	render::Texture _whiteMask;
public:

	/// Constructor \ Destructor
	DXRENDERER_API MFDMaterial();
	DXRENDERER_API MFDMaterial(const MFDMaterial&);

	DXRENDERER_API virtual ~MFDMaterial();

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
};

}

#endif
