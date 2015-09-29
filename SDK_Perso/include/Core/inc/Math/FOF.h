#ifndef _ED_Math_FOF_h_
#define _ED_Math_FOF_h_

namespace Math {

template <class T>
class FOF
{//first-order filter
public:
	template<typename _T> struct RefTrait{typedef const _T& const_ref_t;};

#define NOT_REF_TRAIT(a) template<> struct RefTrait<a>{typedef a const_ref_t;}
	NOT_REF_TRAIT(float);
	NOT_REF_TRAIT(double);
	NOT_REF_TRAIT(unsigned char);
	NOT_REF_TRAIT(char);
	NOT_REF_TRAIT(unsigned short);
	NOT_REF_TRAIT(short);
	NOT_REF_TRAIT(unsigned int);
	NOT_REF_TRAIT(int);
#undef 	NOT_REF_TRAIT

	FOF() : out(T()) {}
	inline const T & step(typename RefTrait<T>::const_ref_t in, double dt, double tau)
    {
		out += (in - out) * dt / tau;
		return out;
    }
public:
    T   out;
};
}

#endif /* _ED_Math_FOF_h_ */
