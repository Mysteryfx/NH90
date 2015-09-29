#pragma  once;
#include "Mem/Shared.h"
#include "osg/vec3f"

namespace PSEffects
{
	class EffectHolder: public Mem::Shared
	{
	public:
		virtual ~EffectHolder(){}

		virtual void detach() = 0;
		virtual void stop() = 0;
		virtual bool isActive() = 0;
		virtual void clear() = 0;
		virtual void changeLocalPos(const osg::Vec3f &pos) = 0;
	};
}

typedef Mem::Ptr<PSEffects::EffectHolder> EffectHolderPtr;
