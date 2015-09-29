#pragma once

#include "ed/map.h"
#include "ed/string.h"

namespace weapons_gun {

	template<typename T>
	class BaseCreator
	{
	public:
		virtual T* create() const = 0;
	};

	template<class T>
	class Factory
	{
	public:

		static T* create(const ed::string& name) 
		{ 
			auto it = fabric.map_.find(name);
			if(it != fabric.map_.end())
				return it->second->create();
			return nullptr;
		};

		/*private:*/
		typedef ed::map<ed::string,BaseCreator<T>*>  Factorymap;
		static struct  Fmap
		{
			~Fmap()
			{
				auto it = map_.begin();
				while(it != map_.end())
				{
					delete it->second;
					++it;
				}
				map_.clear();
			}
			Factorymap map_;
		} fabric;
	};

	template <typename B, typename T, typename F>
	class Creator : public BaseCreator<B>
	{
	protected:
		Creator() {}
	public:
		Creator(const ed::string& name)
		{
			auto it = F::fabric.map_.find(name);
			if(it ==  F::fabric.map_.end())
			{
				F::fabric.map_[name] = new Creator<B,T,F>();
			}
		}

		virtual B* create() const { return new T(); }
	};

}