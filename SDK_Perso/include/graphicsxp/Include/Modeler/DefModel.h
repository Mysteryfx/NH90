#ifndef DefModel_H
#define DefModel_H

#include "graphicsxp.h"
#include "Model.h"

namespace Graphics
{

class ModelInstance;
class AnimationBase;
class AnimTransformNode;
class SelectorNode;
class MultiMaterialNode;
class UserBoxNode;
class GeometryNode;
class FastNopNode;
class HelperNode;
class TransformNode;
class ModelNode;

// оператор сравнения строк без учета регистра
class IdComparer
{
public:
	inline bool operator ()(const ed::string& op1, const ed::string& op2) const
	{
		return (stricmp(op1.c_str(), op2.c_str()) < 0);
	}
};

// информация о использовании динамического параметра
struct ParamInfo
{
	ParamInfo() {start = finish = 0;}
	float start, finish;
};

// Стандартная модель графического объекта
class GRAPHICSXP_API DefModel : public Model
{
protected:
	ModelNode *root;
	bool error_load;	// чтобы не пытаться все-время грузить ошибочный объект
	
	// статистика
	unsigned triangles, nodes, geomnodes;
	
	// имя файла
	ed::string filename;

	// габариты 
	Box box, user_box;
	Vector3 center;
	float radius;

	// диапазоны динамических параметров
	typedef ed::vector<ParamInfo> paramIntervalList;
	paramIntervalList  param_intervals;

	// список разъемов
	typedef ed::vector<HelperNode *> helperList;
	typedef ed::map<ed::string, HelperNode *, IdComparer> helperMap;
	helperList helpersByIndex;
	helperMap helpersByName;

	// добавить коннектор в списки
	void AddConnector(HelperNode *helper);

	virtual void FillStatsAnimation(AnimationBase *anim);
	virtual void FillStatsAnimNode(AnimTransformNode *cur);
	virtual void FillStatsSelector(SelectorNode *cur);
	virtual void FillStatsMultiMatNode(MultiMaterialNode *cur);
	virtual void FillStatsUserBox(UserBoxNode *cur);
	virtual void FillStatsGeometry(GeometryNode *cur);
	virtual void FillStatsNop(FastNopNode *cur);
	
	// заполнить статистику по модели
	// количество и диапазоны параметров, коннекторы
	virtual void ClearStats();
	virtual void FillStats(ModelNode *cur);

	// найти узел с соответствующим именем
	ModelNode *FindNode(ModelNode *cur, const ed::string& nodename);

public:
	// Фабрика
	static Resource *Create(ResourceType type, const ed::string& className);

	DefModel();
	~DefModel();

	// Создать по имени файла
	virtual bool CreateByName(const ed::string& resname);

	// габариты
	virtual const Box& GetBoundingBox()	{return box;}
	virtual const Box& GetUserBox()	{return user_box;}
	virtual const Vector3& GetCenter()	{return center;}
	virtual float GetRadius()			{return radius;}

	// парсинг модели
	virtual void Parse(
		ModelInstance &object, 
		ModelParser &parser, 
		const Position3& pos,
		effectState* effects=NULL);

	// динамические параметры
	virtual int GetDynamicParamCount();
	virtual float GetParamStart(int param);
	virtual float GetParamFinish(int param);

	// получение вспомогательных объектов (dummy)
	virtual int GetConnectorCount();
	virtual ModelConnector *GetConnector(int index);
	virtual ModelConnector *GetConnector(const ed::string& _name);
	
	// получение узлов по имени
	ModelNode *GetNode(const ed::string& nodename);

	// вернуть трансформацию ПОД узлом с соответствующим именем
	TransformNode *GetTransform(const ed::string& nodename);

	// имя класса ресурса
	virtual const ed::string& ClassName();

	// управление загрузкой ресурса (может быть реализовано с поддержкой многопоточности)
	// загрузка производится по шагам, семантика шагов зависит от конкретного типа ресурса
	// на шаге 0 не должно быть захвачено никаких аппаратных ресурсов
	virtual float GetLoadPriority();
	virtual int GetLoadStep();
	virtual int GetLoadStepCount();
	virtual void LoadToStep(int step);
	virtual void FreeToStep(int step);
	virtual void LoadWithDependents(int step);
	
	// сериализация (для полноценного сохранения модели этого вызова не достаточно)
	virtual void serialize(Serializer& serializer);

	// Получить корень дерева
	ModelNode *GetModelNodeRoot();

	// копирование модели (материалы и геометрия - не копируются)
	virtual Model *Clone();

	// получить список материалов, используемых в модели
	virtual void GetMaterialList(ed::vector<Material *> &materials);

	// заменить все вхождения материала - другим материалом
	virtual void ReplaceMaterial(Material *to_replace, Material *to_set);
};

} // namespace Graphics

#endif // DefModel_H
