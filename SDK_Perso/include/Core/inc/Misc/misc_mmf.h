#ifndef _W32_MMF_H__
#define _W32_MMF_H__

#include <stdint.h>
#include "misc_error.h"

#include <fstream>

namespace misc
{
	// Класс для открытия MMF файла
	class mmf
	{
		// Основной файл
		void* hFile;
		void* hMMF;
		char* map;
		unsigned int filesize;
		bool bReadOnly;
		misc::error errormsg;
		//!!!!!
		char *name;
		int tmp;
		std::ofstream *fout;

	public:
		// статистика
		ED_CORE_EXTERN static uint64_t reservedmemory;
	public:
		bool operator!() const
		{
			return (hFile==0);
		}
		operator char*()
		{
//			if( bReadOnly) return NULL;
			return map;
		}
		operator const char*() const
		{
			return map;
		}
		
		int Length()
		{
			return filesize;
		}
		char operator[](int i)
		{
			if(i<0 || (unsigned int)i>=filesize) return 0;
			return map[i];
		}

		template<class T> T* get(unsigned int offset, T*& dest, const char* errortag=NULL)
		{
			if(offset > (filesize-sizeof(T))) 
				if(!errortag) throw misc::error("try outside file access");
					else throw misc::error( "try outside file access on reading %s", errortag);
			return (dest = (T*)(map+offset));
		}
		template<class T> T* getup(unsigned int& offset, T*& dest, const char* errortag=NULL)
		{
			unsigned int res = filesize-sizeof(T);
			if( offset > res) 
				if(!errortag) throw misc::error("try outside file access");
					else throw misc::error( "try outside file access on reading %s", errortag);
			unsigned int oldoffset = offset; offset+=sizeof(T);
			return (dest = (T*)(map+oldoffset));
		}
		ED_CORE_EXTERN const char* getline(unsigned int& offset, int& len);
		ED_CORE_EXTERN mmf(const char* filename=NULL, bool bReadOnly=true);
		ED_CORE_EXTERN ~mmf();
		ED_CORE_EXTERN const char* error();
	public:
		ED_CORE_EXTERN bool open(const char* filename, bool bReadOnly=true);
		// Открытие файла с типом доступа FILE_MAP_COPY для нескольких первых страниц
		// страница = 64кб
		ED_CORE_EXTERN bool open_copyaccess(const char* filename, int pagecount=1);

		// Закрыть
		ED_CORE_EXTERN void close();
	};
};

#endif
