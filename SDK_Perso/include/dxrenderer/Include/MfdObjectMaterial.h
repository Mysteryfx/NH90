#ifndef MfdObjectMaterial_H
#define MfdObjectMaterial_H

#include "DXMaterial.h"

namespace Graphics
{


// материал для отображения объектов на MFD
// matParams[0] - blend factor
// matParams[3] - IR signature [0,1]
class DXRENDERER_API MfdObjectMaterial : public DXMaterial
{
public:
	// фабрика
	static Resource *Create(ResourceType type, const ed::string& className);

	MfdObjectMaterial();

	// Создать ресурс по имени
	virtual bool CreateByName(const ed::string& resname) {return true;}

	// Общие характеристики ресурса
	virtual const ed::string& ClassName();

	// отрисовка
	virtual void DrawGeometry(RenderObject *robject);

	// сортировка и оптимизация
	virtual MaterialKey GetKey() {return 0;}

	// список используемых текстур
	virtual int GetTextureCount() {return 0;}
	virtual render::Texture *GetTexture(int i) {return 0;}

	//tango-lima-bravo
	//virtual DXTexture * GetMainDiffuseMap() {return 0;}

	// сериализация
	virtual void serialize(Serializer& serializer) {};

	virtual float GetLoadPriority()	{return 0;}
    virtual int GetLoadStep() {return 0;}
    virtual int GetLoadStepCount(){return 0;}
    virtual void LoadToStep(int step) {};
	virtual void FreeToStep(int step) {};
	virtual void LoadWithDependents(int step) {};
};

} // namespace Graphics

#endif // MfdObjectMaterial_H
