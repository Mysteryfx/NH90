#ifndef Renderer_H
#define Renderer_H

#include "graphicsxp.h"
#include "RendererCallback.h"
#include "RenderMode.h"
#include "GraphMath/vector.h"
#include "osg/Vec3d"
#include "osg/Matrixd"
#include <ed/vector.h>

namespace edcg{
	class Fog;
}

namespace Graphics
{
	
/// Deprecated
enum FillMode
{
	fmPoint = 0,
	fmWireFrame,
	fmSolid
};

struct SystemDescriptor;
class Material;
class Geometry;
class RenderObject;
class RenderText;
class Camera;
class RenderParser;
class Context;

// костыль.
// интерфейс для удаления треугольников с деревьями, под вертолётными площадками
// заполняется при инициализации миссии (wWorld::spawnHeliports, файл Frogfoot:wWorldSpawn.cpp)
// площадка задаётся позицией и радиусом
class heliport_positions {
public:
	heliport_positions(float r_ = 75 ) { radius = r_; }
	// пока радиус одинаковый для всех площадок, но вдруг понадобиться это изменить...
	void add_heliport(const Vector3 &p, float r = 0) {positions.push_back(p); }
	void clear() { positions.clear(); }

	int get_count() const { return (int)positions.size(); }
	const Vector3 get_pos(int index) const { return positions[index]; }
	float get_radius(int index = 0) const { return radius; }

private:
	ed::vector<Vector3> positions;
	float radius;
};

// Интерфейс для низкоуровневой подсистемы визуализации
class Renderer
{
public:
	virtual ~Renderer() {};

	/// Deprecated
	// context
	virtual void SetContext(Context *context) = 0;
	/// Deprecated
	virtual Context *GetContext() = 0;
	/// Deprecated
	virtual const Context *GetContext() const = 0;
	
	// инициализация/деинициализация
	virtual bool initialize(void *window, int width=-1, int height=-1, int fullscreen=-1) = 0;
	virtual bool isinitialized() = 0;
	virtual void uninitialize() = 0;

	/// Deprecated
	// регистрация call-back
	virtual void RegisterCallback(RendererCallback *callback, RendererCallbackEvent where) = 0;
	/// Deprecated
	virtual void UnregisterCallback(RendererCallback *callback, RendererCallbackEvent where = rcOnEverything) = 0;

	// управление камерой
	/// Deprecated
	virtual void SetCamera(const Camera &cam) = 0;
	/// Deprecated
	virtual const Camera &GetCamera() = 0;

	// управление фазами отрисовки
	virtual void Update() = 0;

	virtual void Clear(
		bool bRenderTarget=true, bool bZBuffer=true, bool bStencil=true, 
		const Vector3* pcolor=NULL) = 0;
	virtual void BeginScene() = 0;
	virtual bool Present() = 0;
	virtual void EndFrame() = 0;
	virtual void BeginParse() = 0;
	virtual void OnParseCallback() = 0;
	virtual void EndParse() = 0;
    virtual void SetRenderMode(RenderMode mode) = 0;
    virtual RenderMode GetRenderMode() = 0;

	// отправка объектов на отрисовку по одному
	/// Deprecated
	virtual void DrawObject(RenderObject* robject) = 0;
	/// Deprecated
	virtual void DrawObjects(RenderObject** robject, unsigned int n) = 0;
	virtual void DrawRenderText(RenderText* rtext, bool bDontUseGlobalVP=false) = 0;
	
	/// Deprecated
	virtual bool isHDR() const = 0;
	/// Deprecated
	virtual void setHDR(int v) = 0;

	virtual int getShadowLevel() const = 0;

	// выдать для следующего кадра отладочную информацию
	virtual void dumpNextFrame() = 0;

	virtual float getCivilLights()const = 0;
	virtual void setCivilLights(float civillights) = 0;

	virtual edcg::Fog& getFog() = 0;
	virtual const edcg::Fog& getFog()const = 0;

	virtual void Reset() = 0;

};

}

extern GRAPHICSXP_API Graphics::Renderer *renderer;
extern GRAPHICSXP_API Graphics::heliport_positions *heli_pos;


#endif // Renderer_H
