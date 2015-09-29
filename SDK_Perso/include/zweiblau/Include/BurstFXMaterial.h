#ifndef _BurstFXMaterial_h_
#define _BurstFXMaterial_h_

#include "DXMaterial.h"

#include "rwrappers/renderUserAPI.h"

#include "osg/Vec3f"
#include "osg/Vec4f"
#include "osg/Matrixf"
#include "osg/BoundingBox"

#include "BurstFX.h"

#include "FogHelper.h"




#define MAX_PTCL_VBS 8



class BurstFXMaterial : public Graphics::DXMaterial
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
	render::handle_t sh_flamecolor;
	render::handle_t sh_temp;
	render::handle_t sh_azim;


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
	BurstFXInstance* cachedEffects[MAX_PTCL_VBS];


	//Particles particles;

	//Particle** sortArray;
	int effectCount;
	ed::vector<FinalParticles>* pFinalParticles; //да, это массив векторов из листов. вот такой я мудак. а кому щас легко?
	ed::unordered_map<int,int> effectMap; //effect type, index into finalParticles

	FrameVector* pFrameVector;


	render::FogHelper fog;
	

////




////
	//ed::vector<Particles::iterator> expendables;

	typedef ed::map<BurstFXInstance*, unsigned long> VBMap;

	VBMap vbMap;

	unsigned long roundRobin;


	bool loadEffect(int idx,const char* fname,ed::vector<FinalParticles>& finalParticles);


protected:
	void DestroyAll();

public:
	BurstFXMaterial();
	~BurstFXMaterial();
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
