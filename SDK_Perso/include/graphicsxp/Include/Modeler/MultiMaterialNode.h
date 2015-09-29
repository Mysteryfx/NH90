#ifndef MultiMaterialNode_H
#define MultiMaterialNode_H

#include "graphicsxp.h"
#include "GeometryNode.h"
#include "Animation.h"

namespace Graphics
{

// Узел с геометрией и различными материалами, переключающимися по аргументу
class GRAPHICSXP_API MultiMaterialNode : public GeometryNode
{
protected:
	AnimationInt *selector;
	typedef ed::vector<Material *> materialList;
	materialList materials;

public:
	MultiMaterialNode(ModelNode *_parent = 0);
	~MultiMaterialNode();

	// получить переключатель материалов
	AnimationInt &GetSelector();
	
	// выставление/получение материала и геометрии
	int GetMaterialCount();
	void SetMaterial(int index, Material *mat);
	Material *GetMaterial(int index);

	// загрузить ресурсы, на котороые ссылается узел или его дети
	virtual void LoadResources(int to_step);

	// проход по иерархии
	// парсинг модели для отрисовки
	virtual bool Render(ModelInstance *object, RenderParser *parser, const Position3& parent_pos, effectState *parent_effects);
	
	// сохранение/загрузка
	virtual void serialize(Serializer &serializer);
};

} // namespace Graphics

#endif // MultiMaterialNode_H