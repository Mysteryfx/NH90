#ifndef ED_MODEL_ICOLLISION_SHELL_H
#define ED_MODEL_ICOLLISION_SHELL_H

#include <stdint.h>

namespace model
{

/// Common interface for collision shells of model.
class ICollisionShell
{
protected:
	template <typename _T> bool testTriangles(const osg::Vec3f &a, const osg::Vec3f& b, float &t, osg::Vec3f &n)const;
	
	virtual ~ICollisionShell() {}
public:

	/// Returns name of shape.
	virtual const char* getName()const = 0;

	/// Returns index of transform matrix in array from RootNode::getControlMatrices for given collision shell.
	virtual unsigned int getTransformId()const = 0;

	/// Returns the number of vertices of shape.
	virtual unsigned int getNumVertices()const = 0;

	/// Returns pointer on vertices array.
	virtual const float* getVertices()const = 0;

	/// Returns vertex size in floats. Vertex position is 3 first floats.
	virtual unsigned int getVertexSize()const = 0;

	/// Returns if shape has normals (next 3 floats after vertex position).
	virtual bool hasNormals()const = 0;

	/// Returns the number of triangles.
	/// Note: triangles strips and fans aren't supported.
	virtual unsigned int getNumTriangles()const = 0;

	/// Returns size of index, i.e. 1, 2, 4.
	virtual uint8_t getIndexSize()const = 0;

	/// Returns pointer on indices.
	virtual const uint8_t* getIndices()const = 0;

	/// Returns bounding box of collision shell.
	virtual const osg::BoundingBox& getBoundingBox()const = 0;

	/// Tests intersection with segment.
	/// \param t parametric position on segment.
	/// \param n normal in intersection point.
	inline bool testWithSegment(const osg::Vec3f &a, const osg::Vec3f &b, float &t, osg::Vec3f &n)const;
};
}

#include "ICollisionShell.inl"

#endif
