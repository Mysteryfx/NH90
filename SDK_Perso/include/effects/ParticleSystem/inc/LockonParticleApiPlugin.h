#ifndef EFFECTS_PARTICLE_SYSTEM_LOCKON_PARTICLE_API_PLUGIN_H
#define EFFECTS_PARTICLE_SYSTEM_LOCKON_PARTICLE_API_PLUGIN_H

#ifndef EDGE

#include "Resourcer/Plugin.h"
#include "ParticleApi.h"

namespace PSEffects {
class LockonParticleApiPlugin : public Plugin{
public:
	LockonParticleApiPlugin():Plugin("ParticleSystemPlg"){}

	virtual EffectHolderPtr createEffect(const char* config, const osg::Matrixd &pos, float scale, float duration = -1.f,osg::Vec3f *color = nullptr) = 0;
	virtual EffectHolderPtr createEffect(const char* config, const viObject* obj, const osg::Vec3f& offset, float scale, float duration = -1.f,osg::Vec3f *color = nullptr) = 0;
};

}

#endif

#endif
