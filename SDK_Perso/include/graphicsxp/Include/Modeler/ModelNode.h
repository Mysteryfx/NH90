#ifndef ModelNode_H
#define ModelNode_H

#include "graphicsxp.h"
#include "Common.h"
#include "position.h"
#include "box.h"
#include "ModelParsePurpose.h"
#include "Renderer\VolumeEffect.h"

#include <ed/vector.h>
#include <ed/string.h>

namespace Graphics
{

class Model;
class Material;
class ModelParser;
class ModelInstance;

// Узел в дереве модели
class GRAPHICSXP_API ModelNode : public Common::Unknown<Serializable>
{
private:
	bool lock;
	ed::string name;

	ModelParsePurpose np_this, np_children;

	typedef ed::vector<ModelNode *>  nodeList;
	nodeList child;

	ModelNode *parent;
	int parent_index;

	int AddChild(ModelNode *node);
	void RemoveChild(ModelNode *node);

protected:
	Box box;

public:

	ModelNode(ModelNode *_parent = 0);
	virtual ~ModelNode();

	const Box& getBox()const{return box;}

	// управление типом узла
	void SetNodePurposes(ModelParsePurpose purposes)	{np_this = purposes;}
	inline ModelParsePurpose GetNodePurpose()	{return np_this;}
	inline ModelParsePurpose GetChildrenPurposes()	{return np_children;}

	// именование
	void SetName(const ed::string& _name);
	const ed::string& GetName();

	// получение информации
	int GetChildrenCount();
	ModelNode *GetChild(int i);
	ModelNode *GetParent();
	int GetParentIndex();
	ModelNode *GetRoot();

	// управление иерархией
	void ChangeParent(ModelNode *_parent);
	void KillChildren();
	
	// получить максимальный индекс динамического параметра, 
	// используемого данным узлом
	virtual int GetMaxParamIndex();
	
	// получить описанный бокс узла
	virtual const Box& GetBox();
	virtual void CalculateBox();

	// заполнить типы узлов по всей иерархии
	void UpdatePurpose();
	void ResetPurpose(ModelParsePurpose to_reset);
	
	// загрузить ресурсы, на котороые ссылается узел или его дети
	virtual void LoadResources(int to_step);

	// получить список материалов, используемых в модели
	virtual void GetMaterialList(ed::vector<Material *> &materials);

	// заменить все вхождения материала - другим материалом
	virtual void ReplaceMaterial(Material *to_replace, Material *to_set);

	// проход по иерархии
	// парсинг модели для отрисовки (возвращает false если необходимо остановиться)
	virtual bool Parse(ModelInstance *object, ModelParser *parser, const Position3& parent_pos, effectState *parent_effects);
	
	// сохранение/загрузка
	virtual void serialize(Serializer &serializer);
	
	// копирование модели
	virtual ModelNode *CloneEmpty();
	virtual void CopyFrom(ModelNode *source);
};

} // namespace Graphics

#endif // ModelNode_H
