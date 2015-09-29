#ifndef MITKA_ERROR_H__
#define MITKA_ERROR_H__

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "_config.h"

namespace misc
{
	ED_CORE_EXTERN extern const char* error_black;
	extern char error_msg[];
	/////////////////////////////////
	// Исключение
	const int buflen = 255;
	class error
	{
	protected:
		char* string;
	public:
		operator const char* () const{ if(!string) return error_black; return string;}
		error& operator =(const char* arg){copy(arg);return *this;};
		error& operator =(const error& arg){copy(arg.string);return *this;};
		bool empty()
		{
			return (!string);
		}
		error(){ string=NULL;};
		error(const error& arg)
		{ 
			string = NULL;
			copy(arg.string);
		};
		error(const char* arg, ...)
		{
			string = NULL;
			va_list argList; va_start(argList, arg);
			printf(arg, argList);
			va_end(argList);
		}
#if 0
		error(HINSTANCE hInstance, UINT stringId, ...)
		{
			string = NULL;
			static char buffer[buflen];
			if( ::LoadString(hInstance, stringId, buffer, buflen)==0) return;

			va_list argList; va_start(argList, stringId);
			printf(buffer, argList);
			va_end(argList);
		}
		#ifdef __AFXWIN_H__
		error(UINT stringId, ...)
		{
			CString str; str.LoadString(stringId);
			va_list argList; va_start(argList, stringId);
			printf(str, argList);
			va_end(argList);
		}
		#endif
#endif
		~error()
		{ 
			delete []string;
		}
	protected:
		void printf(const char* buffer, va_list argList)
		{
			if(string) delete[] string; 
			string = NULL;
			static char sbuf[buflen];
			_vsnprintf(sbuf, buflen-1, buffer, argList);
			if( !strlen(sbuf)) return;
			string = new char[ strlen(sbuf)+1];
			strcpy( string, sbuf);
		}
		void copy(const char* arg)
		{
			if(string) delete[] string; 
			string = NULL;
			if( !arg) return;
			if( !strlen(arg)) return;
			string = new char[ strlen(arg)+1];
			strcpy( string, arg);
		}
	};

	ED_CORE_EXTERN const char *getLastError();
};

#endif
