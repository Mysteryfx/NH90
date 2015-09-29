#ifndef __SIMULATION_SYSTEM_H__
#define __SIMULATION_SYSTEM_H__

#include "../blocksim.h"
#include "simulation_block.h"

class wSimulationSystemScheme;
class wOdeSolver;

///////////////////////////////////////////////////////////////
// SYSTEM
///////////////////////////////////////////////////////////////

class BLOCKSIM_API wSimulationSystem
{
protected:
	class EventsQueue
	{
	public:
		struct Entry
		{
			Entry() : block(-1), time(-1) {}
			Entry(int _block, double _time) : block(_block), time(_time) {}

			bool operator < (const Entry& e) const
			{
				double d = time - e.time;
				if (fabs(d) > 1E-9) return d < 0;
				return block < e.block;
			}

			int block;
			double time;
		};

		const Entry& top();
		bool empty();
		void removeTop();
		void updateTop(double new_time);
		void add(const Entry& e);
		void clear();

	protected:
		void heapify_(int i);

	protected:
		ed::vector<Entry> entries_;
	};

public:
	// scheme - схема
	// runtimeCfg - настройки рантайма
	// t0 - начальный момент времени
	wSimulationSystem(const wSimulationSystemScheme* scheme, Lua::Loader* runtimeCfg, lead_data_ptr_t memory);
	virtual ~wSimulationSystem();

	// Сдвинуть систему вперед (то есть сдвинуть все блоки, у которых время <= time)
	// Вернуть через сколько произойдет следующий сдвиг
	double simulate(double time);

	// Сбросить схему - время в ноль, для всех discrete блоков вызывается reset
	// TODO: рассмотреть этот момент подробнее
	virtual void reset(); 

	wInputPortBase* getInputPort(const ed::string& name, bool throw_ex = true);
	wOutputPortBase* getOutputPort(const ed::string& name, bool throw_ex = true);

	template <class T> T* getInputWireAddress(const ed::string& name, bool throw_ex = true);
	template <class T> const T* getOutputWireAddress(const ed::string& name, bool throw_ex = true);
	template <class T> wInputPort<T>* getInputPort(const ed::string& name, bool throw_ex = true);
	template <class T> wInputPortObjPtrBase<T>* getInputPortObjPtr(const ed::string& name, bool throw_ex = true);
	template <class T> wOutputPort<T>* getOutputPort(const ed::string& name, bool throw_ex = true);

	template <class T> 
	bool updateInputWireSafe(const ed::string& name,const T & value)
	{
		T * wire = getInputWireAddress<T>(name,false);
		if (wire)
		{
			*wire = value;
			return true;
		}
		return false;
	}
	template <class T> 
	bool getInputWireValueSafe(const ed::string& name,T & value)
	{
		T * wire = getInputWireAddress<T>(name,false);
		if (wire)
		{
			T = *wire;
			return true;
		}
		return false;
	}

	template <class T> 
	bool updateInputPortSafe(const ed::string& name,const T & value)
	{
		wInputPort<T>* port = getInputPort<T>(name,false);
		if (port)
		{
			port->update(value);
			return true;
		}
		return false;
	}

	template <class T> 
	bool getInputPortValueSafe(const ed::string& name,T & value)
	{
		wInputPort<T>* port = getInputPort<T>(name,false);
		if (port)
		{
			value = port->value;
			return true;
		}
		return false;
	}

	///////////////////////////////////////
	// Для блоков
	///////////////////////////////////////
	lead_data_ptr_t getDataPtr() { return memory_; }

	///////////////////////////////////////
	// для солверов
	///////////////////////////////////////
	int getDiffStateSize();
	double* getDiffState();
	// вычислить правую часть системы в момент времени t, результат записать в res
	void calcDiffRhs(double t, double* res);

protected:
	void init_(Lua::Loader* runtime_config);

	// update all dirty stateless blocks on input side of a discrete block
	void updateDirty_(const ed::vector<int>& blocks, double curTime);
	// mark all dependent stateless blocks as dirty
	void setDirty_(const ed::vector<int>& blocks);

	// Differential simulation
	double advanceDifferential_(double t, double dt);
	// Reset differential state (from corresponding wires)
	void resetDiff_(bool);
protected:
	lead_data_ptr_t memory_;	  // Может не выделяться схемой
	wSimulationBlock** blocks_;
	bool* dirty_;

	wOdeSolver* odeSolver_;

	ed::vector<wInputPortBase*> inputPorts_;
	ed::vector<wOutputPortBase*> outputPorts_;
	ed::vector<wProxyPortBase*> portProxies_;

	wInputPortImpl<bool, wSimulationSystem> resetDiffPort_;

	const wSimulationSystemScheme* scheme_;

	EventsQueue queue_;

	friend class wBlockSubsystem;
};

////////////////////////////////////////////////////////////////////////
// Template methods
////////////////////////////////////////////////////////////////////////
template <class T> 
T* wSimulationSystem::getInputWireAddress(const ed::string& name, bool throw_ex)
{
	int idx = scheme_->getIOBlock()->getLeadIndex(false, false, name, throw_ex);
	if (idx == -1) return 0;
	if (scheme_->getIOBlock()->getOutputWires()[idx].type() != wSimulationLeadTraits<T>::lead_type)
		if (throw_ex)
			throw wSimulationException(FMT("Invalid type of output wire \"" << name << "\" requested"));
		else
			return 0;

	return (T*)(memory_ + scheme_->getIOBlock()->getOutputWireOffset(idx));
}

template <class T> 
const T* wSimulationSystem::getOutputWireAddress(const ed::string& name, bool throw_ex)
{
	int idx = scheme_->getIOBlock()->getLeadIndex(true, false, name, throw_ex);
	if (idx == -1) return 0;
	if (scheme_->getIOBlock()->getInputWires()[idx].type() != wSimulationLeadTraits<T>::lead_type)
		if (throw_ex)
			throw wSimulationException(FMT("Invalid type of output wire \"" << name << "\" requested"));
		else
			return 0;

	return (T*)(memory_ + scheme_->getIOBlock()->getInputWireOffset(idx));
}

template <class T> 
wInputPort<T>* wSimulationSystem::getInputPort(const ed::string& name, bool throw_ex)
{
	wInputPortBase* res = getInputPort(name, throw_ex);
	if (res && !res->isOfType<T>()) 
		throw wSimulationException(FMT("Invalid type of input port \"" << name << "\"" << ", expected " << getLeadTypeName<T>()));

	return static_cast<wInputPort<T>*>(res);
}

template <class T> 
wInputPortObjPtrBase<T>* wSimulationSystem::getInputPortObjPtr(const ed::string& name, bool throw_ex)
{
	wInputPortBase* res = getInputPort(name, throw_ex);
	if (res && !dynamic_cast<wInputPortObjPtrBase<T>*>(res)) 
	{
		if (dynamic_cast<wInputPortObjPtrUnknown*>(res))
		{
			static wInputPortObjPtrFake<T> res;
			return &res;
		}
		else
			throw wSimulationException(FMT("Invalid type of input port object ptr \"" << name << "\""));
	}

	return static_cast<wInputPortObjPtrBase<T>*>(res);
}

template <class T> 
wOutputPort<T>* wSimulationSystem::getOutputPort(const ed::string& name, bool throw_ex)
{
	wOutputPortBase* res = getOutputPort(name, throw_ex);
	if (res && !res->isOfType<T>()) 
		throw wSimulationException(FMT("Invalid type of output port \"" << name << "\"" << ", expected " << getLeadTypeName<T>()));

	return static_cast<wOutputPort<T>*>(res);
}


#endif