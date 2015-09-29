#pragma once

#include "dPosition.h"
#include "viFlags.h"
#include "Renderer\Camera.h"
#include "Lua/Config.h"

class viObject;

namespace Graphics
{
	class Context;
}


// интерфейс, описывающий камеру с точки зрения визуализации через нее
class smCamera
{
public:
	// конструктор
//	static smCamera *Create(vCameraType_e type, vCameraSubType_e subtype = vCameraSubType_Undefined);
	virtual ~smCamera(){}

	// изменение различных параметров
	virtual void SetCamType(Graphics::Camera::Type ct) = 0;
	virtual void SetClipRegion(int region, float dnear, float dfar) = 0;
	virtual bool SetClipRegion(int region) = 0;
	virtual void SetClipDistances(float near_dist, float far_dist) = 0;
	virtual void SetPosition(const dPosition& pos) = 0;
	virtual void SetObjectMask(viFlag mask = viAny) = 0;
	virtual void SetHorAngle(float angle = 22.5) = 0;
	virtual void SetAspect(float aspect = float(4./3.)) = 0;
	virtual void SetOrigin(const dVector& origin) = 0;
	virtual void SetParallelBox(const Box& box) = 0;
	// индивидуально для каждого типа: коэффициент видимости и абсолютная максимальная дальность видимости
	virtual void SetDistFactors(viFlag mask, float factor, float max_dist,float minimum_size = 15.0f) = 0;
	virtual void DefaultDistFactors(viFlag mask) = 0;

	// общие коэффециенты для дальностей объектов
	virtual void SetDistMultiplier(float mult = 1) = 0;
	virtual void SetMaxDistance(float maxdist = 0) = 0;

	// загрузить параметры из конфига
	virtual void LoadConfig(Lua::Config& cam_cfg) = 0;

	// получение дистанций
	virtual Graphics::Camera::Type GetCamType() = 0;
    virtual float GetAspect() = 0;
	virtual int   GetClipRegion() = 0;
	virtual float GetNearClip() = 0;
	virtual float GetFarClip() = 0;
	virtual float GetDistance(viFlag mask) = 0;
	virtual float GetDistance(viObject *object) = 0;
	virtual const cVector& GetOrigin() = 0;
	virtual const Box& GetParallelBox() = 0;

	virtual void setContext(Graphics::Context *context) = 0; //надоел изврат с кастом в smCamera_Implement

	// получение камеры рендерера
	virtual const Graphics::Camera& GetCamera() = 0;

	// получить все параметры от другой камеры
	virtual void SetParams(smCamera *source) = 0;
};

