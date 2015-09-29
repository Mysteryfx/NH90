#ifndef __SIMULATION_BLOCK_DEF_H__
#define __SIMULATION_BLOCK_DEF_H__

class wBlockSerializer;
class wSimulationBlockDescriptor;

class wInputPortBase;
class wOutputPortBase;

#include "../blocksim.h"

#include "simulation_wire_types.h"

class wSimulationSystem;

class BLOCKSIM_API wSimulationBlock
{
public:
	typedef wSimulationBlockDescriptor desc_type;

private:
	wSimulationBlock(const wSimulationBlockDescriptor* desc) { desc_ = desc; }
public:
	virtual ~wSimulationBlock() {};

	bool isOfType(weBlockType type) const { return desc_->isOfType(type); };

	// Индекс - по соответствующему массиву в дескрипторе
	virtual wInputPortBase* getInputPort(const ed::string& name);
	virtual wOutputPortBase* getOutputPort(const ed::string& name);

	friend class wSimulationBlockDiff;			// Differential block
	friend class wSimulationBlockStateless;		// Stateless block
	friend class wSimulationBlockDisc;			// discrete block

protected:
	struct __input_wires { enum { __last_id = 0 }; };
	struct __output_wires { enum { __last_id = 0 }; };

protected:
	const wSimulationBlockDescriptor* desc_;
};

// PRELIMINARY:
// This block represents the following system:
//   S - state vector (doubles)
//   Y - output vector (doubles)
//   t - time
//   dS/dt = f(S, t)
//   Y     = slice(S)
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!! Все выходы обрабатываются как массив даблов, независимо от их реального типа !!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 

class wSimulationBlockDiffDescriptor;
class BLOCKSIM_API wSimulationBlockDiff : public wSimulationBlock
{
public:
	enum { TYPE = BLOCK_DIFFERENTIAL };
	typedef wSimulationBlockDiffDescriptor desc_type;

public:
	wSimulationBlockDiff(const wSimulationBlockDiffDescriptor* desc) : wSimulationBlock(desc) {};

	// вычислить f(S, t) и положить в dy
	virtual void calculate_dy(double t, lead_data_ptr_t state, lead_data_ptr_t dy) = 0;

	void resetState(lead_data_ptr_t data_ptr);
};

// PRELIMINARY:
// This block represents the following system:
//   X - input vector
//   Y - output vector
//   t - time
//   Y = f(X, t)
// i.e stateless block
class BLOCKSIM_API wSimulationBlockStateless : public wSimulationBlock
{
public:
	typedef wSimulationBlockStatelessDescriptor desc_type;
	enum { TYPE = BLOCK_STATELESS };

	wSimulationBlockStateless(const wSimulationBlockStatelessDescriptor* desc) : wSimulationBlock(desc) {}

	virtual void update(double time, lead_data_ptr_t data_ptr) = 0;	
};


class BLOCKSIM_API wSimulationBlockDisc : public wSimulationBlock
{
public:
	typedef wSimulationBlockDiscDescriptor desc_type;
	enum { TYPE = BLOCK_DISCRETE };

	wSimulationBlockDisc(const wSimulationBlockDiscDescriptor* desc, wSimulationSystem* system) : wSimulationBlock(desc), system_(system) {};

	// simulate discrete block for a specified time
	// return time to next sample
	virtual double simulate(double time) = 0;
	virtual void reset();

protected:
	wSimulationSystem* system_;
};


template <class T>
struct AddConst
{
	typedef const T type;
};


#define DECLARE_BLOCK(type, base, desc) \
	public:\
		typedef base base_type; \
		typedef type self_type; \
		typedef desc desc_type; \
		friend class desc;	    \
		virtual wInputPortBase* getInputPort(const ed::string& name); \
		virtual wOutputPortBase* getOutputPort(const ed::string& name); \
		struct __inputWiresDesc; \
		struct __outputWiresDesc; \
		struct __inputPortsDesc; \
		struct __outputPortsDesc; \
		static const ed::vector<__inputWiresDesc>& __getInputWires(); \
		static const ed::vector<__outputWiresDesc>& __getOutputWires(); \
		static const ed::vector<__inputPortsDesc>& __getInputPorts(); \
		static const ed::vector<__outputPortsDesc>& __getOutputPorts(); \
	protected:\
		/* FIXME: это предотвращает случай, когда в классе список не объявлен, а в реализации используется? */ \
		struct __input_wires; \
		struct __output_wires; \
		const desc_type* getDesc_();


// Wire references
// Выводы хранятся в области памяти, выделяемой системой по смещению, хранящемуся в дескрипторе
// Считается, что указатель на входы называется inputs_address, на выходы - outputs_address
#define INP_WDPTR(name, data) (*((__input_wires::name::type*)(data + desc_->getInputWireOffset(__input_wires::name::id))))
#define OUT_WDPTR(name, data) (*((__output_wires::name::type*)(data + desc_->getOutputWireOffset(__output_wires::name::id))))
#define INP_W(name) INP_WDPTR(name, data_ptr)
#define OUT_W(name) OUT_WDPTR(name, data_ptr)
#define INP_WD(name) INP_WDPTR(name, system_->getDataPtr())
#define OUT_WD(name) OUT_WDPTR(name, system_->getDataPtr())


// Wire declarations
// Это все наворочено, чтобы можно было обращаться к проводу как по имени, так и по номеру
// Вероятно, с помощью технологий, аналогичных бусту можно было бы сделать более прямо, но пока на это нет времени
// В любом случае, исправить это будет не очень сложно
// В данном случае список выводов задается следующим образом (n - количество выводов):
// INPUT_WIRESn(
//		WIRE(name_a, type_a),
//		WIRE(name_b, type_b),
//		...
//		WIRE(name_x, type_x)
// )
// Аналогично OUTPUT_WIRESn
// В идеале хотелось бы избавиться от явного задания количества проводов в заголовке списка

#define CONCAT(x, y) x##y

#define DISCARD_1(x)
#define DO_EXTRACT_1(x) x DISCARD_1

#define DO_EXTRACT_2_(x) x
#define DO_EXTRACT_2(x) DO_EXTRACT_2_

#define EXTRACT_1 DO_EXTRACT_1
#define EXTRACT_2 DO_EXTRACT_2

#define WRITE_WIRE(wire, wire_type, next_wire) struct wire { typedef wire_type type; enum { id = next_wire::id + 1 }; };
#define WRITE_WIRE0(wire, wire_type) struct wire { typedef wire_type type; enum { id = __first_id }; };
#define WIRE_IMPL(w_next, w) WRITE_WIRE(EXTRACT_1 w, EXTRACT_2 w, EXTRACT_1 w_next)
#define WIRE_IMPL_TERM(w) WRITE_WIRE0(EXTRACT_1 w, EXTRACT_2 w)

#define WIRE(type, name) (name)(type)

#define WIRE_LIST0() 
#define WIRE_LIST1(w1) WIRE_IMPL_TERM(w1)
#define WIRE_LIST2(w1,w2) WIRE_LIST1(w1) WIRE_IMPL(w1, w2)
#define WIRE_LIST3(w1,w2,w3) WIRE_LIST2(w1,w2) WIRE_IMPL(w2, w3)
#define WIRE_LIST4(w1,w2,w3,w4) WIRE_LIST3(w1,w2,w3) WIRE_IMPL(w3, w4)
#define WIRE_LIST5(w1,w2,w3,w4,w5) WIRE_LIST4(w1,w2,w3,w4) WIRE_IMPL(w4, w5)
#define WIRE_LIST6(w1,w2,w3,w4,w5,w6) WIRE_LIST5(w1,w2,w3,w4,w5) WIRE_IMPL(w5, w6)
#define WIRE_LIST7(w1,w2,w3,w4,w5,w6,w7) WIRE_LIST6(w1,w2,w3,w4,w5,w6) WIRE_IMPL(w6, w7)
#define WIRE_LIST8(w1,w2,w3,w4,w5,w6,w7,w8) WIRE_LIST7(w1,w2,w3,w4,w5,w6,w7) WIRE_IMPL(w7, w8)
#define WIRE_LIST9(w1,w2,w3,w4,w5,w6,w7,w8,w9) WIRE_LIST8(w1,w2,w3,w4,w5,w6,w7,w8) WIRE_IMPL(w8, w9)
#define WIRE_LIST10(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10) WIRE_LIST9(w1,w2,w3,w4,w5,w6,w7,w8,w9) WIRE_IMPL(w9, w10)
#define WIRE_LIST11(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11) WIRE_LIST10(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10) WIRE_IMPL(w10, w11)
#define WIRE_LIST12(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12) WIRE_LIST11(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11) WIRE_IMPL(w11, w12)
#define WIRE_LIST13(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13) WIRE_LIST12(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12) WIRE_IMPL(w12, w13)
#define WIRE_LIST14(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14) WIRE_LIST13(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13) WIRE_IMPL(w13, w14)
#define WIRE_LIST15(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14,w15) WIRE_LIST14(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14) WIRE_IMPL(w14, w15)
#define WIRE_LIST16(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14,w15,w16) WIRE_LIST15(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14,w15) WIRE_IMPL(w15, w16)
#define WIRE_LIST17(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14,w15,w16,w17) WIRE_LIST16(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14,w15,w16) WIRE_IMPL(w16, w17)
#define WIRE_LIST18(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14,w15,w16,w17,w18) WIRE_LIST17(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14,w15,w16,w17) WIRE_IMPL(w17, w18)
#define WIRE_LIST19(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14,w15,w16,w17,w18,w19) WIRE_LIST18(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14,w15,w16,w17,w18) WIRE_IMPL(w18, w19)
#define WIRE_LIST20(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14,w15,w16,w17,w18,w19,w20) WIRE_LIST19(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14,w15,w16,w17,w18,w19) WIRE_IMPL(w19, w20)

#define INPUT_WIRES(cnt, lst) \
struct __input_wires : public base_type::__input_wires { \
		enum { __first_id = base_type::__input_wires::__last_id }; \
		WIRE_LIST##cnt lst \
		enum { __last_id = __first_id + cnt }; \
		enum { __count = cnt }; \
	};

#define OUTPUT_WIRES(cnt, lst) \
	struct __output_wires : public base_type::__output_wires { \
		enum { __first_id = base_type::__output_wires::__last_id }; \
		WIRE_LIST##cnt lst \
		enum { __last_id = __first_id + cnt }; \
		enum { __count = cnt }; \
	};

#endif