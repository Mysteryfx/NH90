// ******************************************************** 
//               COPYRIGHT  (C) 2010
//                
//     This program is property of Eagle Dynamics 
//            All rights reserved.
// ******************************************************** 
//                
//  SubSystem   : Electric System 
//  Author		: Mikhail Makaruk 
//  Created		: 04.03.2010
//                

#pragma once
#include "assert.h"
#include "Mem/Shared.h"

#include <functional>
#include <algorithm>

namespace lwl
{

struct CSignalInfo
{
	CSignalInfo(void* s = nullptr) : sender(s) {}
	void* sender;
};

class CBaseDelegate : public Mem::Shared
{
public:
	virtual void Invoke(const CSignalInfo* pInfo) = 0;
	virtual bool operator == (const CBaseDelegate& arg) = 0;
};

template <class T>
class CDelegate : public CBaseDelegate
{
public:
	typedef void (T::*PMethod)(const CSignalInfo*);
	CDelegate(T* pThis, PMethod pMethod) : m_pThis(pThis), m_pMethod(pMethod) {}

	virtual bool operator == (const CBaseDelegate& arg)
	{
		const CDelegate<T>* pDelegate = dynamic_cast<const CDelegate<T>*>(&arg);
		return pDelegate && pDelegate->m_pThis == m_pThis && pDelegate->m_pMethod == m_pMethod;
	}

	virtual void Invoke(const CSignalInfo* pInfo)
	{
		(m_pThis->*m_pMethod)(pInfo);
	}

private:
	T* m_pThis;
	PMethod m_pMethod;
};


class CSignal
{
public:
	~CSignal()
	{
		clear();
	}

	void clear() throw()
	{
		m_delegates.clear();
	}

	bool operator () (const CSignalInfo* pInfo)
	{
		std::for_each(m_delegates.begin(), m_delegates.end(), [&] (Delegates::value_type& ptr) {ptr.get()->Invoke(pInfo);});
		return true;
	}

	CBaseDelegate* operator += (CBaseDelegate* pDelegate)
	{
		for (auto it = m_delegates.begin(); it != m_delegates.end(); ++it)
		{
			if (*((*it).get()) == *pDelegate)
			{
				assert(!"Electrics: delegate already exists");
				return nullptr;
			}
		}

		m_delegates.push_back(pDelegate);
		return pDelegate;
	}

	CBaseDelegate* operator -= (CBaseDelegate* pDelegate)
	{
		for (auto it = m_delegates.begin(); it != m_delegates.end(); ++it)
		{
			if (*((*it).get()) == *pDelegate)
			{
				m_delegates.erase(it);
				break;
			}
		}
		return nullptr;
	}

	template <class T>
	CBaseDelegate* operator += (const CDelegate<T>& delegate)
	{
		for (auto it = m_delegates.begin(); it != m_delegates.end(); ++it)
		{
			if (*((*it).get()) == delegate)
			{
				assert(!"Electrics: delegate already exists");
				return nullptr;
			}
		}

		m_delegates.push_back(new CDelegate<T>(delegate));
		return m_delegates.back();
	}

	template <class T>
	CBaseDelegate* operator -= (const CDelegate<T>& delegate)
	{
		for (auto it = m_delegates.begin(); it != m_delegates.end(); ++it)
		{
			if (*((*it).get()) == delegate)
			{
				m_delegates.erase(it);
				break;
			}
		}

		return nullptr;
	}
protected:
	typedef Mem::Ptr<CBaseDelegate> DelegatePtr;
	typedef ed::vector<DelegatePtr> Delegates;

	Delegates m_delegates;
};

template <class T> inline CBaseDelegate* delegate(T* pThis, void (T::*_Pm)(const CSignalInfo*))
{
	return new CDelegate<T>(pThis, _Pm);
}

}
