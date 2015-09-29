#ifndef EDTG_TerrainGpuContext_H
#define EDTG_TerrainGpuContext_H

#include <osg/Vec2f>
#include <osg/Vec3f>
#include <osg/Vec4f>
#include <osg/Vec3d>
#include <osg/Matrixf>
#include <osg/Polytope>

namespace edtg
{
	struct RenderContext;
// Контекст рендера земли на GPU
// Общий для всех renderItem одного прохода
// 
// Хранит:
//   Матрицы камеры
//   Туман
//   Освещение
//   Статические данные для всех хелперов
//   Статические данные для некоторых материалов
//   
struct TerrainGpuContext
{
	friend struct RenderContext;
protected:
	// Камера
	osg::Matrixf mTTF;
	osg::Matrixf mLandLightTTF;
	osg::Matrixf mW;
	osg::Matrixf mV;
	osg::Matrixf mP;
	osg::Vec3f vOrigin;			// перенес начала координат
				float dummy1;
	// производные от камеры
	osg::Matrixf mVinv;
	osg::Matrixf mVP;
	osg::Matrixf mWVP;
	osg::Matrixf mWV;
	// нормализованая матрица V
	osg::Matrixf mVnormal;
	osg::Matrixf mVnormalinv;
	// Vsun: view матрица для солнца
	osg::Matrixf mVsun;
	osg::Matrixf mVsuninv;

	// матрица проекции теней от облаков
	osg::Matrixf mCloudShadowsProj;

	// матрица и текстура карты высот + текстура сплатмапа, отрендеренная с этой же ортокамеры
	osg::Matrixf heightmapMat;

// солнце, луна и ambient
protected:
	// GlobalLights globalLights;

protected:
	// Туман
	// edcg::Fog fog;

};

}

#endif