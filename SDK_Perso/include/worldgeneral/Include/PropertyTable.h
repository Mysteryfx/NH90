#pragma once

#include <ed/map.h>
#include <ed/string.h>

template<typename T>
class PropertyTable_
{
public:

	typedef T*& PropertyValue;

	void set_property(const ed::string& name, const double value) 
	{
		auto it = map_.find(name);

		if(it == map_.end())
		{
			map_.emplace(std::make_pair(name, new double(value)));
			return;
		}
		
		*it->second = value;
	}

	T*& get_property_ptr(const ed::string& name) 
	{
		auto it = map_.find(name);

		if(it == map_.end())
		{
			auto new_it = map_.emplace(std::make_pair(name, new double(0.0)));

			return new_it.first->second;
		}
		return it->second;
	}

	~PropertyTable_()
	{
		for(auto& it : map_)
			delete it.second;
	}

private:
	ed::map<const ed::string,T*> map_;
};

template class PropertyTable_<double>;

typedef PropertyTable_<double> PropertyTable;

// template<typename T>
// class MaskingProperty
// {
// public:
// 
// 	MaskingProperty(PropertyTable<T>& property_map, const ed::string name)
// 		: subst_ptr_(property_map.get_property_ptr(name)),subst_val_ptr_(subst_ptr_), value_(T(0.0))
// 	{
// 		subst_ptr_ = &value_;
// 	}
// 
// 	~MaskingProperty()
// 	{
// 		subst_ptr_ = subst_val_ptr_;
// 	}
// 
// 	T& operator=(const T& val)
// 	{
// 		return value_ = val;
// 	}
// 
// private:
// 
// 	typename PropertyTable<T>::PropertyValue subst_ptr_;		//pointer to the property map element
// 
// 	T*const subst_val_ptr_;									//pointer to the masked value
// 	T		value_;											//masking value
// };

//template class MaskingProperty<double>;

template<typename T>
class MaskingPropertyPtr_
{
public:

	MaskingPropertyPtr_(T* ptr, PropertyTable_<T>& property_map, const ed::string name)
		: subst_ptr_(property_map.get_property_ptr(name)),subst_val_ptr_(subst_ptr_)
	{
		if(ptr != nullptr)
			subst_ptr_ = ptr;
	}

	~MaskingPropertyPtr_()
	{
		subst_ptr_ = subst_val_ptr_;
	}

	T& operator=(const T& val)
	{
		return *subst_ptr_ = val;
	}

	operator T()
	{
		return *subst_ptr_;
	}

private:

	typename PropertyTable_<T>::PropertyValue subst_ptr_;		//pointer to the property map element
	T*const subst_val_ptr_;									//pointer to the masked value
};

template class MaskingPropertyPtr_<double>;

typedef MaskingPropertyPtr_<double> MaskingPropertyPtr;