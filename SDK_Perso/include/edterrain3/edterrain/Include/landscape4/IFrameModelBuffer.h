#ifndef IFRAME_MODEL_BUFFER_H
#define IFRAME_MODEL_BUFFER_H

#include <stdint.h>

namespace fmb
{

class Packet
{
	uint32_t type;
	uint32_t version;
	uint32_t childCount;
	
	void* data;

public:

	Packet(uint32_t type, uint32_t version, uint32_t childCount, void* data) : type(type), version(version),childCount(childCount), data(data) {}

	inline uint32_t getType() 
	{ 
		return this->type; 
	}

	inline uint32_t getVersion() 
	{
		return this->version;
	}

	inline uint32_t getChildCount() 
	{
		return this->childCount;
	}

	inline void* getData()
	{
		return this->data;
	}

	template <class T> const T* getData()
	{
		if (T::packetType != type)
			return (T*)NULL;

		if (T::packetVersion != version)
			return (T*)NULL;

		return (const T*)data;
	}
};

class IFrameModelBuffer
{
public:

	virtual ~IFrameModelBuffer() {}

	/**
	 * Begin of frame. Called before writing any packets
	 */
	virtual bool beginFrame() = 0;

	template <class T> void writePacket(const T& data)
	{
		this->beginPacket(data.packetType, data.packetVersion);
		this->writePacketData(&data, sizeof(data));
		this->endPacket();
	}
	// записывает пакет, но не завершает
	template <class T> void beginPacket(const T& data)
	{
		this->beginPacket(data.packetType, data.packetVersion);
		this->writePacketData(&data, sizeof(data));
//		this->endPacket();
	}

	/**
	 * Begin of new packet. Note that packets can be nested.
	 * @param type of packet. NONE, EDM etc.
	 * @param version of packet of this type
	 */
	virtual void beginPacket(uint32_t type, uint32_t version) = 0;

	/**
	 * Write user data to packet
	 * @param data pointer to user data
	 * @param size int bytes
	 */
	virtual void writePacketData(const void* data, uint32_t size) = 0;

	/**
	 * End of current packet
	 */
	virtual void endPacket() = 0;
	
	/**
	 * After all packets are written we call end frame
	 */
	virtual void endFrame() = 0;

	/**
	 * ...
	 */
	virtual bool beginReadFrame() = 0;

	/**
	 * Get current packet
	 */
	virtual Packet get() = 0;

	/**
	 * Iterate to next packet
	 */
	virtual bool next() = 0;

	/**
	 * Iterate to next packet that is not a child of current one
	 */
	virtual bool skip() = 0;

	/**
	 * ...
	 */
	virtual bool endReadFrame() = 0;

	/**
	 * Save frame model buffer contents to file
	 * For debug purposes only
	 */
	virtual bool saveToFile(const char* fullpath) = 0;
	
	/**
	 * Load frame model buffer contents from file
	 */
	virtual bool loadFromFile(const char* fullpath) = 0;
};

}

#endif
