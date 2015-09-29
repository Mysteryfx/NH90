#ifndef OSG_MATRIXUTILS_H
#define OSG_MATRIXUTILS_H

#include "ed_cmath.h"
#include <osg/Vec3d>
#include <osg/Matrixf>
#include <osg/Matrixd>
#include <ed/vector.h>

namespace ed
{
/// Calculates normals transform matrix using \param mat as transform matrix.
/// Returns \b false if determinant of matrix \param mat is equal zero,
/// otherwise returns \b true.
template <typename M,typename N> bool calculateNormalMatrix(const M& mat,N& n)
{
	static M m;
	m.orthoNormalize(mat);

	double det= m(0,0)*(m(1,1)*m(2,2)-m(1,2)*m(2,1))+
				m(0,1)*(m(1,2)*m(2,0)-m(1,0)*m(2,2))+
				m(0,2)*(m(1,0)*m(2,1)-m(1,1)*m(2,0));

	if(std::abs(det)<1.0e-5)
	{
		n(0,0)=n(0,1)=n(0,2)=typename N::value_type(0.0);
		n(1,0)=n(1,1)=n(1,2)=typename N::value_type(0.0);
		n(2,0)=n(2,1)=n(2,2)=typename N::value_type(0.0);
		return false;
	}

	n(0,0)=typename N::value_type((m(1,1)*m(2,2)-m(1,2)*m(2,1))/det);
	n(1,0)=typename N::value_type((m(0,2)*m(2,1)-m(0,1)*m(2,2))/det);
	n(2,0)=typename N::value_type((m(0,1)*m(1,2)-m(0,2)*m(1,1))/det);

	n(0,1)=typename N::value_type((m(1,2)*m(2,0)-m(1,0)*m(2,2))/det);
	n(1,1)=typename N::value_type((m(0,0)*m(2,2)-m(0,2)*m(2,0))/det);
	n(2,1)=typename N::value_type((m(0,2)*m(1,0)-m(0,0)*m(1,2))/det);

	n(0,2)=typename N::value_type((m(1,0)*m(2,1)-m(1,1)*m(2,0))/det);
	n(1,2)=typename N::value_type((m(0,1)*m(2,0)-m(0,0)*m(2,1))/det);
	n(2,2)=typename N::value_type((m(0,0)*m(1,1)-m(0,1)*m(1,0))/det);

	return true;
}

/// Returns true if matrix is ident, otherwise returns false.
bool isIdentity(const osg::Matrixf& m);
/// Returns true if matrix is ident, otherwise returns false.
bool isIdentity(const osg::Matrixd& m);

/**
 * makeFromBasis - собрать матрицу из базисных векторов и транслейта
 *
 * @param x y z базисные вектора
 * @param translate транслейт
 */
inline osg::Matrixd makeFromBasis(const osg::Vec3d& x, const osg::Vec3d& y, const osg::Vec3d& z, const osg::Vec3d& translate)
{
	return osg::Matrixd(
		x.x(), x.y(), x.z(), 0, 
		y.x(), y.y(), y.z(), 0, 
		z.x(), z.y(), z.z(), 0, 
		translate.x(), translate.y(), translate.z(), 1
		);
}

//умножает массив точек на матрицу
void calculateTransformPoint(
	ed::vector<osg::Vec3d> &res,				//результат
	const ed::vector<osg::Vec3d> &points,		//начальная позиция
	const osg::Matrixd &matrix					//матрица
	);


/**
 *  makePerspectiveLH - Builds a left-handed perspective projection matrix based on a field of view. DX compatible.
 *  @param fovy   - Field of view in the y direction, in radians.
 *  @param aspect - Aspect ratio, defined as view space width divided by height
 *  @param znear  - Z-value of the near view-plane.
 *  @param zfar   - Z-value of the far view-plane.  
 */
inline osg::Matrixf makePerspectiveLH(const float fovy, const float aspect, const float zNear, const float zFar)
{	
	const float yScale = 1.0 / tan(fovy/2.0);	
	const float m22 = zFar/(zNear-zFar);

	return osg::Matrixf(
				yScale/aspect, 0,       0,        0,
				0,             yScale,  0,        0,
				0,             0,      -m22,      1,
		        0,             0,      zNear*m22, 0 );
}

/**
 * makeOrthoLH - Builds a customized, left-handed orthographic projection matrix
 * @param l  - Minimum x-value of view volume.
 * @param r  - Maximum x-value of view volume.
 * @param b  - Minimum y-value of view volume.
 * @param t  - Maximum y-value of view volume.
 * @param zn - Minimum z-value of the view volume.
 * @param zf - Maximum z-value of the view volume.
 */
inline osg::Matrixf makeOrthoLH(float l, float r, float b, float t, float zn, float zf)
{
	return 	osg::Matrixf(
		2/(r-l),     0,           0,           0,
		0,           2/(t-b),     0,           0,
		0,           0,           1/(zf-zn),   0,
		(l+r)/(l-r), (t+b)/(b-t), zn/(zn-zf),  1);
}

/**
 * getNearFar - Retrives near, far Z-value of view-plane from left-handed perspective projection matrix
 * @param[in] projLH  - Left-handed perspective projection matrix
 * @param[out] znear - Z-value of the near view-plane.
 * @param[out] zfar  - Z-value of the far view-plane.  
 */
inline void getNearFarFromPerspectiveLHMatrix(const osg::Matrixf &perspProjLH, double &zNear, double &zFar) {
	float m32 = perspProjLH(3,2); 
	float m22 = perspProjLH(2,2); 
	zNear = -m32/m22;
	zFar = m32/(1.0f-m22);
}

/**
 * getNearFar - Sets near, far Z-value of view-plane to left-handed perspective projection matrix
 * @param[out] projLH  - Left-handed perspective projection matrix
 * @param[in] znear - Z-value of the near view-plane.
 * @param[in] zfar  - Z-value of the far view-plane.  
 */
inline void setNearFarToPerspectiveLHMatrix(osg::Matrixf &perspProjLH, double zNear, double zFar) {
	float m22 = -zFar/(zNear-zFar); 
	perspProjLH(2,2) = m22; 
	perspProjLH(3,2) = -zNear*m22; 
}

/**
 * getNearFar - Retrives near, far Z-value of view-plane from left-handed ortho projection matrix
 * @param[in] orthoProjLH  - Left-handed ortho projection matrix
 * @param[out] znear - Z-value of the near view-plane.
 * @param[out] zfar  - Z-value of the far view-plane.  
 */
inline void getNearFarFromOrthoLHMatrix(const osg::Matrixf& orthoProjLH, double &zNear, double &zFar)
{
	const float m32 = orthoProjLH(3,2);
	const float m22 = orthoProjLH(2,2);
	zNear = -m32/m22;
	zFar  = 1/m22 + zNear;
}

/**
 * getNearFar - Sets near, far Z-value of view-plane to left-handed ortho projection matrix
 * @param[in] orthoProjLH  - Left-handed ortho projection matrix
 * @param[out] znear - Z-value of the near view-plane.
 * @param[out] zfar  - Z-value of the far view-plane.  
 */
inline void setNearFarToOrthoLHMatrix(osg::Matrixf& orthoProjLH, double &zNear, double &zFar)
{
	const float m22 = 1.f/(zFar-zNear);
	const float m32 = zNear/(zNear-zFar);
	orthoProjLH(3,2) = m32;
	orthoProjLH(2,2) = m22;
}

/*
* osgMatrixMultOptimized - оптимизированное умножение osg'шных матриц без перспективных преобразований 
* (4я компонента осей базиса = 0), в ~2.2 раза быстрее чем родной оператор умножения
*/
template<typename T> void osgMatrixMultOptimized(const T &m1, const T &m2, T &out)
{
	out(0,0) =  m1(0,0)*m2(0,0) + m1(0,1)*m2(1,0) + m1(0,2)*m2(2,0);
	out(0,1) =  m1(0,0)*m2(0,1) + m1(0,1)*m2(1,1) + m1(0,2)*m2(2,1);
	out(0,2) =  m1(0,0)*m2(0,2) + m1(0,1)*m2(1,2) + m1(0,2)*m2(2,2);
	//out(0,3) =  0;
	out(1,0) =  m1(1,0)*m2(0,0) + m1(1,1)*m2(1,0) + m1(1,2)*m2(2,0);
	out(1,1) =  m1(1,0)*m2(0,1) + m1(1,1)*m2(1,1) + m1(1,2)*m2(2,1);
	out(1,2) =  m1(1,0)*m2(0,2) + m1(1,1)*m2(1,2) + m1(1,2)*m2(2,2);
	//out(1,3) =  0;
	out(2,0) =  m1(2,0)*m2(0,0) + m1(2,1)*m2(1,0) + m1(2,2)*m2(2,0);
	out(2,1) =  m1(2,0)*m2(0,1) + m1(2,1)*m2(1,1) + m1(2,2)*m2(2,1);
	out(2,2) =  m1(2,0)*m2(0,2) + m1(2,1)*m2(1,2) + m1(2,2)*m2(2,2);
	//out(2,3) =  0;
	out(3,0) =  m1(3,0)*m2(0,0) + m1(3,1)*m2(1,0) + m1(3,2)*m2(2,0) + m1(3,3)*m2(3,0); 
	out(3,1) =  m1(3,0)*m2(0,1) + m1(3,1)*m2(1,1) + m1(3,2)*m2(2,1) + m1(3,3)*m2(3,1); 
	out(3,2) =  m1(3,0)*m2(0,2) + m1(3,1)*m2(1,2) + m1(3,2)*m2(2,2) + m1(3,3)*m2(3,2); 
	out(3,3) =  m1(3,1)*m2(1,3) + m1(3,3)*m2(3,3);
}

template <typename MT, typename T>
inline void decomposePerspectiveLH(const MT &projLH, T& vAngle, T& aspect, T& zNear, T& zFar) {
	T m00 = projLH(0, 0);
	T m11 = projLH(1, 1);
	T m32 = projLH(3, 2);
	T m22 = projLH(2, 2);
	zNear = -m32 / m22;
	zFar = m32 / (1.0 - m22);
	vAngle = atan(1.0 / m11) * 2.0;
	aspect = m00 / m11;
}

template void decomposePerspectiveLH<osg::Matrixf, float>(const osg::Matrixf&projLH, float& vAngle, float& aspect, float& zNear, float& zFar);
template void decomposePerspectiveLH<osg::Matrixd, double>(const osg::Matrixd&projLH, double& vAngle, double& aspect, double& zNear, double& zFar);


}
#endif