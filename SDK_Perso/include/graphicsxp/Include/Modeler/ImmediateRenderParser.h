#ifndef ImmediateRenderParser_H
#define ImmediateRenderParser_H

#include "ModelParser.h"
#include "Renderer\RenderObject.h"

namespace Graphics
{

// Реализация парсера объектов с немедленной отрисовкой
class ImmediateRenderParser : public RenderParser
{
protected:
	RenderObject robject;
	int *objs, *tris;

public:
	GRAPHICSXP_API ImmediateRenderParser();
	
	GRAPHICSXP_API void InitFrame(int *objCounter, int *triCounter);
	GRAPHICSXP_API void DrawAll(void* context=NULL);
	GRAPHICSXP_API RenderObject *GetNextObject(void* context=NULL);
	GRAPHICSXP_API void OnObjectReady(RenderObject *obj);
};

} // namespace Graphics

#endif // ImmediateRenderParser_H
