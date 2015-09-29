#ifndef ED_MODEL_IMODEL_MANAGER_H
#define ED_MODEL_IMODEL_MANAGER_H

namespace model
{

class IModel;

/// Model manager.
class IModelManager
{
protected:
	virtual ~IModelManager() {}
public:
	/// Stored already loaded RootNodes in hash, so you never load one model twice.
	/// This method'll never return NULL. If there is no such model on disk getLoadState returns FAILED_TO_LOAD.
	/// You should set model name without extension.
	virtual IModel* open(const char* name)=0;
};
}
#endif
