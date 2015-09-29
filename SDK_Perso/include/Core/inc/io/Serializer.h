#ifndef EDSIM_SERIALIZER_H
#define EDSIM_SERIALIZER_H

#include <sys/param.h>
#include <stdint.h>

namespace io
{

class Serializer;

// interface for serializable objects
class Serializable
{
public:
	virtual ~Serializable(){}
	// serialize this
	virtual void serialize(Serializer &serializer) {};
};

class File;

// interface for stream - object for serialization of raw data
class Stream
{
public:
	enum Mode 
	{
		READ = 1, 
		WRITE = 2, 
		READNOMAPPING = 3	// for mmfstream. open existing file for reading and dont map file to memory
	};

	virtual ~Stream(){}

	// get stream mode
	virtual Mode getMode() = 0;

	// stream data
	virtual void stream(void *data, unsigned size) = 0;

	// stream secondary data
	virtual void streamsecondary(void** data, uint32_t size, File*& file, uint64_t& offset);

};

// interface for serializer - object able to stream graphs of Serializable objects
class Serializer : public Stream
{
public:
	virtual ~Serializer(){}
	
	// serialize object pointer
	virtual void serialize(Serializable * &object) = 0;

	// serialize object by value
	virtual void serialize(Serializable &object) = 0;
};

// simpliest implementation of Stream: just count amount of data written
class CountStream : public Stream
{
	unsigned count;
public:
	CountStream() : count(0) {}
	virtual ~CountStream(){}
	
	virtual Mode getMode() {return WRITE;}
	virtual void stream(void *data, unsigned size) {count += size;}

	inline unsigned getCount() {return count;}
};

}

// operators for serialization

template <class T> inline
io::Serializer &operator >>(io::Serializer &ser, T* &object)
{
	io::Serializable* t = object;
	ser.serialize(t);
	object = static_cast<T*>(t);
	return ser;
}

inline io::Serializer &operator >>(io::Serializer &ser, io::Serializable &object)
{
	ser.serialize(object);
	return ser;
}

// stream secondary data (default implementation)
inline void io::Stream::streamsecondary(void** data, uint32_t size, File*& file, uint64_t &offset)
{
	file = NULL;
	offset = 0;

	if (getMode() == READ)
	{
		*data = new uint16_t[size]; // Possible memory leak!
	}
	this->stream(*data, size);
}

// template operator not used, so we get error message for objects with no serialization operator defined
inline io::Stream& operator >>(io::Stream& stream, bool& value) {stream.stream(&value, sizeof(value)); return stream;}
inline io::Stream& operator >>(io::Stream& stream, float& value) {stream.stream(&value, sizeof(value)); return stream;}
inline io::Stream& operator >>(io::Stream& stream, double& value) {stream.stream(&value, sizeof(value)); return stream;}
inline io::Stream& operator >>(io::Stream& stream, char& value) {stream.stream(&value, sizeof(value)); return stream;}
inline io::Stream& operator >>(io::Stream& stream, signed char& value) {stream.stream(&value, sizeof(value)); return stream;}
inline io::Stream& operator >>(io::Stream& stream, unsigned char& value) {stream.stream(&value, sizeof(value)); return stream;}

// define endian-correct streaming operators
#if __BYTE_ORDER == __LITTLE_ENDIAN
	#define INT_STREAM_OP(type) \
		inline io::Stream& operator >>(io::Stream& stream, type& value)	\
		{stream.stream(&value, sizeof(type)); return stream;}
#else
	#define INT_STREAM_OP(type) \
		inline io::Stream& operator >>(io::Stream& stream, type& value) \
		{	\
			for(int i=sizeof(type)-1; i>=0; --i)	\
				stream.stream((char *)&value+i, 1);	\
			return stream;	\
		}
#endif

INT_STREAM_OP(int16_t)
INT_STREAM_OP(uint16_t)
INT_STREAM_OP(int32_t)
INT_STREAM_OP(uint32_t)
INT_STREAM_OP(int64_t)
INT_STREAM_OP(uint64_t)

// On some platforms size_t is defined like "long unsigned int" which is not equal to uint16_t - "short unsigned int",
// nor to uint32_t - "unsigned int", not to uint64_t - "long long unsigned int"
//INT_STREAM_OP(size_t)

#undef INT_STREAM_OP

#endif // EDSIM_SERIALIZER
