#pragma once
#include "rwrappers/renderUserAPI.h"
#include "Renderer/RenderObject.h"
#include "fakeSurfaceAndWater.h"
#include "AmbientCube.h"
#include "../include/oPrecipitation.h"

#ifndef USE_TERRAIN4
#include "edtg/renderSurface.h"
#include "edtg/renderLight.h"
#endif

class smCamera_Implement;
class SceneManager_Implement;
class SceneContainer;
class RenderParserImpl;

namespace Graphics
{
	class Camera;
}

class EnvironmentRender 
{
public:
	enum Type
	{
		ET_OUTDOOR,//в куб рисуем небо, облака, дополнительно рендерим землю в отдельный таргет и смешиваем с нижней гранью куба, генерим эмбент куб с учетом земли
		ET_INDOOR,//все тоже самое но без земли, эмбиент вычисляется простым усреднением энвайромент куба
	};

	struct Params
	{
		Type				type;//текущий тип энвайромента для отрисовки
		RenderParserImpl	*staticScene;//текущая статическая сцена для отрисовки
		osg::Vec3d			camPos;//положение камеры для indoor
		bool				bUpdateAmbient;//обновлять ли эмбиент
		bool				bDrawStars;//рисовать звезды, должно быть false если земля еще не загружена
		Params();
	};

private:
	bool bInit;
	bool bUpdateCube;
	bool bUpdateAmbient;
	bool bUpdateSurface;

	unsigned				spaceFlags;

	SceneManager_Implement	*scManager;
	SceneContainer			*ambientScene;//сцена с земелей для эмбиента

	Graphics::RenderObject	overcast;
	Graphics::RenderObject	cirrus;

	Effects::AmbientCubeGen ambientCubeGen;
	Effects::FakeWater		fakeWater;
	Effects::FakeSurface	fakeSurface;
	Effects::OPrecipitation	precipitation;

	render::FrameBuffer		envmap;//кубмап энвайромента
	render::FrameBuffer		fbSurface[2];

	render::Texture			surfaceRT[2];//рендер таргеты земли для environment

	Params					*params;//текущие параметры рендера 
	static Params			defaultParams;//параметры по умолчанию

#ifndef USE_TERRAIN4
	edtg::TerrainOptions	terrainAmbientOptions;
	edtg::handle_renderitem_t surfaceRenderItem;
#endif

	int		curSurfRT;
	float	heightAboveSurface;//высота камеры над поверхностью для эбмиент куба
	float	surfLerpParam; //параметр интерполяции цвета земли
	float	heightCoef;//нормализованая высота над поверхностью

	void	renderSurfaceForAmbient();//рендерит землю в таргет
	void	renderEnvironment();
	void	renderEnvironmentWall(int wall, bool bOvercast, Graphics::Camera &cam, Graphics::Context *context, float param = 1);
	void	drawSurfaceToEnvironment(float param);//рисует землю в энвайромент куб
	void	updateSky();

public:	
	EnvironmentRender(SceneManager_Implement *manager);
	~EnvironmentRender();

	void init();
	// Смотрим что нужно обновить с учетом перемещения камеры и выставляем флаги на перерисовку. 
	// Если params не заданы, сетятся дефолтные
	void update(Params *params = 0);
	// По результатам выполнения update() перерисовывает энвайромент/землю/эмбиент. 
	// Если в Params указана статическая сцена - дорисовывает ее в куб с заданной позиции камеры из Params
	void redrawChanges();

	void drawSky();// рисуем небо в текущем контексте
	void drawCirrus();// рисуем перистые облака в текущем контексте
	void drawOvercast(float pass, Graphics::effectState *estate);// рисуем оверкаст в текущем контексте
	void drawPrecipitation();//рисуем осадки в текущем контексте

	//нужно только для новой земли чтобы передать структурный буфер эмбиента,
 	//т.к. она грузится после инициализации менеджера сцен. Если порядок инициализации будет 
	//изменен и земля будет готова до инициализации EnvironmentRender, функцию можно будет выпилить
	void afterTerrainLoading();//TODO: выпилить если изменится порядок инициализации новой земли и EnvironmentRender

	void restoreAmbient();

};