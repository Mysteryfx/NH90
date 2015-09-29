#pragma once

#include "WorldGeneral.h"
#include "Serializer/Serializer.h"
#include <ed/vector.h>
#include "GraphMath\vector.h"

struct WORLDGENERAL_API C_F_Group
{
	C_F_Group() : Count(0){}

	WORLDGENERAL_API friend Serializer& operator << (Serializer& serializer, C_F_Group& data);

	int     Count;
	int     Count0;
    Vector3 pos;
    Vector3 direction;
};

class WORLDGENERAL_API ChaffFlarePayload
{
public:
	WORLDGENERAL_API friend Serializer& operator << (Serializer& serializer, ChaffFlarePayload& data);

	ed::vector< C_F_Group > chaffs;
	ed::vector< C_F_Group > flares;

	void restore_chaffs()
	{
		ChaffCount = ChaffCount0;
		for (int i = 0; i < chaffs.size(); ++i)
			chaffs[i].Count = chaffs[i].Count0;
	}
	void restore_flares()
	{
		FlareCount = FlareCount0;
		for (int i = 0; i < flares.size(); ++i)
			flares[i].Count = flares[i].Count0;
	}
	int ChaffCount;
	int	FlareCount;

	int ChaffCount0;
	int FlareCount0;
};
