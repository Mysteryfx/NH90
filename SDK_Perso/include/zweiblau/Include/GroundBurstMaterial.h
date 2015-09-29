#ifndef _GroundBurstMaterial_h_
#define _GroundBurstMaterial_h_

#include "DXMaterial.h"

#include "rwrappers/renderUserAPI.h"

#include "osg/Vec3f"
#include "osg/Vec4f"
#include "osg/Matrixf"
#include "osg/BoundingBox"

#include "GroundBurst.h"

#include "FogHelper.h"

#define MAX_PTCL_VBS 8

class GroundBurstMaterial : public Graphics::DXMaterial
{
private:
	bool initialized;
	bool loadFail;


	render::Shader sh;

	render::handle_t sh_tex1;
	render::handle_t sh_tex2;
	render::handle_t sh_inv;
	render::handle_t sh_vp;
	render::handle_t sh_wPos;
	render::handle_t sh_vUp;
	render::handle_t sh_vRight;
	render::handle_t sh_vFwd;
	render::handle_t sh_flash;
	render::handle_t sh_light;
	render::handle_t sh_campos;
	render::handle_t sh_tech;
	render::handle_t sh_color;




	render::stream_handle_t stream0;
	render::stream_handle_t stream1;
	render::stream_handle_t stream2;
	render::stream_handle_t stream3;
	render::stream_handle_t stream4;


	render::ve_handle_t  veHandle0;
	render::ve_handle_t  veHandle1;
	render::ve_handle_t  veHandle2;
	render::ve_handle_t  veHandle3;
	render::ve_handle_t  veHandle4;





	render::Texture face1;//
	render::Texture face2;//

	render::VertexBuffer vb;
	render::IndexBuffer ib;

	render::VertexBuffer ptclVb[MAX_PTCL_VBS];
	GroundBurstInstance* cachedEffects[MAX_PTCL_VBS];


	BurstParticles particles;

	//Particle** sortArray;

	render::FogHelper fog;
	

////
	struct gContext
	{
//local copy of view*projection (in case vp changes later within same frame)
		osg::Matrixf vp;
		osg::Matrixf view;
		osg::Matrixf finTM;
		osg::Vec4f cam;
		osg::Vec4f pos;
		//unsigned long primCount;
		GroundBurstInstance* fei;
		osg::Vec3d origin;
	};

	//RendMan::SortKey rKey;

////
	ed::vector<BurstParticles::iterator> expendables;

	typedef ed::map<GroundBurstInstance*, unsigned long> VBMap;

	VBMap vbMap;

	unsigned long roundRobin;



protected:
	void DestroyAll();

public:
	GroundBurstMaterial();
	~GroundBurstMaterial();
	virtual const ed::string& ClassName();

	//////////////////////////////////////////////////////////////////////
	//DXMaterial
//	virtual Graphics::DXTexture* GetMainDiffuseMap() {return 0;}
	///////////////////
	//resource
	virtual void DrawGeometry(Graphics::RenderObject *robject);

	// сортировка и оптимизация
	virtual Graphics::MaterialKey GetKey() {return 0;}

	// список используемых текстур
	virtual int GetTextureCount();
	virtual render::Texture *GetTexture(int i);

	// Полупрозрачный ли это материал
	virtual bool IsTransparent() {return true;}

	// управление загрузкой ресурса (может быть реализовано с поддержкой многопоточности)
	// загрузка производится по шагам, семантика шагов зависит от конкретного типа ресурса
	virtual float GetLoadPriority(){return 1.0f;}
	virtual int GetLoadStep();
	virtual int GetLoadStepCount(){if (loadFail) return 0;else return 1;}
	virtual void LoadToStep(int step);
	virtual void FreeToStep(int step);
};

#endif
