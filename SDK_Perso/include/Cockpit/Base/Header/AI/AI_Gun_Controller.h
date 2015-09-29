#pragma once

#include "wMovingObject.h"

#include "AI/AI_Gun_Handler.h"

#include <array>

namespace Lua {
	class Config;
}

class IwoLA;

namespace gunner_AI {

class AI_Gunner;
class IGunHandler;
class ISightAI;

class GunController
{
public:

	friend class AI_Gunner;

	GunController(AI_Gunner* host);
	~GunController();

	void l_read(Lua::Config& config, IwoLA* la);

	void simulate(double dt);

	void on_start();
	void on_end();
	void fire(bool on);

	void reload(); /*call when weapon might be changed*/

	float azimuth_e() const; /*azimuth deviation*/
	float elevation_e() const; /*elevation deviation*/
	float eff_dist() const;
	bool  friendly_fire() const; //friendly fire probability

	float  get_ammo_rest() const;  /*returns ammo rest*/

	void set_custom_handler(IGunHandler_Ptr handler);
	void reset_custom_handler();

	ISightAI* swap_sights(ISightAI* new_sight);

protected:
	std::array<IGunHandler_Ptr,2> handlers_; /*handlers: 0 - default; 1 - custom */
	char cur_handler_;

	AI_Gunner*	host_gunner_;
	ISightAI*	 sight_;
};

}