#ifndef ModelFactory_H
#define ModelFactory_H

#include "Factory\Factory.h"

namespace Graphics
{

using Common::Factory;
using Common::Identifier;
using Common::Identifiable;

// Фабрика узлов модели
class ModelFactory : public Common::Unknown<Factory>
{
public:
	ModelFactory();
	virtual ~ModelFactory();

	virtual Identifiable *createInstance(const Identifier &id);
};

extern ModelFactory *model_factory;

} // namespace Graphics

#endif // ModelFactory_H
