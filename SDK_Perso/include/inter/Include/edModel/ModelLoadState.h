#ifndef ED_MODEL_IMODEL_LOAD_STATE_H
#define ED_MODEL_IMODEL_LOAD_STATE_H

namespace model
{
enum LoadState {
	FAILED_TO_LOAD = 0,
	NOT_LOADED,
	LOADING,
	LOGIC_LOADED,
	LOADED
};

}
#endif
