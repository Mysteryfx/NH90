#ifndef SHADOWMAPRENDER_H
#define SHADOWMAPRENDER_H

#include "Visualizer.h"
#include "box.h"
#include "ClipVolumes\ClipVolumeInclined.h"
#include "Modeler\ModelParser.h"
#include "Renderer\Material.h"
#include "Renderer\VolumeEffect.h"

#include "Shadows.h"

#ifdef USE_TERRAIN4
#include "edterrain/TerrainContextCamera.h"
#include "edterrain/ITerrainGraphicsConstants.h"
#endif

class ClipVolume;
class viObject;
class viObjectNode;
class viObjectShape;
class viSearch;
namespace render{
	class TerrainParseContext;
}

// Объект для рендеринга depth-map для теней
class ShadowMapRender : public Graphics::RenderParser, enlight::Shadows::SceneRender
{
public:
			ShadowMapRender();
	virtual	~ShadowMapRender();

	bool	initialize(int shadowLevel, render::TerrainParseContext* terrainParseContext=nullptr);

	void	begin();
	void	addFrustum();
	void	queryCasters();
	void	parseShadowMap();
	void	renderShadowMap();

	// интерфейс RenderParser
	virtual void InitFrame(int *objCounter = 0, int *triCounter = 0);
	virtual void DrawAll(void* context=NULL);
	virtual Graphics::RenderObject *GetNextObject(void* context=NULL);
	virtual void OnObjectReady(Graphics::RenderObject *obj);
    virtual RenderPurpose GetRenderPurpose();

	void	updateSplitDistances();
	void	addShadowCaster(viObjectShape *obj);

	void	disableShadows();
	void	enableShadows();

protected:
	osg::Vec3f origin;
	int shadowLevel;

#ifdef USE_MRT_SHADOWMAP
	const osg::Matrixf	*currentShadowMatrix;	
	Graphics::RenderParser::RenderPurpose renderPurpose;	
	virtual	void				renderShadowMap(const osg::Matrixf *matrix);		// shadowMap callbacks
#else

	struct Caster {
		viObjectShape* obj;
		osg::BoundingBoxd bbox;
		Caster() {}
		Caster(viObjectShape* o, const osg::BoundingBoxd &bb) : obj(o), bbox(bb) {}
	};

	osg::Matrixf		currentShadowMatrix[4];
	osg::BoundingBoxd	castersBounds[4];
	ed::vector<Caster> allShadowCasters;
	ed::vector<viObjectShape*> shadowCasters[4];

	int		currentShadowMap;
	virtual	void				parseShadowMap(int mapNumber, const osg::Matrixf &matrix);	
	virtual	void				renderShadowMap(int mapNumber);	// shadowMap callbacks
#endif

	static const int shadowMapsCount = 4;
	ed::vector<Graphics::RenderObject *> objects[shadowMapsCount];

	osg::BoundingBoxd getBounds(viObjectShape *obj);

    void RenderChildren(viObjectNode *obj, Position3 *pos);
	void RenderSubshape(viObjectShape *obj, Position3 *parent = 0);

	virtual void				precollectCasters(const osg::Matrixf &viewProj);
	virtual	void				queryCasters(int mapNumber, const osg::Matrixf &viewProj);
	virtual	osg::BoundingBoxd	getCastersBounds(int mapNumber);

	bool	inCockpit();

	void	drawObjects();
private:
#ifdef USE_TERRAIN4
	render::TerrainParseContext* mTerrainParseContext;
	render::SampleHandle terrainSamples[shadowMapsCount];
	render::Camera terrainCamera[shadowMapsCount];
#endif
};

#endif // SelfShadowVisualizer_H
