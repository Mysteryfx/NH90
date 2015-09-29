#pragma once

#include "WeaponsBase.h"
#include "descriptor_base.h"

#include "Math/Vector.h"

class wIGunCarriage;
class wAmmoSupply;

namespace weapons_gun
{

class Gun_Effect_Desc;
// ѕушка с несколькими типами патронов и программируемой скорострельностью
class WEAPONSBASE_API wGunDescriptor : public wDescriptorBase
{
	friend class wGun;
	friend class wAircraftGunMount;

public:
	wGunDescriptor(const ed::string& name) : wDescriptorBase(name) {}
	virtual ~wGunDescriptor();

	double getFireRate(int rateIndex) const;
	double getFireDelay(int rateIndex) const;

	int	getIndexByFireDelay(double fireDelay) const; //подобрать подход€щий индекс по фактическому fireDelay

	int getBurstCutoff() const { return maxBurstLength_; }

	double getRecoilCoeff() const { return recoilCoeff_; }

	const Math::Vec3d & getImpulseVectorRot() const { return impulseVectorRot_; }

	virtual void load(wDatabaseLoader* loader);

	// ћинус в том, что у унаследованного класса можно вызвать этот метод (если унаследованный класс не перекрывает его)
	// “о есть в некотором роде некорректно обрабатываетс€ ситуаци€, когда данные и код не совпадают...
	virtual wGun* create(wIGunCarriage* carriage, wAmmoSupply* supply, bool netMaster) const;

protected:
	ed::vector<double> fireDelays_;

	int maxBurstLength_;	// ќтсечка

	double recoilCoeff_;

	Math::Vec3d impulseVectorRot_;

	ed::vector<weapons_gun::Gun_Effect_Desc*> effects_;
};
}