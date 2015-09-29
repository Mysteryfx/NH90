#pragma once

//////////////////////////////////////////////
// Относительный указатель

#include "misc_ptr.h"
#include "misc_mmf_wr.h"

namespace misc
{
	// Использование:
	// ptr<int*> pInt;
	class string
	{
		misc::ptr<char> pData;
	public:
		inline string();
		inline ~string();
		// НЕ КОПИРУЕТ ДАННЫЕ
		inline void Mirror(string& arg);

		inline unsigned int size() const;

		inline bool operator ==(const string& arg) const;
		inline bool operator !=(const string& arg) const;

		inline bool empty() const;

		inline operator char* ()
		{
			return pData;
		}
		inline operator const char* () const 
		{
			return pData;
		}

		inline char& operator [](int i);
		inline const char& operator [](int i) const;

		inline void assign(const char* string, misc::mmf_write* file);
	};

	
};

namespace misc
{
	string::string()
	{
		pData = NULL;
	}
	string::~string()
	{ 
		if( pData) 
		{
			char* d = pData;
			delete d;
		}
	}

	void string::Mirror(string& arg)
	{
		pData = arg.pData;
		arg.pData = NULL;
	};

	bool string::operator ==(const string& arg) const
	{
		if( !(bool)arg.pData ||
			!(bool)pData) 
				return false;

		return strcmp(arg.pData, pData)==0;
	}
	bool string::operator !=(const string& arg) const
	{
		return !(*this==arg);
	}
	unsigned int string::size() const
	{
		if( !pData) return 0;
		return strlen(pData);
	}
	bool string::empty() const
	{
		if( !pData) return true;
		return pData[0]==0;
	}

	char& string::operator [](int i)
	{
		return pData[i];
	}
	const char& string::operator [](int i) const
	{
		return pData[i];
	}

	void string::assign(const char* str, misc::mmf_write* file)
	{
		if( str==0)
		{
			pData = 0;
			return;
		}

		if(file)
			pData = new (*file) char[strlen(str)+1];
		else
			pData = new char[strlen(str)+1];
		char* dst = pData;
		strcpy(dst, str);
	}
};

