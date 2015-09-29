#ifndef DXRenderer_H
#define DXRenderer_H

#include <stdlib.h>
#include <search.h>

#include "Renderer\Renderer.h"
#include "DXRendererAPI.h"
#include "Renderer\Camera.h"
#include "Renderer\VolumeEffect.h"
#include "Renderer\Fog.h"

#include "rwrappers/Enums.h"
#include "rwrappers/renderUserAPI.h"

#include <ed/unordered_map.h>
#include <stack>
#include "osg/Matrixf"


namespace SLI
{
	bool	DXRENDERER_API	SLI_Enabled();

	inline void clearFrameBuffer(const osg::Vec4f& color = osg::Vec4f(0, 0, 0, 0), float z = 1.0f, unsigned int stencil = 0) {
		if(SLI_Enabled())
			render::clearFrameBuffer(color, z, stencil);
	}
	inline void clearColor(const osg::Vec4f& value = osg::Vec4f(0, 0, 0, 0)) {
		if(SLI_Enabled())
			render::clearColor(value); 
	}
	inline void clearColor(const ed::vector<osg::Vec4f>& values) {
		if(SLI_Enabled())
			render::clearColor(values);
	}
	inline void clearDepth(float value = 1.0f) {
		if(SLI_Enabled())
			clearDepth(value);
	}
	inline void clearStencil(unsigned int value = 0) {
		if(SLI_Enabled())
			clearStencil(value);
	}

}


namespace Graphics
{

class DXMesh;
class DXLight;
class DXCubeTexture;


// CW/CCW в зависимости от того, правая или левая тройка камеры используется
extern DXRENDERER_API DWORD DEFAULT_CULLMODE;

#define DXRERR_NOCOMPATIBLEDEVICES          0x82000001
#define DXRERR_HALNOTSUPPORTED              0x82000002
#define DXRERR_NODXSUPPORT                  0x82000003
#define DXRERR_NOSUCHADAPTER                0x82000004
#define DXRERR_NODEPTHSUPPORT               0x82000005


// get D3D9 depth bias from D3D8 value (0-15)
DXRENDERER_API DWORD GetDepthBias(int value);

/////////////////////////////////////////////////////

class DXRENDERER_API DXRenderer : public Renderer
{
private:
    bool initialized;

	Camera cur_cam;
    RenderMode cur_mode;

	effectState *cur_estate;
	bool cur_useprojlights;
	bool es_second_pass, es_advanced;

    class DXProjLightMgr;
    friend class DXProjLightMgr;
//==раз уж здесь все равно помойка==
	int batches, tries;

//==================================
protected:

	int _isHDR;
	bool _isAmbientMap;
    int _shadowLevel;
	Context *_context;
   
	DXLight *Sun;
	DXLight *_moon;
    
	edcg::Fog fog;

	osg::Matrixf cloudShadowMatrix;
	render::Texture cloudShadowsTex, _dummyWhite;

	typedef ed::vector<RendererCallback *>  callbackSet;
	typedef ed::unordered_map<RendererCallbackEvent, callbackSet *> callbackList;
	callbackList callbacks;

public:
    DXRenderer();
    ~DXRenderer();

	virtual void Reset();

	render::Texture getCloudShadowsProjNTexture(osg::Matrixf& proj);
	void setCloudShadowsProjNTexture(const osg::Matrixf& _m, render::Texture _t);

	// context
	virtual void SetContext(Context *context);
	virtual Context *GetContext();
	virtual const Context *GetContext() const;
    
    // инициализация/деинициализация
    virtual bool isinitialized() {return initialized;}

	virtual bool initialize(void *window, int width=-1, int height=-1, int fullscreen=-1);
    virtual void uninitialize();

	// регистрация call-back
	virtual void RegisterCallback(RendererCallback *callback, RendererCallbackEvent where);
	virtual void UnregisterCallback(RendererCallback *callback, RendererCallbackEvent where);

	// управление камерой
	virtual void SetCamera(const Camera &cam);
	virtual const Camera &GetCamera();
    
    // управление фазами отрисовки
    virtual void Clear(
		bool bRenderTarget=true, bool bZBuffer=true, bool bStencil=true, 
		const Vector3* pcolor=NULL);
    virtual void Update();

    virtual void BeginScene();
    virtual bool Present();
	virtual void EndFrame();

	virtual void BeginParse();
	virtual void EndParse();
	virtual void OnParseCallback();
    virtual void SetRenderMode(RenderMode mode);
    virtual RenderMode GetRenderMode();
	
    // отправка объектов на отрисовку
    virtual void DrawObject(RenderObject* robject);
    virtual void DrawObjects(RenderObject** robject, unsigned int n);
	virtual void DrawRenderText(RenderText* rtext, bool bDontUseGlobalVP=false);

 	void    FreeResources();

	void	SetSun(DXLight *sun) {Sun = sun;}
	DXLight *GetSun()	{return Sun;}

	void	SetMoon(DXLight *moon) {_moon = moon;}
	DXLight *GetMoon()	{return _moon;}
	
	virtual bool isHDR() const;
	virtual void setHDR(int v);

	virtual int	getShadowLevel() const;

	// освещение гражданских объектов
	virtual float getCivilLights()const{return civillights;};
	virtual void setCivilLights(float civillights){this->civillights = civillights;}

protected:

// LandLightMap
public:
	virtual void setLandLightMap(osg::Matrixf& ttf, const render::Texture& texture);
	virtual render::Texture getLandLightMap(osg::Matrixf& ttf)const;
protected:
	osg::Matrixf landLightMap_TTF;

public:
	// выдать для следующего кадра отладочную информацию
	virtual void dumpNextFrame();

	// откючает draw
	bool bDebugMode;
	void setDebugMode(bool bDebugMode){this->bDebugMode=bDebugMode;};
	bool getDebugMode(){return bDebugMode;};

	// освещение гражданских объектов
	float civillights;

	// если требуется отладолчная информация по кадру
	bool bDumpNextFrame;
	FILE* dumpfile;
	FILE* getDumpFile();
	void startDumpFrame();
// frame statistic
public:
	void ClearStat();
	void UpdateStat(int batches, int tries);
	void GetStat(int& batches, int& tries);

private:

    bool  ParseEffectState( effectState *effState , bool use_proj_lights );
    void  DrawNormalLights( RenderObject** array, int count );
    void  DrawProjLights( RenderObject** array, int count,
                          bool advanced );

    class DXProjLightMgr
    {
    public:

        DXProjLightMgr( DXRenderer *r ) :
            renderer(r), ready(false), supported(false),
            zbias(0), lightsCreated(false),rnmap(0)
        {}

        void  PushLight( VolumeEffect *light );
        bool  CreateProjLights();
        void  Draw( RenderObject** array, int count, bool adavnced );
        void  Free();
        void  Flush();
        void  ClearLights();
        void  ResetRenderStates();

    private:

        bool  IsShadersSupported();
        int   UseProjLights( bool advanced, int from );
		void  UnuseProjLights( bool advanced, int from);
        void  SetLightsData( const osg::Matrixf &world, int from, bool advanced );
        void  UseLightsShaders( int hasDiffuseMap, int lightsCount );
        void  UnuseLights( bool advanced );
        bool  CreateLightData( int hasDiffuseMap, int lightsCount );
        bool  SetOmniShaders(const osg::Matrixf & wt, int from, bool hasSkin);

        DWORD GetShaderkey( int n, int dm );

		effectState  projLights,omniLights;

        DXCubeTexture* rnmap;

        DXRenderer *renderer;

        bool        ready,
                    supported;

        bool        lightsCreated;

        int         zbias;
    };
    
    DXProjLightMgr projLightMgr;    

    DWORD m_CurrentChar, m_MaxChars;
    bool  m_bFontModeFast;

public:    

	virtual edcg::Fog& getFog();
	virtual const edcg::Fog& getFog()const;

};

} // namespace Graphics

extern DXRENDERER_API Graphics::DXRenderer *&dxrenderer;

#endif // DXRenderer_H

