#ifndef EDSIM_STLSERIALIZER
#define EDSIM_STLSERIALIZER

#include <ed/vector.h>
#include <ed/map.h>
#include <ed/unordered_map.h>
#include <ed/list.h>
#include <ed/set.h>
#include <ed/deque.h>
#include <stdint.h>
#include <ed/string.h>

// сериализация STL
template <class S, class T1, class T2>
S& operator>>(S& serializer, std::pair<T1, T2> &l)
{
	serializer >> l.first;
	serializer >> l.second;
	return serializer;
}

template <class S, class T>
S& serializeContainer(S& serializer, T &l)
{
	uint32_t size = (uint32_t)l.size();
	serializer >> size;

	if (serializer.getMode() == S::READ)
	{
		if (size > 0)
		{
			l.resize(size);
		}
		else
		{
			l = T();
		}
	}
	typename T::iterator iterator = l.begin();
	for (; iterator != l.end(); iterator++)
	{
		serializer >> *iterator;
	}
	return serializer;
}

template <class S>
S& serializeContainer(S& serializer, ed::vector<bool> &l)
{
	uint32_t size = (uint32_t)l.size();
	serializer >> size;

	if (serializer.getMode() == S::READ)
	{
		if (size > 0)
			l.resize(size);
		else
			l = ed::vector<bool>();
	}

	for (uint32_t i = 0; i < size; i++)
	{
		int8_t a = (int8_t)l[i];
		serializer >> a;
		l[i] = (bool)a;
	}

	return serializer;
}

#define SERIALIZE_VECTOR_FOR_SIMPLE_TYPE(T)                                   \
	template <class S> S& serializeContainer(S& serializer, ed::vector<T>& l) \
	{                                                                         \
		uint32_t size = (uint32_t)l.size();                                   \
		serializer >> size;                                                   \
																			  \
		if (serializer.getMode() == S::READ)								  \
		{																	  \
			if (size > 0)                                                     \
				l.resize(size);                                               \
			else															  \
				l = ed::vector<T>();                                          \
		}																	  \
																			  \
		if (size > 0)														  \
			serializer.stream(&l[0], size * sizeof(T));                       \
																			  \
		return serializer;                                                    \
	}

SERIALIZE_VECTOR_FOR_SIMPLE_TYPE(int8_t)
SERIALIZE_VECTOR_FOR_SIMPLE_TYPE(uint8_t)
SERIALIZE_VECTOR_FOR_SIMPLE_TYPE(int16_t)
SERIALIZE_VECTOR_FOR_SIMPLE_TYPE(uint16_t)
SERIALIZE_VECTOR_FOR_SIMPLE_TYPE(int32_t)
SERIALIZE_VECTOR_FOR_SIMPLE_TYPE(uint32_t)

template <class S, class T, class A>
S& operator >> (S& serializer, ed::list<T, A> &l)
{
	return serializeContainer(serializer, l);
}

template <class S, class T, class A>
S& operator >> (S& serializer, ed::vector<T, A> &v)
{
	return serializeContainer(serializer, v);
}

template <class S, class T, class A>
S& operator >> (S& serializer, ed::deque<T, A> &q)
{
	return serializeContainer(serializer, q);
}

template <class S>
S& operator >> (S& serializer, ed::string &s)
{
	return serializeContainer(serializer, s);
}

template <class S, class T>
S& serializeMap(S& serializer, T &m)
{
	unsigned  size = (unsigned)m.size();
	serializer >> size;
	if (serializer.getMode() == S::READ)
	{
		unsigned int i = 0;
		for (; i < size; i++)
		{
			typename T::key_type   k;
#ifndef _MSC_VER
			typename T::mapped_type   t;
#else
			typename T::referent_type   t;
#endif
			serializer >> k;
			serializer >> t;
			m.insert(typename T::value_type(k, t));
		}
		return serializer;
	}
	typename T::iterator iterator = m.begin();
	for (; iterator != m.end(); iterator++)
	{
		serializer >> const_cast<typename T::key_type &>(iterator->first);
		serializer >> iterator->second;
	}
	return serializer;
}

template<class S, class K, class T, class Pr, class A>
S& operator >> (S& serializer, ed::map<K, T, Pr, A> &m)
{
	return serializeMap(serializer, m);
}

template<class S, class K, class T, class Pr, class A>
S& operator >> (S& serializer, ed::unordered_map<K, T, Pr, A> &m)
{
	return serializeMap(serializer, m);
}

template<class S, class K, class T, class Pr, class A>
S& operator >> (S& serializer, std::multimap<K, T, Pr, A> &m)
{
	return serializeMap(serializer, m);
}

template <class S, class C, class T>
S& serializeSet(S& serializer, C &m)
{
	unsigned size = m.size();
	serializer >> size;
	if (serializer.getMode() == S::READ)
	{
		unsigned i = 0;
		for (; i < size; i++)
		{
			T v;
			serializer >> v;
			m.insert(v);
		}
		return serializer;
	}
	typename C::iterator iterator = m.begin();
	for (; iterator != m.end(); iterator++)
	{
		T v(*iterator);
		serializer >> v;
	}
	return serializer;
}

template<class S, class T, class Pr, class A>
S& operator >> (S& serializer, ed::set<T, Pr, A> &m)
{
	return serializeSet<S, ed::set<T, Pr, A>, T>(serializer, m);
}

template<class S, class T, class Pr, class A>
S& operator >> (S& serializer, std::multiset<T, Pr, A> &m)
{
	return serializeSet<S, std::multiset<T, Pr, A>, T>(serializer, m);
}

#endif // EDSIM_STLSERIALIZER
