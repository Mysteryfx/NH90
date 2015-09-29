#ifndef inter_edTerrain_TerrainContextCamera_h
#define inter_edTerrain_TerrainContextCamera_h

#include <osg/Polytope>
#include <osg/Matrixd>
#include <osg/Matrixf>
#include <osg/Vec3f>
#include <osg/Vec3d>
#include "osg/MatrixUtils.h"
#include "osg/FrustumUtils.h"
#include "ITerrainGraphicsConstants.h"
#include "IDump.h"

namespace render
{


class Camera
{
public:
	Camera(){};
	static Camera makePerpective(const osg::Matrixf& view, const osg::Matrixf& proj, const osg::Vec3f& origin, Camera* parent=nullptr);
	static Camera makeOrtho(const osg::Matrixd& view, const osg::Matrixd& proj, const osg::Vec3f& origin, Camera* parent=nullptr);
	static Camera makeCustom(const osg::Polytope& polytope, const osg::Vec3f& origin, Camera* parent=nullptr);

	enProjection projectionType() const;

	osg::Matrixd position(enSpace space=SP_WORLDSPACE) const;  // inv(view)
	osg::Matrixd view(enSpace space=SP_WORLDSPACE) const ;     // inv(position)
	osg::Matrixd proj() const ;
	osg::Matrixd proj(double near, double far) const ;

	const osg::Polytope& polytope(enSpace space=SP_WORLDSPACE) const;
	osg::Polytope polytope(double near, double far, enSpace space=SP_WORLDSPACE) const;

	double nearPlane() const;
	double farPlane() const;
	double fovY() const; 
	double aspect() const;

	const osg::Vec3f origin() const;

	// Камера верхнего уровня
	const Camera* parentCamera() const;

	void dump( IDump* pDump) const;
private:
	// view in origin space
	Camera(const osg::Matrixf& view, const osg::Matrixf& proj, const osg::Vec3f& origin, enProjection type, Camera* parent=nullptr);
	Camera(const osg::Polytope& polytope, const osg::Vec3f origin, Camera* parent=nullptr);

	inline osg::Polytope applySpace(const osg::Polytope& polytope, enSpace space=SP_WORLDSPACE) const ;

	//Все матрицы храняться в SP_WORLDSPACEORIGIN
	osg::Matrixd  mView;
	osg::Matrixd  mInvView;
	osg::Matrixd  mProj;
	osg::Vec3f    mOrigin;
	osg::Polytope mPolytope, mPolytopeOrigin, mPolytopeCameraSpace;
	enProjection  mType;

	const Camera* mParentCamera;
};

bool operator ==(const Camera& lhs, const Camera& rhs);
}//namespace render

#include "inl/TerrainContextCamera.inl"

#endif