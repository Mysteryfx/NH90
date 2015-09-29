#ifndef SCENE_CONTAINER_H
#define SCENE_CONTAINER_H

class smCamera_Implement;
class SceneManager_Implement;
class SurfaceWrapper;
class MapSurfacePass;
class viLight;
class RenderParserImpl;

#include "smCamera.h"
#include "renderer/renderMode.h"
#include "renderer/Pass.h"
#include "renderer/VolumeEffect.h"
#include "IwHumanPlane.h"
#include "Modeler/ModelParser.h"

#ifndef USE_TERRAIN4
#include "edtg/renderSurface.h"
#include "edtg/renderLight.h"
#endif 

namespace enlight
{
	class CloudsRenderer;
}

class SceneContainer
{
public:

	enum SceneContainerType
	{
		SC_REFLECTION,
		SC_REFRACTION,
		SC_BOWWAVE,
		SC_MAIN,
		SC_NEAR,
		SC_MIRROR,
		SC_MFD,
		SC_HOT_AIR,

		SC_ENVIRONMENT,
		SC_CUSTOM,
		SC_UNKNOWN
	};

	SceneContainerType		type;
	Graphics::Pass			pass;
	Graphics::RenderMode	renderMode;

	SceneManager_Implement	*scManager;
	smCamera_Implement		*camera; //TODO заменить на интерфейс, когда они таки совпадут 	
	enlight::CloudsRenderer	*cldRenderer;

	static const int		MAX_LIGHTS = 1024;
	viLight					*lights[MAX_LIGHTS];//список лампочек, попадающих во фрустум
	unsigned				lightsCount;
	Graphics::effectState	landEffects;//земляные лампочки для отрисовки

	TVScreenCondition		mfdMode;	
	std::unique_ptr<SurfaceWrapper> surface;

#ifndef USE_TERRAIN4
	edtg::handle_renderitem_t surfacerenderitem;
#endif

	float	reflectH;

	bool	reflectDraw;
	bool	farplane;
	bool	bNight;
	bool	bDraw;//флаг выставляется во время парса и разрешает вызывать сцену на отрисовку если true
	bool	bDrawSurface;


			void	init(SceneManager_Implement *manager);
	virtual	void	draw();

			SceneContainer();
			SceneContainer(SceneManager_Implement *manager, SceneContainerType type);
	virtual ~SceneContainer();

protected:
	bool	bInit;

	void	drawAsReflection();
	void	drawAsRefraction();
	void	drawAsBowWave();
	void	drawAsHotAir();
	void	drawAsMain();
	void	drawAsMirror();
	virtual void drawAsNear(){}
	virtual void drawAsMFD() {}

	void	drawScene();

	//выставить камеру сцены в контекст + Pass + renderMode
	void	setupContextAndMode(Graphics::Context *context);
};



class MFDContainer: public SceneContainer
{
public:	
	bool					bTwoPasses;

	smCamera_Implement		*cameraNear;
	SurfaceWrapper			*surfaceNear;

#ifdef USE_TERRAIN4
	std::unique_ptr<MapSurfacePass> mapSurfacePass;
#endif

#ifndef USE_TERRAIN4
	edtg::handle_renderitem_t surfacerenderitemNear;
#endif

	void init(SceneManager_Implement *manager);
	MFDContainer(SceneManager_Implement *manager, SceneContainerType type);
	~MFDContainer();
protected:
	void drawAsMFD();
};



//TODO: выпилить поддержку CMD когда наконец умрет последняя античная кабина
namespace Graphics
{
	class RenderObject;	
}

class CMDCockpitParser : public Graphics::RenderParser
{
	typedef ed::vector<Graphics::RenderObject *> objectsList;
	objectsList distObjects, keyObjects;

public:
	void InitFrame(int *objCounter, int *triCounter) {}
	void DrawAll(void* context = nullptr);
	void OnObjectReady(Graphics::RenderObject *obj);
	Graphics::RenderObject *GetNextObject(void* context);	
};

class NearSceneContainer: public SceneContainer
{
public:	
	bool isCMD;	
	bool bDrawCockpit;
	bool bNearChanged;// двигались ли near/far в кабине
	CMDCockpitParser *cmdCockpitParser;
	RenderParserImpl *cockpitParser;

	void init(SceneManager_Implement *manager);
	NearSceneContainer(SceneManager_Implement *manager, SceneContainerType type);
	~NearSceneContainer();

protected:
	void drawAsNear();
};

#endif 
