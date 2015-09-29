/**
 * @file Common/Bufferizer.h
 * @author Dmitry S. Baikov
 */
#ifndef COMMON_BUFFERIZER_H
#define COMMON_BUFFERIZER_H


#include "./Serializer.h"
#include "./Serializable.h"


namespace Common {


class ED_CORE_EXTERN Bufferizer : public Serializer {
public:
		// output bufferizer
		Bufferizer();

		// input bufferizer
		Bufferizer(const void* dataPtr, size_t dataSize);

		~Bufferizer();

	void setBuffer(const void* dataPtr, size_t dataSize);
	void getBuffer(const void* &dataPtr, size_t &dataSize) const;

	virtual bool isLoading() const;

	// has the last operation succedded
	operator bool () const;

	virtual void    persist(Serializable *&) { throw "Not implemented!"; }
    virtual void    serialize(void *data, size_t size);
private:
	class Impl;
	Impl* impl;

}; // class Bufferizer


} // namespace Common

#endif /* COMMON_BUFFERIZER_H */
