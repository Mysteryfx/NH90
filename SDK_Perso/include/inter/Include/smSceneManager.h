#pragma once

#include "smCamera.h"
#include "smViewport.h"
//#include "wTime.h"
#include "vPublic.h"
#include "Inter.h"

class gLabels;
class A2GRadarVisualizer;
class InfraRedVisualizer;
struct lua_State;
#ifdef DEBUG_TEXT
class uiOutput;
class uiLog;
namespace Common {
	class Printer;
}
#endif

namespace Graphics
{
    class Light;
};

class TerrainRenderer;

// TODO: rename
class smSceneManager
{
public:

    typedef ed::vector<smViewport> CameraSetup;

//	static smSceneManager *Create(void);
	virtual ~smSceneManager(void) = 0 {};

	// начало/конец симуляции (смена режима)
	virtual void StartSimulation() = 0;
	virtual void StartInterface() = 0;

    // загрузить ресурсы вокруг точки (используя настройки камеры type)
    virtual void Preload(const dPosition &dp, vCameraType_e type) = 0;
    /// Preloads resources which don't require camera position and preload radius.
    virtual void CameraIndependedPreload() = 0;

	// Установка режима показа первых нескольких кадров
	virtual void ShowFirstFrames(bool show) = 0;

	// Обновить параметры из конфига
	virtual void UpdateParams() = 0;

	// оперировать стандартными камерами
	virtual smCamera *DefaultCamera(vCameraType_e type) = 0;

	// Обработка нажатия клавиш.
	virtual	void CommandSet(int) = 0;

	// Указатель на метки объектов
	virtual gLabels  & GetLabels() = 0;

	// Создание кадра только пользовательского интерфейса
	virtual void RenderUI() = 0;

	// Создание полноценного кадра при симуляции
	virtual void Render() = 0;

	// Изготовление скриншота
	virtual void MakeScreenshot() = 0;
	
	// Нажата клавиша "показать FPS"
	virtual void ToggleFPS() = 0;

	// Рисование заставки ожидания
	virtual void DrawWaitScreen(float in_load_percent = -1.0f) = 0;
	virtual void ShowProgress(float progress, const ed::string& message = "") = 0;
	virtual void HideProgress() = 0;

    // перевод из экранных координат в мировые и обратно
    // sx,sy - экранные координаты, в пикселях
    // ray - вектор в мировой системе координат
    // возвращают false если не попадаем в окно 3D вида
    virtual bool GetRayByScreenPoint(float sx, float sy, Vector3& ray) = 0;
    virtual bool GetScreenPointByRay(const Vector3& ray, float &sx, float &sy) = 0;
	virtual bool GetScreenPointsByRay(const Vector3& ray,ed::vector<Vector3> & all_viewports_points) = 0;

    // получить аспект для всей поверхности отрисовки (значение из Options->System->Graphics->Aspect)
    virtual float GetScreenAspect() = 0;

    // получить Солнце как источник света
    virtual Graphics::Light *GetSun() = 0;

	virtual lua_State* getGUIState() = 0;

    virtual void setUIBackgroundColor(const Vector3& color) = 0;

	virtual InfraRedVisualizer* GetInfraRedVisualizer() = 0 ;

	// Коэффициент растворения по туману
	virtual float GetFogOpacity(const Vector3& camp, const Vector3& objp) = 0;
#ifdef DEBUG_TEXT
	// system state visualizer
	virtual void addOutput(Common::Printer * pPrinter) = 0;
	virtual void removeOutput(Common::Printer * pPrinter_) = 0;
	// log visualizer
	virtual uiLog* addLog(const char * caption, const char * regexp, const char * logFileName) = 0;
	virtual void removeLog(const char * caption) = 0;
	virtual void onEDLogMessage(const char *module, int level, const char *msg) = 0;
	// debug windows
	virtual void clearDebugWindows() = 0;
#endif


    virtual const CameraSetup & getViewportsSetup() const = 0;
	virtual		   smViewport     getViewportFromConfig(const char * name) const = 0;
	virtual void			    updateViewportSetupForUnit(const ed::string & unit_type) =0;

	virtual void* getSurfaceRenderItem() = 0;

	// new SceneManager_Implement(hWnd)
	INTER_API static void CreateSceneManager();

	INTER_API static void regLua(lua_State * L);

	virtual bool onLoadTerrain() = 0;
	virtual TerrainRenderer* getTerrainGraphics() { return nullptr; }
};


extern INTER_API smSceneManager *visualizer;
