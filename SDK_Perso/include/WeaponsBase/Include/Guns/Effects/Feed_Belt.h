#pragma once

#include "Guns/Effects/Gun_Effect.h"

namespace weapons_gun
{

class FeedBeltDesc : public Gun_Effect_Desc
{
	friend class FeedBelt;
public:
	virtual void l_read(Lua::Config& config) override;
	virtual void create(wAircraftGunMount* gun_mount) override;

private:
	int arg_main_;
	int arg_tail_;

	unsigned int tail_load_;
};

class FeedBelt : public VisualEffect
{
	friend class FeedBeltDesc;

public:
	virtual void draw_update() override;

protected:
	FeedBelt(const FeedBeltDesc* desc, wAircraftGunMount* gun_mount);

	const FeedBeltDesc* desc_;

	int prev_rounds_;
	double prev_time_;
	float belt_pos_;
};

//////////////////////////////////////////////////////////////////////////
class FeedBeltBoxDesc : public Gun_Effect_Desc
{
	friend class FeedBeltBox;
public:
	virtual void l_read(Lua::Config& config) override;
	virtual void create(wAircraftGunMount* gun_mount) override;

private:
	int arg_h_;
	int arg_v_;
	unsigned int max_count_;
};

class FeedBeltBox : public SimulateEffect
{
	friend class FeedBeltBoxDesc;

public:
	virtual void simulate(double dt) override;

protected:
	FeedBeltBox(const FeedBeltBoxDesc* desc, wAircraftGunMount* gun_mount);

	const FeedBeltBoxDesc* desc_;

	float prev_rounds_;
};

}