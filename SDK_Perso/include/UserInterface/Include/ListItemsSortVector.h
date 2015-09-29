#pragma once

#include <ed/string.h>
#include <ed/vector.h>
#include "ui_Point.h"

struct ListItemDesc
{
	const char*		String;
	short			RowNumber;
	short			ColumnNumber;
public:
	ListItemDesc(const char* inString, short inRow, short inColumn) :
				String(inString), RowNumber(inRow), ColumnNumber(inColumn)
	{
	}
	friend bool			operator < (const ListItemDesc& inItemA, const ListItemDesc& inItemB)
	{
		return _stricmp(inItemA.String, inItemB.String) < 0;
	}
	friend bool			operator == (const ListItemDesc& inItemA, const ListItemDesc& inItemB)
	{
        return (inItemA.RowNumber == inItemB.RowNumber && 
                inItemA.ColumnNumber == inItemB.ColumnNumber);
		//return _stricmp(inItemA.String, inItemB.String) == 0;
	}
};

typedef ed::vector<ListItemDesc>	ListItemsDesc;

class ListItemsSortVector
{
	ListItemsDesc		m_ListItems;
	bool				m_SortOn;

	ed::string			m_SearchString;
public:
	ListItemsSortVector() : m_SortOn(false)
	{
	}

	int					addItem				(const char* inString, short inRow, short inColumn);
	int					removeItem			(short inRow, short inColumn);
	void				clear				();

	void				sort				();
	Point				search				(const char* inText = 0);

	void				addToSearchString(const ed::string& str) { m_SearchString += str; }
	void				clearSearchString	() { m_SearchString.erase(); } 
};	
