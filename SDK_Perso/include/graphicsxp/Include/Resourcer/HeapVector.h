#pragma once

#include "Resourcer\FrameMemoryHeap.h"

extern GRAPHICSXP_API unsigned int HEAP_VECTOR_INDEX_CHECK;

enum HeapVectorAllocType{
	HV_FRAME_HEAP = 0,
	HV_DYNAMIC,
	HV_STACK
};

template <class DATA> class HeapVector
{
protected:
	DATA* data;
	unsigned usize;
	unsigned reserved;

	HeapVectorAllocType allocType;

	unsigned reservedsize() const {return reserved;};
	void setreservedsize(unsigned new_size)
	{
		reserved = new_size;
	};
public:
	typedef DATA* iterator;
	iterator begin(){return data;};
	iterator end(){return data+usize;};
	
	inline HeapVectorAllocType getAllocType()const {return allocType;}

	inline HeapVector(HeapVectorAllocType allocType = HV_DYNAMIC):
		allocType(allocType)
	{
		usize = 0;
		data = nullptr;
		reserved = 0;
	}
	inline HeapVector(const HeapVector& arg):
		allocType(arg.allocType)
	{
		data = 0;
		usize = 0;
		reserved = 0;

		*this = arg;
	}

	inline HeapVector(HeapVectorAllocType allocType, const HeapVector& arg):
		allocType(allocType)
	{
		usize = 0;
		data = 0;
		reserved = 0;

		*this = arg;
	}

	inline ~HeapVector()
	{
		switch(allocType){
			case HV_FRAME_HEAP:
				break;
			case HV_DYNAMIC:
				delete [] data;
				break;
			case HV_STACK:
				break;
		}
	}

	inline HeapVector& operator=(const HeapVector& arg)
	{
		unsigned int n = arg.size();
		resize(n);
		for(unsigned int i = 0; i < n; ++i){
			data[i] = arg.data[i];
		}
		return *this;
	}

	inline void reserve(unsigned new_size)
	{
		if( reservedsize()>=new_size)
			return;

		DATA* olddata = data;
		data = 0;
		if( !new_size) return;

		new_size = std::max((unsigned int)(reservedsize() * 1.5f) + 1, new_size);

		switch(allocType){
			case HV_FRAME_HEAP:
				data = (DATA*)frame_heap.quick_get_memory(new_size * sizeof(DATA));
				break;
			case HV_DYNAMIC:
				data = new DATA[new_size];
				break;
			case HV_STACK:
				data = (DATA*)_alloca(new_size * sizeof(DATA));
				break;
		}
		memset(data, 0, new_size * sizeof(DATA));
		// Копировать
		if( usize)
			memcpy(data, olddata, usize * sizeof(DATA));	

		setreservedsize(new_size);

		// Старье грохнуть
		switch(allocType){
			case HV_FRAME_HEAP:
				break;
			case HV_DYNAMIC:
				delete [] olddata;
				break;
			case HV_STACK:
				break;
		}
	}
	inline void resize(unsigned new_size)
	{
		reserve(new_size);
		usize = new_size;
	}

	inline void clear()
	{
		resize(0);
	}

	inline void push_back(const DATA& val)
	{
		resize(usize+1);
		(*this)[usize-1] = val;
	}

	inline unsigned size() const
	{
		return usize;
	}
	inline unsigned empty() const
	{
		return usize==0;
	}

	inline DATA& operator [](int index)
	{
		assert(HEAP_VECTOR_INDEX_CHECK != index);

		if((index < 0) || (index >= (int)usize))
		{
			static DATA dummy; dummy = 0;
			/*
			assert(FALSE);
			*/
			return dummy;
		}
		return data[index];
	}

	inline const DATA& operator [](int index) const
	{
		assert(HEAP_VECTOR_INDEX_CHECK != index);

		if((index < 0) || (index >= (int)usize))
		{
			static DATA dummy; dummy = 0;
			/*
			assert(FALSE);
			*/
			return dummy;
		}
		return data[index];
	}

};
