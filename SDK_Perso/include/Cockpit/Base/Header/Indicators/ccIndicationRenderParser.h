#pragma once;
#include "CockpitBase.h"
#include "Modeler/ModelParser.h"
#include "Renderer/RenderObject.h"
#include "Renderer/Color.h"

namespace Lua
{ 
    class Config;
}
namespace cockpit { 

struct COCKPITBASE_API opacity_material
{
	Graphics::Material* mat;
	float               initial_opacity;
};

typedef ed::list<opacity_material>		   ccOpacitySensitiveList;
typedef ed::list<Graphics::Material*>     ccMaterialList;

    
class COCKPITBASE_API ccIndicationRenderParser : public Graphics::RenderParser
{
	Graphics::RenderObject robject;
	Graphics::Material    *mfdmat;
	bool				   colored;
	int                    last_frame_rendered_with_targetmaterial;
	Graphics::Color                  shader_constant;
	bool                   inverted;
	bool                   reset_renderer_transform;
    ccOpacitySensitiveList opacity_sensitive_materials;   
    ccMaterialList		   color_sensitive_materials;
    ccMaterialList		   brightness_sensitive_materials;
    float                  brightness;
    float                  contrast;
public:
	ccIndicationRenderParser();
	ccIndicationRenderParser(Graphics::Material *_mfdmat, bool _colored);
	void operator()(Graphics::Material *_mfdmat, bool _colored);

    void                    change_opacity(float opacity);
    void                    change_color(const Vector3 & color_in);
    void                    change_brightness(float b)
    { 
        brightness = b;
    }


    void                    release();
	void                    InitFrame(int *objCounter, int *triCounter);
	void                    DrawAll(void* context);
	Graphics::RenderObject *GetNextObject(void* context);
	void                    OnObjectReady(Graphics::RenderObject *obj);
	int                     rendered_with_targetmaterial() const { return last_frame_rendered_with_targetmaterial;}
	bool				    is_colored() const {return colored;};
	bool                    is_inverted()const {return inverted;};
	const Graphics::Color  &get_shader_constant()const {return shader_constant;}
	Graphics::Material     *get_target_material()const {return mfdmat;};
    void                    clear_counter() { last_frame_rendered_with_targetmaterial = 0;}
    void                    read_from_state(Lua::Config & config);

	void set_inverted(bool flag_in) { inverted = flag_in;};
	void set_colored (bool flag_in) { colored = flag_in;};
	void set_reset_renderer_transform(bool flag_in)  {reset_renderer_transform = flag_in;};
	void set_shader_constant(float R,float G,float B,float A = 0) 
	{
		shader_constant.r = R;
		shader_constant.g = G;
		shader_constant.b = B;
		shader_constant.a = A;
	}
    void increase_render_target_counter()
    {
        last_frame_rendered_with_targetmaterial++;
    }
};

}