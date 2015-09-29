#ifndef MMF_FRAME_MODEL_BUFFER_H
#define MMF_FRAME_MODEL_BUFFER_H
#include "../landscape3_config.h"

#include "IFrameModelBuffer.h"

#include <stack>
#include <windows.h>
#include <ed/set.h>

namespace fmb
{

class EDTERRAIN_API MmfFrameModelBuffer : public IFrameModelBuffer
{
	HANDLE hMapFile;
	HANDLE hMutex;
	uint8_t* mmfdata;
	uint32_t mmfsize;
	ed::vector<uint8_t> localdata;

	uint8_t* writePos;
	uint8_t* readPos;

	struct PacketHeader
	{
		uint32_t type;
		uint32_t version;
		uint32_t size;
		uint32_t index;
		uint32_t childCount;

		PacketHeader(uint32_t type, uint32_t version, uint32_t index) : type(type), version(version), index(index), size(0), childCount(0) {}
	};

	std::stack<uint8_t*> packetStack;
	int packetCount;

	int currentReadingPacketIndex;

public:

	/**
	 * Create MMF based frame model buffer
	 * @param name of the mmf file
	 * @param size of mmf file in bytes
	 */
	MmfFrameModelBuffer();
	virtual ~MmfFrameModelBuffer();

	bool open(const char* name, uint32_t size);
	void close();

	void copyData(const uint8_t* data, uint32_t size);
	void copyData(const MmfFrameModelBuffer& src, ed::set<int> items);
	/**
	 * Hide current packet
	 */
	void MmfFrameModelBuffer::hidePacket();

public:
	virtual bool lock(int timeoutMS);
	virtual void unlock();

	/**
	 * Begin of frame. Called before writing any packets
	 */
	virtual bool beginFrame();

	/**
	 * Begin of new packet. Note that packets can be nested.
	 * @param type of packet. NONE, EDM etc.
	 * @param version of packet of this type
	 */
	virtual void beginPacket(uint32_t type, uint32_t version);

	/**
	 * Write user data to packet
	 * @param data pointer to user data
	 * @param size int bytes
	 */
	virtual void writePacketData(const void* data, uint32_t size);

	/**
	 * End of current packet
	 */
	virtual void endPacket();
	
	/**
	 * After all packets are written we call end frame
	 */
	virtual void endFrame();

	/**
	 * ...
	 */
	virtual bool beginReadFrame();

	/**
	 * Get current packet
	 */
	virtual Packet get();

	/**
	 * Iterate to next packet
	 */
	virtual bool next();

	/**
	 * Iterate to next packet that is not a child of current one
	 */
	virtual bool skip();

	/**
	 * ...
	 */
	virtual bool endReadFrame();

	/**
	 * Save frame model buffer contents to file
	 * For debug purposes only
	 */
	virtual bool saveToFile(const char* fullpath);
	
	/**
	 * Load frame model buffer contents from file
	 * to local buffer.
	 */
	virtual bool loadFromFile(const char* fullpath);
};

}

#endif
