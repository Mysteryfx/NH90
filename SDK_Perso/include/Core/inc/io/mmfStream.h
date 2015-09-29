#ifndef __EDCORE_MMFSTREAM_H
#define __EDCORE_MMFSTREAM_H

#include "File.h"
#include <ed/list.h>
#include <ed/vector.h>
#include <algorithm>
#include <assert.h>
#include <string.h>

namespace io
{

/*/

MmfStream

Файл состоит из двух частей 
Заголовочная: чтение и запись осуществляются через механизмы сериализации
	Это дает гибкость форматам файлов
Проецируемая: запись отложеная потоковая, чтение через MMF

Для сериализации в проецируемую часть используется:
ed::mmfvector<>
тут могут использоваться только простые типы - без ссылок и необходимости вызывать конструктор

/*/

class MmfStream : public FileStream
{
public:
	ED_CORE_EXPORT MmfStream();
	ED_CORE_EXPORT MmfStream(const char *filename, Mode mode, unsigned useBufSize = BUF_SIZE);
	ED_CORE_EXPORT MmfStream(File *file, Mode mode, File::off_t offset = 0, unsigned useBufSize = BUF_SIZE);
	ED_CORE_EXPORT ~MmfStream();

	ED_CORE_EXPORT bool Open(const char *filename, Mode mode, unsigned useBufSize = BUF_SIZE);
	ED_CORE_EXPORT bool Close();

	// stream secondary data
	ED_CORE_EXPORT virtual void streamsecondary(void** data, uint32_t size, File*& file, uint64_t& offset);

///////////////////////////////
protected:
	// write to cache (writemode)
	File::off_t write(size_t size, void *buffer);

protected:
	void init();
	void close();

	// размерность смещений в файле
	uint32_t bitsperoffset;
protected:
	// write
	File::off_t write_startoffset;
	File::off_t write_current;
	File::off_t mmfstart;
	ed::list< ed::vector<uint8_t> > write_data;
protected:
	// read
	bool bMapToMemory;
	Mem::Ptr<MMF> _mmf;
};

// Использование:
// memvector<int> pInt;
template <class CLASS> class mmfvector
{
public:
	int usize;
	mutable CLASS* pData;

	File* file;
	uint64_t offset;
	mutable bool loaded;

	void load() const
	{
		if (!loaded && file != NULL)
		{
			size_t sz = size();
			if (sz != 0)
			{
				pData = new CLASS[sz];
				file->buffer(offset, sizeof(CLASS) * sz, pData, Stream::READ);
			}
			loaded = true;
		}
	}

public:

	typedef CLASS* iterator;
	typedef const CLASS* const_iterator;

public:
	mmfvector();
	mmfvector(const mmfvector& arg);
	~mmfvector();

	bool operator ==(const mmfvector<CLASS>& arg) const;
	bool operator !=(const mmfvector& arg) const;

	mmfvector& operator =(const mmfvector<CLASS>& arg);

	void clear();
	void resize(size_t sz);
	void resize(size_t sz, const CLASS& val);
	void assign(const CLASS* beg, const CLASS* end);
	void assign(const ed::vector<CLASS>& arg);
	void assign(const mmfvector& arg);
	void append(const CLASS* beg, const CLASS* end);
	void append(const ed::vector<CLASS>& arg);
	void free();

	size_t size() const;


	bool empty() const;

	CLASS* ptr() { load(); return pData; }
	const CLASS* ptr() const { load(); return pData; }

	iterator begin() { load(); return pData; }
	const_iterator begin() const { load(); return pData; }

	iterator end() { load(); return pData + size(); }
	const_iterator end() const { load(); return pData + size(); }

	CLASS& operator [](int i);
	const CLASS& operator [](int i) const;

	CLASS& front() { load(); return *pData; }
	const CLASS& front() const { load(); return *pData; }

	CLASS& back() { load(); return *(pData + size() - 1); }
	const CLASS& back() const { load(); return *(pData + size() - 1); }

	void reserve(size_t);

	// preload
	unsigned char preload() const;
	/*
	template <class S>
	void serialize(S& serializer) 
	{
		uint32_t sz = (uint32_t)size();
		serializer >> sz;
		serializer.streamsecondary( (void**)&pData, sz); 
		if(serializer.getMode()==S::READ)
			val.usize = sz;
	}
	*/

	void discard()
	{
		if (pData != NULL && file != NULL)
		{
			delete [] pData;
			pData = NULL;
			loaded = false;
		}
	}
};

}

ED_CORE_EXPORT unsigned char mmfMemoryBlockPreload(const unsigned char* mem, size_t size);

template <class CLASS> 
unsigned char io::mmfvector<CLASS>::preload() const
{
	if( file != NULL)
	{
		load();
		return 0;
	}
	size_t s = (int)size()*sizeof(CLASS);
	const unsigned char* d = (const unsigned char*)ptr();
	return mmfMemoryBlockPreload(d, s);
}

template <class CLASS>
CLASS& io::mmfvector<CLASS>::operator [](int i)
{
	if( i<0 || i>=(int)size())
	{
		CLASS* pNull = 0;
		return *pNull;
	}
	load();
	return pData[i];
}
template <class CLASS>
const CLASS& io::mmfvector<CLASS>::operator [](int i) const
{
	if( i<0 || i>=(int)size())
	{
		CLASS* pNull = 0;
		return *pNull;
	}
	load();
	return pData[i];
}

template <class S, class CLASS> S& operator >> (S& serializer, io::mmfvector<CLASS>& val) 
{
	uint32_t sz = (uint32_t)val.size();
	serializer >> sz;

	if (serializer.getMode() == S::READ)
	{
		val.free();
		val.usize = sz;
	}

	serializer.streamsecondary((void**)&val.pData, sz * sizeof(CLASS), val.file, val.offset);

	// Если не вернули data, то mmfvector работает не в mmf режиме
	if (val.pData == NULL && val.usize > 0)
	{
		// но тогда нам надо файл и offset откуда будем читать
		assert(val.file != NULL && val.offset != 0);
		val.loaded = false;
	}

	return serializer;
}

template <class CLASS> 
void io::mmfvector<CLASS>::clear()
{
	free();
}


template <class CLASS> 
io::mmfvector<CLASS>::mmfvector()
{
	usize  = 0;
	pData  = NULL;
	file   = NULL;
	offset = 0;
	loaded = false;
}

template <class CLASS> 
io::mmfvector<CLASS>::mmfvector(const mmfvector<CLASS>& arg)
{
	usize  = 0;
	pData  = NULL;
	file   = NULL;
	offset = 0;
	loaded = false;
	*this = arg;
}

template <class CLASS> 
io::mmfvector<CLASS>::~mmfvector()
{
	free();
}

template <class CLASS> 
io::mmfvector<CLASS>& io::mmfvector<CLASS>::operator =(const mmfvector<CLASS>& arg)
{
	resize(arg.size());
	for(int i=0; i<(int)size(); i++)
		pData[i] = arg[i];
	return *this;
}

template <class CLASS> 
void io::mmfvector<CLASS>::resize(size_t usize)
{
	if( usize<=0)
	{
		free();
		return;
	}

	CLASS *newData = new CLASS[usize]; 
	memset(newData, 0, sizeof(CLASS) * usize);

	size_t copy_size = std::min( this->size(), usize);
	if( copy_size>0)
		memcpy(newData, pData, sizeof(CLASS) * copy_size );

	free();

	pData = newData;
	this->usize = -(int)usize;

	//free();
	//
	//if( usize>0)
	//{
	//	pData = new CLASS[usize];
	//	this->usize = -(int)usize;
	//}
}
template <class CLASS> 
void io::mmfvector<CLASS>::resize(size_t usize, const CLASS& val)
{
	resize(usize);
	for(int i=0; i<(int)usize; i++)
		pData[i] = val;
}
template <class CLASS> 
void io::mmfvector<CLASS>::assign(const CLASS* beg, const CLASS* end)
{
	resize(end-beg);
	for(int i=0; i<(int)size(); i++)
		pData[i] = beg[i];
}
template <class CLASS> 
void io::mmfvector<CLASS>::assign(const ed::vector<CLASS>& arg)
{
	resize(arg.size());
	for(int i=0; i<(int)size(); i++)
		pData[i] = arg[i];
}
template <class CLASS> 
void io::mmfvector<CLASS>::assign(const io::mmfvector<CLASS>& arg)
{
	resize(arg.size());
	for(int i=0; i<(int)size(); i++)
		pData[i] = arg[i];
}

template <class CLASS> 
void io::mmfvector<CLASS>::append(const CLASS* beg, const CLASS* end)
{
	int count = end-beg;
	int startsize = this->size();
	this->resize( startsize+count);

	for(int i=0; i<count; i++)
	{
		(*this)[i+startsize] = beg[i];
	}
}
template <class CLASS> 
void io::mmfvector<CLASS>::append(const ed::vector<CLASS>& arg)
{
	int count = (int)arg.size();
	int startsize = this->size();
	this->resize( startsize+count);

	for(int i=0; i<count; i++)
	{
		(*this)[i+startsize] = arg[i];
	}
}

template <class CLASS> 
void io::mmfvector<CLASS>::free()
{
	if(pData && this->usize < 0)
	{
		delete[] pData;
	}
	pData = 0;
	usize = 0;
}

template <class CLASS> 
size_t io::mmfvector<CLASS>::size() const
{
	return abs(usize);
}
template <class CLASS> 
bool io::mmfvector<CLASS>::empty() const
{
	return size() == 0;
}

template <class CLASS> 
bool io::mmfvector<CLASS>::operator ==(const io::mmfvector<CLASS>& arg) const
{
	if( size()!=arg.size())
		return false;
	int s = (int)arg.size();
	for(int i=0; i<s; i++)
	{
		if( (*this)[i]!=arg[i])
			return false;
	}
	return true;
}
template <class CLASS> 
bool io::mmfvector<CLASS>::operator !=(const mmfvector<CLASS>& arg) const
{
	return !operator==(arg);
}

#endif /* __EDCORE_MMFSTREAM_H */
