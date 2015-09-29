#ifndef InfraRedMaterialImpl_H
#define InfraRedMaterialImpl_H

#include "rwrappers/renderUserAPI.h"
#include "Renderer/DynamicParams.h"
#include <ed/map.h>

namespace Graphics
{

class InfraRedMaterialImpl
{
public:
	static const int MATERIAL_TEX_COUNT     = 1;
	static const int MATERIAL_TEX_OPTICAL   = 0;

	static const int PIXEL_SHADER_COUNT     = 3;
	static const int PIXEL_SHADER_O2R       = 0;
	static const int PIXEL_SHADER_BLUR      = 1;
	static const int VERTEX_SHADER_DEFAULT  = 2;
	
	static const int VERTEX_BUFFER_COUNT    = 3;
	static const int VERTEX_BUFFER_CIRCLE   = 0;
	static const int VERTEX_BUFFER_SECTOR   = 1;
	static const int VERTEX_BUFFER_SPRITE   = 2;
	
protected:

    bool initialized;
	
    render::handle_t hOpticalTexture;

	int tex_filter_method;
	bool    rescan_needed;

	void ClearTargets();

	// эффект 
	render::Shader _effect;
	// вертексный буффер
	render::VertexBuffer _vb;
	// индексный буффер
	render::IndexBuffer _ib;

    render::FrameBuffer _tempFrameBuffer;

	render::stream_handle_t _coordStream;
    render::stream_handle_t _textureStream;

    render::ve_handle_t _hPOSITION0;
    render::ve_handle_t _hTEXCOORD0;

    render::handle_t _hInfraRedParams;
    render::handle_t _hTechniqueO2I;
    render::handle_t _hTechniqueBlur;

public:
	// конструктор/деструктор
	InfraRedMaterialImpl();
    ~InfraRedMaterialImpl() {};

    void create();

	void draw(render::FrameBuffer &mfdTexture, render::Texture& opticalTexture, DParamList& params);

};

} // namespace Graphics

#endif // InfraRedMaterialImpl_H
