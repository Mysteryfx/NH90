#ifndef __MISC_SWITCH_STRING_H__
#define __MISC_SWITCH_STRING_H__

#include <ed/map.h>
#include "ed_string.h"

namespace misc
{
	// Класс приведения строк к индексу,
	// который можно использовать в switch() инструкциях
	template<class T, class TYPE=char*> class switch_string
	{
		TYPE T::* textmember;
		// Проверка без учета регистра
		struct check_member
		{
			TYPE T::* textmember;
			check_member(){this->textmember = NULL;}
			check_member(TYPE T::* textmember){this->textmember = textmember;}

			bool operator()(const TYPE& x, const TYPE& y) const
			{
				return (stricmp(x, y) < 0);
			}
		};

		typedef ed::map< TYPE, T*, check_member> tag_list;
		tag_list strlist;
	public:
		switch_string( TYPE T::* textmember, T* array=NULL, unsigned int len=0):
			strlist(check_member(textmember))
		{
			this->textmember=textmember;
			if( array) append(array, len);
		};
		void append(T* array, unsigned int len)
		{
			for(int i=0; i<(int)len; i++)
			{
				T* pT = array + i;
				TYPE str = pT->*textmember;
				if( !str) continue;
				strlist[str] = pT;
			}
		}
		T* match(TYPE str) const
		{
			typename tag_list::const_iterator it = strlist.find(str);
			if( it==strlist.end()) return NULL;
			return it->second;
		}
		unsigned int size()
		{
			return strlist.size();
		}
	};
}

#endif