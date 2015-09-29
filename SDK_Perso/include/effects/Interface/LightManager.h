
#ifndef _LightManager_h_
#define _LightManager_h_

#include "Resourcer\Plugin.h"
#include "Resourcer/ResourceManager.h"

namespace Common{
	class FactoryManager;
}

namespace Graphics {
	class Model;
}

class viObject;
class viObjectShape;

class ModelLightObject;

namespace Effects
{

// флаги разрешения создания соответствующих источников света
const unsigned laCannon = 0x01;
const unsigned laFlash	= 0x02;
const unsigned laObject	= 0x04;

class CannonLight;
class LightFlash;
class ObjectLight;

class LightManager : public Plugin
{
public:
    
    LightManager( const char* name ) : Plugin(name) {}
    virtual ~LightManager() {}

    virtual void init() = 0;
    virtual void uninit() = 0;
	virtual void updateParams() = 0;
	virtual unsigned getCreatable() = 0;

	virtual bool getFactoryMgr( Common::FactoryManager **pfm ) = 0;

    virtual CannonLight * CreateCannonLight(viObject *host = 0) = 0;
    virtual LightFlash  * CreateLightFlash(const Vector3& p, float power)  = 0;
    virtual ObjectLight * CreateObjectLight(viObject *host = 0, const ed::string& lightClass = "") = 0;
	virtual ::ModelLightObject * CreateModelLight(viObjectShape *host, unsigned int lightIndex) = 0;
};

inline LightManager * GetLightManager()
{
    return (LightManager*)resourcer->GetPlugin( "LightManager" );
}

} // namespace Effects

#endif // _LightManager_h_
