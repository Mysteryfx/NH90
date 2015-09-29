#ifndef __Identifiable_h__
#define __Identifiable_h__

#include "Identifier.h"
#include "SharedObject.h"

namespace Common {
class Identifiable : public ISharedObject
{
public:
    Identifier          identifier;
public:
    virtual ~Identifiable() {}
    virtual void            setIdentifier(const Identifier &newIdentifier) { identifier = newIdentifier; }
    virtual Identifier &    getIdentifier() { return identifier; }
    virtual const Identifier &getIdentifier() const { return identifier; }
};
} // namespace Common

#endif
