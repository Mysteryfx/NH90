#ifndef GRAPHICS_MODEL_LIGHT_H
#define GRAPHICS_MODEL_LIGHT_H

#include "Renderer/VolumeEffect.h"
#include "Renderer/Light.h"
#include "graphicsxp.h"


namespace Graphics
{

class ModelLight : public VolumeEffect, public Light
{
private:
	LightType lightType;
	bool enabled;
	Vector3 color;
	float range;
	float phi, theta;

	render::Texture *tex;

	Position3 pos;

	Vector3 dir;

	DParamList dparams;
public:
	GRAPHICSXP_API ModelLight();
	GRAPHICSXP_API virtual ~ModelLight();

	static Resource *Create(ResourceType type, const ed::string& className);

	/// Enabels or disables light source.
	void Enable(bool enable){enabled = enable;}

	// form VolumeEffect
	GRAPHICSXP_API virtual bool IsEnabled()const;
	GRAPHICSXP_API virtual float GetRadius();
	GRAPHICSXP_API virtual void UpdatePos(const Position3& pos);
	GRAPHICSXP_API virtual const Position3& GetPos();
	GRAPHICSXP_API virtual DParamList& GetParamList();
	virtual void Apply(RenderObject *robject){}
	GRAPHICSXP_API virtual Light* QueryLightInterface();
	GRAPHICSXP_API virtual int TextureStageDemand();

	// from Light
	GRAPHICSXP_API virtual void SetLightType(LightType ltype);
	virtual void SetAmbientColor(const Vector3& color){}
	GRAPHICSXP_API virtual void SetDiffuseColor(const Vector3& color);
	virtual void SetSpecularColor(const Vector3& color){}
	GRAPHICSXP_API virtual void SetConeAngles(float fi, float theta);
	/// Sets direction of light source, if it supports it.
	virtual void SetDirection(const Vector3& dir){}
	GRAPHICSXP_API virtual void SetAttenuationParams(float att0, float att1,float att2,float range);
	GRAPHICSXP_API virtual void SetTexture(render::Texture*);

	// получение параметров
	GRAPHICSXP_API virtual LightType GetLightType();
	GRAPHICSXP_API virtual void GetAmbientColor(Vector3& color);
	GRAPHICSXP_API virtual void GetDiffuseColor(Vector3& color);
	GRAPHICSXP_API virtual void GetSpecularColor(Vector3& color);
	GRAPHICSXP_API virtual void GetConeAngles(float& fi, float& theta);
	/// Returns direction of light source, if it supports it.
	/// If source doesn't support fills vector with 0.
	GRAPHICSXP_API virtual void GetDirection(Vector3& dir);
	GRAPHICSXP_API virtual void GetRange(float& dir);
	GRAPHICSXP_API virtual void GetAttenuationParams(float& att0, float& att1,float& att2,float& range);

	GRAPHICSXP_API virtual const render::Texture &GetTexture() const;

	// from Resourcer
	GRAPHICSXP_API virtual const ed::string& ClassName();
	virtual float GetLoadPriority(){return 0.0f;}
	virtual int GetLoadStep(){return 0;}
	virtual int GetLoadStepCount(){return 0;}
	virtual void LoadToStep(int step){}
	virtual void FreeToStep(int step){}
};

}

#endif
