namespace model{

inline bool checkTriangle(const osg::Vec3f &p0, const osg::Vec3f &p1, const osg::Vec3f &p2, const osg::Vec3f &a, const osg::Vec3f& b, float &t, osg::Vec3f &n)
{
	osg::Vec3f u(p1-p0);
	osg::Vec3f v(p2-p0);

	osg::Vec3f nl(u ^ v);

	float an = nl * (a - p0);
	float bn = nl * (b - p0);
	if((an * bn) >= -1.0e-6f) return false;

	float tt = an / (an-bn);
	if(tt >= t) return false;

	osg::Vec3f p(a + (b - a) * tt);

	osg::Vec3f w(p-p0);

	float uu = u*u;
	float uv = u*v;
	float vv = v*v;
	float wu = w*u;
	float wv = w*v;

	float d = (uv*uv - uu*vv);
	float x = (uv*wv - vv*wu) / d;
	float y = (uv*wu - uu*wv) / d;

	if((x<0.0f) || (y<0.0f) || (x+y>1.0f)) return false;

	t = tt;
	n = nl;
	n.normalize();
	return true;
}
}

template <typename _T> bool model::ICollisionShell::testTriangles(const osg::Vec3f &a, const osg::Vec3f& b, float &t, osg::Vec3f &n)const
{
	const _T *ind=(const _T*)getIndices();
	const float *verts = getVertices();

	t = 2.0f;

	osg::Vec3f p0, p1, p2;
	unsigned int nTriangles = getNumTriangles();
	for(unsigned int i = 0; i < nTriangles; ++i){
		unsigned int tmp = ind[i * 3] * getVertexSize();
		p0.set(verts[tmp], verts[tmp + 1], verts[tmp + 2]);
		tmp = ind[i * 3 + 1] * getVertexSize();
		p1.set(verts[tmp], verts[tmp + 1], verts[tmp + 2]);
		tmp = ind[i * 3 + 2] * getVertexSize();
		p2.set(verts[tmp], verts[tmp + 1], verts[tmp + 2]);

		checkTriangle(p0, p1, p2, a, b, t, n);
	}
	
	return (t > 1.0f) ? false : true;
}

inline bool model::ICollisionShell::testWithSegment(const osg::Vec3f &a, const osg::Vec3f &b, float &t, osg::Vec3f &n)const
{
	// проверить бокс
	if(!ed::boxSegmentIntersect(getBoundingBox(), a, b)) return NULL;
	
	switch(getIndexSize()){
		case 1:
			return testTriangles<uint8_t>(a, b, t, n);
		case 2:
			return testTriangles<uint16_t>(a, b, t, n);
		case 4:
			return testTriangles<uint32_t>(a, b, t, n);
		default:
			return false;
	}
	return false;
}
