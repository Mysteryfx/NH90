#ifndef TRANSPORTABLE_H
#define TRANSPORTABLE_H

#include "Transport.h"
#include "iInternalCargo.h"

class TRANSPORT_API Transportable : public iTransportable
{
    int _size;
	bool _randomTransportable;

public:

    Transportable();
    ~Transportable();

    virtual bool setSize(int size);
	virtual void setRandomTransportable(bool randomTransportable);
    virtual int getSize() const;
    virtual bool isTransportable() const;
	virtual bool isRandomTransportable() const;
};

#endif