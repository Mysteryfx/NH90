#ifndef _gOvercast_h_
#define _gOvercast_h_

#include "weather.h"
#include "DXMaterial.h"
#include "Renderer\Light.h"

#include "rwrappers/renderUserAPI.h"
#include "FogHelper.h"

#include "osg/Vec3f"
#include "osg/Vec4f"
#include "osg/Matrixd"
#include "osg/Matrixf"

struct CLD_CLRO
{
	float Elev;
	osg::Vec4f clr;
};

struct OVC_PTCL
{
	osg::Vec3f pos;
	osg::Vec2f dot;
};

struct OMNILIGHT
{
	osg::Vec4f pos;
    float range;
	osg::Vec4f clr;
};


class WEATHER_API gOvercast : public Graphics::DXMaterial
{
//---------------------------------------------
	render::Shader sh;

	render::handle_t shp4;
	render::handle_t shpVP;
	render::handle_t shpT;
	render::handle_t shpTA;
	render::handle_t shpTB;
	render::handle_t shpExpTex;
	render::handle_t shpTSky;
	render::handle_t shpHFTex;
	render::handle_t shpC;
	render::handle_t shpclr;
	render::handle_t sh_techHi;
	render::handle_t sh_techLo;
	render::handle_t sh_techPtcls;

	render::handle_t sh_facecam;

	render::handle_t sh_cv11;
	render::handle_t sh_cv13;
	render::handle_t sh_cv20;
	render::handle_t sh_cv21;

	render::handle_t sh_cv5;
	render::handle_t sh_cv6;
	render::handle_t sh_cv7;
	render::handle_t sh_cv8;
	render::handle_t sh_cv40;
	render::handle_t sh_cv41;
	render::handle_t sh_cv50;
	render::handle_t sh_cv51;
	render::handle_t sh_cv52;
	render::handle_t sh_cv53;
	render::handle_t sh_cv54;


	render::handle_t sh_cp1;
	render::handle_t sh_cp2;



	render::VertexBuffer vb;
	render::IndexBuffer ib;

	render::VertexBuffer ptclVb;
	render::IndexBuffer ptclIb;

	render::VertexBuffer qvb;
	render::IndexBuffer qib;

	render::stream_handle_t streamPtcl,streamPtclUV, streamPos,qstreamPos;



	render::Texture tHi[16];
	render::Texture tLo;
	render::Texture tExpo;
	render::Texture texCldCover;
	render::Texture ptclMap;
	render::Texture flashTex;

	render::FogHelper fog;

	float borderHi;
	float borderLo;
//=================================
	render::Shader shOmni;
	render::handle_t sh_techOmni;
	render::handle_t sho_flashTex;
	render::handle_t sho_cv4;
	render::handle_t sho_cloudTex;
	render::handle_t sho_wrld;
	render::handle_t sho_clr;
	render::handle_t shoVP;
//=================================
//---------------------------------------------
private:
    bool loadFail;
	bool initialized;

    DWORD offsetVB;
    DWORD offsetIB;

    DWORD offsetFlatVB;
    DWORD offsetFlatIB;

    DWORD offsetPtclVB;
    DWORD offsetPtclIB;

    DWORD offsetQuadVB;

//tango-charlie
//    Graphics::DXTexture* texTargHi[16];

    float albedo;
    float tau;

    float lightDist;
    //DWORD integration_steps;
    //float h;
    float heightHi;
    float heightLo;

    float coverSize;

	float fake_sun_height;

	float ptclSize;
	float gridSize;
	DWORD numPtcls;

    DWORD Nx;
    DWORD Ny;

    DWORD targSize;
	DWORD numVertices;
	DWORD numIndices;

    DWORD plashkaSize;

//tango-charlie
//    Graphics::DXTexture* dsk;
//    Graphics::DXTexture* dskHi;

    CLD_CLRO* cloudColors;
    CLD_CLRO* cloudColorsHi;

    osg::Vec4f cldclr;
    DWORD CLR_ARRAY_SIZE;
    DWORD CLD_CLR_STEP;
//-----------------------------
    OMNILIGHT omnis[16];
    int numOmnis;
//-----------------------------
    bool canDoShaders;

 
    DWORD offsetVB_Bud;
    DWORD offsetVB_Bud_Lo;
    DWORD offsetFlatVB_Bud;

    osg::Vec3f *ptclPosArray;
//-----------------------------
	void AddOmni(Graphics::Light* l,const osg::Vec4f& position);
//-----------------------------
        void DestroyAll();
//    float Phase(osg::Vec4f& incident,osg::Vec4f& scattered);

//    bool UpdateVVCubeMap();
//	void CalcRenorm(DWORD x,DWORD y,DWORD z,BYTE* mapper);
/*
    void TraceRayHi(int x,int z, osg::Vec4f& sun,BYTE* mapperTarg,BYTE* mapperRef,
                  DWORD pitchTarg, DWORD pitchRef);
    void TraceRayLo(int x,int z, osg::Vec4f& sun,BYTE* mapperTarg,BYTE* mapperRef,
                  DWORD pitchTarg, DWORD pitchRef);

    float GetDensity(int x,int y, int z, BYTE* mapperRef, DWORD pitchRef);
    float MapRef(int x,int y,int z,BYTE* mapperRef, DWORD pitchRef);

    void GenHi(osg::Vec4f& cam, osg::Vec4f& light, bool force_regen);
    void GenLo(osg::Vec4f& cam, osg::Vec4f& light, bool force_regen);
*/

	void DrawHi(osg::Vec4f& cam, osg::Vec4f& lightVector,osg::Matrixf& view,osg::Matrixf& proj);
	void DrawLo(osg::Vec4f& cam, osg::Vec4f& lightVector,osg::Matrixf& view,osg::Matrixf& proj);
	void DrawPtcls(osg::Vec4f& cam, osg::Vec4f& lightVector,osg::Matrixf& view,osg::Matrixf& proj);


    //void DrawHi(osg::Vec4f& cam, osg::Vec4f& light,osg::Matrixf& m_matView,osg::Matrixf& m_matProj,LPDIRECT3DBASETEXTURE9 SkyTex);
    //void DrawPtcls(osg::Vec4f& cam, osg::Vec4f& light, osg::Matrixf& m_matView,osg::Matrixf& m_matProj);
    bool initialize();

    void DrawHi_Budget(osg::Vec4f& cam, osg::Vec4f& light,osg::Matrixf& m_matView,osg::Matrixf& m_matProj);
    void DrawLo_Budget(osg::Vec4f& cam, osg::Vec4f& light, osg::Matrixf& m_matView,osg::Matrixf& m_matProj);
    void DrawPtcls_Budget(osg::Vec4f& cam, osg::Vec4f& light, osg::Matrixf& m_matView,osg::Matrixf& m_matProj);

    void DrawOmnis(osg::Vec4f& cam,float h,osg::Matrixf& m_matView,bool low,float talf);
	void DrawOmnis(osg::Vec4f& cam,float h,osg::Matrixf& view, osg::Matrixf& proj,bool low,float talf);



//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


////////////////////////////////////////////////////////
public:
    void ResetOmnis(){numOmnis=0;}
    //virtual void DrawLo(osg::Vec4f& cam, osg::Vec4f& light, osg::Matrixf& m_matView,osg::Matrixf& m_matProj,LPDIRECT3DBASETEXTURE9 SkyTex);
	virtual void DrawLo_(osg::Vec4f& cam, osg::Vec4f& lightVector,osg::Matrixf& view,osg::Matrixf& proj)
	{
			DrawLo(cam, lightVector,view,proj);
	}



    gOvercast();
    ~gOvercast(){/*if (initialized) DestroyAll();*/}

    
    virtual const ed::string& ClassName();
//renderer callbacks

//	virtual void OnUpdate();
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
    virtual render::Texture *GetTexture(int i);// {return 0;}//forGrisha;}
	
	// Полупрозрачный ли это материал
    virtual bool IsTransparent() {return false;}

	// управление загрузкой ресурса (может быть реализовано с поддержкой многопоточности)
	// загрузка производится по шагам, семантика шагов зависит от конкретного типа ресурса
    virtual float GetLoadPriority(){return 1.0f;}
    virtual int GetLoadStep(){if (initialized) return 1;else return 0;}
    virtual int GetLoadStepCount(){if (loadFail) return 0;else return 1;}
    virtual void LoadToStep(int step){if (step>0) initialize();}
    virtual void FreeToStep(int step){if (step==0) DestroyAll();}
};


#endif
