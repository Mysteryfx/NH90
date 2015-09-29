#ifndef SelectorNode_H
#define SelectorNode_H

#include "graphicsxp.h"
#include "OnOffNode.h"
#include "Animation.h"

namespace Graphics
{

// Узел, выбирающий по динамическому параметру вариант прохода дерева
class GRAPHICSXP_API SelectorNode : public OnOffNode
{
protected:
	AnimationInt *selector;
	bool use_all;

public:
	SelectorNode(ModelNode *_parent = 0);
	~SelectorNode();

	// получить анимацию выбора
	// результирующий номер - номер ветви для спуска
	// если выставлен UseAll - обходятся все ветви
	// если результат < 0 - выход
	AnimationInt& GetSelector();

	void SetUseAll(bool useAll);
	bool GetUseAll();

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

#endif // SelectorNode_H