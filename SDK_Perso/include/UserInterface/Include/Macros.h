#ifndef	__UI__MACROS_H_
#define	__UI__MACROS_H_

#include <ed/string.h>

//-------------------------------------------------------------------------------
//Фрагмент кода, который должен присутствовать в конструкторах контролей, 
//чтобы они могли взять свои настройки из дефолтного файла.
//Имя дефолтного файла для каждого типа контроля содержится
//в статической переменной defaultResourceFile. Ее обязательно надо 
//инициализировать.
//-------------------------------------------------------------------------------
#define READDEFAULT																\
int storedPreset = m_preset;													\
const ed::string& str = defaultResourceFile();									\
if (resourceKeeper().getReadingFile()											\
						!= resourceKeeper().fileToResourceName(str, m_preset))	\
{																				\
	(*this) = *(resourceKeeper().getResource(str, m_preset));					\
}																				\
m_preset = storedPreset;
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//Хранитель ресурсов объявляется как статическая переменная функции, чтобы избежать
//некорректной инициализации.
//-------------------------------------------------------------------------------
#define RESOURCEKEEPER(T)														\
static ResourceKeeper<T>& resourceKeeper()										\
{																				\
	static ResourceKeeper<T> _resourceKeeper;									\
	return _resourceKeeper;														\
}
//-------------------------------------------------------------------------------
#define RESOURCEKEEPER_H(T) static ResourceKeeper<T>& resourceKeeper();			
//-------------------------------------------------------------------------------
#define RESOURCEKEEPER_C(T)														\
ResourceKeeper<T>& T::resourceKeeper()											\
{																				\
	static ResourceKeeper<T> _resourceKeeper;									\
	return _resourceKeeper;														\
}

//-------------------------------------------------------------------------------
//Возвращение дефолтного имени файла
//-------------------------------------------------------------------------------
#define DEFAULTRESOURCEFILE(fileName)											\
static ed::string& defaultResourceFile()										\
{																				\
	static ed::string _defaultResourceFile = fileName;							\
	return _defaultResourceFile;												\
}
//-------------------------------------------------------------------------------
#define DEFAULTRESOURCEFILE_H static ed::string& defaultResourceFile();	
//-------------------------------------------------------------------------------
#define DEFAULTRESOURCEFILE_C(fileName, T)										\
ed::string& T::defaultResourceFile()											\
{																				\
	static ed::string _defaultResourceFile = fileName;							\
	return _defaultResourceFile;												\
}

//-------------------------------------------------------------------------------
#define RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(T)									\
	RESOURCEKEEPER_H(T)															\
	DEFAULTRESOURCEFILE_H
//-------------------------------------------------------------------------------
#define RESOURCEKEEPER_DEFAULTRESOURCEFILE_C(T, fileName)						\
	RESOURCEKEEPER_C(T)															\
	DEFAULTRESOURCEFILE_C(fileName, T)
//-------------------------------------------------------------------------------
//Аранжировка по имени ресурса
//-------------------------------------------------------------------------------
#define NAMEARRAGNGE(T) {arrange(*T::resourceKeeper().getResource(name));}
#define NAMEARRANGE(T)  {arrange(*T::resourceKeeper().getResource(name));}
//-------------------------------------------------------------------------------
inline ed::string extractTitle(const ed::string& fileName)
{
	ed::string::size_type begin = fileName.find_last_of("\\") + 1;
	if (begin == ed::string::npos) begin = 0;
	ed::string::size_type n = fileName.substr(begin, fileName.length() - begin).find_first_of(".");
	if (n == ed::string::npos) n = fileName.length() - begin;
	return fileName.substr(begin, n);
}
//-------------------------------------------------------------------------------


#endif	//	__UI__MACROS_H__