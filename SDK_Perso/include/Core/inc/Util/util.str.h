#ifndef UTIL_STR_H
#define UTIL_STR_H

#include <ed/string.h>
#include <ed/vector.h>

// ЭТОТ ФАЙЛ НАДО ВМЕРЖИТЬ В Strings.h И УДАЛИТЬ НАФИГ!!!!!

namespace util
{
	// namespace util::str содержит всевозможные утилитные функции работы со строками
	// Все функции работают с абсолютными путями
	// Все функции принимают const ed::string& и никак не изменяют исходную строку
	namespace str
	{
		// Разделяет строку на массив строк, разделителями из delimeter
		// "D:\Folder1\Folder2\file.ext" , "\" -> { "D:", "Folder1", "Folder2", "file.ext" }
		inline ed::vector<ed::string> split(const ed::string &str, const ed::string &delimeter, bool removeEmpty = true)
		{
			ed::vector<ed::string> result;

			for (size_t pos = 0, n = 0; n != ed::string::npos; pos = n + 1)
			{
				n = str.find_first_of(delimeter, pos);
				ed::string ss = str.substr(pos, n - pos);
				if (!removeEmpty || ss != "")
					result.push_back(ss);
			}

			return result;
		}

		// Удаляет пробелы и табы в конце строки
		inline ed::string removeSpaceInBack(const ed::string &str)
		{
			size_t pos = str.size() - 1;

			while (str[pos] == ' ' || str[pos] == '\t')
			{
				if (pos == 0) break;
				pos--;
			}

			return str.substr(0, pos + 1);
		}

		// Удаление табов и пробелов
		inline ed::string removeWhitespace(const ed::string &str)
		{
			ed::string result;

			for (size_t i = 0; i < str.size(); i++)
			{
				char c = str[i];
				if (c != ' ' && c != '\t') result += c;
			}

			return result;
		}

		// удвоить проценты
		inline ed::string duplicatePercent(const ed::string &str)
		{
			ed::string result = str;
			for (size_t i = 0; i < result.size(); ++i)
				if (result[i] == '%')
					result.insert(i++, "%");
			return result;
		}

		// Добавление и удаление слешей
		inline ed::string addSlash(const ed::string &path)
		{
			if (path.size() > 0)
				return (path[path.size() - 1] != '/') ? path + '/' : path;
			else return "";
		}

		inline ed::string removeSlash(const ed::string &path)
		{
			if (path.size() > 0)
				return (path[path.size() - 1] == '/') ? path.substr(0, path.size() - 1) : path;
			else return "";
		}

		inline ed::string addBackslash(const ed::string &path)
		{
			if (path.size() > 0)
				return (path[path.size() - 1] != '\\') ? path + '\\' : path;
			else return "";
		}

		inline ed::string removeBackslash(const ed::string &path)
		{
			if (path.size() > 0)
				return (path[path.size() - 1] == '\\') ? path.substr(0, path.size() - 1) : path;
			else return "";
		}

		// Заменить в строке str все символы before на after
		inline ed::string replace(const ed::string &str, char before, char after)
		{
			ed::string result = str;
			for (size_t i = 0; i < str.size(); ++i)
				if (result[i] == before) result[i] = after;
			return result;
		}

		inline ed::string replace(const ed::string &str, const ed::string &before, const ed::string &after)
		{
			ed::string result(str);

			size_t pos = 0;
			
			while ((pos = result.find(before, pos)) != ed::string::npos)
				result.replace(pos, before.length(), after);

			return result;
		}

		// "C:/Folder1/Folder2/file.ext"
		inline ed::string convertToUnixPath(const ed::string &path)
		{
			return replace(path, '\\', '/');
		}

		// "C:\Folder1\Folder2\file.ext"
		inline ed::string convertToWindowsPath(const ed::string &path)
		{
			return replace(path, '/', '\\');
		}

		// "//C/Folder1/Folder2/file.ext"
		inline ed::string convertToTclPath(const ed::string &path)
		{
			ed::string result = convertToUnixPath(path);
			if (result[1] == ':')
			{
				result.erase(1, 1);
				result.insert(0, "//");
			}
			return result;
		}

		// Конвертация и добавление соответствующего слеша
		inline ed::string convertToUnixPathS(const ed::string &path)
		{
			return addSlash(convertToUnixPath(path));
		}

		inline ed::string convertToWindowsPathS(const ed::string &path)
		{
			return addBackslash(convertToWindowsPath(path));
		}

		// Возвращает относительный путь пути str, относительно папки dir
		// path и dir должны оба иметь либо Unix либо Windows вид записи
		// "D:\Folder1\Folder2\file.ext", "D:\Folder\" -> "Folder2\file.ext"
		// В случае если dir не является префиксом path, возвращает path
		// "D:\Folder1\file.ext", "D:\Folder2\"        -> "D:\Folder1\file.ext";
		inline ed::string extractRelativePath(const ed::string &path, const ed::string &dir)
		{
			size_t pos = dir.length();
			if ( strncmp(path.c_str(), dir.c_str(), pos) == 0 )
			{
				if (path[pos] == '/' || path[pos] == '\\') pos++;
				return path.substr(pos);
			}
			return path;
		}

		// Заменяет в строке str некорректные символы на replaceSymbol
		// если выставлен флаг replaceSpaces то также заменяет пробелы и табуляцию на символ '_'
		inline ed::string correctFileName(const ed::string &filename, const char replaceSymbol = '#', bool replaceSpaces = false)
		{
			ed::string result = filename;

			for (size_t i = 0; i < result.size(); ++i)
			{
				char *c = &result[i];

				if (i == 1 && *c == ':')
					continue;

				if (strchr("|?*\":<>", *c) != NULL)
					*c = replaceSymbol;

				if (replaceSpaces && strchr(" \t\x0A\x0D", *c) != NULL)
					*c = '_';
			}

			return result;
		}

		// Подставляет в строку переменные окружения аналогично студии
		// "$(LOCKON)\Bazar" -> "D:\trunk\LockOnExe\Bazar" если "LOCKONEXE" = "D:\trunk\LockOnExe"
		// Подставляет "как есть"!! Чтоб избежать косяков со слешами надо ставить
		// флаг normalizeSlashes, который убирает дублирующие слеши и возвращает всё в Unix-стиле
		inline ed::string substituteEnvVars(const ed::string &str, bool normalizeSlashes = false)
		{
			ed::string result;

			for (size_t begin = 0, end = 0; true; )
			{
				begin = str.find("$(", end);
				result += str.substr(end, begin - end);
				if (begin == ed::string::npos) break;

				end = str.find(")", begin);
				if (end == ed::string::npos) break;

				// Если такая environment переменная имеется, то добавляем её
				char *env = getenv(str.substr(begin + 2, end - begin - 2).c_str());
				if (env) result += normalizeSlashes ? convertToUnixPathS(env) : env;

				end++;

				if (normalizeSlashes && (str[end] == '\\' || str[end] == '/')) end++;
			}

			return result;
		}

		inline ed::string toHexString(const int8_t* data, size_t length)
		{
			ed::string hex;
			for (size_t i = 0; i < length; ++i)
			{
				char buf[3];
				uint8_t cut = data[i];
				int d = cut;
				sprintf(buf, "%02X", d);
				hex += buf;
			}
			return hex;
		}
	}
}
#endif
