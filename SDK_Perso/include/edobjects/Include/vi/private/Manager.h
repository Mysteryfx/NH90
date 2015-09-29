#ifndef __VI_MANAGER_H
#define __VI_MANAGER_H

namespace vi
{

// Пока здесь
class EDOBJECTS_API Manager
{
	friend int vi::queryObjects( ClipVolume *volume, vi::QueryHandler query, void *data, viFlag flags);

protected:
	virtual int queryObjects( ClipVolume *volume, vi::QueryHandler query, void *data, viFlag flags = viAny)=0;
	
	static Manager* instance;
};

}

#endif
