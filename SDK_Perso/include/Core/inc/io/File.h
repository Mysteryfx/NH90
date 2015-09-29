#ifndef EDSIM_FILE_H
#define EDSIM_FILE_H

#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <ed/string.h>

#include "config.h"
#include "../Mem/Shared.h"
#include "Serializer.h"

#ifndef ED_CORE_EXPORT
	#define ED_CORE_EXPORT ED_DLL_EXPORT
#endif // ED_CORE_EXPORT

#ifndef PATH_MAX
#define PATH_MAX _MAX_PATH
#endif

namespace io
{

class MMF;

// file random access interface
// file objects are refCounted & hashed, so never delete files directly: use ed::Ptr<ed::File> instead
class File : public Mem::Shared{
public:
	// file access mode
	enum Mode
	{
		READ = 1,	// open existing file for reading
		WRITE = 2,	// create file anew, with read & write access
		MODIFY = 3	// open existing file if it exists for reading & writing
	};

	typedef uint64_t off_t;

	// get file name
	virtual const char *getName() = 0;

	// get access mode for this file
	virtual Mode getMode() = 0;

	// read/write data
	virtual void buffer(off_t start, size_t size, void *buffer, Stream::Mode mode) = 0;

	// reserve data block
	// if(mode == MODIFY), initially whole file automatically reserved
	virtual off_t reserve(size_t size) = 0;

	// map file to memory
	// size==0 means map from offset to end
	virtual MMF* mapToMemory(off_t offset, size_t size=0) = 0;

	// close file (destroy file object)
	virtual ~File() {};

	// get file object for real file
	ED_CORE_EXPORT static File* open(const char* name, Mode mode);
	ED_CORE_EXPORT static File* open(const ed::string& name, Mode mode);

	/// Returns true if file exists and readable.
	/// \param name name of file.
	ED_CORE_EXPORT static bool isExists(const char *name);
	ED_CORE_EXPORT static bool isExists(const ed::string& name);

	/// Returns statistic
	/// \param buffer and buflen 
	ED_CORE_EXPORT static bool getStatistic(char *buffer, int buflen);

	// some inline functions for convinience
	inline void bufferInc(off_t &pos, size_t size, void *buffer, Stream::Mode mode)
		{this->buffer(pos, size, buffer, mode); pos += size;}

	template <typename T> inline void bufferInc(off_t &pos, T &value, Stream::Mode mode)
		{bufferInc(pos, sizeof(T), &value, mode);}

	template <typename T> inline void write(off_t &pos, const T &value)
		{bufferInc(pos, const_cast<T &>(value), Stream::WRITE);}

	template <typename T> inline void read(off_t &pos, T &value)
		{bufferInc(pos, value, Stream::READ);}
};

// memory mapped file access interface
// look at File::maptomemory()
class MMF : public Mem::Shared
{
public:
	// get address by offset from mmf begin
	virtual void* getAddressByOffset(File::off_t ) const=0;

	// close file (destroy file object)
	virtual ~MMF() {};
};

// implementation of a stream interface for the file object
class FileStream : public Stream
{
public:
	static const unsigned BUF_SIZE = 2048;

	// open stream from/to given file, use specified size of buffer (no more then BUF_SIZE)
	ED_CORE_EXPORT FileStream();
	ED_CORE_EXPORT FileStream(const char *filename, Mode mode, unsigned useBufSize = BUF_SIZE);
	ED_CORE_EXPORT FileStream(File *file, Mode mode, File::off_t offset = 0, unsigned useBufSize = BUF_SIZE);
	ED_CORE_EXPORT FileStream(const FileStream& fs);
	ED_CORE_EXPORT virtual ~FileStream();

	ED_CORE_EXPORT bool Open( const char *filename, Mode mode, unsigned useBufSize = BUF_SIZE);
	ED_CORE_EXPORT bool Close();

	ED_CORE_EXPORT virtual Mode getMode();
	ED_CORE_EXPORT virtual void stream(void *data, unsigned size);

	// flush everything written into the stream or reset read buffer
	ED_CORE_EXPORT void flush();

	inline File* getFile() { return _file.get(); }

	inline void seek(File::off_t offset) { this->flush(); this->_offset = offset; }
	inline File::off_t tell() { this->flush(); return this->_offset; }

	operator bool() const { return _file.get() != NULL; }

protected:
	Mem::Ptr<File> _file;
	File::off_t _offset;
	Mode _mode;

	char* _buf;
	unsigned _bufSize, _bufRemains;
};

} // namespace io

#endif /* EDSIM_FILE_H */
