#ifndef _gSkyZW_h_
#define _gSkyZW_h_

#include "Renderer\RendererCallback.h"
#include "FogHelper.h"
#include "DXMaterial.h"
#include "gEMDraw.h"
#include "rwrappers/renderUserAPI.h"
#include "GraphMath/position.h"

#define SKYCLR_ARRAY_SIZE 24
#define SKYCLR_STEP 5

typedef struct SKY_CLR
{
	float Elev;

	float mAx;
	float mBx;
	float mCx;
	float mDx;
	float mEx;
	//
	float mAy;
	float mBy;
	float mCy;
	float mDy;
	float mEy;
	//
	float mAl;
	float mBl;
	float mCl;
	float mDl;
	float mEl;
	//
	float mZx;
	float mZy;

} SKY_CLR;

class gSkyZW : public Graphics::RendererCallback, public Graphics::DXMaterial
{
	friend class gEnvCubeZW;
	friend class gEMDraw;
public:
private:
	bool initialized;
	bool loadFail;

	float R;    //радиус купола
	float h;    //высота камеры над диаметральной плоскостью
	DWORD Nx;   //точек по горизонтали
	DWORD Ny;   //точек по вертикали
	//D3DXVECTOR4 skyBaseColor;
	//D3DXVECTOR4 skyTopColor;

	//physique
	float Y_zenith_abs;  //absolute zenith luminance
	float Turbidity;    //

	//LPDIRECT3DDEVICE9 m_pd3dDevice;		//Device Interface
	//LPDIRECT3DVERTEXBUFFER9 m_pVB;		//Vertex Buffer
	//LPDIRECT3DINDEXBUFFER9  m_pIB;		//Index Buffer
	//DWORD offsetVB;
	//DWORD offsetIB;

	//LPDIRECT3DVERTEXBUFFER9 m_pFlatVB;		//Vertex Buffer for flat onscreen quad
	//LPDIRECT3DINDEXBUFFER9  m_pFlatIB;		//Index Buffer for flat onscreen quad
	//DWORD offsetFlatVB;
	//DWORD offsetFlatIB;
	//IDirect3DVertexShader9 *wtrFlatVShader;

	//LPDIRECT3DTEXTURE9 OffScreenTarget;
	//LPDIRECT3DSURFACE9 surfRenderTarget;
//tango-lima-bravo
//	Graphics::DXProceduralTexture* DXProcTex;
//	Graphics::DXProceduralTexture* DXProcTex_Fog;
	//D3DXMATRIX mat;                         //matrix to hold View*Projection result

	DWORD        m_dwNumVertices;
	DWORD        m_dwNumIndices;
	//IDirect3DVertexShader9 *SkyVShader;			//Vertex Shader
	//IDirect3DPixelShader9 *SkyPShader;
	//IDirect3DVertexShader9 *FogVShader;
	//IDirect3DVertexShader9 *SkyVShader_Basic;
	//IDirect3DVertexShader9 *FogVShader_Basic;
	osg::Vec3f SunVector;
	bool isOvercast;
	bool isUnder;
	bool skySimple;

	SKY_CLR skyColors[SKYCLR_ARRAY_SIZE];

	//------------------------------------------
	render::Shader sh;
	render::handle_t tech;

	render::VertexBuffer vb;
	render::IndexBuffer ib;
	render::VertexBuffer svb;
	render::IndexBuffer sib;

	render::stream_handle_t stream;

	render::handle_t shp0;
	//render::handle_t shp1;
	//render::handle_t shp2;
	//render::handle_t shp3;
	render::handle_t shp4;
	//render::handle_t shp5;
	//render::handle_t shp6;
	render::handle_t shp7;
	render::handle_t shp8;
	render::handle_t shp9;

	render::handle_t shp10;
	render::handle_t shp11;
	render::handle_t shp12;
	render::handle_t shp13;
	render::handle_t shp14;
	render::handle_t shp15;
	render::handle_t shp16;
	render::handle_t shp17;
	render::handle_t shp18;
	render::handle_t shp19;

	render::handle_t shp20;

	render::handle_t shp31;
	render::handle_t shp32;

	render::handle_t shp50;

	//RendMan::SortKey rKey;

	render::Shader shq;
	render::handle_t techq;
	render::VertexBuffer qvb;
	render::IndexBuffer qib;
	render::stream_handle_t streamq;

	render::FrameBuffer rt;
	render::FrameBuffer wrt;

	render::handle_t shqp4;
	render::handle_t shqpTex;
	render::handle_t matViewProj;
	render::handle_t matInvProj;
	render::handle_t lightVec;
	render::handle_t camH;
	render::handle_t HDR;
	
	

//=======================================
	float lumA;
	float lumB;
	float lumC;
	float lumD;
	float lumE;

	float Ax;
	float Bx;
	float Cx;
	float Dx;
	float Ex;

	float Ay;
	float By;
	float Cy;
	float Dy;
	float Ey;

	float x_rad_zenith;
	float y_rad_zenith;
	float alt_adjust;
	float zenith_radiance;

	render::FogHelper fog;

protected:

	void makeGeometry(int Nx,int Ny, render::VertexBuffer& vb,render::IndexBuffer& ib);

	void getSkyYxy(osg::Vec3f& v,const osg::Vec4f& sunpos,float altitude,osg::Vec3f* dest);
	void drawSky(const osg::Vec4d& cam,const osg::Vec4f& sunpos,
		         const osg::Matrixf& view,const osg::Matrixf& proj,const osg::Matrixf& vp,bool full);


protected:
	virtual void DestroyAll();
	void CalcVertexColor2(const osg::Vec3f &v, const osg::Vec3f &sunPos, osg::Vec4f& dest);

	float GetRadiance(float A, float B,float C,float D, float E,float Cos_theta, float gamma);
	float GetRadiance2(float A, float B,float C,float D, float E,float Cos_theta, float gamma);
	void gDrawSkyGF3(const Position3 &camPos,const osg::Vec3f& sunPos, bool drawFog);

	bool IsOvercast(){return isOvercast;}

public:
	gSkyZW();
	~gSkyZW() {/*DestroyAll();*/}
	//BOOL initialize(LPDIRECT3DDEVICE9 DeviceIFace);
	//inline BOOL IsInitialized() {return initialized;}
	//inline void uninitialize() {DestroyAll();}

	//void gUpdateSky();//const cPosition& camPos,const cPoint& sunPos);
	//void gRenderSky();
	//LPDIRECT3DTEXTURE9 GetSkyTexture(){return OffScreenTarget;}
	void GetSunColor(osg::Vec4f& suncolor);
	//////////////////////////////////////////////////////////////////////
	virtual const ed::string& ClassName();
	//renderer callbacks

	virtual void OnUpdate();
	//////////////////////////////////////////////////////////////////////
	//DXMaterial
//	virtual Graphics::DXTexture* GetMainDiffuseMap() {return 0;}
	///////////////////
	//resource
	virtual void DrawGeometry(Graphics::RenderObject *robject);

	// сортировка и оптимизация
	virtual Graphics::MaterialKey GetKey() {return 0;}

	// список используемых текстур
	virtual int GetTextureCount(){return 1;}
	virtual render::Texture *GetTexture(int i) {return NULL;}

	// Полупрозрачный ли это материал
	virtual bool IsTransparent() {return false;}

	// управление загрузкой ресурса (может быть реализовано с поддержкой многопоточности)
	// загрузка производится по шагам, семантика шагов зависит от конкретного типа ресурса
	virtual float GetLoadPriority(){return 3.0f;}
	virtual int GetLoadStep();
	virtual int GetLoadStepCount(){if (loadFail) return 0;else return 1;}
	virtual void LoadToStep(int step);
	virtual void FreeToStep(int step);

	void CalcVertexColor(const osg::Vec3f &v, const osg::Vec3f &sunPos, float altitude, osg::Vec4f &dest);

	void UpdateFogColor();
};

#endif
