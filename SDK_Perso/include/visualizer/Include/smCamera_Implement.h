#pragma once

#include <ed/map.h>

#include "Visualizer.h"
#include "smCamera.h"
#include "wTime.h"
#include "RenderParserImpl.h"
#include "vPublic.h"

class ClipVolume;

// коэффициенты видимости для конкретного типа объектов
struct DistFactors
{
	float factor;
	float maxdist;
	float minimum_size;
};

// диапазон плоскостей отсечения
struct ClipRegion
{
	float dnear, dfar;
	ClipRegion() {dnear = dfar = 0;}
};

class VISUALIZER_API smCamera_Implement : public smCamera, public RenderParserImpl
{
private:
	typedef ed::map<viFlag, DistFactors>  DistFactorsType;
	DistFactorsType   factors;
	float mult, max_dist;

	ClipVolumeCamera _clipVolumeCamera;
	ClipVolumePlanes _clipVolumePlanes;

	// Дополнительно тестировать все куски на вхождение в этот объем
	ClipVolumePlanes *clipplanes;
	
	// коэфф. дальности переключения LOD
	float mlod;
    float addLod;

	// диапазоны клиппинга
	typedef ed::vector<ClipRegion> clipDistanceList;
	clipDistanceList clipDistances;
	int clip_region;

	viFlag mask;
	float hor_angle, aspect;
	float near_clip, far_clip;

	vCameraType_e type;
	vCameraSubType_e subtype;

	dPosition pos;
	cVector origin;

	Graphics::Camera camera;

	void LoadDistFactors(viFlag types, Lua::Config& cam_cfg, const ed::string& key);

protected:
	ClipVolume *CreateCameraVolume(float len);

public:
	smCamera_Implement(vCameraType_e _type, vCameraSubType_e _subtype);
	virtual ~smCamera_Implement();

	inline vCameraType_e Type() const	{return type;}
	inline const dPosition &Pos() const	{return pos;}

	inline viFlag Mask() const		{return mask;}
	inline float HorAngle() const	{return hor_angle;}

	// изменение различных параметров
	virtual void SetCamType(Graphics::Camera::Type ct);
	virtual void SetClipRegion(int region, float dnear, float dfar);
	virtual bool SetClipRegion(int region);
	virtual void SetClipDistances(float near_dist, float far_dist);
	virtual void SetPosition(const dPosition& _pos);
	virtual void SetObjectMask(viFlag _mask = viRenderable);
	virtual void SetHorAngle(float _angle = 22.5);
	virtual void SetAspect(float _aspect = float(4./3.));
	virtual void SetOrigin(const dVector& origin);
	virtual void SetParallelBox(const Box& box);

	virtual inline void setContext(Graphics::Context *context) { RenderParser::setContext(context); } 
	
	// индивидуально для каждого типа: коэффициент видимости и абсолютная максимальная дальность видимости
	DistFactors &DistFactor(viFlag type); // type должен иметь только один тип
	virtual void SetDistFactors(viFlag mask, float factor, float max_dist,float minimum_object_size = 15.0f);
	virtual void DefaultDistFactors(viFlag mask);
	
	// общие коэффециенты для дальностей объектов
	virtual void SetDistMultiplier(float mult = 1);
	virtual void SetMaxDistance(float maxdist = 0);

	// загрузить параметры из конфига
	virtual void LoadConfig(Lua::Config& cam_cfg);

	virtual Graphics::Camera::Type GetCamType();
    virtual float GetAspect();
	virtual int   GetClipRegion();
	ClipRegion getClipRegion(int k) const;
	virtual float GetNearClip();
	virtual float GetFarClip();
	virtual float GetDistance(viFlag mask);
	virtual float GetDistance(viObject *object);
	virtual const cVector& GetOrigin();
	virtual const Box& GetParallelBox();

	// получить коэффициент умножения для дальностей объектов в зависимости от угла раствора
	// и параметра mult
	float GetDistanceCoeff();

	// получение камеры рендерера
	virtual const Graphics::Camera& GetCamera();

	// получить все параметры от другой камеры
	virtual void SetParams(smCamera *source);

	// создать объем попадания в камеру по типу объектов
	///WARNING! Returns pointer to inner object!
	ClipVolume *CreateVolume(viFlag mask);

	// установить объем для дополнительного отсечения
	inline void SetClipVolume(ClipVolumePlanes *volume) {clipplanes = volume;}
	inline ClipVolumePlanes *GetClipVolume() {return clipplanes;}

	// сообщение о том, что последний полученный объект готов
	virtual void OnObjectReady(Graphics::RenderObject *obj);
};
