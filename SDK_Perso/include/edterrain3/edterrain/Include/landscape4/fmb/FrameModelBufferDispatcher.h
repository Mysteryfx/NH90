#ifndef FrameModelBufferDispatcher_H
#define FrameModelBufferDispatcher_H
#include "../../landscape3_config.h"

#include "../IFrameModelBuffer.h"
#include "../MmfFrameModelBuffer.h"

namespace fmb
{

class EDTERRAIN_API FrameModelBufferDispatcher
{
	MmfFrameModelBuffer buffer;
public:

	FrameModelBufferDispatcher();
	virtual ~FrameModelBufferDispatcher();

	bool open(const char* name, uint32_t size);
	void close();

public:
	IFrameModelBuffer* lock();
	void unlock();

	int current();

	void swap();
};

}

#endif
