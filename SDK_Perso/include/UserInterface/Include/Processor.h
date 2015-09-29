#pragma once

#include <ed/string.h>
#include "ExpImpSpec.h"
#include "Common.h"
#include "Common/System.h"

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR Processor: public Common::Unknown<Common::Identifiable>, public System
{

#define ABORT() initializedStatus = false; return false;

public:
    Processor();
    virtual ~Processor();
    virtual void        setIdentifier(const Identifier &newIdentifier);
	virtual bool        initialize();
	virtual bool        uninitialize();
    virtual void        log(const ed::string &);
};

typedef Common::SharedPtr<Processor> ProcessorPtr;
