#ifndef __EDCORE_MEMSTREAM_H
#define __EDCORE_MEMSTREAM_H

#include "Serializer.h"

namespace io
{

class MemStream : public Stream
{
    Mode _mode;
    int8_t *_buff;
    int _count;
public:
    MemStream(Mode mode, void *buff) : _mode(mode), _buff((int8_t*)buff), _count(0) {}
	~MemStream(){}

	// get stream mode
    Mode getMode() {return _mode;}

	// stream data
	void stream(void *data, unsigned size)
    {
        if(_mode == READ)
            memcpy(data, _buff + _count, size);
        else
            memcpy(_buff + _count, data, size);
        _count += size;
    }

    int getCount() {return _count;}
};

} // namespace io

#endif // __EDCORE_MEMSTREAM_H

