#ifndef _LOCKON_MATERIAL_BASE_H_
#define _LOCKON_MATERIAL_BASE_H_

#include "DXMaterial.h"
#include "CustomMaterial_Impl.h"
#include "Renderer\Light.h"

namespace Graphics {

#define L_MASK(mask) (2 << mask)
#define IS_SLOT_VALID(id) (_texMask & L_MASK(id) && _maps[id].isValid())

/// Базовый материал для всех материалов в движке
/// Должен стать платформой для перехода от старх материалов
class DXRENDERER_API MaterialBase: public DXMaterial, public CustomMaterial_Impl  {
public:

	enum TextureSlot {
		SLOT_DIFFUSE	= 0,

		SLOT_NORMAL		= 1,
		SLOT_SPECULAR	= 2,
		SLOT_AO			= 3
	};

protected:
	int _texMask;

	/// Текущий шаг загрузки материала
	int _loaded;

	/// Ключ сортировки материала
	int _key;

	/// Возвращаемое имя материала (надо переопрелелить дял каждого наследника)
	ed::string _materialName;

	render::handle_t _uMVP;

	render::handle_t _omni;
	render::handle_t _spot;
	render::handle_t _ambient;
	render::handle_t _zonly;

	// lights
	render::handle_t _uLightPos;
	render::handle_t _uLightAmbient;
	render::handle_t _uLightDiffuse;
	render::handle_t _uLightAtt;
	render::handle_t _uLightDir;
	render::handle_t _uLightMap;
	render::handle_t _uLightMat;
	render::handle_t _uLightAngles;

	virtual void FillDXMat();
	virtual void FillFlags();

	bool _initLightPasses();

	enum LightPasses {
		PASS_OMNI		= 0,
		PASS_SPOT		= 1,
		PASS_AMBIENT	= 2
	};

	bool _beginLightPass(Graphics::Light &light, const osg::Vec3f &pos);
	void _endLightPass();

	void _drawZOnly(RenderObject *robject);
	bool isTextureTransparent(unsigned id);

	virtual bool setLightTech(int type, render::handle_t &tech);

public:

	/// Constructor \ Destructor
	MaterialBase();
	MaterialBase(const MaterialBase&);

	virtual ~MaterialBase();

	virtual bool CreateByName(const ed::string& resname);
	virtual const ed::string& ClassName();

	/// Material key
	virtual MaterialKey GetKey();

	/// Check current load step
	virtual int GetLoadStep();

	/// Textures query
	virtual render::Texture *GetTexture(int i);
	virtual int GetTextureCount();

	virtual void SetRenderTexture(int index, const char *name);
	virtual void SetRenderTexture(int index, render::Texture& t);

	/// CustomMaterial queries
	virtual CustomMaterial *QueryCustomMaterial();
	virtual MaterialCaps GetMaterialCaps();
	virtual void SetAmbientColor(const Vector3& ambientColor);
	virtual void SetDiffuseColor(const Vector3& diffuseColor);
    virtual void SetSpecularColor(const Vector3& specularColor);
    virtual void SetRadianceColor(const Vector3& radianceColor);
	virtual void SetOpacity(float opacity);
	virtual void SetGlossiness(float glossiness);

	/// Serialize
	virtual void serialize(Serializer& serializer);
};

}

#endif
