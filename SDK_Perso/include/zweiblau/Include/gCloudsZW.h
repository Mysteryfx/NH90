#ifndef _gCloudsZW_h_
#define _gCloudsZW_h_

#include "box.h"
#include "Renderer\VolumeEffect.h"
#include "Resourcer\Plugin.h"
#include "Osg/Vec3d"

namespace Graphics{
	class Context;
}

class gCloudsZW : public Plugin
{

public:
    gCloudsZW( const char* name ) : Plugin( name ) {}
    virtual ~gCloudsZW() {}
//self
    virtual void SetMaxShapeCount(DWORD count)=0;
    virtual DWORD GetMaxShapeCount()=0;
    virtual bool init()=0;
    virtual bool IsInit()=0;
    virtual void uninit()=0;

    virtual DWORD AddCloud(const Vector3& pos,float size,DWORD shape)=0;
    virtual void KillCloud(DWORD index)=0;
    virtual void Purge()=0;

    virtual void Draw(DWORD index, Graphics::Context* context,const Vector3& atPos, float meltFar,float worldTime)=0;
    virtual void SetEffectState(DWORD index, Graphics::effectState* effects)=0;
//util
    virtual void SetPosition(DWORD index,const Vector3& pos)=0;
    virtual void SetSize(DWORD index, float size)=0;

    virtual const Box& GetBoundingBox(DWORD index)=0;
    virtual float GetRadius(DWORD index)=0;
	virtual const Vector3& GetCenter(DWORD index)=0;
	virtual unsigned GetPolygonCount(DWORD index)=0;
    virtual bool CanCreate()=0;
/////////////////////
//plugin
////////////////////
//CallBacks
//    virtual void OnUpdate();
//virtual void OnUninitialize();

};


#endif
