#ifndef _GraphicEffectManager_h_
#define _GraphicEffectManager_h_

#include "GraphicEffect.h"
#include "Resourcer\Plugin.h"
#include "Resourcer\ResourceManager.h"
#include "binGraphicEffects.h"
#include "../ParticleSystem2/oEmitterTypes.h"

class viMovingObject;

namespace PSEffects{
	class LockonParticleApiPlugin;
}

namespace Effects
{

enum DensityEffect
{
	NONE	= 0,
	LOW		= 1,
	MEDIUM	= 2,
	HIGH	= 3
};

#define GE_ADD_TO_STATISTICS		::Effects::GetGraphicEffectManager()->addToStatistics(typeid(this).name());
#define GE_REMOVE_FROM_STATISTICS	::Effects::GetGraphicEffectManager()->removeFromStatistics(typeid(this).name());

class GraphicEffectManager : public Plugin
{
protected:
    
    GraphicEffectManager( const char *name ) : Plugin(name) {}

	//описание текущих граф.эффектов
	tMapGraphicEffect m_mGraphicEffect;
    
public:

    virtual ~GraphicEffectManager() {}

	//описание текущих граф.эффектов
	tMapGraphicEffect &GetGraphicEffects(){ return m_mGraphicEffect; };
	
    virtual void	init() = 0;
    virtual void	uninit() = 0;

    virtual DensityEffect getOptionEffect() = 0;

    virtual bool	getFactoryMgr( Common::FactoryManager **pfm ) = 0;

    virtual void    setCameraVelocity( const Vector3 & velocity ) = 0;
    virtual Vector3 getCameraVelocity() = 0;  

	virtual GraphicEffect * CreateExplosion(const cPosition &position, float scale, const char *config, cVector *dir = 0) = 0;
    
    virtual GraphicEffect * CreateLightningGE( Vector3 *startPos, Vector3 *direct,
                                               float length, float startWidth,
                                               float endWidth, float mediumWidth,
                                               int   numberOffshoot ) = 0;
    
    virtual GraphicEffect * CreateClusterBombExplosion( cPoint *_position,cPoint *_direction,
                                                        float _scale,float _length,float _width,
                                                        int _numberBomb ) = 0;
    
    virtual GraphicEffect * CreateDustTail( float _scale,cVector *_localPosition,viMovingObject *_movingObject,float _timeDustTail = FLT_MAX,TypeDust  _typeDust = PLANE ) = 0;
    
	virtual GraphicEffect * CreatePipeSmoke( float _scale,cVector *_Position, const E4::Vector& _color = E4::Vector(1, 1, 1, 1), wModelTime _timeToLive = FLT_MAX) = 0;
    
    virtual GraphicEffect * CreatePlayerSceneGraphicEffect(GraphicEffectType graphicEffectType, cPoint* position, float _scale, cVector* _direct = nullptr, bool isExplosion = false) = 0;
    
    virtual GraphicEffect * CreateSmokeCloud( cPosition *_position, float _scale = 60.0f , float *parameters = NULL) = 0;
    
    virtual GraphicEffect * CreateSmokeTail( TypeSmoke _type,float _widthHeadSmoke ) = 0;

    /*
        Параметры: тип, ширина в начале, позиция относительно объекта, объект, цвет
    */
    virtual GraphicEffect * CreateSmokeTail( TypeSmoke _type,float _widthHeadSmoke,
                                             cVector *_localPosition,viMovingObject *_movingObject,
                                             UINT *color = NULL ) = 0;	   

    virtual GraphicEffect * CreateWaterExplosion( const cPoint& p, float size, bool splash = true) = 0;

	virtual GraphicEffect * CreateFlareTrail( const cPosition& _position, const cVector& _speed, const cVector& _accel, float _flareTime = 6.0f, float _dissipationTime = 2.0f, int color = 0) = 0;
	
	virtual GraphicEffect * CreateSparkBurst( viMovingObject *_movingObject,
						const cVector& _localPosition,
						int _numSparks,
						const cVector& _dir,
						float _angularSpread,
						float _minSpeed, float _maxSpeed,
						float _vertAccel,
						float _timeToLive) = 0;

	virtual GraphicEffect * CreateBlastWave(const cPoint& pos, float endSize, bool onSurface, float ttl) = 0;

	/*создать эффект ODCSParticleSystem по его типу*/	
	virtual GraphicEffect *CreateOPSEffect(OEffectEnum::OEffectType type, viMovingObject *mObject, const cVector *localPos, unsigned preset, float scale = 1.f, float lifetime = -1) = 0;
	virtual GraphicEffect *CreateOPSEffect(OEffectEnum::OEffectType type, viMovingObject *mObject, const Vector3 *localPos, unsigned preset, float scale = 1.f, float lifetime = -1) = 0;
	virtual GraphicEffect *CreateOPSEffect(OEffectEnum::OEffectType type, viMovingObject *mObject, const osg::Vec3f *localPos, unsigned preset, float scale = 1.f, float lifetime = -1) = 0;
	virtual GraphicEffect *CreateOPSEffect(OEffectEnum::OEffectType type, viMovingObject *mObject, const osg::Matrixf *localPos, unsigned preset, float scale = 1.f, float lifetime = -1) = 0;
	virtual GraphicEffect *CreateOPSEffect(OEffectEnum::OEffectType type, viMovingObject *mObject, const Position3 *localPos, unsigned preset, float scale = 1.f, float lifetime = -1) = 0;


	//Statistics
	virtual void	addToStatistics(const char *className) = 0;
	virtual void	removeFromStatistics(const char *className) = 0;
	virtual int		getAliveEffectsCount() = 0;

};

inline GraphicEffectManager * GetGraphicEffectManager()
{
    return (GraphicEffectManager*)resourcer->GetPlugin( "GeManager" );
}

inline PSEffects::LockonParticleApiPlugin * GetLockonParticleApi()
{
    return (PSEffects::LockonParticleApiPlugin*)resourcer->GetPlugin("ParticleSystemPlg");
}

} // namespace Effects

#endif // _GraphicEffectManager_h_
