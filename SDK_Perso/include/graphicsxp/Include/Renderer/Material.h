#ifndef Material_H
#define Material_H

#include "graphicsxp.h"
#include "Resourcer\Resource.h"
#include "MaterialFlags.h"
#include "osg/Matrixf"

namespace render
{
	class Texture;
}


namespace Graphics
{

class CustomMaterial;
class RenderObject;

// ключ для сортировки по материалу
// по возможности, близкие материалы должны иметь близкие ключи
typedef struct MaterialKey_t* MaterialKey;

// Интерфейс для материалов - вид поверхности объекта
class GRAPHICSXP_API Material : public Resource
{
protected:
	MaterialFlags flags;

public:
	Material()	{rtype = rtMaterial; flags = 0;}
	
	// отрисовка
	virtual void DrawGeometry(RenderObject *robject) = 0;

	// сортировка и оптимизация
	virtual MaterialKey GetKey() = 0;

	/// Returns shader name of material.
	virtual const char* GetShaderName()const;

	// список используемых текстур
	virtual int GetTextureCount() = 0;
	virtual render::Texture *GetTexture(int i) = 0;
	
	// Флаги
	inline void SetFlags(MaterialFlags flag, bool value)		{if(value) flags |= flag; else flags &= ~flag;}
	inline void SetFlags(MaterialFlags to_set)				{flags |= to_set;}
	inline void ResetFlags(MaterialFlags to_reset)			{flags &= ~to_reset;}
	inline MaterialFlags GetFlags(MaterialFlags to_get)	const {return flags & to_get;}

	/// If material has flag mfZNudged, method returns distance multiplier, needed for sorting of transparent objects.
	virtual float GetDistMultiplier()const{return (flags&mfZNudged)?0.99f:1.0f;}

	//задать явно кэффициент
	virtual void SetDistMultiplier( float fDistMilt ){}

	// редактирование
	virtual CustomMaterial *QueryCustomMaterial();

	// управление загрузкой ресурса (может быть реализовано с поддержкой многопоточности)
	// загрузка производится по шагам, семантика шагов зависит от конкретного типа ресурса
	virtual float GetLoadPriority();
	virtual int GetLoadStep();
	virtual int GetLoadStepCount();
	virtual void LoadToStep(int step);
	virtual void FreeToStep(int step);

	/// Returns if material can render shadows by itself.
	virtual bool CanRenderShadow()const{return false;}
	/// Renders shadow.
	virtual void RenderShadow(RenderObject *robject, const osg::Matrixf *mat){}
};

}

#endif // Material_H
