#ifndef Light_H
#define Light_H

#include "vector.h"


namespace render
{
	class Texture;
}

namespace Graphics
{
// Интерфейс для источников света
// необязателен, для удобства доступа и анализа из основной программы
class Light
{
public:
	// типы источников света
	enum LightType
	{
		ltAmbient,		// рассеяное освещение
		ltDirectional,	// направленное освещение (в сторону X+)
		ltOmni,			// точечный ненаправленный источник света
		ltSpot,			// направленный источник света (в сторону X+) - прожектор
	};

	virtual ~Light(){}

	/// Enabels or disables light source.
	virtual void Enable(bool enable){}
	/// Returns if light is turn on or off.
	virtual bool IsEnabled()const{return true;}

	// установка параметров
	virtual void SetLightType(LightType ltype) = 0;
	virtual void SetAmbientColor(const Vector3& color) = 0;
	virtual void SetDiffuseColor(const Vector3& color) = 0;
	virtual void SetSpecularColor(const Vector3& color) = 0;
	virtual void SetConeAngles(float fi, float theta) = 0;
	/// Sets direction of light source, if it supports it.
	virtual void SetDirection(const Vector3& dir) = 0;
	virtual void GetRange(float& range) = 0; 
	virtual void SetAttenuationParams(float att0, float att1,float att2,float range) = 0;

	virtual void SetTexture(::render::Texture*) = 0;

	// получение параметров
	virtual LightType GetLightType() = 0;
	virtual void GetAmbientColor(Vector3& color) = 0;
	virtual void GetDiffuseColor(Vector3& color) = 0;
	virtual void GetSpecularColor(Vector3& color) = 0;
	virtual void GetConeAngles(float& fi, float& theta) = 0;
	/// Returns direction of light source, if it supports it.
	/// If source doesn't support fills vector with 0.
	virtual void GetDirection(Vector3& dir) = 0;
	virtual void GetAttenuationParams(float& att0, float& att1,float& att2,float& range) = 0;

	virtual const ::render::Texture &GetTexture() const = 0;
};

}

#endif // Light_H
