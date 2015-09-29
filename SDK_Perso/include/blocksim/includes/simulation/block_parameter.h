#ifndef __BLOCK_PARAMETER_H__
#define __BLOCK_PARAMETER_H__

#include <assert.h>
#include <ed/string.h>
#include <ed/sstream.h>
#include <iomanip>

#include "../blocksim.h"

#include "simulation_exception.h"
#include "lua/lua_loader.h"

#ifdef PARAM_IMPL
#define __PARAM_EXTERN
#else
#define __PARAM_EXTERN extern
#endif

const int PARAM_MAX_ARRAY_SIZE = 256;

template <class T> struct DefaultSetter {};

#include <ed/vector.h>

template <class T>
class wBlockParameter : public DefaultSetter<T>
{
	friend struct DefaultSetter<T>;

	template <class T> struct array_decorator { typedef T type; typedef const T const_type; typedef T& reference_type; };
	template <class T, int i> struct array_decorator<T[i]> { typedef T* type; typedef const T* const_type; typedef T* reference_type; };
	template <class T> struct array_decorator<ed::vector<T> > { typedef ed::vector<T> type; typedef const ed::vector<T>& const_type; typedef ed::vector<T>& reference_type; };
	template <> struct array_decorator<Math::Vec3d> { typedef Math::Vec3d type; typedef const Math::Vec3d& const_type; typedef Math::Vec3d& reference_type; };
	template <> struct array_decorator<ed::string> { typedef ed::string type; typedef const ed::string& const_type; typedef ed::string& reference_type; };
public:
	// convert arrays to pointers
	typedef typename array_decorator<T>::type underlying_type;
	typedef typename array_decorator<T>::const_type underlying_const_type;
	typedef typename array_decorator<T>::reference_type underlying_reference_type;

public:
	wBlockParameter()
	{ 
		delegated_ = false;
	}

	// Convert from/to value
	operator underlying_const_type() const { return value_; }
	underlying_const_type value() const { return value_; }
	underlying_reference_type value() { return value_; }
	wBlockParameter& operator = (T val) { value_ = val; return *this; } // arrays will not work

	bool getDelegated() const { return delegated_; }
	void setDelegated(bool val) { delegated_ = val; }

private:
	T value_;

	bool delegated_;
};

template <> struct DefaultSetter<double> { DefaultSetter() { reinterpret_cast<wBlockParameter<double>*>(this)->value_ = 0; } };
template <> struct DefaultSetter<int> { DefaultSetter() { reinterpret_cast<wBlockParameter<int>*>(this)->value_ = 0; } };
template <> struct DefaultSetter<bool> { DefaultSetter() { reinterpret_cast<wBlockParameter<bool>*>(this)->value_ = 0; } };
template <int N> struct DefaultSetter<int[N]> { DefaultSetter() {  wBlockParameter<int[N]>* p = reinterpret_cast<wBlockParameter<int[N]>*>(this); memset(p->value_, 0, sizeof(p->value_)); } };
template <int N> struct DefaultSetter<double[N]> { DefaultSetter() {  wBlockParameter<double[N]>* p = reinterpret_cast<wBlockParameter<double[N]>*>(this); memset(p->value_, 0, sizeof(p->value_)); } };
template <> struct DefaultSetter<Math::Vec3d> { DefaultSetter() { reinterpret_cast<wBlockParameter<Math::Vec3d>*>(this)->value_.set(0); } };

struct wParameterFormatter
{
protected:
	template <class T> static ed::string format(const T& v)
	{
		ed::ostringstream s;
		format(v, s);
		return s.str();
	}
	
	template <class T> static void format(const T& v, ed::ostringstream& os)
	{
		os << v;
	}


	template <class T> static void parse(T& v, const ed::string& s)
	{
		ed::istringstream is(s);
		parse(v, is);
		if (is.fail())
			throw wSimulationException("Cannot parse: " + s);
	}

	template <class T> static void parse(T& v, ed::istringstream& is)
	{
		is >> v;
	}

	// Vector
	template <> static void format<Math::Vec3d>(const Math::Vec3d& v, ed::ostringstream& os)
	{
		os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	}

	template <> static void parse<Math::Vec3d>(Math::Vec3d& v, ed::istringstream& is)
	{
		char c;
		is >> c >> v.x >> c >> v.y >> c >> v.z >> c;
	}

public:

	template <class T> static ed::string format(wBlockParameter<T>& p)
	{
		return format(p.value());
	}


	template <class T> static void parse(wBlockParameter<T>& p, const ed::string& str)
	{
		parse(p.value(), str);
	}

	template <class T, int N> static ed::string format(wBlockParameter<T[N]>& p)
	{
		ed::ostringstream os;
		os << format(p[0]);
		for (int i=1; i < N; i++)
			os << " " << format(p[i]);
		return os.str();
	}

	template <class T, int N> static void parse(wBlockParameter<T[N]>& p, const ed::string& str)
	{
		ed::istringstream is(str);
		parse(p.value()[0], is);
		for (int i=1; i < N; i++)
			parse(p.value()[i], is);
	}

	template <class T> static ed::string format(wBlockParameter<ed::vector<T> >& p)
	{
		ed::ostringstream os;
		const ed::vector<T>& v = p.value();
		if (v.size())
		{
			os << format(v[0]);
			for (int i=1; i < (int)v.size(); i++)
				os << " " << format(v[i]);
		}
		return os.str();
	}

	template <class T> static void parse(wBlockParameter<ed::vector<T> >& p, const ed::string& str)
	{
		ed::istringstream is(str);
		ed::vector<T>& v = p.value();
		v.clear();

		is >> std::ws;
		while (!is.eof() && !is.fail())
		{
			T val;
			parse(val, is);
			v.push_back(val);
			is >> std::ws;
		}

		if (is.fail()) 
			throw wSimulationException("Cannot parse: " + str);
	}
};

class wParameterSerializer
{
public:
	template <class T> static void write(Lua::Loader* cfg, const ed::string& name, wBlockParameter<T>& p) { cfg->set(name, p.value()); }
	template <class T> static void read(Lua::Loader* cfg, const ed::string& name, wBlockParameter<T>& p) { cfg->get(name, p.value()); }

	template <class T, int N> static void write(Lua::Loader* cfg, const ed::string& name, wBlockParameter<T[N]>& p)
	{
		cfg->createSubtable(name);
			for (int i=0; i < N; i++)
				cfg->set(i+1, p.value()[i]);
		cfg->endSubtable();
	}

	template <class T, int N> static void read(Lua::Loader* cfg, const ed::string& name, wBlockParameter<T[N]>& p)
	{
		cfg->beginSubtable(name);
		for (int i=0; i < N; i++)
			cfg->get(i+1, p.value()[i]);
		cfg->endSubtable();
	}

	template <class T> static void write(Lua::Loader* cfg, const ed::string& name, wBlockParameter<ed::vector<T> >& p)
	{
		cfg->createSubtable(name);
			for (int i=0; i < (int)p.value().size(); i++)
				cfg->set(i+1, p.value()[i]);
		cfg->endSubtable();
	}

	template <class T> static void read(Lua::Loader* cfg, const ed::string& name, wBlockParameter<ed::vector<T> >& p)
	{
		cfg->beginSubtable(name);
		p.value().clear();
		for (int i=0; cfg->hasEntry(i+1); i++)
		{
			T val;
			cfg->get(i+1, val);
			p.value().push_back(val);
		}
		cfg->endSubtable();
	}
};

enum eParameterType
{
	PARAMETER_INT,
	PARAMETER_DOUBLE, 
	PARAMETER_STRING,
	PARAMETER_VECTOR,
	PARAMETER_BOOLEAN,
    PARAMETER_INT_UNSIGNED,
	PARAMETER_TYPE_LAST
};

const ed::string parameterTypeNames[PARAMETER_TYPE_LAST] = 
{
	"int",
	"double",
	"string",
	"vector",
	"boolean",
    "unsigned"
};

class wBlockParameterProxyBase
{
public:
	virtual ~wBlockParameterProxyBase()  { } 

	virtual bool getDelegated() const = 0;
	virtual void setDelegated(bool v) = 0;

	virtual eParameterType getType() const = 0;

	virtual wBlockParameterProxyBase* copy(bool force = false) = 0;
	virtual wBlockParameterProxyBase* createRef() const = 0;

	virtual ed::string toString() = 0;
	virtual void fromString(const ed::string& str) = 0;
	virtual void write(Lua::Loader* ldr, const ed::string& name) = 0;
	virtual void read(Lua::Loader* ldr, const ed::string& name) = 0;

	virtual void set(const wBlockParameterProxyBase& rhs) = 0;

	bool isSameClass(const wBlockParameterProxyBase& rhs)
	{
		const type_info& my = typeid(*this);
		const type_info& his = typeid(rhs);

		return (my == his) != 0;
	}
};

template <class T> struct wParameterTypeTraits { };

template <class T>
struct wBlockParameterProxy : public wBlockParameterProxyBase
{
public:
	// no copy
	wBlockParameterProxy(const wBlockParameterProxy<T>& rhs);
	wBlockParameterProxy<T>& operator = (const wBlockParameterProxy<T>&);

	wBlockParameterProxy(wBlockParameter<T>* _param, bool owner) : param(_param), owner_(owner) {}
	virtual ~wBlockParameterProxy() { if (owner_) delete param; }

	virtual eParameterType getType() const { return (eParameterType)wParameterTypeTraits<T>::param_type; }

	virtual wBlockParameterProxy<T>* copy(bool force_copy_parameter = false)
	{
		if (!owner_ && !force_copy_parameter) return new wBlockParameterProxy<T>(param, false);
		else
		{
			wBlockParameter<T>* np = new wBlockParameter<T>(*param);
			wBlockParameterProxy<T>* res = new wBlockParameterProxy<T>(np, true);
			return res;
		}
	}

	virtual wBlockParameterProxy<T>* createRef() const
	{
		return new wBlockParameterProxy<T>(param, false);
	}

	virtual bool getDelegated() const
	{
		return param->getDelegated();
	}

	virtual void setDelegated(bool v)
	{
		param->setDelegated(v);
	}

	virtual ed::string toString()
	{
		return wParameterFormatter::format(*param);
	}

	virtual void fromString(const ed::string& str)
	{
		wParameterFormatter::parse(*param, str);
	}

	virtual void write(Lua::Loader* ldr, const ed::string& name)
	{
		wParameterSerializer::write(ldr, name, *param);
	}

	virtual void read(Lua::Loader* ldr, const ed::string& name)
	{
		wParameterSerializer::read(ldr, name, *param);
	}

	virtual void set(const wBlockParameterProxyBase& rhs0)
	{
		assert(isSameClass(rhs0));

		wBlockParameterProxy<T>& rhs = (wBlockParameterProxy<T>&)rhs0;
		*param = *rhs.param;
	}

	wBlockParameter<T>* param;
	bool owner_;		// Parameter can be "owned" - meaning that it is a copy of actual
						// parameter
};

class wBlockParameterContainer;

struct wParameterCreatorBase
{
	virtual wBlockParameterContainer createParameter(const ed::string& name) = 0;
};


__PARAM_EXTERN BLOCKSIM_API wParameterCreatorBase* paramGenerators[PARAMETER_TYPE_LAST];

template <class T, int id> struct __ParameterCreator;

template <class T, int n>
struct wParameterTypeTraits<T[n]> { enum { param_type = wParameterTypeTraits<T>::param_type }; enum { is_array = 1 }; enum { array_size = n }; };
template <class T>
struct wParameterTypeTraits<ed::vector<T> > { enum { param_type = wParameterTypeTraits<T>::param_type }; enum { is_array = 1}; enum { array_size = -1 }; };

class wBlockParameterContainer
{
private:
	wBlockParameterContainer(wBlockParameterProxyBase* proxy, const ed::string& name) : proxy_(proxy), name_(name) {}

public:
	wBlockParameterContainer() : proxy_(0) {}
	template <class T>
	wBlockParameterContainer(wBlockParameter<T>& p, const ed::string& name) : name_(name) { proxy_ = new wBlockParameterProxy<T>(&p, false); }
	wBlockParameterContainer(const ed::string& name, wBlockParameterProxyBase* p) : name_(name), proxy_(p) { }
	wBlockParameterContainer(const wBlockParameterContainer& rhs) :name_(rhs.name()) { proxy_ = rhs.proxy_ ? rhs.proxy_->copy() : 0; }
	~wBlockParameterContainer() { delete proxy_; }

	const ed::string& name() const { return name_; }
	void setName(const ed::string& name) { name_ = name; }

	eParameterType getType() const { return proxy_->getType(); }

	wBlockParameterContainer clone() const
	{
		wBlockParameterProxyBase* np = proxy_->copy(true);
		return wBlockParameterContainer(np, name_);
	}

	wBlockParameterContainer createRef() const
	{
		return wBlockParameterContainer(proxy_->createRef(), name_);
	}

	wBlockParameterContainer& operator = (const wBlockParameterContainer& rhs)
	{
		name_ = rhs.name_;
		if (proxy_) delete proxy_;
		proxy_ = rhs.proxy_ ? rhs.proxy_->copy() : 0;

		return *this;
	}

	void setDelegated(bool v) { proxy_->setDelegated(v); }
	bool getDelegated() const { return proxy_->getDelegated(); }

	ed::string toString() { return proxy_->toString(); }
	void fromString(const ed::string& s) { proxy_->fromString(s); }

	virtual void write(Lua::Loader* ldr, const ed::string& name) { proxy_->write(ldr, name); }
	virtual void read(Lua::Loader* ldr, const ed::string& name) { proxy_->read(ldr, name); }

	void set(const wBlockParameterContainer& rhs) 
	{ 
		if (!proxy_->isSameClass(*rhs.proxy_))
			throw wSimulationException(FMT("Cannot assign " << rhs.name() << " to " << name() << ":different types"));

		proxy_->set(*rhs.proxy_); 
	}

	const wBlockParameterProxyBase* getProxy() const { return proxy_; }
protected:
	wBlockParameterProxyBase* proxy_;
	ed::string name_;
};

#define DECL_PARAM(type, var_name) wBlockParameter<type > var_name;

template <class T, int id>
struct __ParameterCreator : public wParameterCreatorBase
{
	__ParameterCreator() { paramGenerators[id] = this; }
	virtual wBlockParameterContainer createParameter(const ed::string& name) { return wBlockParameterContainer(name, new wBlockParameterProxy<T>(new wBlockParameter<T>(), true)); }
};


#define PARAMETER_TYPE(type, id) \
	template <> struct wParameterTypeTraits<type> { enum { param_type = id }; enum { is_array = 0 }; enum { array_size = -1 }; }; \
	__PARAM_EXTERN __ParameterCreator<type, id> __creator##id;

PARAMETER_TYPE(int, PARAMETER_INT);
PARAMETER_TYPE(ed::string, PARAMETER_STRING);
PARAMETER_TYPE(double, PARAMETER_DOUBLE);
PARAMETER_TYPE(Math::Vec3d, PARAMETER_VECTOR);
PARAMETER_TYPE(bool, PARAMETER_BOOLEAN);

#endif
