#ifndef Model_H
#define Model_H

#include "Resourcer\Resource.h"
#include "position.h"
#include "box.h"
#include "Renderer/VolumeEffect.h"
#include "Renderer/Pass.h"

namespace model {
	class IModel;
}

namespace Graphics
{
class Material;
class ModelLight;
class ModelParser;
class ModelInstance;
class ModelConnector;
struct IntersectionInfo;

// Интерфейс модели графического объекта
class Model : public Resource
{
public:
	static const unsigned int INVALID_LIGHT_INDEX = ~0;

	Model()	{rtype = rtModel;}

	// габариты
	virtual const Box& GetBoundingBox() = 0;
	virtual const Box& GetUserBox()	{return GetBoundingBox();}
	virtual const Vector3& GetCenter() = 0;
	virtual float GetRadius() = 0;

	// динамические параметры
	virtual int GetDynamicParamCount() = 0;
	virtual float GetParamStart(int param) = 0;
	virtual float GetParamFinish(int param) = 0;

	// получение вспомогательных объектов (dummy)
	virtual int GetConnectorCount() = 0;
	virtual ModelConnector *GetConnector(int index) = 0;
	virtual ModelConnector *GetConnector(const ed::string& _name) = 0;

	/// Returns the number of lights in model.
	virtual unsigned int GetNumLights(){return 0;}

	/// Returns light name by given light index.
	/// \param lightIndex must be less then the number of sources!
	virtual const char* GetLightName(unsigned int lightIndex){return "";}

	/// Returns light index by name or returns INVALID_LIGHT_INDEX if there is no light with given name.
	/// Warning: this function may be O(n).
	virtual unsigned int GetLightIndexByName(const char *lightName){return INVALID_LIGHT_INDEX;}

	/// Returns position of light at given index.
	/// \param i must be less then the number of sources!
	virtual Position3 GetLightPosition(ModelInstance &mi, const Position3& basePos, unsigned int lightIndex)const{return Position3();}

	/// Returns light at given light index.
	/// \param i must be less then the number of sources!
	virtual void GetLight(ModelInstance &mi, const Position3& pos, unsigned int lightIndex, ModelLight &light)const{}

	/// Returns light at given light index.
	/// This function doesn't calculate light position.
	/// \param i must be less then the number of sources!
	virtual void GetLight(ModelInstance &mi, unsigned int lightIndex, ModelLight &light)const{}

	// парсинг модели
	virtual void Parse(
		ModelInstance &object, 
		ModelParser &parser,
		const Position3& pos,
		effectState* effects=NULL) = 0;
	
	// парсинг модели
	virtual void Parse(
		ModelInstance &object, 
		ModelParser &parser,
		const Position3& pos,
		effectState* effects,
		Pass::PassType pass){
			Parse(object, parser, pos, effects);
	}

	// копирование модели (материалы и геометрия - не копируются)
	virtual Model *Clone() = 0;

	// получить список материалов, используемых в модели
	virtual void GetMaterialList(ed::vector<Material *> &materials){}

	// заменить все вхождения материала - другим материалом
	virtual void ReplaceMaterial(Material *to_replace, Material *to_set){};

	virtual model::IModel* getIModelInterface(){return nullptr;}
};

} // namespace Graphics

#endif // Model_H
