#ifndef CMD_Node_H
#define CMD_Node_H

#include "matrix.h"
#include "ModelNode.h"

class gDrawCommand;
class gShapeObject;
struct kinematicsV;

namespace Graphics
{

class GeometryNode;
class AnimTransformNode;
class LodModelNode;
class SelectorNode;

// узел, умеющий загружать старые (*.cmd) модели и строить под собой соответствующее дерево
class GRAPHICSXP_API CMD_Node : public ModelNode
{
protected:
	// матрица перевода из CMD - координат в нормальные
	Matrix33 cmd_to_engine;
	
	// классы для создаваемых материалов и геометрии
	static ed::string geomClass;
	static ed::string matClass;
	
	// форсирование цветов материала
	static Vector3 *c_amb;
	static Vector3 *c_diff;
	static Vector3 *c_spec;
	static float glossiness;

	// список использованных текстур на время парсинга модели
	ed::vector<ed::string> textures;

	// индекс материала и геометрии в модели
	int mat_index, geom_index;

	// имя файла модели
	ed::string filename;

    // адрес сцены CMD из gShapeObject
    char *cmdPtr;

	// создать материал, возвращает false если такой материал уже есть
	virtual bool CreateMaterial(Material *&mat, gDrawCommand *cmd);
	
	// парсинг CMD
	virtual bool ParseMaterial(GeometryNode *cur, gDrawCommand *gcmd);
	virtual bool ParseGeometry(GeometryNode *cur, gDrawCommand *gcmd);

	// парсинг CE
	virtual ModelNode *ParseCeNop(ModelNode *parent, kinematicsV *kin);
	virtual ModelNode *ParseCeTra(ModelNode *parent, kinematicsV *kin);
	virtual ModelNode *ParseCeRot(ModelNode *parent, kinematicsV *kin);
	virtual ModelNode *ParseCeSca(ModelNode *parent, kinematicsV *kin);

	// парсинг различных типов узлов
	virtual bool ParseSC(ModelNode *cur, gDrawCommand *gsc);
	virtual bool ParseCMD(GeometryNode *cur, gDrawCommand *gcmd);
	virtual bool ParseLD(LodModelNode *cur, gDrawCommand *gld);
	virtual ModelNode *ParseCE(ModelNode *parent, gDrawCommand *gce); // создает и возвращает узел

	// найти и пометить узлы для проверки столкновений
	int GetTriangleCount(ModelNode *node, int maxTris);
	void CheckForCollision(ModelNode *node);
	ModelNode *FindCollisionScene();
	
	// создать узлы с характерными отрезками
	void CreateSegments(gShapeObject *shape);

public:
	CMD_Node(ModelNode *_parent = 0);

	// настройки интерпретации cmd
	// класс материала должен поддерживать CustomMaterial интерфейс
	// т.к. данные будут закачиваться через него
	// класс геометрии должен уметь читать данные из CMD формата самостоятельно
	static void __cdecl SetGeometryClass(const ed::string& _geomClass);
	static void __cdecl SetMaterialClass(const ed::string& _matClass);
	
	// Установка форсированных параметров цвета и ширины блика
	static void __cdecl SetForcedColors(Vector3 *amb, Vector3 *diff, Vector3 *spec);
	static void __cdecl SetGlossiness(float gloss);

	static const ed::string& __cdecl GetGeometryClass();
	static const ed::string& __cdecl GetMaterialClass();
	static float __cdecl GetGlossiness();

	// Для рендеринга земли в текстуру
	static void __cdecl setLandTextureMode(bool bForLandTexture = false);
	// Загрузка и сохранение CMD
	bool LoadFromCMD(const ed::string& filename);
	bool SaveToCMD(const ed::string& filename);
	
	// сохранение/загрузка
	virtual void serialize(Serializer &serializer);
};

} // namespace Graphics

#endif // CMD_Node_H
