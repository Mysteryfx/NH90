#ifndef LOD_Model_H
#define LOD_Model_H

#include "graphicsxp.h"
#include "DefModel.h"

namespace Graphics
{

class ModelManager;
class GeometryNode;

// структура для описания переключения одного материала
struct ModelSkinInfo
{
	ed::string originalName;	// базовое имя из файла - для замены
	int argument;				// номер аргумента для замены

	struct valueMaterial
	{
		float value;			// значение аргумента
		ed::string matclass;	// класс для материала скина
		ed::string texture;	// текстура для данного значения
	};
	ed::vector<valueMaterial> skins;
};

// Модель, загружающая различные уровни детализации из разных файлов
// а также грузящая скины из конфиг-файла
class GRAPHICSXP_API LOD_Model : public DefModel
{
protected:

	class IdComparer
	{
	public:
		inline bool operator ()(const ed::string& op1, const ed::string& op2) const
		{
			return (stricmp(op1.c_str(), op2.c_str()) < 0);
		}
	};

	int load_step;
	ModelNode *collision;	// узел для проверки столкновений

	typedef ed::map<ed::string, ModelSkinInfo, IdComparer> skinlist;
	skinlist skins;

	// заполнить этот узел скинами из it
	void ExpandMultiMaterialNode(MultiMaterialNode *cur, skinlist::iterator& it);

	virtual void FillStatsMultiMatNode(MultiMaterialNode *cur);
	virtual void FillStatsGeometry(GeometryNode *cur);
	
	// создать узел в зависимости от расширения имени файла
	ModelNode *CreateNode(const ed::string& filename, ModelNode *parent, ModelManager *manager);

	// загрузить узел в зависимости от типа узла
	bool LoadNode(ModelNode *node);

public:
	// Фабрика
	static Resource *Create(ResourceType type, const ed::string& className);

	LOD_Model();

	// управление загрузкой ресурса (может быть реализовано с поддержкой многопоточности)
	// загрузка производится по шагам, семантика шагов зависит от конкретного типа ресурса
	// на шаге 0 не должно быть захвачено никаких аппаратных ресурсов
	virtual int GetLoadStep();
	virtual int GetLoadStepCount();
	virtual void LoadToStep(int step);
	virtual void FreeToStep(int step);

	// парсинг модели
	virtual void Parse(
		ModelInstance &object, 
		ModelParser &parser, 
		const Position3& pos,
		effectState* effects=NULL);

	// копирование модели (материалы и геометрия - не копируются)
	virtual Model *Clone();
};

} // namespace Graphics

#endif // LOD_Model_H
