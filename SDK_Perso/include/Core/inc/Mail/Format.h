/**
 * This file is a part of
 * Mail library version 0.5
 * (C)opyright 2002,2003 Dmitry S. Baikov. All rights reserved.
 *
 * Licensed under GNU Lesser General Public License version 2.1 or later.
 * For details see file LICENSE or http://www.gnu.org/copyleft/lesser.html
 */
#ifndef MAIL_FORMAT_H
#define MAIL_FORMAT_H


#include "./config.h"
#include "./Writer.h"
#include "./Reader.h"


namespace Mail {


// --- Skeleton ---

// stream formatter
template<class M>
struct Format {
	M manip;
	Format(const M& m) : manip(m) {}
};


/**
 * Stream formatted value.
 */
template<class M> inline
Stream& operator MAIL_IO_OPERATOR (Stream& s, const Format<M>& f)
{
	if( s.mode() == Stream::Read ) {
		f.manip.get( static_cast<Reader&>(s) );
	} else {
		f.manip.put( static_cast<Writer&>(s) );
	}
	return s;
}


// --- Formatters ------------------------------------------------------------------------

// store T-value as S
template<class S, class T>
class _Convert {
private:
	T& value;

public:
		_Convert(T& t) : value(t) {}

	void put(Writer& out) const
	{
		S store(value);
		out MAIL_IO_OPERATOR store;
	}

	void get(Reader& in) const
	{
		S store;
		in MAIL_IO_OPERATOR store;
		value = T(store);
	}
}; // class Convert<S,T>



// store T-value as N-bit integer
template<int N, class T>
class _Truncate {
private:
	T& value;
public:
		_Truncate(T& v) : value(v) {}

	void put(Writer& s) const
	{
		unit_t u = unit_t(value);
		Bit::IO<N>::put(s, u);
	}

	void get(Reader& s) const
	{
		unit_t u = Bit::IO<N>::get(s);
		value = T(u);
	}
}; // class Truncate<N,T>



// store T-value as compressed-range N-bit integer
template<int N, class T>
class _Compress {
private:
	T& value;
	T low, res;

public:
		_Compress(T& v, T m, T r) : value(v), low(m), res(r) {}

	void put(Writer& s) const
	{
		unit_t u = unit_t( (value - low)/res );
		Bit::IO<N>::put(s, u);
	}

	void get(Reader& s) const
	{
		unit_t u = Bit::IO<N>::get(s);
		value = T(u)*res + low;
	}
}; // class Convert<N,T>


// --- format functions ------------------------------------------------------------------------

/**
 * Store T-value as S
 */
template<class S, class T> inline
Format< _Convert<S,T> > convert(T& val) { return _Convert<S,T>(val); }


/**
 * Store T-value as N-bit integer = (val-min)/res
 * i.e. shift by (-min) and scale by (1/res)
 */
template<int N, class T> inline
Format< _Compress<N,T> > compress(T& t, T low, T res) { return _Compress<N,T>(t, low, res); }


/**
 * Store T-value as N-bit integer
 */
template<int N, class T> inline
Format< _Truncate<N,T> > truncate(T& val) { return _Truncate<N,T>(val); }


} // Mail namespace

#endif /* MAIL_FORMAT_H */
