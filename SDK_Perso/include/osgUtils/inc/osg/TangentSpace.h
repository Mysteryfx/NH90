#ifndef TANGENT_SPACE_H
#define TANGENT_SPACE_H

#include <osg/Matrixf>
#include <osg/Matrixd>
#include <osg/Vec2d>
#include <osg/Vec3d>

#ifndef DISABLE_OBSOLETE_OSGUTILS

// Вычисление преобразования переводящего каждую из P[3] в UV[3]
// матрица обратна матрице tangentSpace
void calcPtoUVtransformation(double outMatrix[16], const osg::Vec3d P[3], const osg::Vec3d UV[3]);

// Вычисление Tangent Space
void CalcTangentSpace2(const osg::Vec3d verts[3], const osg::Vec2d uv[3], osg::Vec3d& T, osg::Vec3d& B);

//Вычисление Tangent Space
void CalcTangentSpace(
    osg::Vec3d verts[3],				//кординаты вершин треугольника
    osg::Vec2d uv[3],					//текстурные координаты треугольника
    osg::Vec3d& s,						//out: вектор s
    osg::Vec3d& t						//out: вектор t
);

//Вычисление Tangent Space
void CalcTangentSpace(
    osg::Vec3f verts[3],				//кординаты вершин треугольника
    osg::Vec2f uv[3],					//кординаты вершин треугольника
    osg::Vec3f& s,						//out: вектор s
    osg::Vec3f& t						//out: вектор t
);

//Строит матрицу по заданным векторам u, v и Tangent Space
void BuildBasisFromTangentSpace(
    osg::Matrixf& m,					//Результирующая матрица
    const osg::Vec2f& u,				//вектор u
    const osg::Vec2f& v,				//вектор v
    const osg::Vec3f& s,				//вектор s
    const osg::Vec3f& t					//вектор t
);

//Перевод из пространства 3D в пространство текстурных координат
//m - базис в 3D
//s, t - базис в Tangent Space
//u, v - базис в пространстве текстурных координат
void BuildBasisForTangentSpace(
	osg::Vec2d &u,						//вектор u
	osg::Vec2d &v,						//вектор v
	const osg::Matrixd &m,				//базис в 3D
	const osg::Vec3d &s,				//вектор s
	const osg::Vec3d &t					//вектор t						  
	);

#endif

namespace ed
{
/// Creates tangent space using normal. The result is stored in \param tangent as vec4 to get binormal.
/// Only triangles draw mode is supported.
/// \param vertexSize holds size of fertex in float, must be >= 3.
void calculateTangentSpace(unsigned int nVertices,unsigned int vertexSize,unsigned int nTriangles,const float* vertices,const float* normals,const float* texCoords,const unsigned int* indices,float* tangents,float* binormals);


}

#endif // TANGENT_SPACE_H