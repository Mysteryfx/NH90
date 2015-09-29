#ifndef UTIL_IO_H
#define UTIL_IO_H

#include "util.str.h"
#include <sys/stat.h>
#include "Util/Strings.h"
#ifdef _WINDOWS
#include <direct.h>
#include <windows.h>
#endif
#include "ed_vfs.h"
#include <assert.h>
#include "ed_vfs.hpp"

namespace util
{
	// namespace util::io содержит всевозможные функции для работы с файловой системой
	namespace io
	{
		// Удаление файла
		inline bool deleteFile(const ed::string& fullpath)
		{
			return remove(fullpath.c_str()) == 0;
		}
		
		inline bool renameFile(const ed::string& oldName, const ed::string& newName)
		{
			return rename(oldName.c_str(), newName.c_str()) == 0;
		}

		inline bool fileExists(const ed::string& filename)
		{
			return VFS_exists(filename.c_str()) != 0;
		}

		inline bool folderExists(const ed::string& foldername)
		{
			return VFS_isdir(foldername.c_str()) != 0;
		}

		// Рекурсивно создаёт папки. Например для пути "C:\Folder1\Folder2\Folder3" 
		// будут созданы все три. Для избежания создания папки "file.ext" для пути
		// "C:\Folder1\Folder2\file.ext" рекомендуется предварительно воспользоваться 
		// функцией util::str::extractFolder. 
		// Возвращает true если всё прошло гут.
		inline bool createFolder(const ed::string& path)
		{
			return VFS_mkdir(path.c_str()) != 0;
		}

		// создать директорию для файла filename
		inline bool createFolderForFile(const ed::string &path)
		{
			return createFolder(ed::extractFolder(path));
		}

		// Найти все файлы с заданными расширениями в папке folder
		// Поиск в подпапках не производится
		// В extensions можно написать несколько расширений через запятую, например "h,cpp"
		// Если выставлен флаг absolutePath, то возвращает абсолютные пути
		
		// Именно из-за того, что мне приходится заводить такие говноструктуры 
		// я и не переношу апи, построенное на коллбэках
		struct FindFilesUserData {
			ed::vector<ed::string>* result;
			ed::string ext;
			bool absolutePath;
		};

		inline void findFilesCallback(void *userdata, const char *dir, const char *filename)
		{
			FindFilesUserData* ffud = (FindFilesUserData*)userdata;
			if (ed::hasSuffix(filename, ffud->ext))
			{
				ed::string path = ed::string(ffud->absolutePath ? dir : "") + filename;
				ffud->result->push_back(path);
			}
		}

		inline ed::vector<ed::string> findFiles(const ed::string &folder, const ed::string &extensions, bool absolutePath = false)
		{
			ed::vector<ed::string> result;

			#ifdef _WINDOWS
				ed::vector<ed::string> ext = str::split(extensions, ",");

				ed::string searchFolder = str::convertToWindowsPathS(folder);
				for (size_t i = 0; i < ext.size(); i++)
				{
					ed::string filter = searchFolder + "*." + ext[i];
				
					WIN32_FIND_DATA findData;
					HANDLE hFind = FindFirstFile(filter.c_str(), &findData);

					if (hFind != 0 && hFind != INVALID_HANDLE_VALUE)
						do
						{
							ed::string file(findData.cFileName);
							if (absolutePath) 
								file = searchFolder + file;

							result.push_back(file);
						} while (FindNextFile(hFind, &findData));
				}
			#else
				ed::vector<ed::string> ext = str::split(extensions, ",");
				for (size_t i = 0; i < ext.size(); i++)
				{
					FindFilesUserData ffud;
				
					ffud.result = &result;
					ffud.ext = "." + ext[i];
					ffud.absolutePath = absolutePath;

					VFS_readdir(folder.c_str(), findFilesCallback, &ffud);
				}
			#endif

			return result;
		}

		// Найти в папке folder все подпапки.
		inline ed::vector<ed::string> findFolders(const ed::string &folder, bool absolutePath = false)
		{
			ed::vector<ed::string> result;

			#ifdef _WINDOWS
				ed::string searchFolder = str::convertToWindowsPathS(folder);

				WIN32_FIND_DATA findData;
				HANDLE hFind = FindFirstFile((searchFolder + "*.*").c_str(), &findData);

				if (hFind != 0 && hFind != INVALID_HANDLE_VALUE)
				{
					do
					{
						if ((findData.dwFileAttributes & 16) != 0)
						{
							ed::string file(findData.cFileName);
							
							if (file != "." && file != "..")
							{
								if (absolutePath) 
									file = searchFolder + file;

								result.push_back(file);
							}
						}

					} while (FindNextFile(hFind, &findData));
				}
			#else
				assert(false && "not implemented");	
			#endif

			return result;
		}

		inline ed::string getTempDirectory()
		{
			return VFS_get_tempdir();
		}

		inline bool copyFile(const ed::string& srcFile, const ed::string dstFile)
		{
			VFS::File src(srcFile.c_str(), VFS::OpenRead);
			if (src.operator VFS_File *() == NULL)
				return false;
			VFS::File dst(dstFile.c_str(), VFS::OpenWrite);
			if (dst.operator VFS_File *() == NULL)
				return false;

			const size_t BUF_SIZE = 4096;
			uint8_t buf[BUF_SIZE];
			do
			{
				auto cnt = src.read(buf, BUF_SIZE);
				if (cnt == 0)
					break;
				auto written = dst.write(buf, cnt);
				if (written == 0)
					return false;
			} while (true);
			
			return true;
		}

#ifdef ENABLE_OBSOLETE_UTIL_IO
		// Удаляет все файлы в папке
		inline bool clearFolder(const ed::string &folder)
		{
			#ifdef _WINDOWS
				ed::vector<ed::string> files = findFiles(folder, "*", true);
				for (size_t i = 0; i < files.size(); i++)
					DeleteFile(files[i].c_str());
			#else
				assert(false && "not implemented");	
			#endif

			return true;
		}

		// Удаляет все папки и файлы в папке
		inline bool clearFullFolder(const ed::string &folder)
		{
			#ifdef _WINDOWS
				ed::vector<ed::string> folders = findFolders(folder, true);

				for(unsigned int i = 0 ; i < folders.size() ; i++)
				{
					clearFullFolder(folders[i]);
					RemoveDirectory(folders[i].c_str());
				}
				clearFolder(folder);
			#else
				assert(false && "not implemented");
			#endif

			return true;
		}

		// Найти все файлы по заданной маске
		// Поиск в подпапках не производится
		// Если выставлен флаг absolutePath, то возвращает абсолютные пути
		inline ed::vector<ed::string> findMaskFiles(const ed::string &folder, const ed::string &mask, bool absolutePath = false)
		{
			ed::vector<ed::string> result;

			#ifdef _WINDOWS
				ed::string searchFolder = str::convertToWindowsPathS(folder);
				ed::string filter = searchFolder + mask;

				WIN32_FIND_DATA findData;
				HANDLE hFind = FindFirstFile(filter.c_str(), &findData);

				if (hFind != 0 && hFind != INVALID_HANDLE_VALUE)
					do
					{
						ed::string file(findData.cFileName);
						if (absolutePath) 
							file = searchFolder + file;

						result.push_back(file);
					} while (FindNextFile(hFind, &findData));
			#else
				assert(false && "not implemented");	
			#endif

			return result;
		}
#endif
	}
}

#endif
