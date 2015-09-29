#ifndef __SIMULATION_BLOCK_FACTORY_H__
#define __SIMULATION_BLOCK_FACTORY_H__

#include "Meta/abstract_factory.h"
#include "simulation_block.h"
#include "simulation_exception.h"

#include <ed/vector.h>


class BLOCKSIM_API wSimulationBlockFactory
{
protected:
    struct error_policy
    {
        static void duplicateClass(const ed::string& str)
        {
            throw wSimulationException("Duplicate simulation block: " + str);
        }

        static void noClass(const ed::string& str)
        {
            throw wSimulationException("Block " + str + " not found");
        }
    };

protected:
    typedef CoreMeta::AbstractFactory<ed::string, wSimulationBlockDescriptor*, TYPE_LIST0(), error_policy> factory_type;

public:
	static wSimulationBlockFactory* instance();

    void register_descriptor(const ed::string& name, const factory_type::functor_type& functor, const ed::string& description);
	wSimulationBlockDescriptor* generate(const ed::string& name) { return factory_.generate(name); }

	ed::vector<std::pair<ed::string, ed::string> > getKeysDescriptions() const;

protected:
    factory_type factory_;

	// отображение ключ -> описание (для редактора)
	// кривовато, конечно
    ed::vector<std::pair<ed::string, ed::string> > descriptions_;
};

#endif