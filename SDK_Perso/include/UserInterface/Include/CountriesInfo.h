#pragma once

#include <ed/string.h>
#include <ed/map.h>
#include "ExpImpSpec.h"
#include "Utilities.h"

//
//	Класс для определения номера иконки с флагом страны по 
//	её названию. Параметры иконок прописаны в файле country_list_icons.res 
//

typedef ed::map<ed::string, int, StringWithoutCaseLess>		CountriesInfoMap;
class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR CountriesInfo : public CountriesInfoMap
{
//	CountriesInfoMap		m_Info;
public:
	CountriesInfo(const char* inFileName = "\\Fui\\Resources\\CountriesIcons.res", 
				  const char* inSectionName = "CountriesInfo");
	~CountriesInfo();

	int								getIconNumber(const char* inCountry);
};

