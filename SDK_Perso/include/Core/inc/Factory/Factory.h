#ifndef __Factory_h__
#define __Factory_h__

#include "_config.h"
#include "Identifier.h"
#include "Identifiable.h"
#include <ed/list.h>

namespace Common {
/**
 * Class Factory creates and destroyes Identifiable objects.
 * User can request supported Identifiers. If the Factory can
 * operate an interface, it must operate a realization, so, both
 * an interface's Identifier and a realization's Identifier must present
 * in the Identifiers. The Factory guarantees the unique reconstruction, so,
 * if user uses the same incoming Identifier, the outcoming Identifier will be 
 * the same too.
 * @author Alexander Matveev
 */
class Factory : public Unknown<ISharedObject>
{
protected:
    typedef ed::list<Identifier> Identifiers;
    Identifiers                   identifiers;

public:
    ED_CORE_EXTERN virtual ~Factory();
	virtual Identifiable * createInstance(const Identifier &) = 0;
	inline virtual void           destroyInstance(Identifiable *identifiable) { delete identifiable;}

	inline void createInstance(const Identifier &identifier, Identifiable **identifiable)
	{
		*identifiable = createInstance(identifier);
		(*identifiable)->AddRef();
	}

	inline const Identifiers &     requestIdentifiers() const { return identifiers; }
    ED_CORE_EXTERN void                    addIdentifier(const Identifier &identifier);
    ED_CORE_EXTERN void                    removeIdentifier(const Identifier &identifier);
};

typedef SharedPtr<Factory> IFactoryPtr;
} // namespace Common

#endif
