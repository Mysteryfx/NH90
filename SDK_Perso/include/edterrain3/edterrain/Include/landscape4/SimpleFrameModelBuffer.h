#ifndef SIMPLE_FRAME_MODEL_BUFFER_H
#define SIMPLE_FRAME_MODEL_BUFFER_H

#include "IFrameModelBuffer.h"

#include <stack>

namespace fmb
{

class SimpleFrameModelBuffer : public IFrameModelBuffer
{
	ed::vector<uint8_t> data;
	uint8_t* writePos;
	
	uint8_t* readPos;
	int currentReadingPacketIndex;

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

public:

	/**
	 * Create simple ed::vector based frame model buffer.
	 * Used mainly for debugging
	 * @param size of buffer in bytes
	 */
	SimpleFrameModelBuffer(uint32_t size);
	virtual ~SimpleFrameModelBuffer();

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
	 * Get current packet
	 */
	virtual Packet get();

	/**
	 * Iterate to next packet
	 * returns true if next packet is avalible
	 */
	virtual bool next();

	/**
	 * Iterate to next packet that is not a child of current one
	 * returns true if next packet is avalible
	 */
	virtual bool skip();
};

SimpleFrameModelBuffer::SimpleFrameModelBuffer(uint32_t size)
{
	this->data.resize(size);
}

SimpleFrameModelBuffer::~SimpleFrameModelBuffer()
{
	//...
}

/**
 * Begin of frame. Called before writing any packets
 */
bool SimpleFrameModelBuffer::beginFrame()
{
	this->writePos = &this->data[0];
	this->readPos  = &this->data[0];
	this->packetCount = 0;
	this->currentReadingPacketIndex = 0;
	return true;
}

/**
 * Begin of new packet. Note that packets can be nested.
 * @param type of packet. NONE, EDM etc.
 * @param version of packet of this type
 */
void SimpleFrameModelBuffer::beginPacket(uint32_t type, uint32_t version)
{
	// Compute size for previous packet if needed
	if (!packetStack.empty())
	{
		uint8_t* packetBeginPos = packetStack.top();
		PacketHeader* header = (PacketHeader*)packetBeginPos;

		if (header->size == 0)
			header->size = this->writePos - packetBeginPos;
	}

	// Start new packet
	packetStack.push(this->writePos);

	PacketHeader header(type, version, this->packetCount);
	this->writePacketData(&header, sizeof(PacketHeader));

	this->packetCount++;
}

/**
 * Write user data to packet
 * @param data pointer to user data
 * @param size int bytes
 */
void SimpleFrameModelBuffer::writePacketData(const void* data, uint32_t size)
{
	memcpy(this->writePos, data, size);
	this->writePos += size;
}

/**
 * End of current packet
 */
void SimpleFrameModelBuffer::endPacket()
{
	uint8_t* packetBeginPos = packetStack.top();

	PacketHeader* header = (PacketHeader*)packetBeginPos;
	if (header->size == 0)
		header->size = this->writePos - packetBeginPos;

	header->childCount = this->packetCount - header->index - 1;

	packetStack.pop();
}

/**
 * After all packets are written we call end frame
 */
void SimpleFrameModelBuffer::endFrame()
{
	//...
}

/**
 * Get current packet
 */
fmb::Packet SimpleFrameModelBuffer::get()
{
	PacketHeader* header = (PacketHeader*)this->readPos;
	
	Packet packet(header->type, header->version, header->childCount, this->readPos + sizeof(PacketHeader));
	return packet;
}

/**
 * Iterate to next packet
 * returns true if next packet is avalible
 */
bool SimpleFrameModelBuffer::next()
{
	PacketHeader* header = (PacketHeader*)this->readPos;

	this->readPos += header->size;
	this->currentReadingPacketIndex++;

	return this->currentReadingPacketIndex < this->packetCount;
}

/**
 * Iterate to next packet that is not a child of current one
 * returns true if next packet is avalible
 */
bool SimpleFrameModelBuffer::skip()
{
	PacketHeader* header = (PacketHeader*)this->readPos;

	this->readPos += header->size;
	this->currentReadingPacketIndex++;

	for (uint32_t i = 0; i < header->childCount; i++)
	{
		PacketHeader* childHeader = (PacketHeader*)this->readPos;

		this->readPos += header->size;
		this->currentReadingPacketIndex++;
	}

	return this->currentReadingPacketIndex < this->packetCount;
}

}

#endif