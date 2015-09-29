#ifndef __FSM_H__
#define __FSM_H__	1

//The Finite State Machine - the mathematical abstraction to model discrete systems.
//Moore / Mealy FSM + state stack support.
//Handlers of output symbols are external object.

#include "_config.h"

#include <ed/vector.h>
#include <ed/deque.h>

namespace Common {

class ED_CORE_EXTERN FSM
{
public:
	typedef unsigned int				NumericType;
	typedef NumericType					State;
	typedef NumericType					InputSymbol;
	typedef NumericType					OutputSymbol;
	static const State					NO_STATE	= 0xFFFFFFFD; //State not changed
	static const State					ANY_STATE	= 0xFFFFFFFD; //State is undefined
	static const State					NEW_STATE	= 0xFFFFFFFE; //
	static const State					PREVIOUS_STATE	= 0xFFFFFFFF; //Return to previous state
	static const OutputSymbol			NO_SYMBOL	= 0xFFFFFFFD; //No output symbol is generated
	typedef ed::deque<State>			StateStack;
	typedef ed::deque<InputSymbol>		InputSymbolsQueue; //Symbols queue for delayed procession
	//Stack operations
	enum StackOperation
	{
		STACK_OPERATION_DONT_PUSH_TO_STACK,
		STACK_OPERATION_PUSH_TO_STACK,
		STACK_OPERATION_REPLACE_STACK_TOP,
		STACK_OPERATION_CLEAR_STACK
	};
	struct Transition
	{
	public:
		ED_CORE_EXTERN Transition();
		ED_CORE_EXTERN Transition(OutputSymbol outputSymbol_, State targetState_, StackOperation stackOperation_ = STACK_OPERATION_DONT_PUSH_TO_STACK);
	public:
		OutputSymbol	outputSymbol;
		State			targetState;
		StackOperation	stackOperation;
	};
	typedef ed::vector<Transition> Transitions; //Transitions mapped by input symbols
	//Recipient of the output symbol
	class SymbolRecepient
	{
	public:
		virtual void onSymbol(OutputSymbol outputSymbol) = 0;
	};
	typedef ed::vector<SymbolRecepient*> SymbolRecepients;
	class ED_CORE_EXTERN Descriptor
	{
	private:
		typedef ed::vector<Transitions>	TransitionMap; //transitions mapped by init state and symbol
		typedef ed::vector<State>			States;
		typedef ed::vector<OutputSymbol>	OutputSymbols;
	public:		
		Descriptor();
		void				setInitializied();
		void				clear();
		void				load();
		void				setInitialState(State state);
		void				addFinishState(State state);
	protected:
		void				addTransition(State oldState_, InputSymbol inputSymbol_, OutputSymbol outputSymbol_, State targetState_, StackOperation stackOperation_ = STACK_OPERATION_DONT_PUSH_TO_STACK);
		void				addOnExitOutputSymbol(State state, OutputSymbol outputSymbol);
		void				addOnEnterOutputSymbol(State state, OutputSymbol outputSymbol);
	public:
		bool				isInitializied() const;
	private:
		friend class		FSM;
		State				getInitialState() const;
		bool				isFinishState(State state) const;
		const Transition *	getTransition(InputSymbol symbol) const;
		const Transition *	getTransition(State initState, InputSymbol symbol) const;
		const Transitions *	getTransitions(State state) const;
	protected:
		bool				initializied;
		State				initState;
		States				finishStates;
		Transitions			transitions; //Old state independent transitions
		TransitionMap		transitionMap; //Old state dependent transitions
		OutputSymbols		onStateExitOutputSymbols; //Symbols which are thrown each time the fsm exits from a state
		OutputSymbols		onStateEnterOutputSymbols;//Symbols which are thrown each time the fsm enters to a state
	};
public:
	FSM(const char * nameIn);
	FSM(const char * nameIn, Descriptor * descriptionIn, SymbolRecepient * symbolRecepientIn);
	void	init(Descriptor * descriptionIn, SymbolRecepient * symbolRecepientIn);
	void	uninit();
	//
	void	reset();
protected:
	void	onSymbol(InputSymbol symbol);
	void	onSymbolDelayed(InputSymbol symbol);
public:	
	void	checkSymbolsQueue();
	//
	bool	valid() const;
	State	getState() const;
	bool	inInitialState() const;
	bool	inFinishState() const;
	bool	symbolsQueueIsEmpty() const;
	//
	const Descriptor *	getDescriptor() const;
private:
	void	onSymbol_(InputSymbol inputSymbol);
	void	exitFromState_();
	void	enterToState_(State targetState_);
	void	sendOutputSymbol_(OutputSymbol outputSymbol);
private:
	SymbolRecepients 	symbolRecepients;
	State				state;
	InputSymbolsQueue	symbolsQueue;
	StateStack			stateStack;
	bool				inProcess;
	bool				finished;
private:
	const Descriptor *	descriptor;
	const char *		name;
};

//Typed FSM used to control symbols type
template<class InputSymbolType, class OutputSymbolType>
class TypedFSM : public FSM
{
public:
	class Descriptor : public FSM::Descriptor
	{
	public:
		void addTransition(FSM::State oldState_, InputSymbolType inputSymbol_, const OutputSymbolType & outputSymbol_, FSM::State targetState_, FSM::StackOperation stackOperation_ = FSM::STACK_OPERATION_DONT_PUSH_TO_STACK)
		{
			FSM::Descriptor::addTransition(oldState_, inputSymbol_, outputSymbol_, targetState_, stackOperation_);
		}
		void addOnExitOutputSymbol(FSM::State state, OutputSymbolType outputSymbol)
		{
			FSM::Descriptor::addOnExitOutputSymbol(state, outputSymbol);
		}
		void addOnEnterOutputSymbol(FSM::State state, OutputSymbolType outputSymbol)
		{
			FSM::Descriptor::addOnEnterOutputSymbol(state, outputSymbol);
		}
	};
	class SymbolRecepient : public FSM::SymbolRecepient
	{
	public:
		virtual void onSymbol(OutputSymbolType outputSymbol)
		{
		}
		inline void onSymbol(FSM::OutputSymbol outputSymbol)
		{
			onSymbol(static_cast<OutputSymbolType>(outputSymbol));
		}
	};
public:
	TypedFSM(const char * nameIn) : FSM(nameIn) {;}
	void onSymbol(InputSymbolType inputSymbol)
	{
		FSM::onSymbol(inputSymbol);
	}
	void onSymbolDelayed(InputSymbolType inputSymbol)
	{
		FSM::onSymbolDelayed(inputSymbol);
	}
};

} // namespace Common

#endif
