#ifndef _CloudMaterial_h_
#define _CloudMaterial_h_

#include "Weather.h"
#include "DXMaterial.h"

namespace Weather
{

class  CloudMaterial : public Graphics::DXMaterial
{
public:
    WEATHER_API static CloudMaterial *create();

    CloudMaterial();
    ~CloudMaterial();

    virtual void DrawGeometry(Graphics::RenderObject *robject);
    virtual Graphics::MaterialKey GetKey();
    virtual int GetTextureCount();
    virtual render::Texture *GetTexture(int i);

    virtual const ed::string& ClassName();
    virtual float GetLoadPriority();
    virtual int GetLoadStep();
    virtual int GetLoadStepCount();
    virtual void LoadToStep(int step);
    virtual void FreeToStep(int step);

    virtual bool AllowProjLights();
};

}

#endif // _CloudMaterial_h_
