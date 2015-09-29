#ifndef DXProjLight_H
#define DXProjLight_H

#include "Renderer\Light.h"
#include "DXRendererAPI.h"
#include "DXVolumeEffect.h"
#include "ClipVolumes\ClipVolumePlanes.h"
#include "rwrappers/RenderUserAPI.h"

#include "osg/Matrixf"
#include "osg/Vec3f"

namespace Graphics
{
    
// реализация источника света для DX
class DXRENDERER_API DXProjLight : public DXVolumeEffect, public Light
{
protected:

    ed::string textureName;
	DParamList dparams;
    int  lightType;
    //D3DLIGHT9 dxLight;
	DWORD lightHandle;
    //D3DMATERIAL9 dxDefLightMat;

	osg::Vec3f direction;

	//tango-lima-bravo
    //DXTexture * clipTex;	// attenuation texture (also used to emulate clipplane)

	render::Texture _texture;

	ClipVolumePlanes volume;

    Vector3 ambientColor;
    Vector3 diffuseColor;
    Vector3 specularColor;
    float   Fi, Theta;
    float   fAtt0, fAtt1, fAtt2, fRange;

    void    FillDXLight();
	void	UpdateLight();
    
public:
    
    // фабрика данного ресурса
    static Resource *Create(ResourceType type, const ed::string& className);
    
    // конструктор/деструктор
    DXProjLight();
    ~DXProjLight();

    osg::Matrixf GetTTFMatrix( const osg::Matrixf &world );
    void       SetLightTexture( int stage );
    void       GetLightExtent( const osg::Matrixf &world, osg::Vec3f &vDir, osg::Vec3f &vPos );
    
	//tango-lima-bravo
	//DXTexture* GetTexture(){return texture;}
    // Интерфейс Light
    // установка параметров
    virtual void SetLightType(LightType ltype);
    virtual void SetAmbientColor(const Vector3& color);
    virtual void SetDiffuseColor(const Vector3& color);
    virtual void SetSpecularColor(const Vector3& color);
    virtual void SetConeAngles(float fi, float theta);
	virtual void SetDirection(const Vector3& dir);
	virtual void GetRange(float& range); 
    virtual void SetAttenuationParams(float att0, float att1,float att2,float range);
    
    // получение параметров
    virtual LightType GetLightType() { return (LightType)lightType; }
    virtual void GetAmbientColor(Vector3& color);
    virtual void GetDiffuseColor(Vector3& color);
    virtual void GetSpecularColor(Vector3& color);
    virtual void GetConeAngles(float& fi, float& theta);
	virtual void GetDirection(Vector3& dir);
    virtual void GetAttenuationParams(float& att0, float& att1,float& att2,float& range);

	virtual void SetTexture(render::Texture*);
	virtual const render::Texture &GetTexture() const;

	// объем
	void CreateVolume();
	const ClipVolumePlanes& GetVolume() const;
	virtual void UpdatePos(const Position3& _pos);
    
    // Интерфейс Effect
    virtual float GetRadius();
    virtual DParamList& GetParamList();
    virtual void  Apply(RenderObject *robject);
	virtual Light *QueryLightInterface() {return this;}
    
    // Интерфейс Resource
    virtual const ed::string& ClassName();
    virtual float GetLoadPriority();
    virtual int   GetLoadStep();
    virtual int   GetLoadStepCount();
    virtual void  LoadToStep(int step);
    virtual void  FreeToStep(int step);
    virtual void  serialize(Serializer& serializer);

	virtual bool CreateByName(const ed::string& resname);
    
	virtual void UsedThisFrame();
    
    // DXVolumeEffect
    virtual DWORD GetHandle() {return 0;}
	virtual int   TextureStageDemand() {return 1;}
    virtual int   Using( int stage );
	void TurnOffLight();
};

} // namespace Graphics

#endif // DXProjLight_H
