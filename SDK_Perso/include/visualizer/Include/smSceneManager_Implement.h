#pragma once

#include "Visualizer.h"
#include "viSearch.h"
#include "IMeteoDispatcher.h"
#include "smSceneManager.h"
//#include "lTerraBasic.h"
#include "Renderer\RenderObject.h"
#include "Renderer\RenderText.h"
#include "gTimer.h"
#include "gDrawLabels.h"
#include "ShadowMapRender.h"
#include "RadarRender.h"
#include "IwHumanPlane.h"
#include "IDebugVisualizer.h"
#ifdef DEBUG_TEXT
#include "Utilities/Printer.h"
#endif

#include "rwrappers/renderUserApi.h"
#include "VisualizerConstants.h"
#include "LightMapRender.h"

#ifdef USE_TERRAIN4
#include "edTerrain/ITerrainGraphics.h"
#include "TerrainParseContext.h"
#include "TerrainRenderer.h"
#endif

#include "edtg/renderSurface.h"
#include "edtg/renderLight.h"
#include "edtg/renderSmallShit.h"
#include "edtg/edtg.h"

#include "SurfaceWrapper.h"

class viObjectManager;
class smCamera_Implement;
class InfraRedVisualizer;

//причесал, OnePride


namespace Graphics
{
	class Material;
	class Context;
}

namespace PostEffects {
	class PostEffectPipeline;
}

namespace water {
	class Ocean;
}

namespace enlight {
	class Clouds;
	class CloudsRenderer;
}

class SurfaceWrapper;
class SceneContainer;
class MFDContainer;
class EnvironmentRender;
class DemoScene;


// реализация визуализатора
class VISUALIZER_API SceneManager_Implement : public smSceneManager, public IDebugVisualizer
{
	friend class SceneContainer;
	friend class MFDContainer;
	friend class NearSceneContainer;
	friend class SurfaceWrapper;
	friend class EnvironmentRender;
	friend class DemoScene;
	friend class RadarRender;
public:
	Graphics::effectState globalEffects;
	/// Needed to transfer flat shadow parameters to objects.
//	FlatShadowEffect flatShadowEffect;

	SceneManager_Implement();
	virtual ~SceneManager_Implement();
	
	// начало/конец симуляции (смена режима)
	virtual void StartSimulation();

	void createSceneContainers();

	virtual void StartInterface();

    // preload around this point
    virtual void Preload(const dPosition &dp, vCameraType_e type);
    virtual void CameraIndependedPreload();

	// Установка режима показа первых нескольких кадров
	virtual void ShowFirstFrames(bool show);

	// оперировать стандартными камерами
	virtual smCamera *DefaultCamera(vCameraType_e type);

	// получить коэффициент растворения объекта
	// по дистанции и туману (если bTarget == true => только по туману)
	float GetObjectOpacity(viObjectShape *object, smCamera_Implement *cam, float &fnear, float &ffar, bool bTarget = false);

	// Коэффициент растворения по туману
	float GetFogOpacity(const Vector3& camp, const Vector3& objp);
	
	// Обновить параметры из конфига
	virtual void UpdateParams();

	virtual void SetLabelsEnable();	// возможен ли показ меток
	virtual	void	   CommandSet(int);					// Обработка нажатия клавиш.
	virtual gLabels  & GetLabels	(){return labels;}// Указатель на метки объектов

	bool isOvercast();
	inline bool GetDrawWarmAir() {return drawWarmAir;}
	inline TVScreenCondition GetMfdMode() {return mfdMode;}

	bool isBelowOvercast(float h);

	// Создание кадра только пользовательского интерфейса
	virtual void RenderUI();

	// Создание полноценного кадра при симуляции
	virtual void Render();

	void RenderMFD(MFDContainer &mfd, int n);
    
    void RenderHumanPostEffects(int camera_type,bool to_ui_target = false);

	void RenderDemoScene();


	// Изготовление скриншота
	virtual void    MakeScreenshot();
	
	// Нажата клавиша "показать FPS"
	virtual void ToggleFPS();

	// Рисование заставки ожидания
	virtual void DrawWaitScreen(float in_load_percent = -1.0f);
	virtual void ShowProgress(float progress, const ed::string& message = "");
	virtual void HideProgress();

    // перевод из экранных координат в мировые и обратно
    // sx,sy - экранные координаты, в пикселях
    // ray - вектор в мировой системе координат
    // возвращают false если не попадаем в окно 3D вида
    virtual bool GetRayByScreenPoint(float sx, float sy, Vector3& ray);
    virtual bool GetScreenPointByRay(const Vector3& ray, float &sx, float &sy);

	virtual bool GetScreenPointsByRay(const Vector3& ray,ed::vector<Vector3> & all_viewports_points);


    // получить аспект для всей поверхности отрисовки (значение из Options->System->Graphics->Aspect)
    virtual float GetScreenAspect() {return screen_aspect;}

    // получить Солнце как источник света
    virtual Graphics::Light *GetSun() {if(sun) return sun->QueryLightInterface(); return 0;}

    lua_State* getGUIState() { return guiState; }

    virtual void setUIBackgroundColor(const Vector3& color) { uiBackground = color; }

	// IDebugVisualizer
	virtual void reg(funcDebugString *f, void *ud);
	virtual void unreg(funcDebugString *f, void *ud);
#ifdef DEBUG_TEXT
	// system state visualizer
	virtual void addOutput(Common::Printer * pPrinter_);
	virtual void removeOutput(Common::Printer * pPrinter_);
	// log visualizer
	virtual uiLog* addLog(const char * caption, const char * regexp, const char * logFileName);
	virtual void removeLog(const char * caption);
	// debug windows
	virtual void clearDebugWindows();
#endif

	InfraRedVisualizer* GetInfraRedVisualizer(){return ir_visualizer;} ;
    

    const CameraSetup & getViewportsSetup() const { return viewports_loader.viewports; };
	void			    updateViewportSetupForUnit(const ed::string & unit_type)
	{
		ConfigureViewports(unit_type.c_str());
	}
	smViewport     getViewportFromConfig(const char * name) const;

private:
	void doRenderUI();

	void UI_wall_render();
	void UI_wall_add(const smViewport & view,float hfov,const dPosition & dp,const dPosition & view_p);
	// При старте симуляции - несколько кадров не уходят на экран, прогружаем ресурсы
	unsigned simulationFrame;
	bool showFirstFrames;

	ed::vector<SceneContainer*> reflectionScene;
	ed::vector<SceneContainer*> refractionScene;
	ed::vector<SceneContainer*> bowwaveScene;
	ed::vector<SceneContainer*> hotAirScene;
	ed::vector<SceneContainer*> mainScene;
	ed::vector<SceneContainer*> nearScene;
	ed::vector<MFDContainer*>	mfdScene;
	SceneContainer*				mirrorScene;

	DemoScene					*demoScene;


	bool	bDrawMFD, bDrawReflection, bDrawRefraction, bDrawMainScene, 
			bDrawNearScene, bDrawCockpit, bDrawBowwave, bDrawShadows,
			bDrawMirrors;

	bool drawTargetView;
	ed::vector<ed::string> targetViewcfgList;

	// Последнее модельное время отрисовки
	wModelTime t_visual, last_dt, last_model;

	// детализация зеркал
	int mirrors_resolution;
	bool mirrors_every_frame;

	typedef ed::map<vCameraType_e, smCamera *>  CameraList;
	CameraList cameras;
    smCamera *currentCam;
	double screen_aspect;


	struct viewportsConfiguration
	{
		viewportsConfiguration();
		~viewportsConfiguration();
		lua_State * L;
		CameraSetup viewports;
	}   viewports_loader;


	render::FrameBuffer _uiTarget;

	Graphics::RenderObject _uiTargetObject;

	bool drawMFD;

	// текущий режим работы MFD
	TVScreenCondition mfdMode;

	// дальность травы
	float clutterMaxDistance;

	// полные тени (с самозатенением)
	ShadowMapRender shadowMapRender;
	RadarRender		radarRender;

	LightMapRender mLightMapRender;

	EnvironmentRender *envRender;//рисовалка куба, неба, эмбиента, и вообще

	bool  use_nv, use_nv_mask;
	
	// глобальные эффекты и источники света
	Graphics::VolumeEffect *sun, *moon;

	PostEffects::PostEffectPipeline *postEffectPipeline;
	water::Ocean *ocean;

	// Модель для поверхности
	edtg::handle_renderitem_t surfacerenderitem;
	edtg::handle_renderitem_t surfacerenderitemMirror;
	edtg::handle_renderitem_t surfacerenderitemReflection;
	edtg::handle_renderitem_t lightrenderitem;
	edtg::handle_renderitem_t smallshitrenderitem;

	edtg::TerrainOptions terrainoptions;
	edtg::TerrainOptions terrainMirroroptions;
	edtg::TerrainOptions terrainReflectionoptions;
    edtg::TerrainOptions terrainMapMFD;
	edtg::TerrainOptions landMaskTerrainOptions;

	render::FrameBuffer landLightTexture;
	render::FrameBuffer landHeightTexture;
	

	ed::vector<LightRenderData> lights;

	static bool FetchLights(viObject *item, viSearch *search, void *data);

	// разрешение показа меток
	bool allow_labels;

	// роза ветров
	bool compassRoseOn;
	Graphics::Model *compassRose;


	int waterLevel;

	friend InfraRedVisualizer ;
	InfraRedVisualizer* ir_visualizer ;

    // флаг отрисовки только кокпита
    bool cockpitOnly;

	// Для более гладкой отрисовки: придерживать время отрисовки
	// Получить dt отрисовки по dt мира
	wModelTime VisualDeltaTime(wModelTime model_dt, wModelTime late);
	wModelTime GetVisualTime();
	

	// Копия таргета для выхлопов
	bool drawWarmAir;
	
	// Метки объектов
	gLabels	 	labels;

    lua_State* guiState;

    Vector3    uiBackground;

	// расширение для скриншотов (сейчас используется ScreenshotExt из graphics.cfg)
	ed::string screenshotExt;

	// получить из конфига установки спецэффектов
	void RetrieveFXQuality();

    // получить из конфига конфигурацию экранов
    bool ConfigureViewports(const char * unit_type);
    // предзагрузить ресурсы из startupResources.lua
    void LoadStartupResources(const char *config);

	// сменить разрешение (при старте симуляции или интерфейса)
	void ChangeResolution();

	// получить параметры сплошной облачности
	void DrawOvercast(Graphics::effectState *eState, float pass);
	
	// возвращает количество отрисованных объектов
	int DrawObjects(viFlag mask, smCamera_Implement *cam, viSearchHandler func);
	int ParseTargets(smCamera_Implement *cam);
	/// Note! This method also parses new airfields.
	int ParseObjects(smCamera_Implement *cam, bool farplane);

	int ParseObjects(smCamera_Implement *cam, SceneContainer &scene);

	//собираем лампочки по фрустуму, обновляем их, и складываем в контейнер
	void CollectLights(SceneContainer &scene);
	
	//перебираем все объекты во фрустуме и формируем локальные списки лампочек (effectState'ы) для каждого объекта
	void FillObjectLights(SceneContainer &scene);

	// нарисовать поверхность земли
	void* getSurfaceRenderItem();
	
	void DrawPostEffect(bool lensEffectsOnly=false);

	void RenderLightMap(SceneContainer &scene);

	void clearWaterRelatedTargets();

	bool shadowsEnabled();
	//временно, пока новые облака не вкрячены на постоянно основе
	bool isNewCloudsEnabled();

	// выставить глобальные источники света (солнце, луна, отраженный и т.д.)
	// передается широта и долгота в градусах, высота над уровнем моря
	void SetGlobalLights(float latitude, float longitude, float altitude);

    // Передать эффектам скорость камеры
    void UpdateCameraVelocity();

	//распарсить все что можно по всем вьюпортам и рендер таргетам, используя cameraPos + cameraFovHorizon(град.) или данные из глобального вида
	void updateDrawData(const dPosition *cameraPos=nullptr, const float *cameraFovH=nullptr);

	void updateNearData(SceneContainer &scene);//распарсить сцену как ближний план
	void updateMainData(SceneContainer &scene, edtg::TerrainOptions &terrainOptions);//распарсить сцену как основную
	void updateMFDdata(MFDContainer &scene, int n);//распарсить как МФД

	// если близко к земле - рисовать ее в ближнем диапазоне
	bool CheckGroundProximity(smCamera *cam);

	// get camera position for the specified viewport
	void GetViewportPos(const smViewport& view, const dPosition& pos, float &hFov, dPosition &res, float &vFov);

	bool checkViewportRay(const dPosition & dp,const smViewport &view,const Vector3& ray,float ray_length,double * distance,float &sx, float &sy);

	// Вывод статистики на экран
	void PrintStats();

	int fps_level;
	Graphics::RenderText	text;	// Параметры вывода статистической информации.
	render::TextRender2		statsTextRender; //новомодная рендерилка текста для статистики
	
	gTimer	fpsTimer;	// FPS.
	UINT	fpsNumb;	// Количество циклов накопления (до 8).
	float	fpsSumm;	// Сумарное время циклов накопления.
	float	fpsMedm;	// Среднее FPS на 8 кадров.

    int		objsPerFrame;
    int		trisPerFrame;
	// IDebugVisualizer
	typedef ed::list< std::pair<funcDebugString *, void *> > dbgFuncList;
	dbgFuncList dbgfuncs;
#ifdef DEBUG_TEXT
	ed::map<Common::Printer*, uiOutput*>	uiOutputs;
	ed::map<ed::string, uiLog*>			uiLogs;
	class PrinterManager : public Common::Printer::Manager
	{
	public:
		virtual void addPrinter(Common::Printer * pPrinter);
		virtual void removePrinter(Common::Printer * pPrinter);
	};
	PrinterManager printerManager;
	void onEDLogMessage(const char *module, int level, const char *msg);
	static void edLogHook(const char *module, int level, const char *msg);
#endif

    struct MFD_render_params
    { 
        MFD_render_params()
        {
            dist_multiplier_fov_base = 1.73;
            second_pass_start_fov = 0.1;
            second_pass_far_clip  = 100;
            single_pass_near_clip = 0.4;
            two_pass_always       = true;
        }
        double dist_multiplier_fov_base;
        double second_pass_start_fov;
        double second_pass_far_clip;
        double single_pass_near_clip;
        bool   two_pass_always;
    };
    MFD_render_params  MFD_params;

	render::FrameBuffer	refractionFrameBuffer, 
						reflectionFrameBuffer, 
						bowwaveFrameBuffer,
						heatairFrameBuffer,
						distanceFrameBuffer,
						mfdFrameBuffer[MAX_MFD_COUNT],
						mirrors;

	int width, height;
	bool fullscreen, renderStarted, irModeAmbient;
    bool _fastMode;

	void	rendererUpdate();

	void	initFrameStatistics();
	void	gatherStatistics();

	bool onLoadTerrain();
	void initTerrain();
#ifdef USE_TERRAIN4	
	//render::TerrainParseContext mTerrainParserContext;	
	SurfaceWrapper mTerrainSurfacePerFrame;
	TerrainRenderer* getTerrainGraphics() override {return &mTerrainRenderer;}
private:
	TerrainRenderer mTerrainRenderer;
#endif

	static const int MAX_VIEWPORT_COUNT = 5;
	int		currentViewportIdx;

	enlight::CloudsRenderer	*cloudsRendererMain[MAX_VIEWPORT_COUNT], *cloudsRendererMirror;
	void	renderClouds(const IMeteoDispatcher::CloudsParams &params, int stype);
	void	setCloudsParam(const IMeteoDispatcher::CloudsParams &params);
	void	updateClouds();

	void	renderDistanceMap();

	void	applyIR(render::Texture& src, int mode, const osg::Vec4f& param);
	void	restoreAmbient();
	void	setIRmodeAmbient();
};

extern	VISUALIZER_API gTimer	tDraw;
extern	VISUALIZER_API gTimer	tDrawSea;
extern	VISUALIZER_API gTimer	tDrawSurf;
extern	VISUALIZER_API gTimer	tDrawAll;
extern	VISUALIZER_API gTimer	tDrawAther;

