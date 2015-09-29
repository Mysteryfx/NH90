#pragma once

#include "AI/Conditions/AI_ICondition.h"

namespace Lua { class Config; }

namespace gunner_AI
{
	typedef bool (*Comparer)(float,float);

	class ArgumentOperator : public Condition
	{
	public: 
		virtual bool l_read(Lua::Config& config) override;
		virtual bool operator()(const AI_Gunner* host) const override;

	protected:
		ArgumentOperator(Comparer comparer) : arg_(-1), pylon_(-1), cond_val_(0.0f),comparer_(comparer) {}

		float	cond_val_;
		int		arg_;
		char	pylon_;

		Comparer comparer_;
	};

	class OperatorE : public ArgumentOperator /*equal*/
	{
	public: 
		OperatorE();
	};

	class OperatorNE : public ArgumentOperator /*not equal*/
	{
	public: 
		OperatorNE();
	};

	class OperatorEG : public ArgumentOperator /*equal or greater*/
	{
	public: 
		OperatorEG();
	};

	class OperatorEL : public ArgumentOperator /*equal or less*/
	{
	public: 
		OperatorEL();
	};

	//////////////////////////////////////////////////////////////////////////
	class OperatorJMP : public Condition
	{
	public: 
		virtual bool operator()(const AI_Gunner* host) const {return true; }

	protected:
	};
	//////////////////////////////////////////////////////////////////////////
	typedef bool (*CtrComparer)(const char*,const char*);

	class CommandOperator : public Condition
	{
	public: 
		virtual bool l_read(Lua::Config& config) override;
		virtual bool operator()(const AI_Gunner* host) const;

	protected:
		CommandOperator(CtrComparer comparer) : comparer_(comparer) {}

		ed::vector<ed::string> commands_;

		CtrComparer comparer_;
	};

	class OperatorStrE : public CommandOperator /*not equal*/
	{
	public: 
		OperatorStrE();
	};
}