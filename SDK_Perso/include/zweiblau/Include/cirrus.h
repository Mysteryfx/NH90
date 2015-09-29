#ifndef _cirrus_h_
#define _cirrus_h_

//#include "Renderer\RendererCallback.h"
#include "DXMaterial.h"
#include "rwrappers/renderUserAPI.h"
#include "FogHelper.h"

#include "osg/Vec3f"
#include "osg/Vec4f"
#include "osg/Matrixd"
#include "osg/Matrixf"

namespace Graphics{
	class Context;
}

#define MAX_CIRRUS_PHASE 1

struct CLD_CLRC
{
	float Elev;
	osg::Vec4f clr;
};


class Cirrus : public Graphics::DXMaterial
{
	bool initialized;
	bool loadFail;

	CLD_CLRC* cloudColors;
//------------------------------------	
	Graphics::Context *context;

    render::Texture t[2][2];//MAX_CIRRUS_PHASES][MAX_CIRRUS_SHAPES];
	render::Shader sh;

	render::handle_t shp4;
	render::handle_t shpVP;
	render::handle_t shpT;
	render::handle_t shpT2;
	render::handle_t shpTime;
	render::handle_t shpPhase;
	//render::handle_t shpTSky;
	render::handle_t shpC;
	render::handle_t shpclr;
	//render::SortKey rKey;

	render::VertexBuffer vb;
	render::IndexBuffer ib;

	render::stream_handle_t stream1;
	render::stream_handle_t stream2;

	render::handle_t tech;
//------------------------------------

	render::FogHelper fog;

	float _phase;
protected:
	void DestroyAll();

public:
	// фабрика
	static Resource *Create(ResourceType type, const ed::string& className);

    Cirrus();
    ~Cirrus() {DestroyAll();}
    
//////////////////////////////////////////////////////////////////////
    virtual const ed::string& ClassName();
//renderer callbacks

	//////////////////////////////////////////////////////////////////////
//DXMaterial
//    virtual Graphics::DXTexture* GetMainDiffuseMap() {return 0;}
///////////////////
//resource
   	virtual void DrawGeometry(Graphics::RenderObject *robject);

	// сортировка и оптимизация
    virtual Graphics::MaterialKey GetKey() {return 0;}

	// список используемых текстур
    virtual int GetTextureCount(){return 2;}
    virtual render::Texture *GetTexture(int i) {return 0;}
	
	// Полупрозрачный ли это материал
    virtual bool IsTransparent() {return false;}

	void	setContext(Graphics::Context *Context) { context = Context; }

	// управление загрузкой ресурса (может быть реализовано с поддержкой многопоточности)
	// загрузка производится по шагам, семантика шагов зависит от конкретного типа ресурса
    virtual float GetLoadPriority(){return 3.0f;}
	virtual int GetLoadStep();
    virtual int GetLoadStepCount(){if (loadFail) return 0;else return 1;}
	virtual void LoadToStep(int step);
	virtual void FreeToStep(int step);
	
};

#endif
