#ifndef FastNopNode_H
#define FastNopNode_H

#include "graphicsxp.h"
#include "OnOffNode.h"

namespace Graphics
{
	
// Узел для быстрой кинематики NOP при загрузке CMD файлов
class GRAPHICSXP_API FastNopNode : public OnOffNode
{
protected:
	float start, end;
	int param;

public:
	FastNopNode(ModelNode *_parent = 0);
	~FastNopNode();

	// установить сегмент, в котором будем продолжать обход
	void SetActiveSegment(float start, float end, int param);
	float GetStart() {return start;}
	float GetEnd() {return end;}

	// интерфейс для обнаружения виден/невиден при проходе снизу вверх
	virtual bool isVisible(ModelInstance *object, int branch);

	// получить максимальный индекс динамического параметра, 
	// используемого данным узлом
	virtual int GetMaxParamIndex();

	// проход по иерархии
	// парсинг модели для отрисовки
	virtual bool Parse(ModelInstance *object, ModelParser *parser, const Position3& parent_pos, effectState *parent_effects);
	
	// сохранение/загрузка
	virtual void serialize(Serializer &serializer);

	// копирование модели
	virtual ModelNode *CloneEmpty();
	virtual void CopyFrom(ModelNode *source);
};

} // namespace Graphics

#endif // FastNopNode_H