#ifndef _DXOmniLight_h_
#define _DXOmniLight_h_

#include "Renderer\Light.h"
#include "DXVolumeEffect.h"

namespace Graphics
{

// реализация источника света для DX
class DXOmniLight : public DXVolumeEffect, public Light
{
protected:
	osg::Vec3f diffuse;
	osg::Vec3f specular;
	osg::Vec3f ambient;

	osg::Vec3f position;
	osg::Vec3f direction;
	
	float range;
	float falloff;
	float attenuation0;
	float attenuation1;
	float attenuation2;
	float theta;
	float phi;
    
    LightType ltype_for_Grisha;
    DWORD lightHandle;

	DParamList dparams;
public:

	// фабрика данного ресурса
	static Resource *Create(ResourceType type, const ed::string& className);

	// конструктор/деструктор
	DXOmniLight();
	~DXOmniLight();
	
	// Интерфейс Light
	// установка параметров
	virtual void SetLightType(LightType ltype);
	virtual void SetAmbientColor(const Vector3& color);
	virtual void SetDiffuseColor(const Vector3& color);
	virtual void SetSpecularColor(const Vector3& color);
	virtual void SetConeAngles(float fi, float theta);
	virtual void SetDirection(const Vector3& dir);
	virtual void GetRange(float& range);
	virtual void SetAttenuationParams(float att0, float att1,float att2,float range);
    
	// получение параметров
    virtual LightType GetLightType() {return ltOmni;}
	virtual void GetAmbientColor(Vector3& color);
	virtual void GetDiffuseColor(Vector3& color);
	virtual void GetSpecularColor(Vector3& color);
	virtual void GetConeAngles(float& fi, float& theta);
	virtual void GetDirection(Vector3& dir);
	virtual void GetAttenuationParams(float& att0, float& att1,float& att2,float& range);
//tango-lima-bravo
//	virtual void GetD3DLIGHT(D3DLIGHT9& light){light = theLight;}

	virtual void SetTexture(render::Texture*){}
	virtual const render::Texture &GetTexture() const;

	// Интерфейс Effect
	virtual float GetRadius();
	virtual void UpdatePos(const Position3& pos);
	virtual DParamList& GetParamList();
	virtual void Apply(RenderObject *robject);
	virtual Light *QueryLightInterface() {return this;}

	// Интерфейс Resource
	virtual const ed::string& ClassName();
	virtual float GetLoadPriority();
	virtual int GetLoadStep();
	virtual int GetLoadStepCount();
	virtual void LoadToStep(int step);
	virtual void FreeToStep(int step);
	virtual void serialize(Serializer& serializer);

    // DXVolumeEffect
    virtual DWORD GetHandle() {return lightHandle;}
	virtual int TextureStageDemand() {return 3;}
    virtual int  Using(int) {return 0;}
};

} // namespace Graphics

#endif
