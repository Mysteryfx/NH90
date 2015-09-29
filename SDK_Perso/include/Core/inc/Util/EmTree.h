#ifndef _ED_Util_EmTree_h_
#define _ED_Util_EmTree_h_

#include <assert.h>

namespace ed {

/**
 * Full-linked embedded tree.
 * Construct with add() & remove()
 * Cycle through children: for(T *cur = first(); cur; cur = cur->next())
 */
class EmTree
{
protected:
	
	EmTree *m_parent;
	EmTree *m_first, *m_last;	// first & last child
	EmTree *m_next, *m_prev;	// next & previous sibling

public:
	EmTree()
	{m_first = m_last = m_next = m_prev = m_parent = 0;}

	~EmTree()
	{
		if(m_parent) m_parent->remove(this);
		while(m_last) remove(m_last);
	}

	inline EmTree *parent() {return m_parent;}
	inline EmTree *first() {return m_first;}
	inline EmTree *last() {return m_last;}
	inline EmTree *next() {return m_next;}
	inline EmTree *prev() {return m_prev;}
	
	inline const EmTree *parent()const {return m_parent;}
	inline const EmTree *first()const {return m_first;}
	inline const EmTree *last()const {return m_last;}
	inline const EmTree *next()const {return m_next;}
	inline const EmTree *prev()const {return m_prev;}

	// add/remove children
	inline void add(EmTree *node, EmTree *before = 0)
	{
		assert(node);
		assert(!before || (before->m_parent == this));
		assert(!isAncestor(node));

		if(node->m_parent)
			node->m_parent->remove(node);
		node->m_parent = this;

		node->m_next = before;
		if(before)
		{
			node->m_prev = before->m_prev;
			before->m_prev = node;
		}
		else
		{
			node->m_prev = m_last;
			m_last = node;
		}

		if(node->m_prev)
			node->m_prev->m_next = node;
		else
			m_first = node;
	}

	inline void remove(EmTree *node)
	{
		assert(node);
		assert(node->m_parent == this);

		node->m_parent = 0;
		if(node->m_next)
			node->m_next->m_prev = node->m_prev;
		else
			m_last = node->m_prev;

		if(node->m_prev)
			node->m_prev->m_next = node->m_next;
		else
			m_first = node->m_next;

		node->m_prev = node->m_next = 0;
	}

	inline bool isAncestor(EmTree *node)
	{
		for(EmTree *cur = m_parent; cur; cur = cur->m_parent)
			if(cur == node) return true;
		return false;
	}
};

template <class T,bool memFreeInDestructor=true>
class EmTreeOf : public EmTree
{
public:
	~EmTreeOf(){
		if(memFreeInDestructor){
			while(last()) delete last();
		}
	}

	inline T *parent() {return static_cast<T *>(EmTree::parent());}
	inline T *first() {return static_cast<T *>(EmTree::first());}
	inline T *last() {return static_cast<T *>(EmTree::last());}
	inline T *next() {return static_cast<T *>(EmTree::next());}
	inline T *prev() {return static_cast<T *>(EmTree::prev());}
	
	inline const T *parent()const {return static_cast<const T *>(EmTree::parent());}
	inline const T *first()const {return static_cast<const T *>(EmTree::first());}
	inline const T *last()const {return static_cast<const T *>(EmTree::last());}
	inline const T *next()const {return static_cast<const T *>(EmTree::next());}
	inline const T *prev()const {return static_cast<const T *>(EmTree::prev());}

	using EmTree::add;
	using EmTree::remove;
};

} // namespace Util

#endif /* _ED_Util_EmTree_h_ */
