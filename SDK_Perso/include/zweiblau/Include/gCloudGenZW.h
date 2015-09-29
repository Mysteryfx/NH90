#ifndef _gCloudGenZW_h_
#define _gCloudGenZW_h_

#include "DXMaterial.h"
#include "Renderer\RendererCallback.h"
#include "Renderer\VolumeEffect.h"

//#include "ZweiBlau.h"
#include "ZweiBlauAPI.h"
#include "gCloudsZW.h"

#include "DblLink.h"
#include "gEMDraw.h"

#include "rwrappers/renderUserAPI.h"
#include "Osg/Vec2f"
#include "Osg/Vec3f"
#include "Osg/Vec4f"
#include "Osg/Vec4d"
#include "osg/Matrixd"
#include "osg/Matrixf"
#include "Osg/BoundingBox"


#include <ed/string.h>
#include <ed/map.h>

//typedef struct CLD_CLRR
//{
//	float Elev;
//	D3DXVECTOR4 clr;
//} CLD_CLRR;
//
//typedef struct VERTEX8
//{
//	D3DXVECTOR3 pos;
//	D3DXVECTOR3 nor;
//	D3DXVECTOR2 clr;
//}VERTEX8;
//
//typedef struct VERTEX12
//{
//	D3DXVECTOR3 pos;
//	D3DXVECTOR3 nor;
//	D3DXVECTOR2 clr;
//	D3DXVECTOR3 trans;
//}VERTEX12;
//
//typedef struct CLD_SHAPE
//{
//	//LPDIRECT3DVERTEXBUFFER9 m_pVB;		//Vertex Buffer
//	LPDIRECT3DINDEXBUFFER9  m_pIB;		//Index Buffer
//	VERTEX8* mesh;
//	WORD* main_indices;
//	DWORD num_ptcls;
//	DWORD offsetVB;
//	Box unitBoundBox;
//} CLD_SHAPE;
//
//typedef struct CLOUD
//{
//	//=world==
//	D3DXVECTOR4 pos;
//	float size;
//	DWORD shape_index;
//	DWORD num_ptcls;
//	float bs_radius;	//radius of bounding sphere
//
//	D3DXVECTOR4 front;	//last update front point, world
//	D3DXVECTOR4 rear;  // last update rear point, world
//
//	float dist;			//distance to camera
//	bool straightMap;
//	bool lastWasStraight;
//	float Aspect;
//	D3DXVECTOR4 view_vector; //vector to camera, world
//	//D3DXVECTOR4 bounds[8];
//	Box boundBox;
//	//world to screen
//	D3DXVECTOR4 shifts;  // shifters to move object into port, view space
//	D3DXVECTOR4 spans;   // x,y,z spans, view space
//	D3DXVECTOR4 scalers; // scalers to squeeze/expand object to fit into unit port
//	float rwidth;
//	float rheight;
//	float mindistance;
//	float ptcl_size;
//
//
//	//impostor
//	bool imp_exist;     //whether impostor exists for this cloud
//	DWORD last_LOD;
//	DWORD new_LOD;
//
//	D3DXVECTOR4 unit_shifts;  // shifters to move object into position, pp unit space
//	D3DXVECTOR4 unit_scalers; // scalers to squeeze/expand object to fit into unit port
//
//	DWORD targ_pos_index;  //index of target texture window assigned to this impostor
//	D3DXVECTOR2 targ_uv;   //offsets to assigned target window in unit texture coords(u,v)
//	float uv_span_x;
//	float uv_span_y;
//
//	D3DXVECTOR2 scr_offsets;  //offsets to target window from target's upper left corner
//	float scr_span;		//size of target window, in texels
//
//	bool UsedLastFrame;
//	bool UsingThisFrame;
//	D3DXMATRIX cld_rpy;	//rotation of texcoords inside volume noise
//	D3DXMATRIX lt;		//local transform
//	D3DXMATRIX lt2;		
//	D3DXMATRIX ilt;		//impostor local transform
//	float lastAccessTime;
//
//} CLOUD;
//
//typedef KidList<CLOUD> CloudList;

typedef unsigned short WORD;
typedef unsigned long DWORD;

struct CLOUD
{
	//=world==
	osg::Vec4d pos;
	float size;
	DWORD shape_index;
	DWORD num_ptcls;
	float bs_radius;	//radius of bounding sphere

	osg::Vec4f front;	//last update front point, world
	osg::Vec4f rear;  // last update rear point, world

	float dist;			//distance to camera
	bool straightMap;
	bool lastWasStraight;
	float Aspect;
	osg::Vec4f view_vector; //vector to camera, world
	//D3DXVECTOR4 bounds[8];
	osg::BoundingBox boundBox;
	//world to screen
	osg::Vec4f shifts;  // shifters to move object into port, view space
	osg::Vec4f spans;   // x,y,z spans, view space
	osg::Vec4f scalers; // scalers to squeeze/expand object to fit into unit port
	float rwidth;
	float rheight;
	float mindistance;
	float ptcl_size;


	//impostor
	bool imp_exist;     //whether impostor exists for this cloud
	DWORD last_LOD;
	DWORD new_LOD;

	osg::Vec4f unit_shifts;  // shifters to move object into position, pp unit space
	osg::Vec4f unit_scalers; // scalers to squeeze/expand object to fit into unit port

	DWORD targ_pos_index;  //index of target texture window assigned to this impostor
	osg::Vec2f targ_uv;   //offsets to assigned target window in unit texture coords(u,v)
	float uv_span_x;
	float uv_span_y;

	osg::Vec2f scr_offsets;  //offsets to target window from target's upper left corner
	float scr_span;		//size of target window, in texels

	bool UsedLastFrame;
	bool UsingThisFrame;
	osg::Matrixf cld_rpy;	//rotation of texcoords inside volume noise
	osg::Matrixf lt;		//local transform
	osg::Matrixf lt2;		
	osg::Matrixf ilt;		//impostor local transform
	float lastAccessTime;

	int inscatterID;

};

typedef KidList<CLOUD> CloudList;

struct CLD_CLRR
{
	float Elev;
	osg::Vec4f clr;
};

struct VERTEX8
{
	osg::Vec3f pos;
	osg::Vec3f nor;
	osg::Vec2f clr;
};

struct VERTEX12
{
	osg::Vec3f pos;
	osg::Vec3f nor;
	osg::Vec2f clr;
	osg::Vec3f trans;
};

struct CLD_SHAPE
{
	render::IndexBuffer ib;		//Index Buffer
	VERTEX8* mesh;
	WORD* main_indices;
	DWORD num_ptcls;
	DWORD offsetVB;
	osg::BoundingBox unitBoundBox;
};

struct ShadowDesc
{
	osg::Vec3d origin;
	osg::Vec4d frustrum[6];
	double farClip;
	double nearClip;
	//
	osg::Matrixf viewproj;
	osg::Matrixf viewtrapezoid;
	osg::Matrixf texproj;
	osg::Matrixf lightproj;
	//RendAPI::ITexture* shadowtex;

	osg::Vec4f lightdir;
};

enum AXIS
{
	X=0,
	Y=1,
	Z=2,
};



class ZWEIBLAU_API gCloudGenZW : /*public ZweiBlau,*/ public gCloudsZW, public Graphics::RendererCallback,
	public Graphics::DXMaterial
{

	Box GrishinBox;
	Vector3 GrishinVector;

//-----------------------------------
//***********************************
//-----------------------------------
	KidList<CLOUD> clouds;
	CLOUD** pclouds;
	CLOUD** aspect_que;
	VERTEX8** ique;
	DWORD maxptcls;
	bool* freeSlots; //freeSlots[targ_pos_index] is TRUE if slot is free, FALSE if occupied


	VERTEX12* main_mesh;

	CLD_SHAPE* shapes;
	DWORD plashka_size;

    // imposter texture size
	DWORD cldTargSizeX;
	DWORD cldTargSizeY;

    // maximum/minimum size for one imposter
    DWORD maxImposterSize;
    DWORD minImposterSize;

    DWORD nonMinLodCount;   // number of clouds possible to draw at non-minimum size
    DWORD minLodLine;       // line on imposter texture where minimum lod starts

	osg::Vec4f Light;
	float aspect_threshold;

	float shift_x;
	float shift_y;
	float shift_z;

	float scaler_x;
	float scaler_y;
	float scaler_z;


	float span_x;
	float span_y;
	float span_z;

	float scale;

	DWORD num_VisibleClouds;

	bool initialized;
	bool Updated;
	bool m_bRegistered;

	render::VertexBuffer vb;
	render::stream_handle_t stream;
	render::IndexBuffer ib;

	render::VertexBuffer tvb;
	render::stream_handle_t tvb_stream0;
	render::stream_handle_t tvb_stream1;
	render::stream_handle_t tvb_stream2;
	render::stream_handle_t tvb_stream3;

	render::Texture texTan;

	render::Texture texNoise;


	render::FrameBuffer target1;
	render::FrameBuffer shadowTarget;
//	render::RenderTarget shadowTarget;

	render::Texture shadTex;




	render::Shader CldShader;


	render::Shader DrawShader;

	render::Shader ImpostShader;

	render::Shader PtclShader;

	render::Texture PtclMap;
	render::Texture GradMap;

	DWORD NUM_CLOUDS;
	DWORD NUM_SHAPES;

	//Box GrishinBox;
	//Vector3 GrishinVector;

	bool must_init_clouds;
	DWORD total_verts;
	CLD_CLRR* cloudColors;

	osg::Vec4f cldclr;
	DWORD CLR_ARRAY_SIZE;
	DWORD CLD_CLR_STEP;
	//////////////
	osg::Vec4f lightVector;
	osg::Vec4f cameraPos;

	typedef ed::map<uint32_t,unsigned long> CloudIndexMap;
	CloudIndexMap cloudIndexMap;
//-----------------------------------
	osg::BoundingBox bb;

//--------------------------------
	render::handle_t dsh_param_texNoise;
	render::handle_t dsh_param_texTan;
	render::handle_t dsh_param_PtclMap;
	render::handle_t dsh_param_GradMap;

	render::handle_t dsh_param_const5;
	render::handle_t dsh_param_const6;
	render::handle_t dsh_param_const7;
	render::handle_t dsh_param_const8;
	render::handle_t dsh_param_const9;
	render::handle_t dsh_param_const10;
	render::handle_t dsh_param_const11;
	render::handle_t dsh_param_const12;
	render::handle_t dsh_param_const13;
	render::handle_t dsh_param_const14;
	render::handle_t dsh_param_const15;
	render::handle_t dsh_param_const34;
	render::handle_t dsh_param_const40;
	render::handle_t dsh_param_const50;
	render::handle_t dsh_param_cldclr;
	render::handle_t dsh_param_straightmap;

	render::handle_t dsh_param_cview;
	render::handle_t dsh_param_viewproj;
	render::handle_t dsh_param_cld_rpy;

	render::handle_t dsh_techAlpha;
	render::handle_t dsh_techColor;

	render::handle_t	dsh_param_pix_const0;
	render::handle_t	dsh_param_pix_const1;
	render::handle_t	dsh_param_pix_const2;
	render::handle_t	dsh_param_pix_const3;
	render::handle_t	dsh_param_pix_const4;
	render::handle_t	dsh_param_pix_const5;
	render::handle_t	dsh_param_pix_const6;
	render::handle_t	dsh_computeInscatterResults, dsh_inscatterID;



	render::handle_t ish_param_texTarg;
	render::handle_t ish_param_SkyTex;
	render::handle_t ish_param_const5;
	render::handle_t ish_param_const6;
	render::handle_t ish_param_const12;
	render::handle_t ish_param_viewproj;
	render::handle_t ish_param_cldclr;
	render::handle_t ish_param_campos;
	render::handle_t ish_param_world;

	render::handle_t ish_computeInscatterResults, ish_inscatterID;

//--------------------------------

	osg::Vec3f terrestrial_axis;


//****************************************
	ShadowDesc shadowDesc;
	void makeShadow(const osg::Vec4f& lightDir);
	unsigned long collect(osg::Vec4d* frustrum,const osg::Vec4d& campos,osg::Vec3d& lightdir,CLOUD** dest, unsigned long maxcount);
	unsigned long collect(osg::Vec4d* frustrum,CLOUD** dest, unsigned long maxcount);
	void computeFrustrum(const osg::Matrixd& view,
						 const osg::Matrixd& proj,
						 const osg::Vec4d& pos,//=camera->getPos();
						 osg::Vec4d* frustrum,osg::Vec4d* frustrumPoints);

	void drawTest(osg::Matrixf& mView,osg::Matrixf& mProj,osg::Vec4f& vCam);

	render::Shader shadowShader;

	render::handle_t shad_param_vp;
	render::handle_t shad_param_pos;
	render::handle_t shad_param_inv;
	render::handle_t shad_param_tex;
	render::handle_t shad_param_rot;

	render::Shader testShader;
	render::handle_t test_param_vp;
	render::handle_t test_param_cam;
	render::handle_t test_param_texproj;
	render::handle_t test_param_tex;
	render::handle_t test_tech;

	render::VertexBuffer qvb;
	render::stream_handle_t qvb_stream0;
	render::IndexBuffer qib;

	render::handle_t imp_tech;
	render::handle_t imp_tech_simple;
	render::handle_t shad_tech;

protected:
	DWORD GetLodFromIndex(DWORD index);
	float GetCurrentAspect(CLOUD* cld,const osg::Vec4d& camPos);
	void ReleaseSlot(DWORD idx);
	void FindFreeSlot(CLOUD* cld);
	void UpdateCloud(CLOUD* cld, osg::Matrixf& m_matView,osg::Matrixf& m_matProj,osg::Vec4f& light,bool nodraw, float worldTime);




	void onUpdate();
	void Draw(DWORD index, const osg::Vec3d& atPos, float meltFar, float worldTime);//,edge::Context *ctx);
	



public:
	gCloudGenZW( const char *name );// : gCloudsZW( name ) {}
	~gCloudGenZW();

	void DestroyAll();
	//self
	virtual void SetMaxShapeCount(DWORD count);
	virtual DWORD GetMaxShapeCount();
	virtual bool init();
	virtual bool IsInit();
	virtual void uninit();
	virtual bool CanCreate();// {return init();}

	virtual DWORD AddCloud(const osg::Vec3d& pos,float size,DWORD shape);
	virtual DWORD AddCloud(const Vector3& pos,float size,DWORD shape);
	virtual void KillCloud(DWORD index);
	virtual void Purge();

	virtual void Draw(DWORD index, Graphics::Context* context, const Vector3& atPos, float meltFar,float worldTime);
	virtual void SetEffectState(DWORD index, Graphics::effectState* effects);
	void DrawAll(osg::Matrixf& mView,osg::Matrixf& mProj,osg::Vec4f& vCam);

	//util
	virtual void SetPosition(DWORD index,const Vector3& pos);
    void SetPosition(DWORD index,const osg::Vec3d& pos);
	virtual void SetSize(DWORD index,float size);

	virtual const Box& GetBoundingBox(DWORD index);
	virtual float GetRadius(DWORD index);
	virtual const Vector3& GetCenter(DWORD index);
	virtual unsigned GetPolygonCount(DWORD index);
	/////////////////////
	//plugin
	////////////////////
	//CallBacks
	virtual void OnUpdate();
	virtual void OnUninitialize();
	//DXMaterial
//	virtual Graphics::DXTexture* GetMainDiffuseMap() {return 0;}
	///////////////////
	//resource
	virtual void DrawGeometry(Graphics::RenderObject *robject) {}

	// сортировка и оптимизация
	virtual Graphics::MaterialKey GetKey() {return 0;}

	// список используемых текстур
	virtual int GetTextureCount(){return 0;}
	virtual render::Texture *GetTexture(int i) {return 0;}

	// Полупрозрачный ли это материал
	virtual bool IsTransparent() {return false;}

	// управление загрузкой ресурса (может быть реализовано с поддержкой многопоточности)
	// загрузка производится по шагам, семантика шагов зависит от конкретного типа ресурса
	virtual float GetLoadPriority(){return 1.0f;}
	virtual int GetLoadStep(){if (initialized) return 1;else return 0;}
	virtual int GetLoadStepCount(){return 1;}
	virtual void LoadToStep(int step){init();}
	virtual void FreeToStep(int step){if (step>0) return;if (initialized) DestroyAll();}
	virtual const ed::string& ClassName();



};


#endif
