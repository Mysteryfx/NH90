#ifndef ILIVERY_MANAGER_H
#define ILIVERY_MANAGER_H

#include "Resourcer/TypedResourceManager.h"

class ILivery;

static const unsigned int INVALID_LIVERY_HANDLE=~0;

/// Interface for livery/skin manager.
class ILiveryManager : public TypedResourceManager{
public:
	virtual ~ILiveryManager(){}

	/// Returns handle of livery or INVALID_LIVERY_HANDLE if there is no such livery.
	/// \param model string representing type of mode is case insensitive.
	/// \param liveryName name of livery.
	/// \param silent     do not print error message
	virtual unsigned int getLiveryHandle(const char* model,const char* livery,bool silent = false)=0;

	/// Creates livery from list of valid liveries handles. All elements equal to INVALID_LIVERY_HANDLE will be skipped.
	/// Returns livery handle or INVALID_LIVERY_HANDLE if list is empty.
	virtual unsigned int createCompositeLivery(unsigned int *liveryHandles, unsigned int liveryHandlesSize) = 0;
	
	/// Creates livery from list of valid liveries handles. List of handles must be ended by INVALID_LIVERY_HANDLE.
	/// Returns livery handle or INVALID_LIVERY_HANDLE if list is empty.
	inline unsigned int createCompositeLivery(unsigned int *liveryHandles){
		unsigned int n = 0;
		for(auto i = liveryHandles; (*i) != INVALID_LIVERY_HANDLE; ++i){
			++n;
		}
		return createCompositeLivery(liveryHandles, n);
	}

	/// Returns livery corresponding to given handle. May return NULL if \param liveryHandle is invalid.
	virtual ILivery* getLivery(unsigned int liveryHandle) = 0;

	// from TypedResourceManager
	/// This method isn't supported.
	virtual Resource *CreateByName(ResourceType type, const ed::string& name){return NULL;}
};

#endif
