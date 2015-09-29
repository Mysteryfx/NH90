#pragma once

#include "wTime.h"

class wShellGroup;

namespace weapons_gun
{

class wGun;

class wGunShellActivity : public wTime
{
public:
	wGunShellActivity(wGun* const host);
	~wGunShellActivity();

	void DoStop();
	void open_fire();

protected:

	double	fireBullet_();	// Вернуть через сколько пушка будет готова к стрельбе 
	virtual void NextEvent();
	double	simulate_();

	wShellGroup* shellGroup_;
	wGun* const host_;
	bool to_cancel_;
};
}