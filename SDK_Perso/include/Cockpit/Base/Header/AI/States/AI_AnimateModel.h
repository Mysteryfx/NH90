#pragma once

#include "AI/States/AI_IState.h"

namespace gunner_AI
{
	struct ArgumentState
	{
		ArgumentState() : arg_(-1), pylon_(-1), speed_(0.0f), target_(0.0f), init_(UNDEF_VALUE), finished_(false) {}
		virtual ~ArgumentState(){}

		virtual bool	l_read(Lua::Config& config);

		int		arg_;
		char	pylon_; /*arg location, -1 - main LA model*/
		float	speed_; /*changing speed*/
		float	target_;
		float	init_; /*used when initialized*/ 
		bool	finished_;

		float	cur_speed_;
	};

	class CndArgumentState : public ArgumentState
	{
	public:
		~CndArgumentState();

		virtual bool l_read(Lua::Config& config) override;

		ConditionVector conditions_;
	};

	typedef ed::vector<CndArgumentState*> CndArgStateVector;


	class AnimateModelState : public State
	{
	public:

		AnimateModelState();
		~AnimateModelState();

		virtual void l_read(Lua::Config& config) override;
		virtual void on_start() override;
		virtual void simulate(double dt) override;

	private:
		CndArgStateVector arguments_;
	};

	class InitModelState : public State
	{
	public:

		InitModelState();
		~InitModelState();

		virtual void l_read(Lua::Config& config) override;
		virtual void on_finish() override;

	private:

		ArgsVector arguments_;
	};
}