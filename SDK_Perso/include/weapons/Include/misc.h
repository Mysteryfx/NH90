#ifndef __MISC_H__
#define __MISC_H__

inline dPosition cPosition2dPosition(const cPosition& pos)
{
	dPosition res;

	res.x = pos.x;
	res.y = pos.y;
	res.z = pos.z;
	res.p = pos.p;

	return res;
}

inline cPosition dPosition2cPosition(const dPosition& pos)
{
	cPosition res;

	res.x = pos.x;
	res.y = pos.y;
	res.z = pos.z;
	res.p = pos.p;

	return res;
}

#endif