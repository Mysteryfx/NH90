
#ifndef _vector_map_H_
#define _vector_map_H_

#include <utility>
#include <algorithm>

#include <ed/vector.h>

namespace ed
{
	template<typename K, typename T>
	class vector_map_pair : public std::pair<K, T>
	{
	public:
		vector_map_pair() : pair<K, T>() {;}
		vector_map_pair(const K & k) : pair<K, T>(k, 0) {;}
		vector_map_pair(const K & k, const T & t) : pair<K, T>(k, t) {;}
		bool operator<(const vector_map_pair & vector_map_pair_) const
		{
			return first < vector_map_pair_.first;
		}
	};

	template<typename K, typename T>
	class vector_map : public vector< vector_map_pair<K, T> >
	{
	public:
		iterator insert(const K & k, const T & t)
		{
			return insert_(vector_map_pair<K, T>(k, t), true);
		}
		iterator insert(const vector_map_pair<K, T> & vector_map_pair_)
		{
			return insert_(vector_map_pair_, true);
		}
		const_iterator find(const K & key) const
		{
			const_iterator it_ins = std::lower_bound(begin(), end(), vector_map_pair_);
			if(it_ins->first == key)
			{
				return it_ins;
			}
			else
			{
				return end();
			}
		}
		iterator find(const K & key)
		{
			iterator it_ins = std::lower_bound(begin(), end(), vector_map_pair<K, T>(key, T()));
			if(it_ins->first == key)
			{
				return it_ins;
			}
			else
			{
				return end();
			}
		}
		T & get(const K & key)
		{
			return insert_(vector_map_pair<K, T>(key, T()), false)->second;
		}
	private:
		iterator insert_(const vector_map_pair<K, T> & vector_map_pair_, bool replace)
		{
			iterator it_ins = lower_bound(begin(), end(), vector_map_pair_);
			if(	it_ins != end() &&
				it_ins->first == vector_map_pair_.first)
			{
				if(replace)
				{
					(*it_ins) = vector_map_pair_;
				}				
				return it_ins;
			}
			else
			{
				return vector< vector_map_pair<K, T> >::insert(it_ins, vector_map_pair_);
			}			
		}
	};
	
	template<typename K, typename T>
	class vector_map_interpolator : public vector_map<K, T>
	{
	private:
		typedef std::pair< const vector_map_pair<K, T>*, const vector_map_pair<K, T>* > surround;
	public:
		bool set_begin(const K & key, bool enlarge = true)
		{
			vector_map_pair<K, T> vector_map_pair_(key, T());
			iterator it = lower_bound(begin(), end(), vector_map_pair_);
			if(it != end())
			{
				if(it->first != key)
				{
					if(	it == begin() &&
						enlarge)
					{
						return false;
					}
					insert_key(key);
				}
				erase(begin(), it);
			}
			else
			{
				clear();
			}
			return true;
		}
		bool set_end(const K & key, bool enlarge = true)
		{
			vector_map_pair<K, T> vector_map_pair_(key, T());
			iterator it = std::lower_bound(begin(), end(), vector_map_pair_);
			if(it != end())
			{
				if(it->first != key)
				{
					insert_key(key);
					erase(it, end());
				}
				else
				{
					if(it != begin())
					{
						erase(it++, end());
					}					
					else
					{
						clear();
					}
				}				
			}
			else
			{
				if(enlarge)
				{
					insert_key(key);
				}				
				else
				{
					return false;
				}
			}
			return true;
		}
		iterator insert_key(const K & key)
		{
			vector_map_pair<K, T> vector_map_pair_(key, T());
			iterator it = std::lower_bound(begin(), end(), vector_map_pair_);
			if(it != end() &&
				it->first == key)
			{
				interpolate(it);
				return it;
			}
			else
			{
				surround surround_;
				if(get_surround_(it - begin(), surround_, false, true))
				{
					vector_map_pair_.second = surround_.first->second + (surround_.second->second - surround_.first->second) * ((vector_map_pair_.first - surround_.first->first) / (surround_.second->first - surround_.first->first));
					return vector< vector_map_pair<K, T> >::insert(it, vector_map_pair_);
				}
				else
				{
					return end();
				}
			}
		}
		void interpolate(iterator it)
		{
			surround surround_;
			if(get_surround_(it - begin(), surround_, false, false))
			{
				it->second = surround_.first->second + (surround_.second->second - surround_.first->second) * ((it->first - surround_.first->first) / (surround_.second->first - surround_.first->first));
			}
		}
		T get(const K & key, bool limit) const
		{
			vector_map_pair<K, T> vector_map_pair_(key, T());
			const_iterator it = lower_bound(begin(), end(), vector_map_pair_);			
			if(	it != end() &&
				it->first == key)
			{
				return it->second;
			}
			else
			{
				surround surround_;				
				if(get_surround_(it - begin(), surround_, limit, true))
				{
					if(surround_.second != NULL)
					{
						return surround_.first->second + (surround_.second->second - surround_.first->second) * ((vector_map_pair_.first - surround_.first->first) / (surround_.second->first - surround_.first->first));
					}
					else
					{
						return surround_.first->second;
					}
				}
				else
				{
					return T();
				}
			}
		}
	private:
		bool get_surround_(size_t num, surround & surround_, bool limit, bool between) const
		{
			if(num == 0)
			{
				if(limit)
				{
					surround_.first		= &(*this)[1];
					surround_.second	= NULL;
					return true;
				}
				else
				{
					if(size() > 1)
					{
						surround_.first	= &(*this)[1];
						surround_.second= &(*this)[0];
						return true;
					}
					else
					{
						return false;
					}
				}
			}
			else if(num == size())
			{
				if(limit)
				{
					surround_.first		= &(*this)[size() - 1];
					surround_.second	= NULL;
					return true;
				}
				else
				{
					if(size() > 1)
					{
						surround_.first = &(*this)[size() - 2];
						surround_.second = &(*this)[size() - 1];
						return true;
					}
					else
					{
						return false;
					}
				}
			}
			else
			{
				if(between)
				{
					surround_.first = &(*this)[num - 1];
					surround_.second = &(*this)[num];
				}
				else
				{
					surround_.first = &(*this)[num - 1];
					surround_.second = &(*this)[num + 1];
				}
				return true;
			}
		}
	};
}

#endif
