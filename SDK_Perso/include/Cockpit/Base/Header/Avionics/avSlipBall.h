#pragma once
#include "CockpitBase.h"
#include "Utilities/avBasicTimer.h"
#include "cAlgorithm.h"

namespace Lua
{ 
    class Config;
}

namespace EagleFM 
{ 
    class SlipBall;
}

namespace cockpit
{

class avSlipBallTimer;
class COCKPITBASE_API avSlipBall
{
public:
	avSlipBall();
	virtual ~avSlipBall();
	double  get_sideslip() const;
    void    initialize();

	void    set_dt(wModelTime dtIn);
	void	set_tube_lenght(double tube_lenght);
	void	set_tube_sector(double tube_sector);

	void	post_initialize();
	void    loadFromState(Lua::Config& config, const char* table);

private:
    avSlipBallTimer    * timer;
    EagleFM::SlipBall  * slip_ball_physic;

	double len, sector;
	wModelTime dt;
};


}
