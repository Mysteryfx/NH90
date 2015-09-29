#ifndef __OSG_CONVERT_UTILS_H__
#define __OSG_CONVERT_UTILS_H__

/// \file Misc functions to convert from lockon math and directx to osg and back.
#include "osg/Matrixd"
#include "osg/Matrixf"
#include "osg/BoundingBox"
#include "position.h"

class Box;

namespace ed{

template<typename _T> void Position3ToMatrix(const Position3 &pos,_T &m)
{
	m(0,0)=(_T::value_type)pos.x.x;
	m(0,1)=(_T::value_type)pos.x.y;
	m(0,2)=(_T::value_type)pos.x.z;
	m(0,3)=(_T::value_type)0.0;

	m(1,0)=(_T::value_type)pos.y.x;
	m(1,1)=(_T::value_type)pos.y.y;
	m(1,2)=(_T::value_type)pos.y.z;
	m(1,3)=(_T::value_type)0.0;
	
	m(2,0)=(_T::value_type)pos.z.x;
	m(2,1)=(_T::value_type)pos.z.y;
	m(2,2)=(_T::value_type)pos.z.z;
	m(2,3)=(_T::value_type)0.0;

	m(3,0)=(_T::value_type)pos.p.x;
	m(3,1)=(_T::value_type)pos.p.y;
	m(3,2)=(_T::value_type)pos.p.z;
	m(3,3)=(_T::value_type)1.0;
}

template<typename _T> _T Position3ToMatrix(const Position3 &pos)
{
	_T m;
	m(0,0)=(_T::value_type)pos.x.x;
	m(0,1)=(_T::value_type)pos.x.y;
	m(0,2)=(_T::value_type)pos.x.z;
	m(0,3)=(_T::value_type)0.0;

	m(1,0)=(_T::value_type)pos.y.x;
	m(1,1)=(_T::value_type)pos.y.y;
	m(1,2)=(_T::value_type)pos.y.z;
	m(1,3)=(_T::value_type)0.0;
	
	m(2,0)=(_T::value_type)pos.z.x;
	m(2,1)=(_T::value_type)pos.z.y;
	m(2,2)=(_T::value_type)pos.z.z;
	m(2,3)=(_T::value_type)0.0;

	m(3,0)=(_T::value_type)pos.p.x;
	m(3,1)=(_T::value_type)pos.p.y;
	m(3,2)=(_T::value_type)pos.p.z;
	m(3,3)=(_T::value_type)1.0;

	return m;
}

template<typename _T> void PositionToViewMatrix(const Position3 &pos,_T &m)
{
	m(0,0)=(_T::value_type)pos.z.x;
	m(1,0)=(_T::value_type)pos.z.y;
	m(2,0)=(_T::value_type)pos.z.z;

	m(0,1)=(_T::value_type)pos.y.x;
	m(1,1)=(_T::value_type)pos.y.y;
	m(2,1)=(_T::value_type)pos.y.z;

	m(0,2)=(_T::value_type)pos.x.x;
	m(1,2)=(_T::value_type)pos.x.y;
	m(2,2)=(_T::value_type)pos.x.z;

	m(0,3)=(_T::value_type)0.0;
	m(1,3)=(_T::value_type)0.0;
	m(2,3)=(_T::value_type)0.0;
	m(3,3)=(_T::value_type)1.0;

	m(3,0)=(_T::value_type)-(pos.p.x*m(0,0)+pos.p.y*m(1,0)+pos.p.z*m(2,0));
	m(3,1)=(_T::value_type)-(pos.p.x*m(0,1)+pos.p.y*m(1,1)+pos.p.z*m(2,1));
	m(3,2)=(_T::value_type)-(pos.p.x*m(0,2)+pos.p.y*m(1,2)+pos.p.z*m(2,2));
}

template<typename _T1, typename _T2> void MatrixToPosition(_T1 &pos,const _T2 &m)
{
	pos.x.x=(_T2::value_type)m(0,0);
	pos.x.y=(_T2::value_type)m(0,1);
	pos.x.z=(_T2::value_type)m(0,2);

	pos.y.x=(_T2::value_type)m(1,0);
	pos.y.y=(_T2::value_type)m(1,1);
	pos.y.z=(_T2::value_type)m(1,2);
	
	pos.z.x=(_T2::value_type)m(2,0);
	pos.z.y=(_T2::value_type)m(2,1);
	pos.z.z=(_T2::value_type)m(2,2);
    
	pos.p.x=(_T2::value_type)m(3,0);
	pos.p.y=(_T2::value_type)m(3,1);
	pos.p.z=(_T2::value_type)m(3,2);
}

void BoundingBoxToBox(Box &box,const osg::BoundingBox &bb);

}

#endif 
