#ifndef InfraRedMaterial_H
#define InfraRedMaterial_H

#include "DXRendererAPI.h"
#include "CustomMaterial_Impl.h"
#include "DXMaterial.h"
#include "rwrappers/renderUserAPI.h"
#include "InfraRedMaterialImpl.h"

namespace Graphics
{

class DXRENDERER_API InfraRedMaterial : public DXMaterial, public CustomMaterial_Impl
{
public:

	static const int MATERIAL_TEX_COUNT     = 4;
	static const int MATERIAL_TEX_NOISE     = 0;
	static const int MATERIAL_TEX_OPTICAL   = 1 ;
	static const int MATERIAL_TEX_SAR       = 2 ;
	static const int MATERIAL_TEX_RBM_BLUR  = 3 ;

	static const int PIXEL_SHADER_COUNT     = 3;
	static const int PIXEL_SHADER_O2R       = 0;
	static const int PIXEL_SHADER_BLUR      = 1;
	static const int VERTEX_SHADER_DEFAULT  = 2;
	
	static const int VERTEX_BUFFER_COUNT    = 3;
	static const int VERTEX_BUFFER_CIRCLE   = 0;
	static const int VERTEX_BUFFER_SECTOR   = 1;
	static const int VERTEX_BUFFER_SPRITE   = 2;

	
protected:
    bool initialized;
	
    InfraRedMaterialImpl _impl;
	render::FrameBuffer	*target;

public:
	// фабрика
	static Resource *Create(ResourceType type, const ed::string& className);

	// конструктор/деструктор
	InfraRedMaterial();
	~InfraRedMaterial();

	// Создать ресурс по имени
	virtual bool CreateByName(const ed::string& resname);

	// Общие характеристики ресурса
	virtual const ed::string& ClassName();

	// отрисовка
	virtual void DrawGeometry(RenderObject *robject);

	// сортировка и оптимизация
	virtual MaterialKey GetKey();

	// список используемых текстур
	virtual int GetTextureCount();
	virtual render::Texture* GetTexture(int i);
	
	// редактирование (возможно двумя способами - хотя бы один должен быть реализован)
	virtual CustomMaterial *QueryCustomMaterial();

    virtual MaterialCaps GetMaterialCaps();

	virtual float GetLoadPriority()	{return 3;}
    virtual int GetLoadStep() {return (initialized)?1:0;}
    virtual int GetLoadStepCount(){return 1;}

    virtual void LoadToStep(int step);
	virtual void FreeToStep(int step);

	virtual void LoadWithDependents(int step);

	void	SetTarget(render::FrameBuffer& fb)		{	target = &fb;	}
};

} // namespace Graphics

#endif // InfraRedMaterial_H
