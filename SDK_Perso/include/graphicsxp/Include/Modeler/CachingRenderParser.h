#ifndef CachingRenderParser_H
#define CachingRenderParser_H

#include "ModelParser.h"

namespace Graphics
{

// Реализация парсера объектов с немедленной отрисовкой
class CachingRenderParser : public RenderParser
{
protected:
	typedef ed::vector<Graphics::RenderObject *> distanceList;
	typedef ed::vector<Graphics::RenderObject *> keyList;

	distanceList distObjects;	// объекты, отсортированные по дальности
	keyList keyObjects;			// объекты, отсортированные по ключам
	
	int *objs, *tris;
public:

	bool shadows;

	GRAPHICSXP_API CachingRenderParser();
	
	GRAPHICSXP_API void InitFrame(int *objCounter, int *triCounter);
	GRAPHICSXP_API void DrawAll(void* context = NULL);
	GRAPHICSXP_API void DrawAllAndKeepCache(void* context = NULL);	
	GRAPHICSXP_API void DrawShadowMapObjects(const osg::Matrixf& shadowMatrix);
	GRAPHICSXP_API RenderObject *GetNextObject(void* context=NULL);
	GRAPHICSXP_API void OnObjectReady(RenderObject *obj);	
	GRAPHICSXP_API RenderPurpose GetRenderPurpose() override;};

} // namespace Graphics

#endif // CachingRenderParser_H
