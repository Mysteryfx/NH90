#ifndef ED_RENDERER_FOG_HELPER_H
#define ED_RENDERER_FOG_HELPER_H

#include "rwrappers/renderUserAPI.h"

#ifdef EDGE
namespace edge{
	class Context;
}

#include "config.h"

#ifndef GRAPHICS_EXPORT
    #define GRAPHICS_EXPORT EDSIM_DLL_IMPORT
#endif

#define DXRENDERER_API GRAPHICS_EXPORT

#else
#include "DXRendererAPI.h"

namespace Graphics{
	class DXEffect;
};
#endif

namespace edcg {
	class Fog;
}

namespace render {

/// Class to set fog in models.
class FogHelper{
public:
	enum Handles{
		FOG_COEFFICIENTS,
		FOG_COLOR,
		FOG_MAP,
		
		LAST_HANDLE
	};
private:
	::render::handle_t _handles[LAST_HANDLE];
	static osg::Vec4f COLOR;
	::render::Texture _fogMap;
public:
	DXRENDERER_API static void setFogColor(const osg::Vec3f& color);

	DXRENDERER_API FogHelper();
	~FogHelper(){}

	/// Initializes fog and fog parameters.
	DXRENDERER_API void init(::render::Shader &shader);

	/// Sends all fog uniforms and texture to effect.
	DXRENDERER_API void apply(::render::Shader &shader, const edcg::Fog &fog, double camHeight);

	/// Sends all fog uniforms and texture to effect.
	/// WARNING! \param shader must be the same as was passed to initHandles method.
#ifndef EDGE
	DXRENDERER_API void apply(::render::Shader &shader, const edcg::Fog &fog);
#else
	DXRENDERER_API void apply(::render::Shader &shader, const edge::Context &ctx);
#endif
};

}

#endif
