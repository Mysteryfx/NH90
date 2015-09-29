
// реальный индекс для зацикленого массива
inline int offshore::cycle(int v, int size)
{
	if(v<0) 
		v += size*( 1-v/size);
	v = v%size;
	return v;
}

template <class S>
S& operator >>(S& s, offshore::TreeNode& v) 
{
	uint32_t version = 4;
	s >> version;
	if(version>=5)
	{
		throw "unsupported format";
	}
	if(version>=0)
	{
		s >> v.type;
		s >> v.index;
		s >> v.nodename;
		s >> v.parent;
		s >> v.childs;
	}
	if(version>=1)
	{
		s >> v.nodetype;
	}
	if(version>=2)
	{
		s >> v.pMaxNode;
	}
	if(version>=3)
	{
		s >> v.params;
	}
	if(version>=4)
	{
		s >> v.pMaxParentNode;
	}
	return s;
}

template <class S>
S& operator >>(S& s, offshore::Transform& v) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=1)
	{
		throw "unsupported format";
	}
	if(version>=0)
	{
		s >> v.treenode;
		s >> v.m;
	}
	return s;
}

template <class S>
S& operator >>(S& s, offshore::Face& v) 
{
	uint32_t version = 1;
	s >> version;
	if(version>=2)
	{
		throw "unsupported format";
	}
	if(version==0)
	{
		if( s.getMode()==io::Stream::READ)
		{
			int16_t v16[3];
			s >> v16[0] >> v16[1] >> v16[2];
			v.v[0] = v16[0];
			v.v[1] = v16[1];
			v.v[2] = v16[2];
		}
	}
	if(version>=1)
	{
		s >> v.v[0] >> v.v[1] >> v.v[2];
	}
	return s;
}

template <class S>
S& operator >>(S& s, offshore::Polygon& v) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=1)
	{
		throw "unsupported format";
	}
	if(version>=0)
	{
		s >> v.indicies;
		s >> v.faces;
	}
	return s;
}

template <class S>
S& operator >>(S& s, offshore::Manifold& v) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=1)
	{
		throw "unsupported format";
	}
	if(version>=0)
	{
		s >> v.xyzTOuv;
	}
	return s;
}

template <class S>
S& operator >>(S& s, offshore::uvSet& v) 
{
	uint32_t version = 2;
	s >> version;
	if(version>=3)
	{
		throw "unsupported format";
	}
	if(version>=0)
	{
		s >> v.name;
		s >> v.stride;
		s >> v.uv;
		s >> v.uvpolygons;
	}
	if(version>=1)
	{
		s >> v.manifolds;
	}
	if(version>=2)
	{
		s >> v.manifoldSourceName;
	}
	return s;
}

template <class S>
S& operator >>(S& s, offshore::Mesh& v) 
{
	uint32_t version = 2;
	s >> version;
	if(version>=3)
	{
		throw "unsupported format";
	}
	if(version>=0)
	{
		s >> v.treenode;
		s >> v.material;
		s >> v.verts;
		s >> v.polygons;
		s >> v.srcvertindicies;
		s >> v.uvsets;
		s >> v.normals;
		s >> v.npolygons;
	}
	if(version >= 1)
	{
		s >> v.smoothgroup;
	}
	if(version >= 2)
	{
		s >> v.vertcolors;
	}
	return s;
}

template <class S>
S& operator >>(S& s, offshore::SkinMesh& v) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=1)
	{
		throw "unsupported format";
	}
	if(version>=0)
	{
		offshore::Mesh* pv = &v;
		operator >>(s, *pv);
		s >> v.joints;
	}
	return s;
}

template <class S>
S& operator >>(S& s, offshore::SkinMesh::Joint& v) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=1)
	{
		throw "unsupported format";
	}
	if(version>=0)
	{
		s >> v.jointname;
		s >> v.pos;
		s >> v.invbindpos;
		s >> v.vertsweights;
	}
	return s;
}

inline osg::Vec3d offshore::Spline::findClosestPoint(const osg::Vec3d& point, PointOnSpline& pointOnSpline, double& distance) const
{
	return this->findClosestPoint(point, pointOnSpline.segment, pointOnSpline.parameter, distance);
}
inline osg::Vec3d offshore::Spline::findClosestPoint2d(const osg::Vec3d& point, PointOnSpline& pointOnSpline, double& distance, PointOnSpline* start, PointOnSpline* end) const
{
	return this->findClosestPoint2d(point, pointOnSpline.segment, pointOnSpline.parameter, distance, start, end);
}

template <class S>
S& operator >>(S& s, offshore::Spline& v) 
{
	uint32_t version = 1;
	s >> version;
	if(version>=2)
	{
		throw "unsupported format";
	}
	if(version>=0)
	{
		s >> v.treenode;
		s >> v.material;
		s >> v.verts;
	}
	if(version>=1)
	{
		s >> v.tangents_forward;
		s >> v.tangents_backward;
	}

	return s;
}

template <class S>
S& operator >>(S& s, offshore::Points& v) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=1)
	{
		throw "unsupported format";
	}
	if(version>=0)
	{
		s >> v.treenode;
		s >> v.material;
		s >> v.verts;
	}
	return s;
}

template <class S>
S& operator >>(S& s, offshore::Error& v) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=1)
	{
		throw "unsupported format";
	}
	if(version>=0)
	{
		s >> v.text;
		s >> v.position;
		s >> v.color;
		s >> v.size;
	}
	return s;
}

template <class S>
S& operator >> (S& s, offshore::Notification& n)
{
	uint32_t version = 0;
	s >> version;
	if (version >= 1)
	{
		throw "unsupported format";
	}
	if (version >= 0)
	{
		s >> n.type;
		s >> n.position;
		s >> n.size;
		s >> n.text;
	}
	return s;
}

template <class S>
S& operator >>(S& s, offshore::Material& v) 
{
	uint32_t version = 2;
	s >> version;
	if(version>=3)
	{
		throw "unsupported format";
	}
	if(version==0)
	{
		// subdiv
		float stochasticXZ=0;
		float stochasticY=0;
		float priority=0;
		float creaseForce=0;
		bool bSmoothCorner=0;
		bool bAddRoadPolygons=0;
		// road&river
		double width=0;			// ширина ленты
		double u_min=0, u_max=0;	// вилка по u
		double v_tile_lenght=0;	// длина текстуры (метров)
		int32_t materialSubId=0;
		ed::string nodename;

		s >> nodename;
		s >> v.semantic;
		s >> v.edmaterial;
		s >> stochasticXZ;
		s >> stochasticY;
		s >> priority;
		s >> creaseForce;
		s >> bSmoothCorner;
		s >> bAddRoadPolygons;
		s >> v.materialId;
		s >> materialSubId;
		s >> v.params;

		// road&river
		s >> width;			// ширина ленты
		s >> u_min>>u_max;	// вилка по u
		s >> v_tile_lenght;	// длина текстуры (метров)
	}
	if(version>=1)
	{
		s >> v.semantic;
		s >> v.edmaterial;
		s >> v.materialId;
		s >> v.params;
	}
	if(version>=2)
	{
		s >> v.pMaxMat;
	}
	return s;
}

template <class S>
S& operator >>(S& s, offshore::AnimationCurve& v) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.startTime;
		s >> v.endTime;
		s >> v.keys;
	}
	if(version>=1)
	{
		throw "unsupported format";
	}
	return s;
}

template <class S>
S& operator >>(S& s, offshore::AnimationCurveMatrixd& v) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.keys;
	}
	if(version>=1)
	{
		throw "unsupported format";
	}
	return s;
}

template <class S>
S& operator >>(S& s, offshore::AnimationClip::AnimationParams& v) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.params;
	}
	if(version>=1)
	{
		throw "unsupported format";
	}
	return s;
}

template <class S>
S& operator >>(S& s, offshore::AnimationClip& v) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.duration;
		s >> v.clipname;
		s >> v.nodes;
	}
	if(version>=1)
	{
		throw "unsupported format";
	}
	return s;
}

template <class S>
S& operator >> (S& s, offshore::Instance& instance)
{
	uint32_t version = 1;
	s >> version;

	if (version >= 2)
	{
		throw "unsupported format";
	}

	if (version >= 1)
	{
		s >> instance.name;
		s >> instance.position;
		s >> instance.size;
		s >> instance.scene;
	}

	return s;
}

template <class S>
S& operator >> (S& s, offshore::Light& light)
{
	uint32_t version = 2;
	s >> version;

	if (version >= 3)
	{
		throw "unsupported format";
	}

	if (version >= 0)
	{
		s >> light.treenode;
		s >> light.type;
		s >> light.omniAttenuation;
	}
	if (version >= 1)
	{
		s >> light.color;
	}

	return s;
}

template <class S>
S& operator >> (S& s, offshore::Camera& camera)
{
	uint32_t version = 1;
	s >> version;

	if (version >= 2)
	{
		throw "unsupported format";
	}

	if (version >= 0)
	{
		s >> camera.treenode;
		s >> camera.type;
		s >> camera.fovType;
		s >> camera.fov;
		s >> camera.nearClipPlane;
		s >> camera.farClipPlane;
		s >> camera.isViewport;
	}

	if (version >= 1)
	{
		s >> camera.aspect;
	}

	return s;
}

template <class S>
S& operator >> (S& s, offshore::Scene& scene) 
{
	uint32_t version = 7;
	s >> version;

	if (version >= 8)
	{
		throw "unsupported format";
	}

	if (version >= 0)
	{
		s >> scene.allnodes;
		s >> scene.rootnode;
		s >> scene.transforms;
		s >> scene.meshes;
		s >> scene.splines;
		s >> scene.points;
		s >> scene.skinmeshes;
		s >> scene.materials;
	}
	if (version >= 1)
	{
		s >> scene.errors;
	}
	if (version >= 2)
	{
		s >> scene.params;
	}
	if (version >= 3)
	{
		s >> scene.clips;
	}
	if (version >= 4)
	{
		s >> scene.notifications;
	}
	if (version >= 5)
	{
		s >> scene.instances;
	}
	if (version >= 6)
	{
		s >> scene.lights;
	}
	if (version >= 7)
	{
		s >> scene.cameras;
		s >> scene.activeViewport;
	}

	return s;
}

template <class S>
S& operator >>(S& s, offshore::SceletonAnimation::Joint& v) 
{
	uint32_t version = 3;
	s >> version;
	if (version >= 4)
	{
		throw "unsupported format";
	}
	if(version>=3)
	{
		s >> v.name;
		s >> v.exclusivetransforms;
		s >> v.inclusivetransforms;
	}
	return s;
}

inline int offshore::uvSet::faceverticescount() const
{
	int facevert = 0;
	for( int p=0; p<(int)this->uvpolygons.size(); p++)
	{
		const offshore::Polygon& poly = this->uvpolygons[p];
		facevert += (int)poly.indicies.size();
	}
	return facevert;
}

inline int offshore::Mesh::faceVerticesCount() const
{
	int facevert = 0;
	for( int p=0; p<(int)this->polygons.size(); p++)
	{
		const offshore::Polygon& poly = this->polygons[p];
		facevert += (int)poly.indicies.size();
	}
	return facevert;
}


inline offshore::Camera::Camera():
	treenode(-1),
	type(),
	fovType(offshore::Camera::WIDTH),
	fov(60.0),
	nearClipPlane(0.0001),
	farClipPlane(1e10),
	isViewport(false)
{
}

// mesh.queryVerts([&](int v, osg::Vec3d& pt)
template <typename Function> void offshore::Spline::queryVerts(const Function& f) 
{
	for (size_t i = 0; i < this->verts.size(); i++)
	{
		f(i, this->verts[i]);
	}
}
// mesh.querySegments([&](int v, osg::Vec3d& pt1, osg::Vec3d& pt2)
template <typename Function> void offshore::Spline::querySegments(const Function& f) 
{
	for (size_t i = 0; i < this->verts.size()-1; i++)
	{
		f(i, this->verts[i], this->verts[i+1]);
	}
}


// uvSet.queryVerts([&](int v, double* val)
// uvSet.queryPolygons([&](int p, offshore::Polygon& poly)
// uvSet.queryFaces([&](int p, int f, offshore::Face& face)
// uvSet.queryPolygonVerts([&](int p, int pv, double* val)
template <typename Function> void offshore::uvSet::queryVerts(const Function& f) 
{
	for (size_t i = 0; i < this->size(); i++)
	{
		f(i, this->getUV(i));
	}
}
template <typename Function> void offshore::uvSet::queryPolygons(const Function& f) 
{
	for (size_t p = 0; p < uvpolygons.size(); p++)
	{
		f(p, uvpolygons[p]);
	}
}
template <typename Function> void offshore::uvSet::queryFaces(const Function& f) 
{
	for (size_t p = 0; p < uvpolygons.size(); p++)
	{
		offshore::Polygon& poly = uvpolygons[p];
		for( int face=0; face<poly.faces.size(); face++)
		{
			f(p, face, poly.faces[face]);
		}
	}
}
template <typename Function> void offshore::uvSet::queryPolygonVerts(const Function& f) 
{
	for (size_t p = 0; p < uvpolygons.size(); p++)
	{
		offshore::Polygon& poly = uvpolygons[p];
		for( int pv=0; pv<poly.indicies.size(); pv++)
		{
			f(p, pv, this->getUV(poly.indicies[pv]));
		}
	}
}




// mesh.queryVerts([&](int v, osg::Vec3d& pt)
template <typename Function> void offshore::Mesh::queryVerts(const Function& f) 
{
	for (size_t i = 0; i < verts.size(); i++)
	{
		f(i, verts[i]);
	}
}
// mesh.queryPolygons([&](int p, offshore::Polygon& poly)
template <typename Function> void offshore::Mesh::queryPolygons(const Function& f) 
{
	for (size_t p = 0; p < polygons.size(); p++)
	{
		f(p, polygons[p]);
	}
}
// mesh.queryPolygons([&](int p, const offshore::Polygon& poly)
template <typename Function> void offshore::Mesh::queryPolygons(const Function& f) const
{
	for (size_t p = 0; p < polygons.size(); p++)
	{
		f(p, polygons[p]);
	}
}
// mesh.queryFaces([&](int p, int f, offshore::Face& face)
// face.v - индексы в массиве mesh.verts (пересчитаны)
template <typename Function> void offshore::Mesh::queryFaces(const Function& f) 
{
	for (size_t p = 0; p < polygons.size(); p++)
	{
		offshore::Polygon& poly = polygons[p];
		for( int face=0; face<poly.faces.size(); face++)
		{
			offshore::Face mface = poly.faces[face];
			mface.v[0] = poly.indicies[mface.v[0]];
			mface.v[1] = poly.indicies[mface.v[1]];
			mface.v[2] = poly.indicies[mface.v[2]];
			f(p, face, mface);
		}
	}
}
// mesh.queryPolygonVerts([&](int p, int pv, osg::Vec3d& pt)
template <typename Function> void offshore::Mesh::queryPolygonVerts(const Function& f) 
{
	for (size_t p = 0; p < polygons.size(); p++)
	{
		offshore::Polygon& poly = polygons[p];
		for( int pv=0; pv<poly.indicies.size(); pv++)
		{
			f(p, pv, verts[poly.indicies[pv]]);
		}
	}
}

// mesh.queryPolygonEdges([&](int p, int pv1, int pv2, osg::Vec3d& pt1, osg::Vec3d& pt2)
template <typename Function> void offshore::Mesh::queryPolygonEdges(const Function& f) 
{
	for (size_t p = 0; p < polygons.size(); p++)
	{
		offshore::Polygon& poly = polygons[p];
		for( int pv1=0; pv1<poly.indicies.size(); pv1++)
		{
			int pv2 = pv1+1;
			if(pv2>=poly.indicies.size())
				pv2 = 0;
			f(p, pv1, pv2, verts[poly.indicies[pv1]], verts[poly.indicies[pv2]]);
		}
	}
}

// scene.queryShapes([&](const osg::Matrixd& transform, int nodeIndex, offshore::TreeNode* node)
template <typename Function> void offshore::Scene::queryShapes(const Function& f)
{
	ed::vector<std::pair<osg::Matrixd, int32_t> > shapes;
	this->queryShapes(shapes);

	for (size_t i = 0; i < shapes.size(); i++)
	{
		offshore::TreeNode* node = this->Node(shapes[i].second);

		f(shapes[i].first, shapes[i].second, node);
	}
}
// scene.queryShapes([&](const osg::Matrixd& transform, int nodeIndex, offshore::TreeNode* node)
template <typename Function> void offshore::Scene::queryShapes(int rootnode, const Function& f)
{
	ed::vector<std::pair<osg::Matrixd, int32_t> > shapes;
	this->queryShapes(shapes, rootnode);

	for (size_t i = 0; i < shapes.size(); i++)
	{
		offshore::TreeNode* node = this->Node(shapes[i].second);

		f(shapes[i].first, shapes[i].second, node);
	}
}

// scene.queryMeshes([&](const osg::Matrixd& transform, int nodeIndex, offshore::TreeNode* node, offshore::Mesh& mesh)
template <typename Function> void offshore::Scene::queryMeshes(const Function& f)
{
	ed::vector<std::pair<osg::Matrixd, int32_t> > shapes;
	this->queryMeshes(shapes);

	for (size_t i = 0; i < shapes.size(); i++)
	{
		offshore::TreeNode* node = this->Node(shapes[i].second);

		offshore::Mesh& mesh = this->meshes[node->index];
		f(shapes[i].first, shapes[i].second, node, mesh);
	}
}
// scene.queryMeshes([&](const osg::Matrixd& transform, int nodeIndex, offshore::TreeNode* node, offshore::Mesh& mesh)
template <typename Function> void offshore::Scene::queryMeshes(int rootnode, const Function& f)
{
	ed::vector<std::pair<osg::Matrixd, int32_t> > shapes;
	this->queryMeshes(shapes, rootnode);

	for (size_t i = 0; i < shapes.size(); i++)
	{
		offshore::TreeNode* node = this->Node(shapes[i].second);

		offshore::Mesh& mesh = this->meshes[node->index];
		f(shapes[i].first, shapes[i].second, node, mesh);
	}
}

// scene.querySplines([&](const osg::Matrixd& transform, int nodeIndex, offshore::TreeNode* node, offshore::Spline& spline)
template <typename Function> void offshore::Scene::querySplines(const Function& f)
{
	ed::vector<std::pair<osg::Matrixd, int32_t> > shapes;
	this->querySplines(shapes);

	for (size_t i = 0; i < shapes.size(); i++)
	{
		offshore::TreeNode* node = this->Node(shapes[i].second);

		offshore::Spline& spline = this->splines[node->index];
		f(shapes[i].first, shapes[i].second, node, spline);
	}
}
// scene.querySplines([&](const osg::Matrixd& transform, int nodeIndex, offshore::TreeNode* node, offshore::Spline& spline)
template <typename Function> void offshore::Scene::querySplines(int rootnode, const Function& f)
{
	ed::vector<std::pair<osg::Matrixd, int32_t> > shapes;
	this->querySplines(shapes, rootnode);

	for (size_t i = 0; i < shapes.size(); i++)
	{
		offshore::TreeNode* node = this->Node(shapes[i].second);

		offshore::Spline& spline = this->splines[node->index];
		f(shapes[i].first, shapes[i].second, node, spline);
	}
}

// [&](const osg::Matrixd& m, int nodeindex, offshore::TreeNode* node)
template <typename Function> void offshore::Scene::queryTransforms(int rootnode, const Function& f) 
{
	ed::vector<std::pair<osg::Matrixd, int32_t> > shapes;
	this->queryTransforms(shapes, rootnode);

	for (size_t i = 0; i < shapes.size(); i++)
	{
		offshore::TreeNode* node = this->Node(shapes[i].second);
		if( node->type!=offshore::TRANSFORM)
			continue;
		offshore::Transform& t = this->transforms[node->index];
		osg::Matrixd m = t.m * shapes[i].first;
		f(m, shapes[i].second, node);
	}
}

// scene.traverseChilds( rootnode, [&](int nodeindex, offshore::TreeNode* node)
template <typename Function> void offshore::Scene::traverseChilds(int rootnode, const Function& f)
{
	offshore::TreeNode* node = this->Node(rootnode);
	if( !node) return;

	for (auto it=node->childs.begin(); it != node->childs.end(); it++)
	{
		int nodeindex = *it;
		offshore::TreeNode* childnode = this->Node(nodeindex);
		f(nodeindex, childnode);
	}
}


inline bool offshore::Spline::valid() const
{
	bool bInvalid = false;
	for(int v=0; v<this->verts.size(); v++)
		bInvalid |= !this->verts[v].valid();
	return !bInvalid;
}

