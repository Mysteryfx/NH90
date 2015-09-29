#ifndef _ED_VFS_HPP_
#define _ED_VFS_HPP_

#include "ed_vfs.h"
#include "ed/string.h"
#include "ed/vector.h"

#include <assert.h>

namespace VFS {

enum OpenMode {
    OpenRead,
    OpenWrite,
    OpenAppend
};

class File {
private:
    VFS_File *_f;
public:
	File(): _f(0) {}

	File(const char *filename, OpenMode mode): _f(0)
    {
        open(filename, mode);
    }

    ~File()
    {
		if(_f)
	        VFS_close(_f);
    }

	VFS_File* open(const char *filename, OpenMode mode)
	{
		if(_f) 
			close();

		if (mode == OpenRead)
			_f = VFS_open_read(filename);
		else if (mode == OpenWrite)
			_f = VFS_open_write(filename);
		else if (mode == OpenAppend)
			_f = VFS_open_append(filename);
		return _f;
	}

	operator VFS_File*() const { return _f; }

	bool readall(ed::vector<int8_t> &buf){
		if(!_f){
			buf.clear();
			return false;
		}

		auto size = getsize();
		buf.resize(size);
		if(read(&buf.front(), size) != size){
			buf.clear();
			return false;
		}
		return true;
	}

	ed::vector<int8_t> readall(){
		if(!_f){
			return ed::vector<int8_t>();
		}

		auto size = getsize();
		ed::vector<int8_t> res(size);
		if(read(&res.front(), size) != size){
			res.clear();
		}
		return std::move(res);
	}

    uint64_t read(void *buf, uint64_t len)
    {
        return VFS_read(_f, buf, len);
    }

    uint64_t write(const void *buf, uint64_t len)
    {
        return VFS_write(_f, buf, len);
    }

    uint64_t tell()
    {
        return VFS_tell(_f);
    }

    uint64_t seek(uint64_t pos)
    {
        return VFS_seek(_f, pos);
    }

    uint64_t getsize()
    {
        return VFS_getsize(_f);
    }

	void close()
	{
		VFS_close(_f);
		_f = nullptr;
	}
};

#define _VFS_GEN_IOX(NativeType, ioType, ioTag) \
    inline VFS::File& operator << (VFS::File& f, NativeType c) {\
    VFS_put_##ioTag(f, (ioType)c); \
        return f; } \
    inline VFS::File& operator >> (VFS::File& f, NativeType &c) {\
        *(ioType*)&c = VFS_get_##ioTag (f); \
        return f; }

#define _VFS_GEN_IO(Type, Tag) _VFS_GEN_IOX(Type, Type, Tag)

_VFS_GEN_IOX(char, uint8_t, uint8)

_VFS_GEN_IOX(unsigned char, uint8_t, uint8)
_VFS_GEN_IOX(signed char, int8_t, int8)

_VFS_GEN_IO(uint16_t, uint16)
_VFS_GEN_IO(int16_t, int16)

_VFS_GEN_IO(uint32_t, uint32)
_VFS_GEN_IO(int32_t, int32)

_VFS_GEN_IO(uint64_t, uint64)
_VFS_GEN_IO(int64_t, int64)

_VFS_GEN_IO(float, float)
_VFS_GEN_IO(double, double)

#undef _VFS_GEN_IO
#undef _VFS_GEN_IOX

inline VFS::File& operator << (VFS::File& f, const ed::string &s)
{
    VFS_put_string(f, s.c_str(), s.size());
    return f;
}

inline VFS::File& operator >> (VFS::File& f, ed::string &c)
{
    char buf[4096];
    uint64_t len = VFS_get_string(f, buf, sizeof(buf));
    assert (len < sizeof(buf));
    c = buf;
    return f;
}

};

#endif /* _ED_VFS_HPP_ */
