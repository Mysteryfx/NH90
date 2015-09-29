#ifndef __LANDSCAPE3_LMATERIALALIAS_H
#define __LANDSCAPE3_LMATERIALALIAS_H
#include "../landscape3_config.h"

#include "edterrain.h"

#include "lTerraBasic.h"
#include "landscape3/lmaterial.h"

#include <ed/map.h>
#include <stdint.h>
#include <ed/vector.h>
#include "ed_string.h"

namespace Lua
{
class Config;
}

// Новый файл материалов (lua)
//Переименовать в material4. Виталик
namespace material2
{
struct EDTERRAIN_API MaterialAlias
{
	// Список материалов
	struct EDTERRAIN_API RealMaterial
	{
		// Категория
		ed::string category;
		ed::string queue;
		// Строка сборки материала
		ed::string mattext;
		// Строка сборки геометрии
		ed::string geomtext;
		
		// Список добавления параметров
		struct ParameterPosition
		{
			// Позиция в тексте
			int position;

			// Номер параметра
			ed::string parameterName;
		};
		ed::vector<ParameterPosition> matparamspos;
		ed::vector<ParameterPosition> geomparamspos;

		// Вкл/выкл
		bool bOn;
		// Приоритет (если -1, то из семантики)
		int prior;
		// условия для включения
		ed::string conditions;

		RealMaterial();

		// проверить условия использования материала
		bool checkConditions(
		    material3::Material* pMaterial
		);

		/**
		 * Распарсить строку вида: edgemat FxMaterial_SomeMaterial someParam={someparam} {{someParam2}}
		 * и построить список параметров строки
		 *
		 * @param parseString входная строка
		 * @param outputString выходная строка
		 * @param parameters список параметров строки
		 */
		static bool parseString(const ed::string& parseString, ed::string& outputString, ed::vector<ParameterPosition>& parameters);

		// Создать по строке. Старье. Использовать parseString
		/* deprecated */static bool FromString(
		/* deprecated */    const char* parsestring,
		/* deprecated */	ed::string& text,		// dst:
		/* deprecated */    ed::vector<ParameterPosition>& paramspos);

		// Строкой
		bool ToString(
		    char* buffer,
		    int bufferlen,
		    char* text,		// src:
		    ed::vector<ParameterPosition>& paramspos);
		// Собрать строку материала
		bool BuildMaterialString(
		    material3::Material* pMaterial,
		    char* buffer,
		    int bufferlen,
		    ed::string& errors
		);
		// Собрать строку материала
		bool BuildMaterialString(
		    material3::Material* pMaterial,
		    char* buffer,
		    int bufferlen,
		    ed::map<ed::string, ed::string>& externalparameters,
		    ed::string& errors
		);
		// Собрать строку геометрии
		bool BuildGeometryString(
		    material3::Material* pMaterial,
		    void* pointer,
		    char* buffer,
		    int bufferlen,
		    ed::string& errors
		);
	protected:
		// Собрать строку
		bool BuildString(
		    material3::Material* pMaterial,
		    char* buffer,
		    int bufferlen,
		    const char* text,		// src:
		    ed::vector<ParameterPosition>& paramspos,
		    ed::string& errors,
		    ed::map<ed::string, ed::string>* externalparameters=0
		);
	};

	// Описание от куда брать параметр
	struct MaxParamSource
	{
		enum SOURCE
		{
			TEXNAME = 1,		// Имя текстуры
			PROPERTY = 2,		// Пропертя
			WIRECOLOR = 3,		// Цвет WIRECOLOR
			NODENAME = 4,		// Имя объекта
		};
		
		SOURCE type;
		
		// Канал
		int channel;
		
		// Sub
		int submat; // (диффуз/прозр.)
		ed::string submatname;
		
		// number
		int number;
		
		// Проперти
		ed::string propertyName;
		
		// Значениие по умолчанию, если = "" параметр обязательный
		ed::string defaultValue;

		// для мержа текстур: имя текстурного массива
		ed::string texturearray;

		// для текстур: обязательно должен быть назначен этот Uvset
		ed::string requiredUvset;

		// Убрать расшинение имени файла
		bool hideExtension;
	};

	// секция "streams"
	struct Stream
	{
		ed::string name; // имя стрима
		int type;         // тип стрима. см. geometry4::GeometrySource::STREAMTYPE_ENUM
		bool required;    // обязательный ли стрим
		bool invertV;     // надо ли инвертировать V (для текстурных координат)
		ed::string tangentSpaceFor;		// командует считать тангент или бинормаль для UVSET
		ed::string tangentSpaceComponent;	// какую компоненту "tangent" or "binormal"

		Stream(const char* name = "", int type = 0) : name(name), type(type), required(false), invertV(false) {}
	};

	// потоки
	ed::vector<Stream> streams;

	bool hasStream(const ed::string& name)
	{
		for (size_t i = 0; i < this->streams.size(); i++)
			if (this->streams[i].name == name)
				return true;

		return false;
	}

	// Группа
	ed::string group;
	// псевдоним материала
	ed::string matalias;
	// описание материала
	ed::string comment;


	// Процедура преэкспорта
	ed::string preexport;

	// типы боксов(маска enBoxTypes) в которых участвует материал
	int boxtypes;

	// Брать нормали
	bool bNormals;
	// Брать тангенты и бинормали
	int tangentChannel;
	// Брать цвета
	bool bColors;
	// Каналы
	ed::vector<int> tvsources;
	// Источники параметров
	ed::map<ed::string, MaxParamSource> paramsources;
	// Категории
	ed::vector<RealMaterial> realmaterials;

	// Имя материала для экспорта поверхности
	// Чтоб решить проблему разного числа текстурных каналов
	ed::string lsa3_material;

	MaterialAlias();
};

// Описание текстурного массива
class EDTERRAIN_API TextureArray
{
public:
	// имя массива
	ed::string name;
	// текстуры массива
	ed::vector<ed::string> layers;

	// найти индекс текстуры в массиве
	int findLayer(const ed::string& name);
};

class EDTERRAIN_API MaterialAliasFile
{
	static MaterialAliasFile* loadMaterialsExport(Lua::Config& setup);
	static MaterialAliasFile* loadMaterialsRender(Lua::Config& setup);
	static MaterialAliasFile* loadTextureArrays(Lua::Config& setup, MaterialAliasFile* dst);
	static MaterialAliasFile* loadSemantics(Lua::Config& setup, MaterialAliasFile* dst);

//	static MaterialAliasFile* loadMaterials(Lua::Config& setup);
public:

	// Семантики
	ed::vector<ed::string> semantics;
	// Материалы
	ed::vector<MaterialAlias> materials;
	// текстурные массивы
	ed::vector<TextureArray> texturearrays;

	MaterialAliasFile();

	// Описание материала по имени
	MaterialAlias* getAlias(const char* aliasname)
	{
		for(int i = 0; i < (int)this->materials.size(); i++)
		{
			if(strcasecmp(aliasname, this->materials[i].matalias.c_str()) == 0)
				return &this->materials[i];
		}

		return NULL;
	}

	MaterialAlias* getAlias(const ed::string& aliasName)
	{
		return this->getAlias(aliasName.c_str());
	}

	TextureArray* getTextureArray(const char* name);

	// Открыть файл
	static MaterialAliasFile* Open(
	    const char* filename,
	    const char* rootdir,
	    const char* shaderdir, 
		const char* backend
	);

	static MaterialAliasFile* Open(const ed::string& filename, const ed::string& rootdir, const ed::string& shaderdir, const ed::string& backend);
};

}


#endif