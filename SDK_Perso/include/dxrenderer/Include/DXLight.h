#ifndef DXLight_H
#define DXLight_H

#include "Renderer\Light.h"
#include "DXVolumeEffect.h"
#include "rwrappers/RenderUserAPI.h"
#include "osg/Vec3f"

namespace Graphics
{

// реализация источника света для DX
class DXLight : public DXVolumeEffect, public Light
{
protected:


enum LIGHTTYPE
{
  LIGHT_POINT         = 1,
  LIGHT_SPOT          = 2,
  LIGHT_DIRECTIONAL   = 3,
};

struct COLORVALUE
{
  float r;
  float g;
  float b;
  float a;
};

struct D3D9COMPATIBLELIGHT
{
  LIGHTTYPE  Type;
  COLORVALUE Diffuse;
  COLORVALUE Specular;
  COLORVALUE Ambient;
  osg::Vec3f     Position;
  osg::Vec3f     Direction;
  float         Range;
  float         Falloff;
  float         Attenuation0;
  float         Attenuation1;
  float         Attenuation2;
  float         Theta;
  float         Phi;
};

    D3D9COMPATIBLELIGHT theLight;
/*
	LightType light_type;
	Vector3 ambient_color;
	Vector3 diffuse_color;
	Vector3 spec_color;
	float fi, theta;
	float dnear, dfar;
*/
    LightType ltype_for_Grisha;
    uint32_t lightHandle;

	DParamList dparams;

	render::Texture _texture;

	void update();
public:

	// фабрика данного ресурса
	static Resource *Create(ResourceType type, const ed::string& className);

	// конструктор/деструктор
	DXLight();
	~DXLight();
	
	// Интерфейс Light
	// установка параметров
	virtual void SetLightType(LightType ltype);
	virtual void SetAmbientColor(const Vector3& color);
	virtual void SetDiffuseColor(const Vector3& color);
	virtual void SetSpecularColor(const Vector3& color);
	virtual void SetConeAngles(float fi, float theta);
	virtual void SetDirection(const Vector3& dir);
	virtual void SetAttenuationParams(float att0, float att1,float att2,float range);
    
	// получение параметров
	virtual LightType GetLightType();
	virtual void GetAmbientColor(Vector3& color);
	virtual void GetDiffuseColor(Vector3& color);
	virtual void GetSpecularColor(Vector3& color);
	virtual void GetConeAngles(float& fi, float& theta);
	virtual void GetDirection(Vector3& dir);
	virtual void GetRange(float& range); 
	virtual void GetAttenuationParams(float& att0, float& att1,float& att2,float& range);

	//virtual void GetDirection(D3DXVECTOR4 *dir);
	virtual void GetDirection(osg::Vec3f &dir);
//tango-lima-bravo
//	virtual void GetD3DLIGHT(D3DLIGHT9& light){light = theLight;};

	virtual void SetTexture(::render::Texture*);
	virtual const ::render::Texture &GetTexture() const;

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
    virtual uint32_t GetHandle() {return lightHandle;}
	virtual int TextureStageDemand() {return 0;}
    virtual int  Using(int) {return 0;}
};

} // namespace Graphics

#endif // DXLight_H
