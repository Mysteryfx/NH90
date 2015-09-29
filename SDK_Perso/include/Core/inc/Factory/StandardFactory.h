#ifndef	__STANDARDFACTORY_H_
#define	__STANDARDFACTORY_H_

#include "Factory.h"

namespace Common {

template <class T>
class StandardFactory: virtual public Factory
{
public:
    StandardFactory()	{ addIdentifier(identify<T>()); }

    virtual Identifiable *  createInstance(const Identifier &identifier)
	{
		Identifiable *identifiable = new T;
		identifiable->setIdentifier(identifier);
        return identifiable;
	}
};

} // namespace Common
#endif	//	__STANDARDFACTORY_H__
