#ifndef __SHELL_GROUP_H__
#define __SHELL_GROUP_H__

#include "activity_group.h"
#include "Shells/shell_new.h"	// Это нужно, потому что иначе wActivityGroupEnvelope не компилируется. Плохо, надо исправить

#include "Math/Vector.h"


const double cShellDefaultDt = 0.05;
namespace Effects
{
	class GraphicEffect;
	struct IGraphicEffectParamBlock;
}
// FIXME: Можно это на функтор заменить, надо ли?
// Пока смущает, что функторы в общей куче выделяются - хреново
class WEAPONSBASE_API wShellExplosionListener
{
public:
	virtual void onShellExplosion(const Math::Vec3d& point) = 0;
};

class WEAPONSBASE_API wShellGroup : public wActivityGroupEnvelope<wShellGroup, wShell>
{
private:
	typedef wActivityGroupEnvelope<wShellGroup, wShell> base;
	friend class base;

public:
	wShellGroup(double dt);
	wShellGroup(double dt, wShellExplosionListener* listener);
	virtual ~wShellGroup();

	void setExplosionListener(wShellExplosionListener* listener);


	void makeSmokeEffect(const dVector & pnt,
						 const cVector& speed,
						 float lifetime,
						 float mass,
						 float cx);

protected:
	wActivityGroupMember::eSimulationResult makeObjectStep_(wShell* shell, double dt);

protected:
	wShellExplosionListener * listener_;

	Effects::GraphicEffect  * effect_;
	Effects::IGraphicEffectParamBlock *effectPB_;
};


#endif
