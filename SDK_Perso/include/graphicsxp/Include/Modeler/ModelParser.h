#ifndef ModelParser_H
#define ModelParser_H

#include "graphicsxp.h"
#include "ModelParsePurpose.h"
#include "position.h"
#include "Renderer\DynamicParams.h"
#include <ed/string.h>

namespace Graphics
{

class RenderObject;
class Geometry;
class Context;

// интерфейс обходчика модели
class ModelParser
{
protected:
	ModelParsePurpose purpose;
public:
	ModelParser(ModelParsePurpose _purpose) : purpose(_purpose) {}
	virtual ~ModelParser() {};

	inline ModelParsePurpose Purpose()const	{return purpose;}

	inline void setPurpose(ModelParsePurpose purpose){this->purpose = purpose;}
};

// интерфейс обходчика для отрисовки
class RenderParser : public ModelParser
{

protected:
	/// Frame context
	Context *_context;

public:

	///Deprecated
    // дополнительно: тип отрисовки
    enum RenderPurpose
    {
        rpMain      = 1,    // основной
        rpMirrors   = 2,    // зеркала
		rpShadow0   = 4,    // first shadowMap
        rpShadow1   = 8,    // shadowMap > 0
		rpShadow    = 12,   // all shadowMap
        rpAuxTV     = 16,   // дополнительный экран (MFD), телекартинка
        rpAuxIR     = 32    // дополнительный экран (MFD), инфракрасный
    };

	RenderParser() : ModelParser(mpRender), _context(nullptr) {}

	// вызов для начала кадра (передаются переменные для ведения статистики)
	virtual void InitFrame(int *objCounter = 0, int *triCounter = 0) = 0;

	/// Deprecated
	// отправка на отрисовку всех скэшированных объектов
	virtual void DrawAll(void* context=NULL) = 0;

	// получить очередной объект для заполнения данных
	// если возвращает 0 - обход останавливается
	virtual RenderObject *GetNextObject(void* context=NULL) = 0;

	/// Deprecated
	// сообщение о том, что последний полученный объект готов
	virtual void OnObjectReady(RenderObject *obj) = 0;

	/// Deprecated
    // получить тип отрисовки
    virtual RenderPurpose GetRenderPurpose() {return rpMain;}

	/// Set frame context
	GRAPHICSXP_API void setContext(Context *context);

	/// Get frame context
	inline Context *getContext() const {return _context;}
};

// интерфейс обходчика для поиска столкновений
class IntersectionParser : public ModelParser
{
public:
	IntersectionParser(ModelParsePurpose _purpose = mpIntersect) : ModelParser(_purpose) {}

	// Протестировать геометрию на пересечение
	// должны вернуть false для остановки поиска
	virtual bool TestGeometry(Geometry *geom, const DParamList& dparams, const Position3& pos, const char* node) = 0;

	virtual bool isAvail(const unsigned int index){return false;};
	virtual bool isCheckingDamage(){return false;};

	virtual bool getImpactPos(Vector3 &pos){return false;};
	virtual bool getLastBoxPos(Vector3 &pos){return false;};
	virtual void setPartClosed(const unsigned int index, const char *closedBy){};
};

// интерфейс обходчика для поска пересечений с моделью
class SegmentParser : public IntersectionParser
{
public:
	SegmentParser(ModelParsePurpose _purpose = mpSegment) : IntersectionParser(_purpose) {}

	// обработка характерного отрезка
	// должны вернуть false для остановки поиска
	virtual bool SegmentFound(const ed::string& _segment, const Vector3& _a, const Vector3& _b) = 0;
};

} // namespace Graphics

// глобальный обходчик модели с немедленной отрисовкой
extern GRAPHICSXP_API Graphics::RenderParser *immediateParser;

#endif // ModelParser_H
