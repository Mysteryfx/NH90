#pragma once

#include "misc_vector.h"
namespace misc
{
	class bitarray
	{
		vector<unsigned int> bits;
		unsigned int usize;
	public:
		bitarray()
		{
			usize = 0;
		}
		
		void resize(unsigned int usize, misc::mmf_write& file)
		{
			this->usize = usize;
			bits.resize(usize/32 + 1, file);
			memset( bits.begin(), 0, 4*(bits.size()));
		}
		void resize_inmem(unsigned int usize)
		{
			this->usize = usize;
			bits.resize_inmem(usize/32 + 1);
			memset( bits.begin(), 0, 4*(bits.size()));
		}
		unsigned int size() const
		{
			return usize;
		}
		void assign(const bitarray& arg, misc::mmf_write& file)
		{
			usize = arg.usize;
			bits.assign( arg.bits.begin(), arg.bits.end(), file);
		}

		void set(int b, bool value)
		{
			unsigned int& dw = bits[b/32];
			unsigned int bit = 1 << (b&0x1F);
			dw &= ~bit;
			if( value) dw |= bit;
		}
		bool get(int b) const
		{
			unsigned int dw = bits[b/32];
			unsigned int bit = 1 << (b&0x1F);
			return (dw&bit) != 0;
		}
		bool operator[](int b) const
		{
			return get(b);
		}
	};
}
