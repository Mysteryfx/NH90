#pragma once

#include <ed/map.h>
#include <ed/vector.h>
#include <math.h>

#ifndef __MY_INTERPOLATOR_H__
#define __MY_INTERPOLATOR_H__	1

#include "./_config.h"

#include "Lua/Config.h"

enum InterpolatorType
{
	INTERPOLATOR_TYPE_NONE,
	INTERPOLATOR_TYPE_MAP,
	INTERPOLATOR_TYPE_VECTOR
};

enum InterpolatorBorderType //тип границ интерполятора
{
	BORDER_NONE, //нет границ: величина по выходящим за мин. и макс. ключевые значения ключам экстраполируются в соотв. сторону
	BORDER_LIMITED, //жесткие границы: величина по выходящим за мин. и макс. ключевые значения ключам ограничивается величинами по соотв. границам
	BORDER_CYCLIC //жесткие границы и зацикленное значение ключа: ключ зацикливается между максимумом и минимумом
};

//-=Declarations=-

template<typename kT, typename sT>
class InterpolatorTable
{
public:
	//birth & death
	inline InterpolatorTable() : border_type(BORDER_NONE) {;}
	inline virtual void load_from_state(lua_State * config_) = 0;
	inline virtual void clear() = 0;
	//set
	inline void set_border_type(InterpolatorBorderType border_type_) {border_type = border_type_;}
	//get
	inline virtual InterpolatorType			get_type() const = 0;
	inline InterpolatorBorderType	get_border_type() const {return border_type;}	
	inline virtual kT get_key_min() const = 0;
	inline virtual kT get_key_max() const = 0;
	inline virtual sT & get_value_by_key(kT key) = 0;
	inline virtual int size() const = 0;
	//func
	//inline virtual sT operator [](kT key) const {return get(key);}
	inline virtual sT get(kT key) const;
protected:
	inline virtual void get_points_pair(kT & key, kT & key1, sT & value1, kT & key2, sT & value2, bool & use_2_point) const = 0;
	InterpolatorBorderType border_type;
	static sT void_value;
};

template<typename kT, typename sT>
class MapInterpolator : 	public InterpolatorTable<kT, sT>,
							public ed::map<kT, sT>
{
public:
	inline virtual void clear() { ed::map<kT, sT>::clear(); }
	inline virtual void load_from_state(lua_State * config_);
	inline virtual void setBorders(kT minKey, kT maxKey);
	//set
	//inline void add_element(kT key, sT value);
	inline virtual InterpolatorType get_type() const {return INTERPOLATOR_TYPE_MAP;}
	inline virtual kT get_key_min() const;
	inline virtual kT get_key_max() const;
	inline virtual sT & get_value_by_key(kT key);
	inline virtual int size() const;
protected:
	inline virtual void get_points_pair(kT & key, kT & key1, sT & value1, kT & key2, sT & value2, bool & key_in_table) const;
};

template<typename kT, typename sT>
class VectorInterpolator : 	public InterpolatorTable<kT, sT>,
							public ed::vector<sT>
{
public:
	inline VectorInterpolator() {key_min = kT(0); key_max = kT(0);}
	inline virtual void clear() { ed::vector<sT>::clear(); key_min = key_max = 0;}
	inline virtual void load_from_state(lua_State * config_);
	//set
	inline void set_key_min(kT key_min_) {key_min = key_min_;}
	inline void set_key_max(kT key_max_) {key_max = key_max_;}
	//get
	inline virtual InterpolatorType get_type() const {return INTERPOLATOR_TYPE_VECTOR;}
	inline virtual kT get_key_min() const {return key_min;}
	inline virtual kT get_key_max() const {return key_max;}
	inline virtual sT & get_value_by_key(kT key);
	inline virtual int size() const;
	kT key_min;
	kT key_max;
protected:
	//func
	inline virtual void get_points_pair(kT & key, kT & key1, sT & value1, kT & key2, sT & value2, bool & key_in_table) const;
};

//-=Inline=-

//-=InterpolatorTable=-

template<typename kT, typename sT>
sT InterpolatorTable<kT, sT>::void_value;

template<typename kT, typename sT>
inline sT InterpolatorTable<kT, sT>::get(kT key) const
{
	kT key1, key2;
	sT value1, value2;
	bool use_2_points;
	get_points_pair(key, key1, value1, key2, value2, use_2_points);
	if(use_2_points)
	{
		return  (value2 - value1) / (key2 - key1) * (key - key1) + value1;
	}
	else
	{
		return value1;
	}
}

//-=MapInterpolator=-

//template<typename kT, typename sT>
//inline void MapInterpolator<kT, sT>::add_element(kT key, sT value)
//{
//	ed::map<kT, sT>::operator[](key) = value;
//}

template<typename kT, typename sT>
inline void MapInterpolator<kT, sT>::load_from_state(lua_State * config_)
{
	Lua::Config config(config_, lua_gettop(config_));
    config.iterate_begin();
    while(config.iterate_next())
    {
		kT key;
		sT value;
		config.get(1, &key);
		config.get(2, &value);
		ed::map<kT, sT>::operator[](key) = value;

	}
	config.iterate_end();
}
template<typename kT, typename sT>
inline void MapInterpolator<kT, sT>::setBorders(kT minKey, kT maxKey)
{
	if(size() > 1)
	{
		kT key1 = begin()->first;
		sT val1 = begin()->second;
		kT key2 = rbegin()->first;
		sT val2 = rbegin()->second;
		if(	minKey < key1 ||
			maxKey > key2)
		{
			sT borderValue = val2 + (val1 - val2) / ((maxKey - key2) + (key1 - minKey)) * (maxKey - key2);
			if(minKey < key1)
			{
				(*this)[minKey] = borderValue;
			}
			if(maxKey > key2)
			{
				(*this)[maxKey] = borderValue;
			}
		}
	}
	else
	{
		(*this)[minKey] = begin()->first; 
		(*this)[maxKey] = begin()->first;
	}
}

template<typename kT, typename sT>
inline kT MapInterpolator<kT, sT>::get_key_min() const
{
	if(!empty())
	{	
		return begin()->first;
	}
	else
	{
		return kT(0);
	}
}

template<typename kT, typename sT>
inline kT MapInterpolator<kT, sT>::get_key_max() const
{
	if(!empty())
	{
		return rbegin()->first;
	}
	else
	{
		return kT(0);
	}
}

template<typename kT, typename sT>
inline sT & MapInterpolator<kT, sT>::get_value_by_key(kT key)
{
	return ed::map<kT, sT>::operator[](key);
}

template<typename kT, typename sT>
inline int MapInterpolator<kT, sT>::size() const
{
	return (int)ed::map<kT, sT>::size();
}

template<typename kT, typename sT>
inline void MapInterpolator<kT, sT>::get_points_pair(kT & key, kT & key1, sT & value1, kT & key2, sT & value2, bool & use_2_point) const
{
	if(empty())
	{
		return;
	}
	use_2_point = true;
	if(size() == 1)
	{
		key1 = begin()->first;
		value1 = begin()->second;
		use_2_point = false;
	}
	if(border_type == BORDER_CYCLIC)
	{
		if(key < get_key_min())
		{
			key = get_key_max() + fmod(key - get_key_max(), get_key_max() - get_key_min());
		}
		else if(key > get_key_max())
		{
			key = get_key_min() + fmod(key - get_key_min(), get_key_max() - get_key_min());
		}
	}
	ed::map<kT, sT>::const_iterator itM = lower_bound(key);
	if(itM == begin())
	{
		if(border_type == BORDER_NONE)
		{
			ed::map<kT, sT>::const_iterator itM = begin();
			key2 = itM->first;
			value2 = itM->second;
			itM++;
			key1 = itM->first;
			value1 = itM->second;
		}
		else if(border_type == BORDER_LIMITED)
		{
			use_2_point = false;
			key1 = get_key_min();
			value1 = begin()->second;		
		}
	}
	else if(itM == end())
	{
		if(border_type == BORDER_NONE)
		{
			ed::map<kT, sT>::const_reverse_iterator itM = rbegin();
			key2 = itM->first;
			value2 = itM->second;
			itM++;
			key1 = itM->first;
			value1 = itM->second;
		}
		else if(border_type == BORDER_LIMITED)
		{
			use_2_point = false;
			key1 = get_key_max();
			value1 = rbegin()->second;
		}
	}
	else
	{
		if(itM->first == key)
		{
			use_2_point = false;
			key1 = itM->first;
			value1 = itM->second;
		}
		else
		{
			key2 = itM->first;
			value2 = itM->second;
			itM--;
			key1 = itM->first;
			value1 = itM->second;
		}
	}
}

//-=VectorInterpolator=-

template<typename kT, typename sT>
inline void VectorInterpolator<kT, sT>::load_from_state(lua_State * config_)
{
	Lua::Config config(config_, lua_gettop(config_));
    config.iterate_begin();
	bool key_min_valid = false, key_max_valid = false;
	kT key_min_, key_max_;
	while(config.iterate_next())
    {
		kT key;
		config.get(1, &key);
		if(!key_min_valid ||
			key < key_min_)
		{
			key_min_ = key;
			key_min_valid = true;
		}
		if(!key_max_valid ||
			key > key_max_)
		{
			key_max_ = key;
			key_max_valid = true;
		}
		sT value;		
		config.get(2, &value);
		ed::vector<sT>::push_back(value);
	}
	key_min = key_min_;
	key_max = key_max_;
	config.iterate_end();
}

template<typename kT, typename sT>
inline sT & VectorInterpolator<kT, sT>::get_value_by_key(kT key)
{
	if(	key < key_min ||
		key > key_max)
	{
		return void_value;
	}
	sT key_step = (key_max - key_min) / (size() - 1);
	int num = int(floor((key - key_min) / key_step));
	return ed::vector<sT>::operator [](num);
}

template<typename kT, typename sT>
inline int VectorInterpolator<kT, sT>::size() const
{
	return (int)ed::vector<sT>::size();
}

template<typename kT, typename sT>
inline void VectorInterpolator<kT, sT>::get_points_pair(kT & key, kT & key1, sT & value1, kT & key2, sT & value2, bool & use_2_point) const
{
	if(empty())
	{
		return;
	}
	use_2_point = true;
	if(size() == 1)
	{
		key1 = front();
		value1 = front();
		key2 = key1;
		value2 = value1;
		use_2_point = false;
	}
	kT key_range = (get_key_max() - get_key_min());
	kT key_step = key_range / (size() - 1);
	if(border_type == BORDER_CYCLIC)
	{
		if(key < get_key_min())
		{
			key = get_key_max() + fmod(key - get_key_max(), key_range);
		}
		else if(key > get_key_max())
		{
			key = get_key_min() + fmod(key - get_key_min(), key_range);
		}
	}
	else
	{
		//to small
		if(key < get_key_min())
		{
			if(border_type == BORDER_NONE)
			{		
				ed::vector<sT>::const_iterator itM = begin();
				key2 = get_key_min();
				value2 = *(itM);
				itM++;
				key1 = get_key_min() + key_step;
				value1 = *(itM);
			}
			else if(border_type == BORDER_LIMITED)
			{
				use_2_point = false;
				key1 = get_key_min();
				value1 = front();		
			}
			return;
		}
		//too big
		else if(key > get_key_max())
		{
			if(border_type == BORDER_NONE)
			{
				ed::vector<sT>::const_reverse_iterator itM = rbegin();
				key2 = get_key_max();
				value2 = *(itM);
				itM++;
				key1 = get_key_max() - key_step;
				value1 = *(itM);
			}
			else if(border_type == BORDER_LIMITED)
			{
				use_2_point = false;
				key1 = get_key_max();
				value1 = *(rbegin());
			}
			return;
		}
	}
	{
		int num = int(floor((key - get_key_min() / key_step)));
		key1 = key_min + key_step * num;
		value1 = ed::vector<sT>::operator [](num);
		key2 = key_min + key_step * (num + 1);
		value2 = ed::vector<sT>::operator [](num + 1);
	}
}

typedef MapInterpolator<float, float> MapInterpolatorff;
typedef MapInterpolator<double, double> MapInterpolatordd;

typedef VectorInterpolator<float, float> VectorInterpolatorff;
typedef VectorInterpolator<double, double> VectorInterpolatordd;

#endif
