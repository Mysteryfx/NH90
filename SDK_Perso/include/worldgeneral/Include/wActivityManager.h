//                    file wActivityManager.h
#ifndef __wActivityManager_h__
#define __wActivityManager_h__

#include "WorldGeneral.h"
#include "wActivity.h"
#include <ed/list.h>


class WORLDGENERAL_API wActivityManager
{
public:
	wActivityManager() {}
	~wActivityManager() { clear(); }

	void	addActivity(wtPointer activity);
	void	removeActivity(wtPointer activity);

	void	clear();

protected:

	typedef ed::list<wtPointer> ActivityList;
	ActivityList m_activities;
};


#endif
