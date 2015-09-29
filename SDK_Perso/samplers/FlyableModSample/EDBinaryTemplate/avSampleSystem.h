#pragma once
#include "Avionics\avDevice.h"
#include "Avionics/Weapon/avWeaponsCommon.h"

namespace cockpit
{
	namespace Samples
	{
		class avSampleSystem : public avDevice
		{
		public:
			avSampleSystem();
			~avSampleSystem();

			virtual void initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name);
			virtual void SetCommand(int command, float value = 0);
			virtual void update();

			const dVector* get_gun_sight_funnel() const
			{
				return FunnelPoints;
			}

		private:

			GunSight gun_sight;

			void update_gun_sight_funnel();

			dVector FunnelPoints[12];
			std::queue<dVector> Omega[6];
			dVector Angles[6];
			cVector Om_old;
		};
	}
}
