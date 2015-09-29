#ifndef __Record_h__
#define __Record_h__

#include "Factory/Identifier.h"

namespace Common {

struct ED_CORE_EXTERN Record
{
    Identifier  identifier;
    ed::string persistorName;
    ed::string persistableName;
    Record() {}
    Record(const Identifier &newIdentifier, 
        const ed::string &newPersistorName, 
        const ed::string &newPersistableName)
        : identifier(newIdentifier), 
        persistorName(newPersistorName), 
        persistableName(newPersistableName) {}
    bool operator<(const Record &record) const;
};

} // namespace Common

#endif
