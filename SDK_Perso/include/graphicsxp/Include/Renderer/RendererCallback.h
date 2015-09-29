#ifndef RendererCallback_H
#define RendererCallback_H

#include "graphicsxp.h"
#include "DeviceParams.h"

namespace Graphics
{

class RenderObject;
class Camera;
struct SystemDescriptor;

// константы для регистрации
enum RendererCallbackEvent
{
	rcOnGetSystemDescriptor	=	0x00000001,
	rcOnInitialize			=	0x00000002,
	rcOnUninitialize		=	0x00000004,
	rcOnRetreiveParams		=	0x00000008, // not used
	rcOnSetParams			=	0x00000010,
	rcOnUpdate				=	0x00000020,
	rcOnSetCamera			=	0x00000040,
	rcOnSetRenderTarget		=	0x00000080,
	rcOnClear				=	0x00000100,
	rcOnParse				=	0x00000200,
	//rcOnEndScene			=	0x00000400,
	rcBeforePresent			=	0x00000800,
	rcAfterPresent			=	0x00001000,
	rcBeforeDrawObject		=	0x00002000,
	rcAfterDrawObject		=	0x00004000,
	rcOnEndFrame			=	0x00008000,
	rcOnFreeResources		=	0x00010000,
	rcAfterResetDevice		=	0x00020000,
	rcOnEndParse			=	0x00040000,
	rcOnBeginParse			=	0x00080000,
	rcOnEverything			=	0x001FFFFF,
};

// Интерфейс для отслеживания вызовов Renderer
class RendererCallback
{
public:
	GRAPHICSXP_API virtual ~RendererCallback();

	// инициализация/деинициализация
	virtual void OnGetSystemDescriptor(SystemDescriptor& descriptor) {};
	virtual void OnInitialize() {};
	virtual void OnUninitialize() {};

	// управление параметрами
	virtual void OnSetParams() {};
	
	// управление фазами отрисовки
	virtual void OnUpdate() {};
	virtual void OnSetCamera(const Camera &cam) {};
	virtual void OnClear() {};
	virtual void OnBeforePresent() {};
	virtual void OnAfterPresent() {};

	// отправка объектов на отрисовку
	virtual void OnBeforeDrawObject(RenderObject* robject) {};
	virtual void OnAfterDrawObject(RenderObject* robject) {};

	virtual void OnEndFrame() {};
	virtual void OnFreeResources() {};
	
	virtual void OnAfterResetDevice() {};

	virtual void OnBeginParse() {}
	virtual void OnEndParse() {}
	virtual void OnParse() {}
};

}

#endif // RendererCallback_H
