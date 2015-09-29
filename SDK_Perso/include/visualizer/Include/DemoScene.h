#pragma once
#include "visualizer.h"
#include "Lua/State.h"
#include "rwrappers/renderUserAPI.h"
#include "renderer/volumeEffect.h"
#include "renderer/rendererCallback.h"
#include "EnvironmentRender.h"
#include "componentModel/Entity.h"

#ifndef USE_TERRAIN4
#include "edtg/renderSurface.h"
#include "edtg/renderLight.h"
#endif

class SceneManager_Implement;
class SceneContainer;

class ModelComponent;
class TransformComponent;
class LightComponent;
class CameraComponent;
class FlightComponent;

namespace Graphics
{
	class ModelInstanceImpl;
	class AttachableModelInstance;
}

class VISUALIZER_API DemoScene: public Graphics::RendererCallback
{
public:
	struct Viewport
	{
		float x, y, width, height;
		Viewport(float X,float Y,float w,float h): x(X), y(Y), width(w), height(h) {}
	};
private:
	SceneManager_Implement	*scManager;
	SceneContainer			*scene;

#ifndef USE_TERRAIN4
	edtg::TerrainOptions	terrainAmbientOptions;
	edtg::handle_renderitem_t surfaceRenderItem;
#endif

	bool	bInit, bValidAmbient;
	bool	bDrawCirrus, bDrawLensEffects, bDraw, bDrawSky;
	float	aspect, hFov;	
	float	cldDensity, cldLow, cldHigh;

	double	curTime;
	long	timeOld;

	ed::string scriptName;
	ed::string updateFuncName;

	Graphics::effectState	*effects;

	ed::vector<Entity*>			entities;

	ComponentStorage compStorage[COMPONENT_TYPE_ENUM::COMPONENTS_COUNT];//локальное хранилище

	ed::vector<TransformComponent*> &transforms;//ссылка на хранилище, сюда ничего не добавлять вручную
	ed::vector<LightComponent*> &lights;//ссылка на хранилище, сюда ничего не добавлять вручную
	ed::vector<ModelComponent*> &models;//ссылка на хранилище, сюда ничего не добавлять вручную
	ed::vector<CameraComponent*> &cameras;//ссылка на хранилище, сюда ничего не добавлять вручную
	ed::vector<FlightComponent*> &flightControllers;//ссылка на хранилище, сюда ничего не добавлять вручную

	ed::vector<ModelComponent*> staticModels;//модели, попадающие на отрисовку в энвайромент куб и влияющие вычислении эмбиента

	CameraComponent	*activeCamera;

	RenderParserImpl *staticScene;
	EnvironmentRender::Params eParams;

	static osg::Matrixf	projScale;
	render::Texture		texBlack;//затычка для теней, чтобы всегда лежали

	ModelComponent *addModelInstance(const char *name, const osg::Matrixf &transform, bool drawToEnvironment = false);
	ModelComponent *addModelInstance(const char *name, const osg::Vec3f &pos, bool drawToEnvironment = false);
	LightComponent *addLightInstance(const char *type, const osg::Vec3f &pos, const osg::Vec3f &color, const osg::Vec4f &attParams);
	CameraComponent *addCameraInstance(const osg::Vec3f &pos);

	void	setActiveCamera(CameraComponent *cam);
	void	setClouds(float density, float low=-1.f, float high=-1.f);//кучевые облака
	void	setCirrus(bool enable);//перистые облака вкл/выкл
	void	setSky(bool enable);//небо вкл/выкл
	void	setLensEffects(bool enable);//вкл/выкл эффекты линзы
	bool	removeEntity(Entity *e);
	void	setEnable(bool enable);
	void	setUpdateFuncName(const char *funcName);

	void	reset();

public:	
	DemoScene(SceneManager_Implement *manager, lua_State *L = nullptr);
	~DemoScene();

	void init();
	void loadScene(const char *filename);
	void update();
	void draw(DemoScene::Viewport *viewport=nullptr);

	static void	regLuaFunctions(lua_State *L);

private:
	double		startTime;
	Lua::State	luaStateOwn;
	lua_State	*luaState;	

	///basic:
	static int setUpdateFuncL(lua_State* L);
	static int setEnableL(lua_State* L);
	static int createInstanceL(lua_State* L);
	static int loadScriptL(lua_State* L);
	static int setCloudsL(lua_State* L);
	static int setCloudsCirrusL(lua_State* L);
	static int setLensEffectsL(lua_State* L);
	static int setSkyL(lua_State* L);

	//ptr		addModel(scene, name, positionXYZ)
	static int addModelL(lua_State* L);
	static int addLightL(lua_State* L);
	static int addCameraL(lua_State* L);
	static int addComonentL(lua_State* L);
	//void		remove(scene, ptr)
	static int removeL(lua_State* L);
	//void		clear(scene)
	static int clearL(lua_State* L);

	///transform:
	//void		attach(scene, child ptr, parent ptr, connectorName)
	static int	attachL(lua_State* L);
	//void		detach(scene, ptr)
	static int	detachL(lua_State* L);
	//void		move(scene, ptr, x,y,z);
	static int	moveL(lua_State* L);
	//void		scale(scene, ptr, x,y,z);
	static int	scaleL(lua_State* L);
	//void		rotate(scene, ptr, x,y,z);
	static int	rotateL(lua_State* L);
	//void		setPosition(scene, ptr, x,y,z);
	static int	setPositionL(lua_State* L);
	//void		setOrient(scene, ptr, x,y,z);
	static int	setOrientL(lua_State* L);
	//void		lookAt(scene, ptr, ptr OR xyz);
	static int	lookAtL(lua_State* L);

	///model:
	//void		setArgument(scene, obj, arg, value);
	static int	setArgumentL(lua_State* L);
	static int	setLiveryL(lua_State* L);
	//void		drawToEnvironment(scene, obj, bool enabled);
	static int	drawToEnvironmentL(lua_State* L);
	static int	getModelRadiusL(lua_State* L);
	static int	getModelBBoxL(lua_State* L);


	///light:
	//void		setLightAmount(scene, obj, flat amount);
	static int	setLightAmount(lua_State* L);
	//void		setLightRadius(scene, obj, flat radius);
	static int	setLightRadius(lua_State* L);
	//void		setLightAngles(scene, obj, inner, outer);
	static int	setLightAngles(lua_State* L);


	///camera:
	//void		setCameraFov(ptr, flat fov);
	static int	setCameraFov(lua_State* L);
	//void		setCameraNearClip(ptr, flat near);
	static int	setCameraNearClip(lua_State* L);
	//void		setCameraFarClip(ptr, flat far);
	static int	setCameraFarClip(lua_State* L);
	//void		setCameraActive(ptr);
	static int	setCameraActive(lua_State* L);
	//void		setProjection(ptr, 'o' OR 'p');
	static int	setProjection(lua_State* L);

	///flight controller
	static int	addFlightKnot(lua_State* L);
	static int	setFlightSpeed(lua_State* L);
	static int	setFlightTargetOffset(lua_State* L);
	static int	getFlightKnot(lua_State* L);
};