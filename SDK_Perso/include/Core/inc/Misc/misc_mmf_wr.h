#ifndef _W32_MMF_WRITE_H__
#define _W32_MMF_WRITE_H__

// MMF Файл для записи 
#include "misc_error.h"

namespace misc
{
	// Файл тупого наполнения (выделеная память не удаляется)
	class mmf_write
	{
	public:
		// Тип ссылки
		typedef int valuepointer;
	protected:
		struct header
		{
			int realAllocSize;		// Полная длина файла
			int bookMarkPtr;		// смещение на закладку
		};

		// Основной файл
		char filename[256];
//		HANDLE hFile;
//		HANDLE hMMF;
		char* map;
		int filesize;
		// Заголовок:
		header theHeader;
		header* pheader;

		// Зарезервированная память
		void* mapping;
		char* reservMemory;
		size_t reservSize;
		int commitSize;
		int realUserSize;
		int dwPageSize;

		// Прогнозирование выделения памяти
		valuepointer predictionpointer;	// Предсказываемый указатель
		int predictionlen;				// Предсказываемый размер
		// Выравнивание
		int allignment;

		int align(int value)
		{
			return ((value-1) & (~(dwPageSize-1))) + dwPageSize;
		}
		int alignsize(int value)
		{
			if( allignment==1) return value;
			return ((value-1) & (~(allignment-1))) + allignment;
		}
	// функции выделения памяти
	public:	
		int partition(const valuepointer* pvaluepointer) const
		{
			if(*pvaluepointer>=filesize) return 1;
			return 0;
		}
		void* get( const valuepointer* pvaluepointer) const
		{
			if(*pvaluepointer>=filesize) 
				return reservMemory + *pvaluepointer - filesize;
			else
				return map + *pvaluepointer;
		};
		template<class T> T* alloc(T*& newobject, unsigned int* offset=NULL)
		{
			int ofs;
			if( !alloc(sizeof(T), &ofs)) return NULL;
			if(offset) *offset = ofs;
			newobject = (T*)get(&ofs);
			return newobject;
		}
		template<class T> T* alloccopy(const T& srcobject, unsigned int* offset=NULL)
		{
			int ofs;
			if( !alloc(sizeof(T), &ofs)) return NULL;
			if(offset) *offset = ofs;
			T* newobject = (T*)get(&ofs);
			*newobject = srcobject;
			return newobject;
		}
		template<class T> T* allocarray(T*& newobject, int size, unsigned int* offset=NULL)
		{
			int ofs;
			if( !alloc(sizeof(T)*size, &ofs)) return NULL;
			if(offset) *offset = ofs;
			newobject = (T*)get(&ofs);
			return newobject;
		}
		ED_CORE_EXTERN bool alloc(int len, valuepointer* pvaluepointer);
		void free( valuepointer* pvaluepointer)
		{
			*pvaluepointer = 0;
		};
		int GetRollback()
		{
			return realUserSize;
		}
		void Rollback(int rollback)
		{
			realUserSize = rollback;
		}
		static void null(valuepointer* pvaluepointer)
		{
			*pvaluepointer = 0;
		}
		static bool isvalid( valuepointer* pvaluepointer)
		{
			return *pvaluepointer!=0;
		}

		// reserve contingous memory block with size = amount
		static void *reserve(size_t amount)
		{
			/*for(unsigned int p=0; p<0x80000000;)
			{
				MEMORY_BASIC_INFORMATION info;
				VirtualQuery((LPVOID)p, &info, sizeof(info));

				if((info.State == MEM_FREE) && (info.RegionSize >= amount))
				{
					if(VirtualAlloc((LPVOID)p, amount, MEM_RESERVE, PAGE_READWRITE))
						return (void *)p;
					p += 0x100;
					continue;
				}

				p += info.RegionSize;
			}*/
			try
			{
				return new char[amount];
			}
			catch(...)
			{
				throw misc::error("Reserve %d failed: insufficient address space", amount);
			}
		}

		// reserve maximum possible contingous memory block
		static ED_CORE_EXTERN void *reserveMax(size_t &amount);

	// Дополнительные операции:
	public:
		// Прогнозирование выделения памяти
		// Используется для выделения памяти в уже выделенной области
		void allocprediction( valuepointer valuepointer, int len)
		{
			this->predictionpointer = valuepointer;
			this->predictionlen = len;
		}

	public:
			
		ED_CORE_EXTERN mmf_write(int allignment=1);
		ED_CORE_EXTERN ~mmf_write();

		// allocates file in memory
		// by default takes largest memory block available
		ED_CORE_EXTERN bool Create(const char* filename, int maxfilesize=0);
		// Закрыть без записи
		ED_CORE_EXTERN void Reset();
		// Закрыть и записать
		ED_CORE_EXTERN void Close();
		int getSize()
		{
			return commitSize;
		}
	};
}

// Оператор выделения памяти new
// использование:
//              misc::mmf_write file;
//              BBB* pbbb = new(file) BBB;
//              ptr<BBB> ptb = new(file) BBB;
#ifdef new
    #undef new
#endif

inline void* __cdecl operator new(
	size_t s, 
	misc::mmf_write& file)
{ 
	int ofs;
	if( !file.alloc((int)s, &ofs)) return NULL;
	char* p = (char*)file.get(&ofs);
	return p;
}

inline void __cdecl operator delete(
	void*, 
	misc::mmf_write& file)
{
}

#endif
