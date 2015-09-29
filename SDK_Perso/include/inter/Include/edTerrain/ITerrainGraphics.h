#ifndef inter_edTerrain_ITERRAINGRAPHICS_H__
#define inter_edTerrain_ITERRAINGRAPHICS_H__

#include <osg/BoundingBox>
#include <osg/Polytope>
#include <osg/Vec3d>
#include <osg/Matrixd>
#include "ITerrainGraphicsConstants.h"
#include "IParseContext.h"
#include "IRenderContext.h"
#include "TerrainContextCamera.h"

struct lua_State;
namespace render 
{
	class IParseContext;
	class IRenderContext;
	class Camera;
}
namespace edterrain
{
	class ITerrain;
}


namespace edterrain
{

// ���� �� ����� ��� ��������� ����������
const double ITerrainGraphicsVersion = 0.0;

class ITerrainGraphics 
{
public:
	 // ������ ��� ���������� 
	ITerrainGraphics(){};
	virtual ~ITerrainGraphics(){};

	// �������
	virtual void release()=0;

	// ���� ��� �� ����
	virtual void parse(const render::IParseContext& parseContext) = 0;

	// BB �������
	virtual osg::BoundingBoxd getBounds(const render::SampleHandle& sample, render::enSpace space=render::SP_WORLDSPACE)=0;

	// ��������� ��������� ������� � ������� ������� �������
	virtual void render(const render::SampleHandle& sample, const render::IRenderContext& renderContext) = 0;

	// �������
	virtual ed::string dump(const render::SampleHandle& sample) = 0;

	// ����.��������
	virtual bool forceLoading(const osg::Vec3d& pt, double radius) = 0;
};

// ������� ��� �������� terrainGraphicsX.dll � ������� � ITerrainGraphics
class ITerrainGraphicsEntryPoint
{
public:
	ITerrainGraphicsEntryPoint();
	~ITerrainGraphicsEntryPoint();

public:
	ITerrainGraphics* createTerrainGraphics(ITerrain* pTerrain, lua_State* options);
	edterrain::ITerrainGraphics* instance();
	void close();

private:
	bool providerLoaded;
	void* provider_dlhandle;
	edterrain::ITerrainGraphics* terrainGraphics;
};

}

#include "inl/ITerrainGraphicsEntryPoint.inl"

#endif

/*/
use case:

{
	ITerrainGraphics* terrainGraphics;
	ITerrainGraphicsEntryPoint terrainGraphicsEntryPoint;

	{
		terrainGraphics = terrainGraphicsEntryPoint.createTerrainGraphics(ITerrain::GetDefaultTerrain());
	}

	render::FrameBuffer fb_heightMap;
	render::FrameBuffer fb_pespLighting;
	render::FrameBuffer fb_shadowMaps[4];
	render::FrameBuffer fb_screens[3];
	render::FrameBuffer fb_reflectionMap;

	Camera mainCameras[3];
	Camera shadowMapCameras[4];
	Camera pespLightingCamera;
	Camera heightMapCamera;

	// ������������� �����
	//...
}

// parse
	ParseContext parseContext;

	SampleHandle sm[3];
	sm[0] = parseContext.addSample(mainCameras[0], SHM_FINALCOLOR, SHM_TRANSPARENT, SHM_REFLECTIONMAP);
	sm[1] = parseContext.addSample(mainCameras[1], SHM_FINALCOLOR, SHM_TRANSPARENT, SHM_REFLECTIONMAP);
	sm[2] = parseContext.addSample(mainCameras[2], SHM_FINALCOLOR, SHM_TRANSPARENT, SHM_REFLECTIONMAP);

	SampleHandle ss[4];
	ss[0] = parseContext.addSample(shadowMapCameras[0], SHM_SHADOWMAP);
	ss[1] = parseContext.addSample(shadowMapCameras[1], SHM_SHADOWMAP);
	ss[2] = parseContext.addSample(shadowMapCameras[2], SHM_SHADOWMAP);
	ss[3] = parseContext.addSample(shadowMapCameras[3], SHM_SHADOWMAP);

	SampleHandle spl = parseContext.addSample(pespLightingCamera, SHM_LIGHTMAP);
	SampleHandle sh0 = parseContext.addSample(heightMapCamera, SHM_HEIGHTMAP);

	terrainGraphics->parse(&parseContext);

// ������ BB
	osg::BoundingBoxd bb0 = terrainGraphics->getBound(ss[0]);
	osg::BoundingBoxd bb1 = terrainGraphics->getBound(ss[1]);
	osg::BoundingBoxd bb2 = terrainGraphics->getBound(ss[2]);
	osg::BoundingBoxd bb3 = terrainGraphics->getBound(ss[3]);
	// ...

// ������ �� ������


	{
		render::UseFrameBuffer ufb(fb_heightMap);
		RenderContext renderContext(heightMapCamera);
		_fillRenderContext1(renderContext, ...);
		terrainGraphics->render(sh0, SHM_HEIGHTMAP, renderContext);
	}
	{
		render::UseFrameBuffer ufb(fb_pespLighting);
		RenderContext renderContext(pespLightingCamera);
		_fillRenderContext2(renderContext, ...);
		terrainGraphics->render(sh0, SHM_LIGHTMAP, renderContext);
	}

	for(int i=0; i<4; i++)
	{
		render::UseFrameBuffer ufb(fb_shadowMaps[i]);
		RenderContext renderContext(shadowMapCameras[i]);
		_fillRenderContext3(renderContext, ...);
		terrainGraphics->render(ss[i], SHM_REFLECTIONMAP, renderContext);
	}

	for(int i=0; i<3; i++)
	{
		{
			render::UseFrameBuffer ufb(fb_reflectionMap);
			RenderContext renderContext(mainCameras[i]);
			_fillRenderContext4(renderContext, ...);
			terrainGraphics->render(sm[i], SHM_REFLECTIONMAP, renderContext);
		}
		{
			render::UseFrameBuffer ufb(fb_screens[i]);
			{
				RenderContext renderContext(mainCameras[i]);
				_fillRenderContext5(renderContext, ...);
				terrainGraphics->render(sm[i], SHM_FINALCOLOR, renderContext);
			}
			{
				RenderContext renderContext(mainCameras[i]);
				_fillRenderContext6(renderContext, ...);
				terrainGraphics->render(sm[i], SHM_TRANSPARENT, renderContext);
			}
		}
	}
	
	/*/
