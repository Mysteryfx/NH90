#ifndef _ED_Util_EmList_h_
#define _ED_Util_EmList_h_

namespace Util {

/**
 * Embedded double-linked list.
 */
class EmList {
public:
	class Node;
	class IteratorBase;
	class ConstIterator;
	class Iterator;

	class Node {
		friend class EmList;
		friend class IteratorBase;

		Node* next;
		Node* prev;

			Node(Node* self) : next(self), prev(self) {}
//			Node(EmList*) : next(this), prev(this) {}

		Node* insert_before(Node* n);
		Node* insert_after(Node* n);

		Node* insert_before(EmList&);
		Node* insert_after(EmList&);

	public:
			Node() : next(0), prev(0) {}
			~Node() { remove_from_list(); }

		Node* remove_from_list();
		bool is_in_list() const { return next!=0; }
	};

	class IteratorBase {
		typedef IteratorBase self_t;
	protected:
		Node* m_node;

			IteratorBase(Node* n) : m_node(n) {}

		self_t& inc() { m_node = m_node->next; return *this; }
		self_t& dec() { m_node = m_node->prev; return *this; }

		bool eq(const self_t& i) const { return m_node==i.m_node; }
		bool ne(const self_t& i) const { return m_node!=i.m_node; }

		Node* insert_before(Node* n) { return m_node->insert_before(n); }
		Node* insert_after(Node* n) { return m_node->insert_after(n); }

		Node* insert_before(EmList& d) { return m_node->insert_before(d); }
		Node* insert_after(EmList& d) { return m_node->insert_after(d); }

		Node* do_remove() { return m_node->remove_from_list(); }
	};

	class ConstIterator : public IteratorBase {
		typedef IteratorBase base_t;
		typedef ConstIterator self_t;
	public:
			ConstIterator(const Node* n) : base_t(const_cast<Node*>(n)) {}

		Node* node() const { return m_node; }
		const Node* operator->() const { return m_node; }

		self_t& operator++() { return static_cast<self_t&>(base_t::inc()); }
		self_t& operator--() { return static_cast<self_t&>(base_t::dec()); }

		bool operator == (const self_t& i) const { return base_t::eq(i); }
		bool operator != (const self_t& i) const { return base_t::ne(i); }
	};

	class Iterator : public ConstIterator {
		typedef ConstIterator base_t;
		typedef Iterator self_t;
	public:
			Iterator(Node* n) : base_t(n) {}

		//using base_t::node;
		//Node* node() { return m_node; }
		Node* operator->() const { return m_node; }

		self_t& operator++() { return static_cast<self_t&>(base_t::inc()); }
		self_t& operator--() { return static_cast<self_t&>(base_t::dec()); }

		Iterator insert_before(Node* n) { return base_t::insert_before(n); }
		Iterator insert_after(Node* n) { return base_t::insert_after(n); }

		Iterator insert_before(EmList& d) { return base_t::insert_before(d); }
		Iterator insert_after(EmList& d) { return base_t::insert_after(d); }

		Node* remove() { return base_t::do_remove(); }
	};

	friend class Node;

	bool empty() const { return m_list.next == &m_list; }

	ConstIterator front() const { return m_list.next; }
	ConstIterator end() const { return &m_list; }
	ConstIterator back() const { return m_list.prev; }

	Iterator front() { return m_list.next; }
	Iterator end() { return &m_list; }
	Iterator back() { return m_list.prev; }

	Iterator push_front(Node* n) { return front().insert_before(n); }
	Iterator push_back(Node* n) { return back().insert_after(n); }
	Node* pop_front() { return front().remove(); }
	Node* pop_back() { return back().remove(); }

	void clear() { Iterator i=front(); while( i!=end() ) { Iterator e = i; ++i; e.remove(); } }

protected:
		EmList() : m_list(&m_list) {}
//		EmList() : m_list(this) {}
		~EmList() { clear(); }

	Node m_list; // next = head, prev = tail

	void reset() { m_list.next = &m_list; m_list.prev = &m_list; }

};


inline
EmList::Node* EmList::Node::remove_from_list()
{
	if( next ) {
		next->prev = prev;
		prev->next = next;
		next = 0;
		prev = 0;
	}
	return this;
}


inline
EmList::Node* EmList::Node::insert_before(Node* n)
{
	if( n->next == 0 )
	{
		n->next = this;
		n->prev = prev;
		prev->next = n;
		prev = n;
	}
	return this->prev;
}

inline
EmList::Node* EmList::Node::insert_after(Node* n)
{
	if( n->next == 0 )
	{
		n->next = next;
		n->prev = this;
		next->prev = n;
		next = n;
	}
	return next;
}


inline
EmList::Node* EmList::Node::insert_before(EmList& list)
{
	if( !list.empty() )
	{
		Node* head = list.m_list.next;
		Node* tail = list.m_list.prev;
		head->prev = prev;
		prev->next = head;
		tail->next = this;
		this->prev = tail;
		list.reset();
		return head;
	}
	return this;
}

inline
EmList::Node* EmList::Node::insert_after(EmList& list)
{
	if( !list.empty() )
	{
		Node* head = list.m_list.next;
		Node* tail = list.m_list.prev;
		tail->next = next;
		next->prev = tail;
		head->prev = this;
		this->next = head;
		list.reset();
		return head;
	}
	return next;
}


/**
 * Type-safe wrapper for embedded double-linked list.
 */
template<class T>
class EmListOf : private EmList {
	typedef EmList base_t;
public:
	class ConstIterator;
	class Iterator;

	friend class ConstIterator;
	friend class Iterator;

	class ConstIterator : protected EmList::IteratorBase {
		friend class EmListOf<T>;
		typedef EmList::IteratorBase base_t;
		typedef ConstIterator self_t;
	protected:
			ConstIterator(const EmList::IteratorBase& i) : base_t(i) {}
	public:
			ConstIterator(const T* n) : base_t(const_cast<T*>(n)) {}

		const T& operator*() const { return *static_cast<const T*>(m_node); }
		const T* operator->() const { return static_cast<const T*>(m_node); }
		self_t& operator++() { return static_cast<self_t&>(base_t::inc()); }
		self_t& operator--() { return static_cast<self_t&>(base_t::dec()); }

		bool operator==(const ConstIterator& i) { return base_t::eq(i);}
		bool operator!=(const ConstIterator& i) { return base_t::ne(i);}
	};

	class Iterator : public ConstIterator {
		friend class EmListOf<T>;
		typedef ConstIterator base_t;
		typedef Iterator self_t;

			Iterator(const EmList::IteratorBase& i) : base_t(i) {}
	public:
			Iterator(T* n) : base_t(n) {}

		T& operator*() const { return *static_cast<T*>(m_node); }
		T* operator->() const { return static_cast<T*>(m_node); }
		self_t& operator++() { return static_cast<self_t&>(base_t::inc()); }
		self_t& operator--() { return static_cast<self_t&>(base_t::dec()); }

		self_t insert_before(T* n) { return static_cast<T*>(base_t::insert_before(n)); }
		self_t insert_after(T* n) { return static_cast<T*>(base_t::insert_after(n)); }

		self_t insert_before(EmListOf<T>& d) { return static_cast<T*>(base_t::insert_before(d)); }
		self_t insert_after(EmListOf<T>& d) { return static_cast<T*>(base_t::insert_after(d)); }

		T* remove() { return static_cast<T*>(base_t::do_remove()); }
	};

		EmListOf() {}
		~EmListOf() {}

	using EmList::empty;
	using EmList::clear;

	ConstIterator front() const { return base_t::front(); }
	ConstIterator end() const { return base_t::end(); }
	ConstIterator back() const { return base_t::back(); }

	Iterator front() { return base_t::front(); }
	Iterator end() { return base_t::end(); }
	Iterator back() { return base_t::back(); }

	Iterator push_front(T* n) { return front().insert_before(n); }
	Iterator push_back(T* n) { return back().insert_after(n); }
	T* pop_front() { return front().remove(); }
	T* pop_back() { return back().remove(); }

};


} // namespace Util

#endif /* _ED_Util_EmList_h_ */
