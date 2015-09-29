#ifndef __CommonSerializer_h__
#define __CommonSerializer_h__

#include "Serializer/Serializer.h"
#include <ed/string.h>
#include <ed/map.h>
#include "Factory/Factory.h"
#include "Serializer/Record.h"
#include "Serializer/Serializable.h"

namespace Common {;

class CommonSerializer : public Serializer
{
public:
    enum Mode
    {
        READ,   // read only
        WRITE   // write only
    };

	Mode GetMode() const { return mode; }

    virtual ~CommonSerializer();
    virtual bool            isLoading() const { return mode == READ; }
    virtual void            persist(Serializable *&);

    void        setFactory(Factory *newFactory) { factory = newFactory; }
    Factory *   getFactory() const { return factory; }

protected:
    CommonSerializer();

	void SetMode(Mode m) { mode = m; }

	void reset()
	{
		id_count = 0;
		records.clear();
		persistables.clear();
	}

private:
    Mode mode;

    IFactoryPtr             factory;
    typedef ed::map<Record, ISerializablePtr >  Records;
    typedef ed::map<ISerializablePtr, Record>   Persistables;

    Records                 records;
    Persistables            persistables;

	unsigned id_count;

    /** The allocate function generates new free Address */
    ed::string     allocate(const Identifier &);

};

} // namespace Common;

#endif
