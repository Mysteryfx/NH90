#ifndef _gSunZW_h_
#define _gSunZW_h_

#include "DXMaterial.h"

namespace Graphics{
	class Context;
}

class gSunZW : public Graphics::DXMaterial
{
private:
	bool initialized;
	bool loadFail;

    float sunDist;
    unsigned sunSize;
    bool isOvercast;
    bool isUnder;

	Graphics::Context *context;

    render::Texture t1;
	render::Texture t2;
	render::Texture tb;

	render::Shader sh;

	render::handle_t shp_const5;
	render::handle_t shp_const6;
	render::handle_t shp_phase;
	render::handle_t shp_proj;

	render::handle_t shpT1;
	render::handle_t shpT2;

	render::Shader sh2;

	render::handle_t sh2p_const5;
	render::handle_t sh2p_const7;
	render::handle_t sh2p_viewproj;
	render::handle_t sh2p_cam;

	render::handle_t sh2p_diff;
	render::handle_t sh2p_sky;




	render::VertexBuffer vb;
	render::IndexBuffer ib;

	render::stream_handle_t stream1;

	render::handle_t tech;
	render::handle_t tech2;


public:
    gSunZW();
 
//////////////////////////////////////////////////////////////////////
    virtual const ed::string& ClassName();

//resource
   	virtual void DrawGeometry(Graphics::RenderObject *robject);

    virtual Graphics::MaterialKey GetKey() {return 0;}

    virtual int GetTextureCount(){return 2;}
    virtual render::Texture *GetTexture(int i) {return 0;}
	
    virtual bool IsTransparent() {return false;}

    virtual float GetLoadPriority(){return 3.0f;}
	virtual int GetLoadStep();
    virtual int GetLoadStepCount(){if (loadFail) return 0;else return 1;}
	virtual void LoadToStep(int step);
	virtual void FreeToStep(int step);
	
};

#endif
