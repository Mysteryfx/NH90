#ifndef _gWtrExplosionZW_h_
#define _gWtrExplosionZW_h_

#include "DXMaterial.h"

typedef struct EXPLOSION_DATA
{
	float size;
	float HeightScaler;
	float Phase;
	float dumper;
	float AnimationStep;
	bool InProcess;
	float delta_tutv;
	osg::Vec4f pos;
	float terminator;
	float originator;
	DWORD CurrentFoamPhase;
	DWORD NextFoamPhase;
	float Transition;
	DWORD FadeOutStart;
	float TemporalFadeOut;
	double Timer;
	float defoam;
	float centrum_defoam;
	DWORD Cntr_CurrentFoamPhase;
	DWORD Cntr_NextFoamPhase;
	float Cntr_Transition;
	float Cntr_FadeOut;

} EXPLOSION_DATA;



class gWtrExplosionZW : public Graphics::DXMaterial
{
private:
	bool initialized;
	bool loadFail;

	//LPDIRECT3DDEVICE9 m_pd3dDevice;

	//circles geometry
	//LPDIRECT3DVERTEXBUFFER9 m_pVB;		//
	DWORD offsetVB;
	//LPDIRECT3DINDEXBUFFER9  m_pIB;		//
	DWORD offsetIB;
	DWORD        m_dwNumVertices;
	DWORD        m_dwNumIndices;

	DWORD Nx;   //
	DWORD Ny;   //

	//particles
	DWORD NumParticles;
	//LPDIRECT3DVERTEXBUFFER9 m_pPVB;
	//LPDIRECT3DINDEXBUFFER9 m_pPIB;
	DWORD offsetPVB;
	DWORD offsetPIB;

	DWORD MaxDelay;
	float MaxSpeed;
	float dims;

	// rendering



	DWORD TotalAnimSteps;

	//    ExpDataList ExpList;
	//    gWtrExplGeometry* pMesh;


	//LPDIRECT3DTEXTURE9 texNormalMap;
//	Graphics::DXTexture* texNormalMap;




	float shineFactor;
	float fresnelFactor;
	float Delta_phase;
	float f_BumpScaler;

	DWORD m_dwMaxFoamPhases;
	//LPDIRECT3DTEXTURE9* ptr_FoamTexPool;	//ptr to array of foam maps
//	Graphics::DXTexture** ptr_FoamTexPool;


	DWORD offsetFlatVB;

	DWORD offsetFlatIB;
	////////////////////
	//Texture* forGrisha;
	char* rp;
	char* ResourcePath;

protected:
	void DestroyAll();

public:
	gWtrExplosionZW();
	~gWtrExplosionZW(){DestroyAll();}
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
