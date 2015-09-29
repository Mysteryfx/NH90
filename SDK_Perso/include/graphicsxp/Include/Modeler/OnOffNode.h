#ifndef OnOffNode_H
#define OnOffNode_H

#include "graphicsxp.h"
#include "ModelNode.h"

namespace Graphics
{
	
// Интерфейс для узлов, которые могут скрывать часть своих детей
class GRAPHICSXP_API OnOffNode : public ModelNode
{
public:
	OnOffNode(ModelNode *_parent = 0);
	~OnOffNode();

	// интерфейс для обнаружения виден/невиден при проходе снизу вверх
	virtual bool isVisible(ModelInstance *object, int branch) = 0;

	// сохранение/загрузка
	virtual void serialize(Serializer &serializer);

protected:
	// ссылка на следующий сверху узел трансформации, для вычисление полной трансформации
	OnOffNode *parentOnOff_;
	int parentBranch_;

	void UpdateParentOnOff();
};

} // namespace Graphics

#endif // OnOffNode_H
