#ifndef __SIMULATION_WIRES_H__
#define __SIMULATION_WIRES_H__

#include <assert.h>

#include <ed/string.h>
#include <ed/vector.h>

#include "Math/Vector.h"
#include "Math/Rotation3.h"
#include "simulation_exception.h"

#include "simulation_wire_types.h"

#ifdef __WIRES_IMPL
#define __WIRE_EXTERN
#else
#define __WIRE_EXTERN extern
#endif

const ed::string leadTypeNames[] = {
	"integer",
	"boolean",
	"double",
	"vector",
	"quaternion",
	"object ptr",
    "unsigned"
};

class wInputPortBase;
class wOutputPortBase;
class wProxyPortBase;
template <class T> class wOutputPort;
template <class T> class wProxyPort;

struct wLeadCreator
{
	wLeadCreator() {}

	template <class T> T* create();
	template <> wOutputPortBase* create() { return createOutputPort(); }

	virtual wInputPortBase* createFakeInputPort() = 0;
	virtual wOutputPortBase* createOutputPort() = 0;

private:
	wLeadCreator(const wLeadCreator&);
	wLeadCreator& operator = (const wLeadCreator&);
};

extern wLeadCreator* leadGenerators[LEADTYPE_LAST];

template <class T, int id> struct wConcreteLeadCreator : public wLeadCreator
{
	wConcreteLeadCreator()
	{
		leadGenerators[id] = this;
	}

	virtual wInputPortBase* createFakeInputPort() { return new wInputPort<T>(); }
	virtual wOutputPortBase* createOutputPort() { return new wOutputPort<T>(); }
};

class wWireObjectPtrBase;

template <int id> struct wConcreteLeadCreator<wWireObjectPtrBase, id> : public wLeadCreator
{
	wConcreteLeadCreator() { leadGenerators[id] = this; }

	virtual wInputPortBase* createFakeInputPort() { return new wInputPortObjPtrUnknown(); }
	virtual wOutputPortBase* createOutputPort()  { throw wSimulationException("Cannot create output port of type wWireObjectPtrBase"); }
};

/////////////////////////////////////////////////////////////////
// PORTS
/////////////////////////////////////////////////////////////////
template <class T> struct __port_parameter_deriver { typedef const T& type; };
template <> struct __port_parameter_deriver<int> { typedef int type; };
template <> struct __port_parameter_deriver<double> { typedef double type; };
template <> struct __port_parameter_deriver<bool> { typedef bool type; };

// Input port 
class wInputPortBase
{
public:
	virtual ~wInputPortBase() {};

	virtual eLeadType getType() const = 0;
	virtual wProxyPortBase* getProxy() = 0;

	template <class T> bool isOfType() const { return getType() == (eLeadType)wSimulationLeadTraits<T>::lead_type; }
};

template <class T>
class wInputPort : public wInputPortBase
{
public:
	typedef T type;
	wInputPort()
	{
		value = T();
	}
	virtual void update(typename __port_parameter_deriver<T>::type value)
	{
		this->value = value;
	}
	virtual eLeadType getType() const { return (eLeadType)wSimulationLeadTraits<T>::lead_type; }
	virtual wProxyPortBase* getProxy() { return new wProxyPort<T>(); }
public:
	T value;
};

template <class T, class O>
class wInputPortImpl : public wInputPort<T>
{
public:
	typedef T type;

	typedef void (O::*update_function_ptr)(typename __port_parameter_deriver<T>::type value);

public:
	wInputPortImpl(O* owner, update_function_ptr func)
	{
		owner_ = owner;
		func_ = func;
	}

	void update_func(update_function_ptr func)
	{
		func_ = func;
	}

	virtual void update(typename __port_parameter_deriver<T>::type value)
	{
		this->value = value;
		(owner_->*func_)(this->value);
	}
protected:
	O* owner_;
	update_function_ptr func_;
};

/*template <class T> 
class wInputPortFunctor : public wInputPort<T>
{
public:
	typedef Functor<void, TYPE_LIST1(T)> functor_type;

	wInputPortFunctor(const functor_type& functor) : functor_(functor) {};

	virtual void update(typename __port_parameter_deriver<T>::type value)
	{
		functor_(value);
	}

protected:
	functor_type functor_;
};*/

class wOutputPortBase
{
public:
	virtual ~wOutputPortBase() {};

	template <class T> bool isOfType() const { return getType() == (eLeadType)wSimulationLeadTraits<T>::lead_type; }

	virtual eLeadType getType() const = 0;
	virtual void connect(wInputPortBase* port) = 0;

	virtual wProxyPortBase* getProxy() const = 0;
};

template <class T>
class wOutputPort : public wOutputPortBase
{
public:
	typedef T type;

	virtual eLeadType getType() const { return (eLeadType)wSimulationLeadTraits<T>::lead_type; }

	void send(typename __port_parameter_deriver<T>::type value)
	{
		for (int i=0; i < (int)receivers_.size(); i++)
			receivers_[i]->update(value);
	}

	virtual void connect(wInputPortBase* port)
	{
		if (port->getType() != wSimulationLeadTraits<T>::lead_type)
			throw wSimulationException("Attempt to connect ports of different types");

		receivers_.push_back(static_cast<wInputPort<T>*>(port));
	}

	virtual wProxyPortBase* getProxy() const { return new wProxyPort<T>(); }

protected:
	ed::vector<wInputPort<T>*> receivers_;
};

class wProxyPortBase
{
public:
	virtual ~wProxyPortBase() {}

	virtual wInputPortBase* getInput() = 0;
	virtual wOutputPortBase* getOutput() = 0;
};


template <class T>
class wProxyPort : public wProxyPortBase, public wInputPort<T>
{
public:
	virtual wInputPortBase* getInput() { return this; }
	virtual wOutputPortBase* getOutput() { return &output_; }
	
	virtual void update(typename __port_parameter_deriver<T>::type value)
	{
		output_.send(value);
	}

	virtual void connect(wInputPortBase* port)
	{
		output_.connect(port);
	}

	virtual eLeadType getType() const { return output_.getType(); }

protected:
	wOutputPort<T> output_;
};

#include "simulation_wires_object_ptr.h"

///////////////////////////////////////////////////////////////////
// Type info & other
///////////////////////////////////////////////////////////////////
#define TYPE_TRAIT(type, _id) \
	template <> struct wSimulationLeadTraits<type> { enum { lead_type =  _id }; }; \
	template <> struct wSimulationLeadTypeFromIndex<_id> { typedef type wire_type; }; \
	__WIRE_EXTERN wConcreteLeadCreator<type, _id> __leadCreator##_id;

template <class T> struct wSimulationLeadTraits {};
template <int T>   struct wSimulationLeadTypeFromIndex {};
TYPE_TRAIT(int, LEAD_INTEGER);
TYPE_TRAIT(bool, LEAD_BOOLEAN);
TYPE_TRAIT(double, LEAD_DOUBLE);
TYPE_TRAIT(Math::Vec3d, LEAD_VECTOR);
TYPE_TRAIT(Math::Rot3d, LEAD_QUATERNION);
TYPE_TRAIT(wWireObjectPtrBase, LEAD_OBJECT_PTR);
TYPE_TRAIT(unsigned, LEAD_INTEGER_UNSIGNED);

#undef TYPE_TRAIT


// Port declarations
#define INPUT_PORT(type, name) wInputPortImpl<type, self_type> name
#define INPUT_PORT_CONST(type, name) wInputPort<type> name
#define OUTPUT_PORT(type, name) wOutputPort<type> name

#define INIT_PORT(name, func) name(this, &self_type::func)

// useful functions
template <class T>
const ed::string& getLeadTypeName()
{
	int t = wSimulationLeadTraits<T>::lead_type;
	assert(t >= 0 && t < sizeof(leadTypeNames)/sizeof(leadTypeNames[0]));
	return leadTypeNames[t];
}

// Не самый прямой способ
template <class T, class R, class Arg> R decodeWireType(eLeadType type, T func, Arg a)
{
	switch (type)
	{
#define TRY(x) case x : return func.process<wSimulationLeadTypeFromIndex<x>::wire_type>(a);
		TRY(LEAD_INTEGER);
		TRY(LEAD_BOOLEAN);
		TRY(LEAD_DOUBLE);
		TRY(LEAD_VECTOR);
		TRY(LEAD_QUATERNION);
		TRY(LEAD_OBJECT_PTR);
        TRY(LEAD_INTEGER_UNSIGNED);
#undef TRY
	}

	// PANIC
	assert(0);
	abort();

	return R();
}

template <class T, class Arg> void decodeWireTypeV(eLeadType type, T& func, Arg a)
{
	switch (type)
	{
#define TRY(x) case x : func.process<wSimulationLeadTypeFromIndex<x>::wire_type>(a); return;
		TRY(LEAD_INTEGER);
		TRY(LEAD_BOOLEAN);
		TRY(LEAD_DOUBLE);
		TRY(LEAD_VECTOR);
		TRY(LEAD_QUATERNION);
		TRY(LEAD_OBJECT_PTR);
        TRY(LEAD_INTEGER_UNSIGNED);
#undef TRY
	}

	assert(0);
	abort();
}

struct lead_size_decoder
{
	template <class T>
	int process(int) { return sizeof(T); }
};


inline int getLeadSize(eLeadType type)
{
	lead_size_decoder d;
	return decodeWireType<lead_size_decoder, int, int>(type, d, 0);
}


namespace Lua { class Loader; }
void readLeadsList(Lua::Loader* cfg, ed::vector<wLeadDescription>& leads, const ed::string& name);
void writeLeadsList(Lua::Loader* cfg, ed::vector<wLeadDescription>& leads, const ed::string& name);

wInputPortBase** generateInputFunctorPorts(const ed::vector<wLeadDescription>& desc);
wOutputPortBase** generateOutputPorts(const ed::vector<wLeadDescription>& desc);

#endif 
