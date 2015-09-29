#ifndef __FactoryManager_h__
#define __FactoryManager_h__

#include <ed/vector.h>
#include "Factory.h"

namespace Common {

class FactoryManager: public Factory
{
protected:
	struct	Record
	{
		Identifier	        identifier;
		IFactoryPtr         factory;
	};
	typedef ed::vector<Record>			Records;
	Records								records;
   	typedef ed::vector<IFactoryPtr>    Factories;
	Factories                           factories;

public:
    using Factory::createInstance;
    ED_CORE_EXTERN virtual Identifiable *              createInstance(const Identifier &);
    ED_CORE_EXTERN virtual void                        destroyInstance(Identifiable *);

    ED_CORE_EXTERN void addFactory(Factory *f);
    ED_CORE_EXTERN void removeFactory(Factory *f);
};

extern ED_CORE_EXTERN FactoryManager *globalFactoryManager;
} // namespace Common

#endif
