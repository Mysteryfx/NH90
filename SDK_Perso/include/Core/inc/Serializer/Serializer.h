#ifndef __Serializer_h__
#define __Serializer_h__

#include "../_config.h"

class Serializable;

class Serializer
{
public:
	Serializer() {}
	virtual ~Serializer() {}

    virtual bool            isLoading() const = 0;
    virtual void            persist(Serializable *&) = 0;

public:
    virtual void            serialize(void *data, size_t size) = 0;
    template <class T> inline	Serializer &    serialize(T &t) { serialize(&t, sizeof(T)); return *this; }

    virtual Serializer &    operator<<(bool &value) { return serialize(value); }
    virtual Serializer &    operator<<(char &value) { return serialize(value); }
    virtual Serializer &    operator<<(int &value) { return serialize(value); }
    virtual Serializer &    operator<<(short &value) { return serialize(value); }
    virtual Serializer &    operator<<(long &value) { return serialize(value); }
    virtual Serializer &    operator<<(float &value) { return serialize(value); }
    virtual Serializer &    operator<<(double &value) { return serialize(value); }
    virtual Serializer &    operator<<(long double &value) { return serialize(value); }
    virtual Serializer &    operator<<(unsigned char &value) { return *this << (char &)value; }
    virtual Serializer &    operator<<(unsigned short &value) { return *this << (short &)value; }
    virtual Serializer &    operator<<(unsigned long &value) { return *this << (long &)value; }
	virtual Serializer &    operator<<(unsigned int &value) { return *this << (int &)value; }
};

#endif
