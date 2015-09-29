#pragma  once

namespace FuzzyLogic
{
	template <typename ValueType>
	class Trapezoid
	{
	public:
		Trapezoid(ValueType a, ValueType b, ValueType c, ValueType d)
			:_a(a), _b(b), _c(c), _d(d)
		{}
		~Trapezoid()
		{}
		
		ValueType getValue(ValueType point) const;
	private:
		ValueType _a, _b, _c, _d;
	};

	template <typename ValueType>
	ValueType Trapezoid<ValueType>::getValue(ValueType point) const
	{
		if (point < _a)
			return 0;
		
		if (point < _b)
			return (point-_a)/(_b-_a);
		if (point < _c)
			return 1;
		if (point < _d)
			return (_d - point)/(_d - _c);
		
		return 0;
	}
}

typedef FuzzyLogic::Trapezoid<double> dTrapezoid;