#ifndef __SIMULATION_WIRES_OBJECT_PTR_H__
#define __SIMULATION_WIRES_OBJECT_PTR_H__

#define INPUT_PORT_OBJ_PTR(obj_type, name) wInputPortObjPtr<obj_type> name; 
#define INPUT_PORT_OBJ_PTR_C(obj_type, name) wInputPortObjPtrCallback<obj_type, self_type> name;
#define OUTPUT_PORT_OBJ_PTR(obj_type, name) wOutputPortObjPtr<obj_type> name;

class wWireObjectPtrBase
{
/*public:
	virtual ~wWireObjectPtrBase() {}*/
};

/*template <class T> 
class wWireObjectPtr : public wWireObjectPtrBase
{
public:
	wWireObjectPtr(T* obj) { obj_ = obj; }
	virtual ~wWireObjectPtr() {}

	T* get() const { return obj_; }

protected:
	mutable T* obj_;
};*/

template <> class wInputPort<wWireObjectPtrBase> : public wInputPortBase
{
public:
	typedef wWireObjectPtrBase type;

protected:
	wInputPort() {}
public:
	virtual eLeadType getType() const { return LEAD_OBJECT_PTR; }
};

template <> class wOutputPort<wWireObjectPtrBase> : public wOutputPortBase
{
public:
	typedef wWireObjectPtrBase type;

protected:
	wOutputPort() {}

public:
	virtual eLeadType getType() const { return LEAD_OBJECT_PTR; }
};

class wInputPortObjPtrUnknown : public wInputPort<wWireObjectPtrBase>
{
public:
	wProxyPortBase* getProxy() { assert(0); return 0; }
};

template <class T> class wInputPortObjPtrBase : public wInputPort<wWireObjectPtrBase>
{
protected:
	wInputPortObjPtrBase() {}; 

public:
	virtual void update(T* ptr) = 0;
};

template <class T> class wInputPortObjPtrFake : public wInputPortObjPtrBase<T>
{
public:
	wInputPortObjPtrFake() {};

	wProxyPortBase* getProxy() { assert(0); return 0; }
	
	virtual void update(T* ptr) {};
};

template <class T> class wInputPortObjPtr : public wInputPortObjPtrBase<T>
{
public:
	wInputPortObjPtr() { ptr_ = 0; }

	virtual void update(T* ptr) { ptr_ = ptr; }
	operator T*() { return ptr_; }
	T* operator ->() { return ptr_; }
	const T* operator->() const { return ptr_; }

	virtual wProxyPortBase* getProxy() { return new wProxyPortObjPtr<T>(); }
protected:
	T* ptr_;
};

template <class T, class O> class wInputPortObjPtrCallback : public wInputPortObjPtrBase<T>
{
public:
	typedef void (O::*function_ptr)(T*);

public:
	wInputPortObjPtrCallback(O* owner, function_ptr func) : owner_(owner), func_(func) {};

	virtual void update(T* ptr) { (owner_->*func_)(ptr); }
	virtual wProxyPortBase* getProxy() { return new wProxyPortObjPtr<T>(); }

protected:
	O* owner_;
	function_ptr func_;
};

template <class T> class wOutputPortObjPtr;

template <class T> class wProxyPortObjPtr : public wProxyPortBase, public wInputPortObjPtr<T>
{
public:
	virtual void update(T* ptr) { output_.update(ptr); }

	virtual wInputPortBase* getInput() { return this; }
	virtual wOutputPortBase* getOutput() { return &output_; }
protected:
	wOutputPortObjPtr<T> output_;
};

template <class T> class wOutputPortObjPtr : public wOutputPort<wWireObjectPtrBase>
{
public:
	void update(T* ptr)
	{
		for (int i=0; i < (int)ports_.size(); i++)
			ports_[i]->update(ptr);
	}

	virtual void connect(wInputPortBase* port)
	{
		if (!dynamic_cast<wInputPortObjPtrBase<T>*>(port))
		{
			if (dynamic_cast<wInputPortObjPtrUnknown*>(port)) return;
			throw wSimulationException("Attempt to connect obj ptr ports of different types");
		}

		ports_.push_back(static_cast<wInputPortObjPtr<T>*>(port));
	}

	virtual wProxyPortBase* getProxy() const { return new wProxyPortObjPtr<T>(); }


protected:
	ed::vector<wInputPortObjPtr<T>* > ports_;
};



#endif 