#ifndef _BIN_RENDER_INFO_H_
#define _BIN_RENDER_INFO_H_

#include <ed/string.h>

struct binRenderInfo
{
	binRenderInfo():iPortH2IG(0)
		, iPortIG2H(0)
		, iModelID(0)
		, bMaster(false)
		, bRecord(false)
	{};

	int iPortH2IG;
	int iPortIG2H;
	int iModelID;
	bool bMaster;
	bool bRecord;
	ed::string sRenderIP;
};

#endif	//_BIN_RENDER_INFO_H_
