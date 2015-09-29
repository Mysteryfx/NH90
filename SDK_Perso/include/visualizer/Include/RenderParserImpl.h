#pragma once

#include "Visualizer.h"
#include "ClipVolumes\ClipVolumePlanes.h"
#include "Modeler\ModelParser.h"
#include "Misc\misc_evaltime.h"

namespace Graphics{
	class Material;
}

// Обходчик для отрисовки моделей
class VISUALIZER_API RenderParserImpl : public Graphics::RenderParser
{
protected:
	float	meltFactor;
	float	meltNear;
	float	meltFar;
	float	maxDist;

	// Дополнительно тестировать все куски на вхождение в этот объем
	ClipVolumePlanes *clipplanes;

	// для инфракрасного режима - светимость объекта
	float IR_signature;
	Graphics::Material *IR_material;
	
	// предварительно заполнить z-buffer
	Graphics::Material *zwriter;

	misc::FunEvalTimeCounter frametime, rendertime;

	typedef ed::vector<Graphics::RenderObject *> distanceList;

	typedef ed::vector<Graphics::RenderObject *> keyList;

	distanceList distObjects;	// объекты, отсортированные по дальности
	keyList keyObjects;			// объекты, отсортированные по ключам
    
    int objsPerFrame;
    int trisPerFrame;

	void PassAsObject(Graphics::RenderObject *obj);
	/// crutch for airfields as they are seen through clouds
	void PassAsAirfield(Graphics::RenderObject *obj);
	void PassAsIR_MFD(Graphics::RenderObject *obj);
    void PassAsCloud(Graphics::RenderObject *obj);

public:
	RenderParserImpl();

	// новый кадр
	virtual void InitFrame(int *objCounter = 0, int *triCounter = 0);
	void FinishFrame();

	// Изменить IR светимость
	void SetIRSignature(float signature) {IR_signature = signature;}
	void ResetIRMode() {IR_signature = -1;}
	
	// получить очередной объект для заполнения данных
	// если возвращает 0 - обход останавливается
	virtual Graphics::RenderObject *GetNextObject(void* context=NULL);

	// сообщение о том, что последний полученный объект готов
	virtual void OnObjectReady(Graphics::RenderObject *obj);

    // получить тип отрисовки
    virtual RenderPurpose GetRenderPurpose();

	// степень растворимости объекта.
	void	ObjectTransparent (float m){meltFactor = m;}
	float	ObjectTransparent (void){return meltFactor;}

	// Дистанции растворения объектов.
	void	ObjectTransparentNear (float m){meltNear = m;}
	float	ObjectTransparentNear (void){return meltNear;}
	void	ObjectTransparentFar  (float m){meltFar = m;}
	float	ObjectTransparentFar  (void){return meltFar;}
	void	ObjectMaxDist  (float m){maxDist = m;}
	float	ObjectMaxDist  (void){return maxDist;}

	// отправить на отрисовку накопившиеся объекты
	virtual void DrawAll(void* context=NULL);
	void DrawZWrited();
	void DrawZWritedNoClear();//не очищать очередь после отрисовки, для возможности повторного вызова

	void	SortTransparent();
	void	DrawTransparent();
	void	DrawTransparentAll();
	void	DrawTransparentAbove(float y);
	void	DrawTransparentBelow(float y);
	//для случая когда камера находится в облаках
	void	DrawTransparentInsideClouds(float low, float high);
	void	DrawTransparentOutsideClouds(float low, float high);

	void	ClearTransparent();

    int   GetObjsCount () { return objsPerFrame; }
    int   GetTrisCount () { return trisPerFrame; }
	float GetFrameTime () { return (float)frametime.sumtime; }
	float GetRenderTime() { return (float)rendertime.sumtime; }

public:
	bool isEmpty();

	unsigned opaqueCount(){return keyObjects.size();}
	unsigned transparentCount(){return distObjects.size();}
};
