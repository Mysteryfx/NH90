#pragma once
#ifndef USE_TERRAIN4
#include "Visualizer.h"
#include <osg/Vec3f>
#include "edtg/eDrawRoutines.h"
#include "edtg/TerrainOptions.h"
#include "edtg/renderSurface.h"

class SceneManager_Implement;
class smCamera_Implement;
namespace Graphics {
	class Context;
}

//������� ��� ����, ����� ���� � ������ ����� ��������� �� �������,
//��� ������ �������� ����� �� �������� ������, � ������ ���� � ������ �� ���������.
//��-�� ����� ���� �� �������� �������� ��� ������������ ������� ����� ���������� �����
class VISUALIZER_API SurfaceWrapper
{
	SceneManager_Implement *scManager;
	Graphics::Context *ctx;

	osg::Vec3f origin;
	edtg::GlobalLights globallights;
	edtg::TerrainOptions terrainoptions;
	edtg::SurfaceRenderData surfaceRenderData;
	edtg::handle_renderitem_t surfacerenderitem;

	osg::Matrixf lightView, lightProj;
	osg::Matrixf view, proj;

	bool bValid;
	bool bDrawGrass;
	bool bGrassParsed;

public:

	static bool bNewGrass;

	SurfaceWrapper(SceneManager_Implement *manager=0);//�������� �� ����� ������ ��� �����

	void	parse(smCamera_Implement *cam,
		edtg::eDrawRoutines routine,
		edtg::handle_renderitem_t renderitem,
		const edtg::TerrainOptions& _terrainoptions);

	void	parseAsMap(const osg::Matrixf &view, const osg::Matrixf &proj, const osg::Vec3f &origin,
		edtg::handle_renderitem_t renderitem, const edtg::TerrainOptions& _terrainoptions,
		Graphics::Context *ctx);


	//������ ��� ��� ����
	void	draw() { drawOpaque(); drawTransparent(); }

	void	drawOpaque(bool renderLightmap = true);
	void	drawTransparent();
	void	drawAsMap();
};

#else //USE_TERRAIN4
#include "SurfaceWrapper4.h"
#endif //USE_TERRAIN4
