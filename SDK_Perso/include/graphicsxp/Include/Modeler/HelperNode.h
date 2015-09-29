#ifndef HelperNode_H
#define HelperNode_H

#include "graphicsxp.h"
#include "TransformNode.h"
#include "OnOffNode.h"
#include "ModelConnector.h"

namespace Graphics
{
	
class ModelInstance;

// Узел для присоединения к нему других объектов на высоком уровне
class GRAPHICSXP_API HelperNode : public TransformNode, public ModelConnector
{
public:
	HelperNode(ModelNode *_parent = 0);

	// Интерфейс ModelConnector
	virtual const char* GetConnectorName();
	virtual bool IsConnectorVisible(ModelInstance &object);
	virtual const Position3& GetConnectorPos(ModelInstance &object);
    virtual const Position3& GetConnectorPosLocal(ModelInstance &object);
	virtual const dPosition& GetConnectorPosd(ModelInstance &object);
    virtual const dPosition& GetConnectorPosdLocal(ModelInstance &object);
	
	// проход по иерархии
	// парсинг модели для отрисовки
	virtual bool Parse(ModelInstance *object, ModelParser *parser, const Position3& parent_pos, effectState *parent_effects);
	
	// сохранение/загрузка
	virtual void serialize(Serializer &serializer);

	// копирование модели
	virtual ModelNode *CloneEmpty();
	virtual void CopyFrom(ModelNode *source);

protected:
	ed::string defSubmodel;
	OnOffNode *parentOnOff_;
	int parentBranch_;

	void UpdateParentOnOff();

};

} // namespace Graphics

#endif // HelperNode_H
