#ifndef __STRINGS_H__
#define __STRINGS_H__

#include <ed/sstream.h>
#include <ed/string.h>

#include "ed_string.h"

#include "../_config.h"

#include <stdint.h>

namespace osg{
	class Vec2i;
	class Vec2f;
	class Vec2d;
	
	class Vec3i;
	class Vec3f;
	class Vec3d;
	
	class Vec4i;
	class Vec4ui;
	class Vec4f;
	class Vec4d;
}

namespace ed {

ED_CORE_EXPORT ed::string string_printf(const char* format, ...);
ED_CORE_EXPORT ed::wstring string_printf(const wchar_t* format, ...);

/// Covert value to string.
template<typename T>
inline ed::string toString(const T& x) {
	ed::ostringstream o;
	o << x;
	return o.str();
}

// Vec2
ED_CORE_EXPORT ed::string toString(const osg::Vec2i& v);
ED_CORE_EXPORT ed::string toString(const osg::Vec2f& v);
ED_CORE_EXPORT ed::string toString(const osg::Vec2d& v);

// Vec3
ED_CORE_EXPORT ed::string toString(const osg::Vec3i& v);
ED_CORE_EXPORT ed::string toString(const osg::Vec3f& v);
ED_CORE_EXPORT ed::string toString(const osg::Vec3d& v);

// Vec4
ED_CORE_EXPORT ed::string toString(const osg::Vec4i& v);
ED_CORE_EXPORT ed::string toString(const osg::Vec4ui& v);
ED_CORE_EXPORT ed::string toString(const osg::Vec4f& v);
ED_CORE_EXPORT ed::string toString(const osg::Vec4d& v);

/// Convert string to value
template<typename T>
inline bool convert(const ed::string& s, T& x,
				 bool failIfLeftoverChars = true)
{
	ed::istringstream i(s);
	char c;
	bool failed = (! (i >> x) || (failIfLeftoverChars && i.get(c)));
	return !failed;
}

/// More convinient way to convert string to something
template<typename T>
inline T convertTo(const ed::string& s, bool failIfLeftoverChars = true)
{
	T x;
	convert(s, x, failIfLeftoverChars);
	return x;
}

// Заменяет все what на with
ED_CORE_EXPORT void findNreplaceAll(ed::string& s, const ed::string& what, const ed::string& with);

/// Заменяет все what на with
ED_CORE_EXPORT void findNreplaceAll(char *s, char what, char with);

// Заменяет все what на with
ED_CORE_EXPORT ed::string findNreplaceAll2(const char *s, const ed::string& what, const ed::string& with);

// отрезать префикс разделеный одним из символов из delims
ED_CORE_EXPORT ed::string removePrefix(const ed::string& src, const ed::string& delims);

/// Even more convinient way to convert string to something if you don't want write try {} catch(...) {} every time
template<typename T>
inline bool convertTo(const ed::string& s, T& value)
{
	ed::istringstream i(s);
	char c;
	if (!(i >> value) || (i.get(c)))
		return false;

	return true;
}

/// Splits string by delimiter.
/// \param str string to split
/// \param delimiter delimiter
/// \param res list where result will store.
template <typename _T> void splitString(const ed::string &str,const ed::string &delimiter,_T &res)
{
	res.clear();

	auto nd=delimiter.size();
	auto n=str.size();

	for(ed::string::size_type i=0;i<n;++i){
		auto pos=str.find(delimiter,i);
		
		if(pos==ed::string::npos){
			res.push_back(str.substr(i));
			return;
		}else{
			res.push_back(str.substr(i,pos-i));
			i=pos+nd-1;
		}
	}
}

/// Splits string by delimiter.
/// \param str string to split
/// \param delimiter delimiter
/// \param res list where result will store.
template <typename _T> void splitString(const ed::wstring &str,const ed::wstring &delimiter,_T &res)
{
	res.clear();

	auto nd=delimiter.size();
	auto n=str.size();

	for(ed::wstring::size_type i=0;i<n;++i){
		auto pos=str.find(delimiter,i);
		
		if(pos==ed::wstring::npos){
			res.push_back(str.substr(i));
			return;
		}else{
			res.push_back(str.substr(i,pos-i));
			i=pos+nd-1;
		}
	}
}


/**
 * Разделить строку
 * @param input исходная строка
 * @param output коллекция полученных строк
 * @param delims разделители
 */
template <typename T> void splitString(const ed::string& input, T& output, const ed::string& delims = ", \t")
{
	//Пропускаем разделители вначале
	ed::string::size_type lastPos = input.find_first_not_of(delims, 0);
	//Ищем следующий разделитель/конец слова
	ed::string::size_type pos = input.find_first_of(delims, lastPos);
 
	//Список слов
	ed::vector<ed::string> tokens;
	tokens.reserve(delims.size());
 
	while ((ed::string::npos != pos) || (ed::string::npos != lastPos))
	{
		//Добавляем слово
		tokens.push_back(input.substr(lastPos, pos - lastPos));
		//Пропустить разделители после слова
		lastPos = input.find_first_not_of(delims, pos);
		//Найти конец следующего слова
		pos = input.find_first_of(delims, lastPos);
	}
 
	//Копируем слова в коллекцию
	output.insert(output.end(), tokens.begin(), tokens.end());
}

template <typename Function> void splitStringLambda(const ed::string& input, const ed::string& delims, const Function& func)
{
	//Пропускаем разделители вначале
	ed::string::size_type lastPos = input.find_first_not_of(delims, 0);
	//Ищем следующий разделитель/конец слова
	ed::string::size_type pos = input.find_first_of(delims, lastPos);
 
	//Список слов
	while ((ed::string::npos != pos) || (ed::string::npos != lastPos))
	{
		//Добавляем слово
		func(input.substr(lastPos, pos - lastPos));
		//Пропустить разделители после слова
		lastPos = input.find_first_not_of(delims, pos);
		//Найти конец следующего слова
		pos = input.find_first_of(delims, lastPos);
	}
 }

/**
 * Разделить строку, сохранив строки с разделителями
 * @param input исходная строка
 * @param output коллекция пар (pair<bool, string>) полученных строк и строк с разделителями:
 *               true - строка
 *               false - строка с разделителями
 * @param delims разделители
 */
template <typename T> void splitStringAndDelims(const ed::string& input, T& output,
												const ed::string& delims = ", \t")
{
	//Пропускаем разделители вначале
	ed::string::size_type lastPos = input.find_first_not_of(delims, 0);
	if (lastPos > 0)
	{
		//Добавляем последовательность разделителей
		output.push_back(std::make_pair(false, input.substr(0, lastPos)));
	}

	//Ищем следующий разделитель/конец слова
	ed::string::size_type pos = input.find_first_of(delims, lastPos);
 
	//Список слов
	ed::vector<std::pair<bool, ed::string>> tokens;
 	tokens.reserve(delims.size());

	while ((ed::string::npos != pos) || (ed::string::npos != lastPos))
	{
		//Добавляем слово
		tokens.push_back(std::make_pair(true, input.substr(lastPos, pos - lastPos)));
		auto delimsLastPos = pos;
		//Пропустить разделители после слова
		lastPos = input.find_first_not_of(delims, pos);
		//Добавляем последовательность разделителей
		tokens.push_back(std::make_pair(false, input.substr(delimsLastPos,
															lastPos - delimsLastPos)));
		//Найти конец следующего слова
		pos = input.find_first_of(delims, lastPos);
	}
 
	//Копируем слова в коллекцию
	output.insert(output.end(), tokens.begin(), tokens.end());
}

/// Convert string to lower case
ED_CORE_EXPORT ed::string toLower(const ed::string& str);

/// Convert string to upper case
ED_CORE_EXPORT ed::string toUpper(const ed::string& str);

/// Adds slash to the end of string if it was missed. If string is empty does nothing.
ED_CORE_EXPORT ed::string completePathWithSlash(const ed::string& path);

/// Removes slashes from the and of string
/// "ddd/ddfsdf/" -> "ddd/ddfsdf"
ED_CORE_EXPORT ed::string removeSlashFromEnd(const ed::string& path);

/// Removes all white spaces from string.
///White spaces are detected using isspace function from ctypes.h.
ED_CORE_EXPORT ed::string removeWhiteSpaces(const ed::string& str);

/// Removes './' and coneverts to lower case if it specified.
ED_CORE_EXPORT void normalizePath(const char* in, ed::string& out,bool make_lower_case = true);

/// Removes './' and coneverts to lower case if it specified.
inline void normalizePath(const ed::string& in, ed::string& out,bool make_lower_case = true)
{
	normalizePath(in.c_str(),out,make_lower_case);
}

/// Removes './' and coneverts to lower case if it specified.
inline ed::string normalizePath(const char* in, bool make_lower_case = true)
{
	ed::string str;
	normalizePath(in, str, make_lower_case);
	return std::move(str);
}

// Returns file name
// "D:/Folder/File.extension" -> "File.extension"
ED_CORE_EXPORT ed::string extractFileName(const ed::string& path);

// Returns file name without extension
// "D:\Folder1\Folder2\file.ext" -> "file"
// "D:\Folder1\Folder2\file_no_ext" -> "file_no_ext"
ED_CORE_EXPORT ed::string extractFileNameNoExt(const ed::string& path);

// Removes extension from path
// "D:/Folder/File.extension" -> "D:/Folder/File"
ED_CORE_EXPORT ed::string removeExtension(const ed::string& path);

// Returns file extension without dot
// "D:\Folder1\Folder2\file.ext" -> "ext" (not ".ext")
ED_CORE_EXPORT ed::string extractExtension(const ed::string& path);

// Adds file extension to the end of path if needed
// ext param must be without leading dot. "txt" - corrent, ".txt" ot "*.txt" - incorrect
// "D:/Folder/SomeFile", ".ext"     -> "D:/Folder/SomeFile.ext"
// "D:/Folder/SomeFile.ext", ".ext" -> "D:/Folder/SomeFile.ext"
ED_CORE_EXPORT ed::string addExtension(const ed::string& path, const ed::string& ext);

// Extract full folder name
// "D:\Folder1\Folder2\file.ext" -> "D:\Folder1\Folder2\"
ED_CORE_EXPORT ed::string extractFolder(const ed::string& path);

// Check if string has such prefix
// "aaaBBB", "aaa"    -> true
// "aaaBBB", "BBB"    -> false
// "aaaBBB", "aaaBBB" -> canBeEqual
ED_CORE_EXPORT bool hasPrefix(const ed::string& str, const ed::string& prefix, bool canBeEqual = true);

// Check if string has such suffix
// "aaaBBB", "aaa"    -> false
// "aaaBBB", "BBB"    -> true
// "aaaBBB", "aaaBBB" -> canBeEqual
ED_CORE_EXPORT bool hasSuffix(const ed::string& str, const ed::string& suffix, bool canBeEqual = true);

/// Prints size in bytes in human readable format.
ED_CORE_EXPORT ed::string printSize(uint64_t s);

}

#endif
