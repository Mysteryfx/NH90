
template <class S>
S& operator >>(S& s, intpoint& v ) 
{
	s >> v.x;
	s >> v.z;
	return s;
}

template <class S>
S& operator >>(S& s, Vector3& v ) 
{
	s >> v.x;
	s >> v.y;
	s >> v.z;
	return s;
}

template <class S>
S& operator >>(S& s, Box& v ) 
{
	s >> v.min;
	s >> v.max;
	return s;
}

// индекс квадрата куда попадает точка. squaresize = 1, 2, 4, 8, 16, ...
inline osg::Vec2i edterrain::getSquareIndex( const osg::Vec3d& pt, int squaresize)
{
	return osg::Vec2i(
		(int)floor( pt.x()/squaresize),
		(int)floor( pt.z()/squaresize));
}

// Баунд квадрата по индексу squaresize = 1, 2, 4, 8, 16, ...
inline osg::BoundingBoxf edterrain::getSquareBound(int x, int z, int squaresize)
{
	return osg::BoundingBoxf(
		x*squaresize, 0, z*squaresize, 
		(x+1)*squaresize, 0, (z+1)*squaresize
		);
}

inline edterrain::RectOfSquare::RectOfSquare()
{
	count = 0;
}
inline void edterrain::RectOfSquare::init(const osg::Vec2i& SW, const osg::Vec2i& NE)
{
	this->SW.x() = std::min(SW.x(), NE.x());
	this->SW.y() = std::min(SW.y(), NE.y());
	this->NE.x() = std::max(SW.x(), NE.x());
	this->NE.y() = std::max(SW.y(), NE.y());
	count = (NE.x()-SW.x()+1)*(NE.y()-SW.y()+1);
}
inline int edterrain::RectOfSquare::size()
{
	return count;
}
inline osg::Vec2i edterrain::RectOfSquare::operator [](int i)
{
	int line = (NE.x()-SW.x()+1);
	int y = i/line;
	int x = i - y*line;
	return osg::Vec2i(SW.x()+x, SW.y()+y);
}
// разбиение квадрата на 4
inline edterrain::RectOfSquare edterrain::subSquares(osg::Vec2i sqind)
{
	osg::Vec2i base(sqind.x()*2, sqind.y()*2);
	RectOfSquare rectofsquare;
	rectofsquare.init( base, base+osg::Vec2i(1, 1));
	return rectofsquare;
}
