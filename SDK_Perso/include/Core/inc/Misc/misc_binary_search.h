#pragma once

#include <algorithm>
namespace misc
{
	// TEMPLATE FUNCTION binary_search WITH PRED
	template<class _FI, class _Ty> inline
	_FI binary_search(_FI _F, _FI _L, const _Ty& _V)
	{
		_FI _I = std::lower_bound(_F, _L, _V);
		if (_I != _L && !(_V < *_I))
			return _I;
		return _L;
	}
	// TEMPLATE FUNCTION binary_search WITH PRED
	template<class _FI, class _Ty, class _Pr> inline
	_FI binary_search(_FI _F, _FI _L, const _Ty& _V, _Pr _P)
	{
		_FI _I = std::lower_bound(_F, _L, _V, _P);
		if(_I != _L && !_P(_V, *_I))
			return _I;
		return _L;
	}
}
