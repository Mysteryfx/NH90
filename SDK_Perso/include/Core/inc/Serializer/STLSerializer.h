#ifndef __STLSerializer_h__
#define __STLSerializer_h__

#include <ed/list.h>
#include <queue>
#include <ed/deque.h>
#include <ed/vector.h>
#include <ed/map.h>
#include <ed/unordered_map.h>
#include <ed/set.h>
#include "./Serializer.h"

/**
 * Interfaces STLSerializer
 *
 *  template <class T, class A> inline 
 *  Serializer& operator << (Serializer& serializer, ed::list<T, A> &);
 *
 *  template <class T, class A> inline 
 *  Serializer& operator << (Serializer& serializer, ed::vector<T, A> &);
 *
 *  template<class K, class T, class Pr, class A> inline 
 *  Serializer& operator << (Serializer& serializer, ed::map<K, T, Pr, A> &);
 *
 *  inline Serializer& operator << (Serializer& serializer, ed::string &string);
 *
 * @author Alexander Matveev
 */

//===============================================================================================
template <class T> inline
Serializer &    operator<<(Serializer &serializer, T *&t)
{
	Serializable *p = 0;
	if (!serializer.isLoading())
	{
		p = dynamic_cast<Serializable *>(t);
	}
	serializer.persist(p);
	if (serializer.isLoading())
	{
		t = dynamic_cast<T *>(p);
	}
	return serializer;
}

template <class T> inline 
Serializer& serializeContainer(Serializer& serializer, T &l)
{
	unsigned int size = (unsigned int)l.size();
	serializer << size;
	if (serializer.isLoading())
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
		serializer << *iterator;
	}
	return serializer;
}
//===============================================================================================
template <class T, class A> inline 
Serializer& operator << (Serializer& serializer, ed::list<T, A> &l)
{
	return serializeContainer(serializer, l);
}
//===============================================================================================
template <class T, class A> inline 
Serializer& operator << (Serializer& serializer, ed::vector<T, A> &v)
{
	return serializeContainer(serializer, v);
}
//===============================================================================================
template <class T, class A> inline 
Serializer& operator << (Serializer& serializer, ed::deque<T, A> &q)
{
	return serializeContainer(serializer, q);
}
//===============================================================================================
inline Serializer& operator << (Serializer& serializer, ed::string &s)
{
	return serializeContainer(serializer, s);
}
//===============================================================================================
template <class T, class A> 
class QueueSerializer: public std::queue<T, A>
{
public:
	QueueSerializer(const std::queue<T, A> &q): std::queue<T, A>(q) {}
	void serialize(Serializer& serializer) { serializer << std::queue<T, A>::c; }
};
//===============================================================================================
template <class T, class A, class P> 
class PriorityQueueSerializer: public std::priority_queue<T, A, P>
{
public:
	PriorityQueueSerializer(const std::priority_queue<T, A, P> &q):std::priority_queue<T, A, P>(q) {}
	void serialize(Serializer& serializer) { serializer << std::priority_queue<T, A, P>::c; }
};
//===============================================================================================
template <class T, class A> inline 
Serializer& operator << (Serializer& serializer, std::queue<T, A> &q)
{
	QueueSerializer<T, A> qs(q);
	qs.serialize(serializer);
	if (serializer.isLoading())
	{
		q = qs;
	}
	return serializer;
}
//===============================================================================================
template <class T, class A, class P> inline 
Serializer& operator << (Serializer& serializer, std::priority_queue<T, A, P> &q)
{
	PriorityQueueSerializer<T, A, P> qs(q);
	qs.serialize(serializer);
	if (serializer.isLoading())
	{
		q = qs;
	}
	return serializer;
}
//===============================================================================================
template <class T> inline 
Serializer& serializeMap(Serializer& serializer, T &m)
{
	unsigned int size = (unsigned int)m.size();
	serializer << size;
	if (serializer.isLoading())
	{
		unsigned int i = 0;
		for (; i < size; i++)
		{
			typename T::key_type   k;
			typename T::referent_type   t;
			serializer << k << t;
			m.insert(T::value_type(k, t));
		}
		return serializer;
	}
	typename T::iterator iterator = m.begin();
	for (; iterator != m.end(); iterator++)
	{
		serializer << const_cast<typename T::key_type &>(iterator->first) << iterator->second;
	}
	return serializer;
}
//===============================================================================================
template<class K, class T, class Pr, class A> inline 
Serializer& operator << (Serializer& serializer, ed::map<K, T, Pr, A> &m)
{
	return serializeMap(serializer, m);
}

template<class K, class T, class Pr, class A> inline 
Serializer& operator << (Serializer& serializer, ed::unordered_map<K, T, Pr, A> &m)
{
	return serializeMap(serializer, m);
}

//===============================================================================================
template<class K, class T, class Pr, class A> inline 
Serializer& operator << (Serializer& serializer, std::multimap<K, T, Pr, A> &m)
{
	return serializeMap(serializer, m);
}

//===============================================================================================
template <class C, class T> inline 
Serializer& serializeSet(Serializer& serializer, C &m)
{
	unsigned int size = (unsigned int)m.size();
	serializer << size;
	if (serializer.isLoading())
	{
		unsigned i = 0;
		for (; i < size; i++)
		{
			T v;
			serializer << v;
			m.insert(v);
		}
		return serializer;
	}
	typename C::iterator iterator = m.begin();
	for (; iterator != m.end(); iterator++)
	{
		T v(*iterator);
		serializer << v;
	}
	return serializer;
}

//===============================================================================================
template<class T, class Pr, class A> inline 
Serializer& operator << (Serializer& serializer, ed::set<T, Pr, A> &m)
{
	return serializeSet<ed::set<T, Pr, A>, T>(serializer, m);
}

//===============================================================================================
template<class T, class Pr, class A> inline 
Serializer& operator << (Serializer& serializer, std::multiset<T, Pr, A> &m)
{
	return serializeSet<std::multiset<T, Pr, A>, T>(serializer, m);
}

#endif
