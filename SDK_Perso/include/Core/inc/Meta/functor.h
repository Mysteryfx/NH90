#ifndef __FUNCTOR_H__
#define __FUNCTOR_H__

#include <assert.h>
#include "types_list.h"
#include "type_traits.h"

// TODO: надо функторы в small_value_object запихнуть

namespace CoreMeta
{

namespace functor_private
{
	#define IMPLEMENT_FUNCTOR_CLONE(cls) virtual cls* clone() const { return new cls(*this); } 

	class FunctorImplBase
	{
	public:
		typedef NullType Param1;
		typedef NullType Param2;
		typedef NullType Param3;
		typedef NullType Param4;

		virtual ~FunctorImplBase() {};
	};

	template <class ResultType, class Args>
	class FunctorImpl
	{
	};

	template <class ResultType>
	class FunctorImpl<ResultType, TYPE_LIST0()> : public FunctorImplBase
	{
		typedef FunctorImpl<ResultType, TYPE_LIST0()> self_type;

	public:
		virtual ResultType operator ()() const = 0;
		virtual self_type* clone() const = 0;
	};
    
    // IMPLEMENTATIONS - PARAMS COUNT
	template <class ResultType, class P1>
	class FunctorImpl<ResultType, TYPE_LIST1(P1)> : public FunctorImplBase
	{
		typedef FunctorImpl<ResultType, TYPE_LIST1(P1)> self_type;

	public:
		typedef typename TypeTraits<P1>::parameter_type Param1;

		virtual ResultType operator ()(Param1 p) const = 0;
		virtual self_type* clone() const = 0;
	};

    template <class ResultType, class P1, class P2>
	class FunctorImpl<ResultType, TYPE_LIST2(P1, P2)> : public FunctorImplBase
	{
		typedef FunctorImpl<ResultType, TYPE_LIST2(P1, P2)> self_type;

	public:
		typedef typename TypeTraits<P1>::parameter_type Param1;
        typedef typename TypeTraits<P2>::parameter_type Param2;

		virtual ResultType operator ()(Param1 p1, Param2 p2) const = 0;
		virtual self_type* clone() const = 0;
	};
    
    template <class ResultType, class P1, class P2, class P3>
	class FunctorImpl<ResultType, TYPE_LIST3(P1, P2, P3)> : public FunctorImplBase
	{
		typedef FunctorImpl<ResultType, TYPE_LIST3(P1, P2, P3)> self_type;

	public:
		typedef typename TypeTraits<P1>::parameter_type Param1;
        typedef typename TypeTraits<P2>::parameter_type Param2;
        typedef typename TypeTraits<P3>::parameter_type Param3;

		virtual ResultType operator ()(Param1 p1, Param2 p2, Param3 p3) const = 0;
		virtual self_type* clone() const = 0;
	};
    
    template <class ResultType, class P1, class P2, class P3, class P4>
	class FunctorImpl<ResultType, TYPE_LIST4(P1, P2, P3, P4)> : public FunctorImplBase
	{
		typedef FunctorImpl<ResultType, TYPE_LIST4(P1, P2, P3, P4)> self_type;

	public:
		typedef typename TypeTraits<P1>::parameter_type Param1;
        typedef typename TypeTraits<P2>::parameter_type Param2;
        typedef typename TypeTraits<P3>::parameter_type Param3;
        typedef typename TypeTraits<P4>::parameter_type Param4;
        

		virtual ResultType operator ()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const = 0;
		virtual self_type* clone() const = 0;
	};
    // END IMPLEMENTATIONS

	template <class T, class ResultType, class  Args>
	class ForwardFunctor : public FunctorImpl<ResultType, Args>
	{
	protected:
		typedef ForwardFunctor<T, ResultType, Args> self_type;
		typedef FunctorImpl<ResultType, Args> base_type;
		typedef typename base_type::Param1 Param1;
		typedef typename base_type::Param2 Param2;
		typedef typename base_type::Param3 Param3;
		typedef typename base_type::Param4 Param4;

	public:
		ForwardFunctor(const ForwardFunctor& f) : obj_(f.obj_) {}
		ForwardFunctor(const T& obj) : obj_(obj) {}

		ResultType operator ()()  const
		{
			return obj_();
		}

		ResultType operator ()(Param1 p1) const
		{
			return obj_(p1);
		}
    
        ResultType operator ()(Param1 p1, Param2 p2) const
		{
			return obj_(p1, p2);
		}
        
        ResultType operator ()(Param1 p1, Param2 p2, Param3 p3) const
		{
			return obj_(p1, p2, p3);
		}
        
        ResultType operator ()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const
		{
			return obj_(p1, p2, p3, p4);
		}
    
		IMPLEMENT_FUNCTOR_CLONE(self_type);

	protected:
		T obj_;
	};

	template <class T, class FT, class ResultType,  class  Args>
	class MemFunFunctor : public FunctorImpl<ResultType, Args>
	{
	protected:
		typedef MemFunFunctor<T, FT, ResultType, Args> self_type;
		typedef FunctorImpl<ResultType, Args> base_type;
		typedef typename base_type::Param1 Param1;
		typedef typename base_type::Param2 Param2;
		typedef typename base_type::Param3 Param3;
		typedef typename base_type::Param4 Param4;

	public:
		MemFunFunctor(FT func, T* obj) 
		{
			func_ = func;
			obj_ = obj;
		}

		ResultType operator ()() const
		{
			return (obj_->*func_)();
		}

		ResultType operator ()(Param1 p1) const
		{
			return (obj_->*func_)(p1);
		}
    	
        ResultType operator ()(Param1 p1, Param2 p2) const
		{
			return (obj_->*func_)(p1, p2);
		}

        ResultType operator ()(Param1 p1, Param2 p2, Param3 p3 ) const
		{
			return (obj_->*func_)(p1, p2, p3);
		}
        
        ResultType operator ()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const
		{
			return (obj_->*func_)(p1, p2, p3, p4);
		}


		IMPLEMENT_FUNCTOR_CLONE(self_type);

	protected:
		T* obj_;
		FT func_;
	};

	#undef IMPLEMENT_FUNCTOR_CLONE
};

template <class ResultType, class  Args>	
class Functor
{
public:
	typedef Functor<ResultType, Args> self_type;
	typedef functor_private::FunctorImpl<ResultType, Args> impl_type;

	typedef typename impl_type::Param1 Param1;
	typedef typename impl_type::Param2 Param2;
	typedef typename impl_type::Param3 Param3;
	typedef typename impl_type::Param4 Param4;

public:
	Functor() : impl_(0) { };
	Functor(const self_type& f) : impl_(0) { *this = f; }

	self_type& operator = (const self_type& f)
	{
		if (impl_) delete impl_;
		impl_ = f.impl_->clone();

		return *this;
	}

	template <class T>
	Functor(T t) : impl_(new functor_private::ForwardFunctor<T, ResultType, Args>(t)) {}

	template <class T, class FT>
	Functor(FT fun, T* obj) : impl_(new functor_private::MemFunFunctor<T, FT, ResultType, Args>(fun, obj)) {}

	~Functor() { if (impl_) delete impl_; }

	ResultType operator ()()  const { assert(impl_);  return (*impl_)(); }
	ResultType operator ()(Param1 p1) const { assert(impl_); return (*impl_)(p1); }
    ResultType operator ()(Param1 p1, Param2 p2) const { assert(impl_); return (*impl_)(p1, p2); }
    ResultType operator ()(Param1 p1, Param2 p2, Param3 p3) const { assert(impl_); return (*impl_)(p1, p2, p3); }
    ResultType operator ()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const { assert(impl_); return (*impl_)(p1, p2, p3, p4); }
    
protected:
	impl_type* impl_;
};

}

#endif