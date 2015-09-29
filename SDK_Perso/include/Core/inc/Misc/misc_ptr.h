#ifndef _MISC_PTR_H__
#define _MISC_PTR_H__

#include <stdint.h>

//////////////////////////////////////////////
// Относительный указатель

namespace misc
{
	// Использование:
	// ptr<int*> pInt;
	template <class CLASS> class ptr
	{
//#ifdef _WIN64
//		int64_t offset;
//#else
		int offset;
//#endif
		void put(CLASS* arg)
		{
			if(!arg) 
				offset = 0;
			else
//#ifdef _WIN64
//				offset = (int64_t)((char*)arg - (char*)this);
//#else
				offset = (int)((char*)arg - (char*)this);
//#endif
		}
		CLASS* get() const
		{
			if(!offset) return 0;
			return (CLASS*)((char*)this + offset);
		}
	public:
		typedef CLASS* POINTER;
		ptr():offset(0){}
		ptr(const ptr<CLASS>& arg):offset(0)
		{
			put(arg.get());
		}
		ptr(const POINTER & arg)
		{
			put(arg);
		}
		operator bool() const
		{
			return offset!=0;
		}
		bool operator !() const
		{
			return offset==0;
		}
		bool operator ==(const POINTER & arg) const
		{
			return get()==arg;
		}
		bool operator !=(const POINTER & arg) const
		{
			return get()!=arg;
		}
		bool operator <(const POINTER & arg) const
		{
			return get()<arg;
		}
		bool operator >(const POINTER & arg) const
		{
			return get()>arg;
		}
		ptr<CLASS>& operator=(const POINTER & arg)
		{
			put(arg); return *this;
		}
		ptr<CLASS>& operator=(const ptr<CLASS>& arg)
		{
			put(arg.get()); return *this;
		}
		operator CLASS*() const
		{
			return get();
		}
		operator void*() const
		{
			return get();
		}
		CLASS* operator ->()
		{
			return get();
		}
		const CLASS* operator ->() const
		{
			return get();
		}
		CLASS& operator[](int i)
		{
			return get()[i];
		}
		const CLASS& operator[](int i) const
		{
			return get()[i];
		}
	};
};

#endif
