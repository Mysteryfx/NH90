#ifndef CustomMaterial_H
#define CustomMaterial_H

class Vector3;
class Serializer;

namespace render { class Texture;}

namespace Graphics
{

// маска для указания поддерживаемых свойств материала
typedef  __int64  MaterialCaps;

// поддерживаемые свойства стандартного материала
const MaterialCaps mcAmbientColor			= 0x1;
const MaterialCaps mcDiffuseColor			= 0x2;
const MaterialCaps mcSpecularColor			= 0x4;
const MaterialCaps mcRadianceColor			= 0x8;
const MaterialCaps mcOpacity				= 0x10;
const MaterialCaps mcGlossiness				= 0x20;
const MaterialCaps mcAmbientMap				= 0x40;
const MaterialCaps mcDiffuseMap				= 0x80;
const MaterialCaps mcDiffuseMap2			= 0x100;
const MaterialCaps mcSpecularColorMap		= 0x200;
const MaterialCaps mcSpecularLevelMap		= 0x400;
const MaterialCaps mcGlossMap				= 0x800;
const MaterialCaps mcBumpMap				= 0x1000;
const MaterialCaps mcRadianceMap			= 0x2000;
const MaterialCaps mcTransparencyMap		= 0x4000;
const MaterialCaps mcCubeMap				= 0x8000;
const MaterialCaps mcReflectionMap			= 0x10000;
const MaterialCaps mcRefractionMap			= 0x20000;
const MaterialCaps mcDisplacementMap		= 0x40000;
const MaterialCaps mcShadowMap				= 0x80000;


// Интерфейс для материалов, которые можно программно формировать
class /* RENDERER_API */ CustomMaterial
{
public:

	virtual ~CustomMaterial() {};

	// получение capabilities
	virtual MaterialCaps GetMaterialCaps() = 0;

	// задание характеристик материала
	virtual void SetAmbientColor(const Vector3& ambientColor) = 0;
	virtual void SetDiffuseColor(const Vector3& diffuseColor) = 0;
    virtual void SetSpecularColor(const Vector3& specularColor) = 0;
	virtual void SetRadianceColor(const Vector3& radianceColor) = 0;
	virtual void SetOpacity(float opacity) = 0;
	virtual void SetGlossiness(float glossiness) = 0;

	// задание текстур материала	
	virtual const render::Texture	GetRenderTexture(int index) = 0;
	virtual void					SetRenderTexture(int index, const char *name) = 0;
	virtual	void					SetRenderTexture(int index, render::Texture& t) = 0;
	virtual unsigned				getTexturesCount() = 0;

	// задание характеристик, значимых для любого материала
	virtual void SetTwoSided(bool twoSided) = 0;
	virtual void SetLighted(bool lighted) = 0;
	virtual void SetWireframe(bool wire) = 0;

	// получение характеристик
	virtual const Vector3& GetAmbientColor() = 0;
	virtual const Vector3& GetDiffuseColor() = 0;
    virtual const Vector3& GetSpecularColor() = 0;
	virtual const Vector3& GetRadianceColor() = 0;
	virtual float GetOpacity() = 0;
	virtual float GetGlossiness() = 0;	

	// задание характеристик, значимых для любого материала
	virtual bool GetTwoSided() = 0;
	virtual bool GetLighted() = 0;
	virtual bool GetWireframe() = 0;

	// сериализация
	virtual void serialize(Serializer& serializer) = 0;
	
	// получение всех параметров из другого материала
	virtual void GetDataFrom(CustomMaterial *mat) = 0;
};

}

#endif // CustomMaterial_H