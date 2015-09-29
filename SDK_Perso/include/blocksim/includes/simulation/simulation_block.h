#ifndef __SIMULATION_BLOCK_H__
#define __SIMULATION_BLOCK_H__

#include <ed/vector.h>
#include <ed/map.h>
#include <algorithm>
#include <iterator>

#include "../blocksim.h"

#include "simulation_wires.h"
#include "block_parameter.h"

class wSimulationSystemScheme;
class wSimulationSystem;
class wBlockSerializer;
class wSimulationBlock;

enum weBlockType
{
	BLOCK_STATELESS,
	BLOCK_DIFFERENTIAL,
	BLOCK_DISCRETE,
	BLOCK_SUBSYSTEM				// special type of block
};

typedef unsigned char lead_data_t;
typedef lead_data_t* lead_data_ptr_t;


class BLOCKSIM_API wSimulationBlockDescriptor
{
private:
	wSimulationBlockDescriptor();

public:
	wSimulationBlockDescriptor(const wSimulationBlockDescriptor& desc);
	virtual ~wSimulationBlockDescriptor();

	const wSimulationBlockDescriptor& operator = (const wSimulationBlockDescriptor& desc);

	virtual const ed::string& getClassName() const = 0;
	virtual const ed::string& getBlockTypeName() const = 0 ;	// User-friendly
	virtual bool isOfType(weBlockType type) const { return false; }

	virtual void serialize(wBlockSerializer& ser);

	virtual wSimulationBlockDescriptor* clone() const = 0;

	// по умолчанию эти функции возвращают get*_()
	virtual const ed::vector<wLeadDescription>& getInputWires() const { return getInputWires_(); }
	virtual const ed::vector<wLeadDescription>& getOutputWires() const { return getOutputWires_(); }
	virtual const ed::vector<wLeadDescription>& getInputPorts() const { return getInputPorts_(); }
	virtual const ed::vector<wLeadDescription>& getOutputPorts() const { return getOutputPorts_(); }
	const ed::vector<wLeadDescription>& getLeads(bool input, bool port) const;

	bool hasInputWire(const ed::string& name) const;
	bool hasOutputWire(const ed::string& name) const;
	bool hasInputPort(const ed::string& name) const;
	bool hasOutputPort(const ed::string& name) const;
	bool hasLead(bool input, bool port, const ed::string& name) const;
	int  getLeadIndex(bool input, bool port, const ed::string& name, bool throw_ex = true) const;

	eLeadType getInputType(const ed::string& name) const;
	eLeadType getOutputType(const ed::string& name) const;
	eLeadType getInputPortType(const ed::string& name) const;
	eLeadType getOutputPortType(const ed::string& name) const;
	eLeadType getLeadType(bool input, bool port, const ed::string& name) const;

	virtual ed::vector<wBlockParameterContainer> getParametersList();

	const ed::string& getName() const { return name_; }
	void setName(const ed::string& name) { name_ = name; }

	void setScheme(wSimulationSystemScheme* parent) { parentScheme_ = parent; }

	// Wire offsets
	inline int getInputWireOffset(int wire) const { return inputOffsets_[wire]; }
	inline int getOutputWireOffset(int wire) const { return outputOffsets_[wire]; }

	// Только схема может вызвать эти функции
	void setInputWireOffsets(int* offsets); 
	void setOutputWireOffsets(int* offsets);

	// debug
	virtual void debug_dump();

protected:
	virtual const ed::vector<wLeadDescription>& getInputWires_() const;
	virtual const ed::vector<wLeadDescription>& getOutputWires_() const;
	virtual const ed::vector<wLeadDescription>& getInputPorts_() const;
	virtual const ed::vector<wLeadDescription>& getOutputPorts_() const;

protected:
	ed::string name_;
	wSimulationSystemScheme* parentScheme_;
	int* inputOffsets_;
	int* outputOffsets_;

	friend class wSimulationBlockStatelessDescriptor;
	friend class wSimulationBlockDiffDescriptor;
	friend class wSimulationBlockDiscDescriptor;
};

class wSimulationBlockStateless;
class wSimulationBlockDiff;
class wSimulationBlockDisc;

class BLOCKSIM_API wSimulationBlockStatelessDescriptor : public wSimulationBlockDescriptor
{
public:
	enum { TYPE = BLOCK_STATELESS };

	wSimulationBlockStatelessDescriptor() {}

	virtual wSimulationBlockStateless* create(Lua::Loader* runtime_cfg) = 0;

	virtual bool isOfType(weBlockType type) const { return type == (weBlockType)TYPE; }
};

class BLOCKSIM_API wSimulationBlockDiffDescriptor : public wSimulationBlockDescriptor
{
public:
	enum { TYPE = BLOCK_DIFFERENTIAL };

	wSimulationBlockDiffDescriptor() {}

	virtual const ed::vector<wLeadDescription>& getInputWires() const;

	virtual wSimulationBlockDiff* create(Lua::Loader* runtime_cfg) = 0;

	virtual bool isOfType(weBlockType type) const { return type == (weBlockType)TYPE; }
};

class BLOCKSIM_API wSimulationBlockDiscDescriptor : public wSimulationBlockDescriptor
{
public:
	enum { TYPE = BLOCK_DISCRETE };

	wSimulationBlockDiscDescriptor() {}

	virtual wSimulationBlockDisc* create(Lua::Loader* runtime_cfg, wSimulationSystem* data) = 0;

	virtual bool isOfType(weBlockType type) const { return type == (weBlockType)TYPE; }
};


#include "simulation_block_def.h"


#define TEMPL_TYPENAME_I0
#define TEMPL_TYPENAME_I1 typename
#define TEMPL_TYPENAME(is_templ) TEMPL_TYPENAME_I##is_templ

#define DECLARE_BLOCK_DESCRIPTOR_T_I(block_type_, is_templ)				 \
	public:																 \
	typedef int ___type_guard[block_type_::TYPE == TYPE ? 1 : -1];		 \
	typedef block_type_ block_type;										 \
	typedef TEMPL_TYPENAME(is_templ) block_type::base_type::desc_type base_type;		 \
	typedef TEMPL_TYPENAME(is_templ) block_type::desc_type self_type;					 \
	public:																 \
	wSimulationBlockDescriptor* clone() const;							 \
	virtual const ed::string& getBlockTypeName() const;				 \
	virtual const ed::string& getClassName() const;					 \
	virtual ed::vector<wBlockParameterContainer> getParametersList();   \
	static  self_type* __create_instance();								 \
	protected:															 \
	virtual const ed::vector<wLeadDescription>& getInputWires_() const; \
	virtual const ed::vector<wLeadDescription>& getOutputWires_() const; \
	virtual const ed::vector<wLeadDescription>& getInputPorts_() const;  \
	virtual const ed::vector<wLeadDescription>& getOutputPorts_() const; \
	public:																 \
	static const ed::string& getBlockTypeNameS();						 \
	static const ed::string& getBlockClassNameS();						 

#define DECLARE_BLOCK_DESCRIPTOR(block_type) DECLARE_BLOCK_DESCRIPTOR_T_I(block_type, 0)
#define DECLARE_BLOCK_DESCRIPTOR_T(block_type) DECLARE_BLOCK_DESCRIPTOR_T_I(block_type, 1)


#define IMPLEMENT_BLOCK_DESCRIPTOR_TEMP(temp_def, type) \
	temp_def const ed::string& type::getBlockTypeName() const { return getBlockTypeNameS(); } \
	temp_def const ed::string& type::getClassName() const { return getBlockClassNameS(); }	\
	temp_def const type* type::block_type::getDesc_() { return static_cast<const type*>(desc_); }

#define __REG_VAR_NAME1(x, y) x##y
#define __REG_VAR_NAME(x, y) __REG_VAR_NAME1(x, y)

#define INSTANTIATE_TEMPLATE_BLOCK(type, user_name) \
	template <> const ed::string& type::getBlockTypeNameS() { static ed::string res = user_name; return res; } \
	template <> const ed::string& type::getBlockClassNameS() { static ed::string res = #type; return res; } \
	template <> wSimulationBlockDescriptor* type::clone() const { return new type(*this); } \
	template <> type* type::__create_instance() { return new type(); } \
    const static bool __REG_VAR_NAME(__registered,__COUNTER__) = (wSimulationBlockFactory::instance()->register_descriptor(#type, CoreMeta::Functor<type*, TYPE_LIST0() >(type::__create_instance), user_name),true); \
	template class type;

#define IMPLEMENT_ABSTRACT_BLOCK_DESCRIPTOR__(type, user_name) \
	IMPLEMENT_BLOCK_DESCRIPTOR_TEMP(, type) \
	const ed::string& type::getBlockTypeNameS() { static ed::string res = user_name; return res; } \
	const ed::string& type::getBlockClassNameS() { static ed::string res = #type; return res; };

#define IMPLEMENT_ABSTRACT_BLOCK_DESCRIPTOR(type) IMPLEMENT_ABSTRACT_BLOCK_DESCRIPTOR__(type, "")

#define IMPLEMENT_BLOCK_DESCRIPTOR(type, user_type_name) \
	IMPLEMENT_ABSTRACT_BLOCK_DESCRIPTOR__(type, user_type_name)	\
	type* type::__create_instance() { return new type(); } \
	wSimulationBlockDescriptor* type::clone() const { return new type(*this); } \
	const static bool __REG_VAR_NAME(__registered,type) = (wSimulationBlockFactory::instance()->register_descriptor(#type, &type::__create_instance, user_type_name),true);

template <class B, class T, class A> void __is_subclass_of(A T::*)
{
	A* a = 0;
	B* b = a;
}

template <class T, class A> eLeadType __deduce_type_from_ptr(A T::*)
{
	return (eLeadType)wSimulationLeadTraits<typename A::type>::lead_type;
}

// Wires listing
#define LIST_WIRES(block_name, struct_name, func_name, t1, is_templ) \
	t1 \
	const ed::vector<wLeadDescription>& block_name::desc_type::func_name() const \
	{	\
		static ed::vector<wLeadDescription> res(base_type::func_name());  \
		static bool init = false;  \
		typedef block_name block_type; \
		typedef block_type::struct_name struct_type; \
		if (!init) \
		{	\
			init = true;


// Годится ли тут assert???
#define WIRE_NAME(user_name, name) \
	assert(struct_type::name::id == res.size());	\
	res.push_back(wLeadDescription(user_name, (eLeadType)wSimulationLeadTraits<struct_type::name::type>::lead_type));

#define END_LIST_WIRES() } return res; }
		

// Ports listing
#define LIST_PORTS(block_name, struct_name, func_name, type, t1, is_templ) \
	t1 \
	struct block_name::struct_name { \
		struct_name(wLeadDescription& desc_, type block_name::*ptr_) : desc(desc_), ptr(ptr_) {} \
		wLeadDescription desc; \
		type block_name::*ptr; \
	}; \
	\
	t1 \
	const ed::vector<TEMPL_TYPENAME(is_templ) block_name::struct_name>& block_name::func_name()	\
	{	\
		static ed::vector<TEMPL_TYPENAME(is_templ) block_name::struct_name> res;	\
		static bool init = false;	\
		typedef block_name block_type; \
		typedef struct_name struct_type; \
		typedef type block_name::*ptr_type; \
		typedef type lead_type;	\
		if (!init)	\
		{ init = true; \
			
#define PORT_NAME(name, var)	\
	__is_subclass_of<lead_type>(&block_type::var);	\
	{\
		wLeadDescription wld(name, __deduce_type_from_ptr(&block_type::var));\
		res.push_back(struct_type(wld, (ptr_type)(&block_type::var)));\
	}

#define END_LIST_PORTS() } return res; }

// Wire listing
#define LIST_INPUT_WIRES_TT(block, t1, ist) LIST_WIRES(block, __input_wires, getInputWires_, t1, ist)
#define LIST_OUTPUT_WIRES_TT(block, t1, ist) LIST_WIRES(block, __output_wires, getOutputWires_, t1, ist)
#define LIST_INPUT_PORTS_TT(block, t1, ist) LIST_PORTS(block, __inputPortsDesc, __getInputPorts, wInputPortBase, t1, ist)
#define LIST_OUTPUT_PORTS_TT(block, t1, ist) LIST_PORTS(block, __outputPortsDesc, __getOutputPorts, wOutputPortBase, t1, ist)

#define LIST_INPUT_WIRES_T(desc, t1) LIST_INPUT_WIRES_TT(desc, t1, 1)
#define LIST_OUTPUT_WIRES_T(desc, t1) LIST_OUTPUT_WIRES_TT(desc, t1, 1)
#define LIST_INPUT_PORTS_T(desc, t1) LIST_INPUT_PORTS_TT(desc, t1, 1)
#define LIST_OUTPUT_PORTS_T(desc, t1) LIST_OUTPUT_PORTS_TT(desc, t1, 1)

#define LIST_INPUT_WIRES(block) LIST_INPUT_WIRES_TT(block, ,0)
#define LIST_OUTPUT_WIRES(block) LIST_OUTPUT_WIRES_TT(block, ,0)
#define LIST_INPUT_PORTS(block) LIST_INPUT_PORTS_TT(block, ,0)
#define LIST_OUTPUT_PORTS(block) LIST_OUTPUT_PORTS_TT(block, ,0)

// Ports implementation
// Function generation in descriptor and block 
template <class T> wLeadDescription __extract_description(const T& v)
{
	return v.desc;
}

template <class R, class T> std::pair<ed::string, R> __extract_pointer(const T& v)
{
	return make_pair(v.desc.name(), v.ptr);
}

#define IMPLEMENT_DESC_PORTS(desc, method_name, func_name, struct_name, t) \
	t \
	const ed::vector<wLeadDescription>& desc::method_name() const \
	{	\
	static ed::vector<wLeadDescription> res(base_type::method_name().begin(), base_type::method_name().end());	\
		static bool init = false; \
		if (!init) \
		{ init = true; \
			std::transform(desc::block_type::func_name().begin(), desc::block_type::func_name().end(), inserter(res, res.end()), __extract_description<desc::block_type::struct_name>); \
		} \
		return res; \
	}

//#define IMPLEMENT_DESC_INPUTS_T(desc, t) IMPLEMENT_DESC_WIRES(desc, getInputWires, __getInputWires, __inputWiresDesc, t)
//#define IMPLEMENT_DESC_OUTPUTS_T(desc, t) IMPLEMENT_DESC_WIRES(desc, getOutputWires,  __getOutputWires, __outputWiresDesc, t)
#define IMPLEMENT_DESC_IN_PORTS_T(desc, t) IMPLEMENT_DESC_PORTS(desc,  getInputPorts_, __getInputPorts, __inputPortsDesc, t)
#define IMPLEMENT_DESC_OUT_PORTS_T(desc, t) IMPLEMENT_DESC_PORTS(desc,  getOutputPorts_, __getOutputPorts, __outputPortsDesc, t)

#define IMPLEMENT_DESC_LEADS_T(desc, t) \
	IMPLEMENT_DESC_IN_PORTS_T(desc, t) \
	IMPLEMENT_DESC_OUT_PORTS_T(desc, t)

#define IMPLEMENT_DESC_LEADS(desc) IMPLEMENT_DESC_LEADS_T(desc, )

#define IMPLEMENT_BLOCK_PORTS(block, func_name, table_name, struct_name, type) \
	type* block::func_name(const ed::string& name) \
	{ \
		static ed::map<ed::string, type block::*> res; \
		static bool init = false; \
		if (!init) \
		{ init = true; \
			std::transform(table_name().begin(), table_name().end(), inserter(res, res.end()), __extract_pointer<type block::*, struct_name>); \
		} \
			\
		ed::map<ed::string, type block::*>::iterator i = res.find(name);\
		return i == res.end() ? base_type::func_name(name) : &(this->*(i->second)); \
	}

//#define IMPLEMENT_BLOCK_INPUTS_T(block, t) t IMPLEMENT_BLOCK_WIRES(block, getInputWire, __getInputWires, __inputWiresDesc,  wInputWireBase)
//#define IMPLEMENT_BLOCK_OUTPUTS_T(block, t) t IMPLEMENT_BLOCK_WIRES(block, getOutputWire, __getOutputWires, __outputWiresDesc,  wOutputWireBase)
#define IMPLEMENT_BLOCK_IN_PORTS_T(block, t) t IMPLEMENT_BLOCK_PORTS(block, getInputPort, __getInputPorts,  __inputPortsDesc, wInputPortBase)
#define IMPLEMENT_BLOCK_OUT_PORTS_T(block, t) t IMPLEMENT_BLOCK_PORTS(block, getOutputPort, __getOutputPorts,  __outputPortsDesc, wOutputPortBase)

#define IMPLEMENT_BLOCK_LEADS_T(block, t) \
	IMPLEMENT_BLOCK_IN_PORTS_T(block, t) \
	IMPLEMENT_BLOCK_OUT_PORTS_T(block, t)

#define IMPLEMENT_PORTS_T(desc, block, t) \
	IMPLEMENT_BLOCK_LEADS_T(block, t) \
	IMPLEMENT_DESC_LEADS_T(desc, t)

//#define IMPLEMENT_INPUT_WIRES(block) IMPLEMENT_BLOCK_INPUTS_T(block,) IMPLEMENT_DESC_INPUTS_T(block::desc_type, )
//#define IMPLEMENT_OUTPUT_WIRES(block) IMPLEMENT_BLOCK_OUTPUTS_T(block,) IMPLEMENT_DESC_OUTPUTS_T(block::desc_type, )
#define IMPLEMENT_INPUT_PORTS(block) IMPLEMENT_BLOCK_IN_PORTS_T(block,) IMPLEMENT_DESC_IN_PORTS_T(block::desc_type, )
#define IMPLEMENT_OUTPUT_PORTS(block) IMPLEMENT_BLOCK_OUT_PORTS_T(block,) IMPLEMENT_DESC_OUT_PORTS_T(block::desc_type, )

#define IMPLEMENT_PORTS(block) IMPLEMENT_PORTS_T(block::desc_type, block, )

////////////////////////////////////////////////////////////////////////
// PARAMETERS
////////////////////////////////////////////////////////////////////////
#define LIST_PARAMETERS(desc) \
	ed::vector<wBlockParameterContainer> desc::getParametersList() \
	{	\
		ed::vector<wBlockParameterContainer> res = base_type::getParametersList();
	
#define PARAMETER(var_name, user_name) res.push_back(wBlockParameterContainer(var_name, user_name));

#define END_LIST_PARAMETERS() return res; }

// For static linking
#define CREATE_LINKER_ANCHOR(name) extern "C" { int __anchor_##name; };

#endif
