#ifndef GeometryNode_H
#define GeometryNode_H

#include "graphicsxp.h"
#include "ModelNode.h"

namespace Graphics
{

class Geometry;
class Material;
class FileModelNode;
class RenderParser;
class IntersectionParser;

// Узел модели с геометрией
class GRAPHICSXP_API GeometryNode : public ModelNode
{
protected:
	Geometry *geom;
	Material *mat;

	// для сохранения/восстановления из MMF файла
	unsigned geom_offset, geom_size;

public:
	GeometryNode(ModelNode *_parent = 0);
	
	// выставление/получение материала и геометрии
	void SetGeometry(Geometry *_geom);
	void SetMaterial(Material *_mat);
	Geometry *GetGeometry();
	Material *GetMaterial();

	// для сохранения/восстановления файла модели
	// установка смещения (от начала области данных) и размера
	virtual void SetGeomOffset(unsigned offset, unsigned size);
	virtual void GetGeomOffset(unsigned& offset, unsigned& size);

	// получить описанный бокс узла
	virtual void CalculateBox();
	
	// загрузить ресурсы, на котороые ссылается узел или его дети
	virtual void LoadResources(int to_step);

	// получить список материалов, используемых в модели
	virtual void GetMaterialList(ed::vector<Material *> &materials);

	// заменить все вхождения материала - другим материалом
	virtual void ReplaceMaterial(Material *to_replace, Material *to_set);

	// проход по иерархии
	// парсинг модели для отрисовки
	virtual bool Parse(ModelInstance *object, ModelParser *parser, const Position3& parent_pos, effectState *parent_effects);

	virtual bool Render(ModelInstance *object, RenderParser *parser, const Position3& parent_pos, effectState *parent_effects);
	virtual bool Intersect(ModelInstance *object, IntersectionParser *parser, const Position3& parent_pos);

	// сохранение/загрузка
	virtual void serialize(Serializer &serializer);

	// загрузка данных (после сериализации)
	virtual void LoadData(FileModelNode *filenode);

	// копирование модели
	virtual ModelNode *CloneEmpty();
	virtual void CopyFrom(ModelNode *source);
};

} // namespace Graphics

#endif // GeometryNode_H