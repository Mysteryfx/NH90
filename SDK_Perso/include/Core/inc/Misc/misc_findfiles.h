#ifndef MISC_FINDFILES_H__
#define MISC_FINDFILES_H__

#include <windows.h>

#include <ed/map.h>
#include <ed/string.h>

#include "misc_error.h"

namespace misc
{
	// Время записи файла
	inline bool getFileTime(const char* fullfilename, FILETIME& time)
	{
		// Открыть файл
		time.dwLowDateTime=0;
		time.dwHighDateTime=0;
		HANDLE File = CreateFile(fullfilename, 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if( File!=INVALID_HANDLE_VALUE)
		{
			GetFileTime( File, 0, 0, &time);
			CloseHandle( File);
			return true;
		}
		return false;
	}
	// Размер файла
	inline bool getFileSize(const char* fullfilename, unsigned int& size)
	{
		// Открыть файл
		size=0;
		HANDLE File = CreateFile(fullfilename, 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if( File!=INVALID_HANDLE_VALUE)
		{
			size = GetFileSize( File, 0);
			CloseHandle( File);
			return true;
		}
		DWORD err = GetLastError();
		return false;
	}

	struct FindFile
	{
		ed::string fullfilename;
		FILETIME lastwritetime;
	};
	typedef ed::map<ed::string, FindFile> tag_FindFileList;

	// Поиск файлов с заданным расширением в каталоге 
	inline int findfiles(
		const char* folder_src, 
		const char* extention,
		tag_FindFileList& findFileList
		)
	{
		char folder[256]; 
		strcpy(folder, folder_src);
		int end = (int)strlen(folder)-1;
		char last = '\\';
		if( end>=0) last = folder[end];

		if(last!='\\' && last!='/') strcat(folder, "\\");
		strcat(folder, "*.");
		strcat(folder, extention);

		// Читаем все файлы из каталога
		WIN32_FIND_DATA findData;
		HANDLE hFind = FindFirstFile( folder, &findData);
		BOOL bFind = (hFind!=0)&&(hFind!=INVALID_HANDLE_VALUE);
		for(; bFind; bFind=FindNextFile(hFind, &findData))
		{
			// Имя файла
			_strlwr( findData.cFileName);
			FindFile data;
			data.fullfilename = folder_src;
			if(last!='\\' && last!='/') data.fullfilename += "\\";
			data.fullfilename += findData.cFileName;

			FILETIME time;
			getFileTime(data.fullfilename.c_str(), time);

			// В список
			data.lastwritetime = time;
			findFileList[findData.cFileName] = data;
		}
		return (int)findFileList.size();
	}

	// Найти в каталоге свободное имя файла
	inline ed::string findUnusedFilename( 
		const char* foldername,		// Каталог
		const char* filename,		// Имя файла без расширения
		const char* extention)		// расширение
	{
		misc::tag_FindFileList findlist;
		misc::findfiles(foldername, extention, findlist);
		for(int i=0; ; i++)
		{
			misc::error filename("%s%d.%s", filename, i, extention);
			if( findlist.find( ed::string(filename)) != findlist.end()) continue;
			// Имя файла
			ed::string res = foldername;
			char last = foldername[strlen(foldername)-1];
			if(last!='\\' && last!='/') res += "\\";
			res += ed::string(filename);
			return res;
		}
		return "";
	}

	// Расчет суммарного FILETIME
	inline FILETIME sumLastWriteTime( 
		tag_FindFileList& findFileList)
	{
		FILETIME sourceversion;
		sourceversion.dwHighDateTime = 0;
		sourceversion.dwLowDateTime = 0;
		misc::tag_FindFileList::iterator itff = findFileList.begin();
		for(;itff != findFileList.end(); itff++)
		{
			misc::FindFile& find = itff->second;
			sourceversion.dwLowDateTime  += find.lastwritetime.dwLowDateTime;
			sourceversion.dwHighDateTime += find.lastwritetime.dwHighDateTime;
		}
		return sourceversion;
	}

}

#endif
