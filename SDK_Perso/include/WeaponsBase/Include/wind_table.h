#pragma once
/************************************************************************

	Class  WindTable is used for 
	storing wind data and 
	getting a spherical interpolated wind vector at desired altitude      

************************************************************************/

#include <algorithm>

#include "ed/deque.h"
#include "Math/Vector.h"
#include "Mem/Shared.h"

#ifdef WIND_SLERP_INT
namespace Math
{
	template<int N, class T>
	inline
	Vector<N, T> slerp_v(const Vector<N, T>& v1, const Vector<N, T>& v2, T t)
	{
		T alpha = acos(dot(v1,v2));
		T sin_a = sin(alpha);

		if (sin_a > 0.0005)
			return (v1 * sin( (1.0 - t) * alpha) + v2 * sin(t * alpha)) / sin_a;
		else
			return v1 * (1.0-t) + v2*t;
	}

	template<int N, class T>
	inline
	Vector<N, T> normalize_s(const Vector<N, T>& v)
	{
		T len = length(v);
		return len ? v / len : v;
	}
}
#endif

namespace weapon_base
{

template<typename T, class Vector_ = Math::Vector<3,T>>
class  IWindTable : public Mem::Shared
{
public:

	typedef Vector_ Vector;

	virtual Vector get_wind(const T& altitude) const = 0;
	virtual void insert_wind(const Vector& wind, const T& altitude = 0.0) = 0;
	virtual void clear() = 0;
	virtual bool empty() const { return false; }

	virtual ~IWindTable(){}
};

template<typename T>
class BaseSingleVectorTable : public IWindTable<T>
{
public:

	BaseSingleVectorTable()
		:wind_(0)
	{}

	virtual void insert_wind(const Vector& wind, const T& altitude = 0.0)
	{
		wind_ = wind;
	}

	virtual Vector get_wind(const T& altitude /*not used*/) const 
	{
		return wind_;
	}

	virtual void clear()
	{
		wind_ = Vector(0);
	}

private:
	Vector wind_;
};


using ed::deque;

template<typename T>
class BaseWindTable : public IWindTable<T>
{
public:

	virtual void clear()
	{
		wind_db_.clear();
	}

	virtual bool empty() const { return wind_db_.empty(); }

	virtual void insert_wind(const Vector& wind, const T& altitude)
	{
		insert_wind(wind,altitude,true);
	}

	void insert_wind(const Vector& wind, const T& altitude, bool sort = true)
	{
		if(!sort || wind_db_.empty())
		{
			wind_db_.push_back(WindItem(altitude,wind));
			return;
		}

		auto it = get_wind_above_alt(altitude);

		if(it != wind_db_.end() && it->altitude_ == altitude)
			return;

		wind_db_.insert(it,WindItem(altitude,wind));
	}

	Vector get_wind(const T& altitude) const
	{
		if (wind_db_.empty())
			return Vector(0.,0.,0.);

		auto it = get_wind_above_alt(altitude);

		if (it == wind_db_.end())
			return (--it)->wind_;

		if(it->altitude_ == altitude)
			return it->wind_;

		const Vector next_wind = it->wind_;
		T next_alt = it->altitude_;

		if (it == wind_db_.begin())
			return next_wind;

		it--;

		const Vector prev_wind = it->wind_;
		const T prev_alt = it->altitude_;

		T scale =  (altitude - prev_alt) / (next_alt - prev_alt);

#ifdef WIND_SLERP_INT
		T speed = Math::length(prev_wind) + (Math::length(next_wind) -  Math::length(prev_wind)) * scale;
		return Math::slerp_v(Math::normalize_s(prev_wind),Math::normalize_s(next_wind),scale) * speed;
#else
		return Math::lerp(prev_wind,next_wind,scale);
#endif
	}

private:

	struct WindItem
	{
		WindItem(T altitude, Vector wind)
			: altitude_(altitude), wind_(wind) {}

		Vector wind_;
		T altitude_;
	};

	typedef deque<WindItem> WindDatabaseType;
	typedef typename deque<WindItem>::const_iterator WindBaseIterator;

	WindBaseIterator get_wind_above_alt(const T& altitude) const
	{
		deque<WindItem>::const_iterator it;
		return  std::lower_bound(wind_db_.begin(),wind_db_.end(),altitude,
			[](decltype(*wind_db_.begin())lhs, const T rhs) -> bool
		{
			return lhs.altitude_ < rhs;
		});
	}

	WindDatabaseType wind_db_;
};

template<typename T>
class WindTableProxy_
{
public:

	typedef IWindTable<T>* WindTablePtr;
	typedef Mem::Ptr<IWindTable<T>> WindTableSharedPtr;

	WindTablePtr get_w_table() const 
	{
		return current_.get();
	}

	void set_table(WindTableSharedPtr new_table)
	{
		current_ = new_table;
	}

private:
	WindTableSharedPtr current_;
};


typedef float float_type;
typedef BaseWindTable<float_type> WindTable;
typedef BaseSingleVectorTable<float_type> SingleVectorTable;
typedef SingleVectorTable::Vector WindVector;
typedef WindTableProxy_<float> WindTableProxy;
typedef WindTableProxy::WindTablePtr IWindTablePtr;

}