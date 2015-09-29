#ifndef _MISC_VECTOR_H__
#define _MISC_VECTOR_H__

//////////////////////////////////////////////
// Относительный указатель

#include "misc_ptr.h"
#include "misc_mmf_wr.h"

namespace misc
{
	// Использование:
	// ptr<int*> pInt;
	template <class CLASS> class vector
	{
		unsigned int usize;
		misc::ptr<CLASS> pData;
	public:
		vector();
		~vector()
		{
			free_inmem();
		}
		// НЕ КОПИРУЕТ ДАННЫЕ
		void Mirror(vector<CLASS>& arg)
		{
			usize = arg.usize;
			pData = arg.pData;
			arg.usize = 0;
			arg.pData = NULL;
		};

		void resize(unsigned int usize, misc::mmf_write& file);
		void resize_inmem(unsigned int usize);
		void free_inmem();
		unsigned int size() const;

		bool operator ==(const vector<CLASS>& arg) const
		{
			if( this->size() != arg.size()) return false;
			if( this->size()==0) return true;
			for(unsigned i=0; i<size(); i++)
				if( (*this)[i]!=arg[i]) return false;
			return true;
		}
		bool operator !=(const vector<CLASS>& arg) const{return !(*this==arg);}

		bool empty() const;
		typedef CLASS* iterator;
		typedef const CLASS* const_iterator;

		iterator begin(){return pData;};
		const_iterator begin() const {return pData;};

		iterator end() {CLASS* d = pData; return d+usize;};
		const_iterator end() const {const CLASS* d = pData; return d+usize;};

		CLASS& operator [](int i);
		const CLASS& operator [](int i) const;

		CLASS& back();
		const CLASS& back() const;

		template <class CLASS2> 
		void assign(CLASS2* beg, CLASS2* en, misc::mmf_write& file)
		{
			resize( en-beg, file);
			for(int i=0; beg<en; beg++, i++)
			{
				pData[i] = *beg;
			}
		}

		template <class CLASS2> 
		void assign(vector<CLASS2>& src, misc::mmf_write& file)
		{
			assign(src.begin(), src.end(), file);
		}

		template <class CLASS2> 
		void assign(CLASS2* psrc, int srccount, unsigned int pos=0)
		{
			for(int i=0; i<srccount; i++)
			{
				pData[int(pos+i)] = psrc[i];
			}
		}
		int preload()
		{
			int sum = 0;
			int s = usize*sizeof(CLASS)/sizeof(int);
			int* d = (int*)(CLASS*)pData;
			for(int a=0; a<s; ++a, ++d)
				sum += *d;
			return sum;
		}
	};

	
};

namespace misc
{
	template <class CLASS> 
	vector<CLASS>::vector()
	{
		usize = 0;
		pData = NULL;
	}

	template <class CLASS> 
	void vector<CLASS>::resize(unsigned int usize, misc::mmf_write& file)
	{
		if(pData) 
			return;
		if( usize==0) 
			return;

		pData = new (file) CLASS[usize];
		this->usize = usize;
	}
	template <class CLASS> 
	void vector<CLASS>::resize_inmem(unsigned int usize)
	{
		if(pData) 
			return;
		if( usize==0) 
			return;
		pData = new CLASS[usize];
		this->usize = usize;
	}
	template <class CLASS> 
	void vector<CLASS>::free_inmem()
	{
		CLASS* dat = pData;
		delete[] dat; 
		pData = NULL;
		this->usize = 0;
	}

	template <class CLASS> 
	unsigned int vector<CLASS>::size() const
	{
		return usize;
	}
	template <class CLASS> 
	bool vector<CLASS>::empty() const
	{
		return pData?false:true;
	}

	template <class CLASS> 
	CLASS& vector<CLASS>::operator [](int i)
	{
		return pData[i];
	}
	template <class CLASS> 
	const CLASS& vector<CLASS>::operator [](int i) const
	{
		return pData[i];
	}
	template <class CLASS> 
	CLASS& vector<CLASS>::back()
	{
		return pData[int(usize)-1];
	}
	template <class CLASS> 
	const CLASS& vector<CLASS>::back() const
	{
		return pData[int(usize)-1];
	}
};

#endif
