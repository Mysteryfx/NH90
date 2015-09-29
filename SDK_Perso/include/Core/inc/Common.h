#ifndef __Common_h__
#define __Common_h__

#include "_config.h"

#include "Factory/FactoryManager.h"
#include "Factory/Registrator.h"
#include "Serializer/Serializer.h"
#include "Serializer/Serializable.h"
#include "Serializer/STLSerializer.h"

extern ED_CORE_EXTERN Common::FactoryManager *registry;

void ED_CORE_EXTERN getRegistry(Common::FactoryManager **);
#define REGISTER_COMMON_FACTORY(factory) REGISTER_FACTORY(getRegistry, factory);
#define REGISTER_COMMON_FACTORY_EX(factory, name) REGISTER_FACTORY_EX(getRegistry, factory, name);

#define SAFETRY
#define SAFECATCH

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) {if (p != NULL) {delete p; p = NULL;}}
#endif

#endif // __Common_h__
