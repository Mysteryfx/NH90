#pragma once

#include <ed/vector.h>
#include <ed/list.h>

struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ColumnsPrms
{
	ColumnsPrms(float newWidth, float newDist, ed::string newItemSet = ed::string("")) 
						: width(newWidth), dist(newDist), itemSet(newItemSet)
	{
	}
	ColumnsPrms()
	{
	}
	float		width; //Ширина колонки
	float		dist;  //Расстояние для следующей
	ed::string itemSet;
};

struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR RowsPrms
{
	RowsPrms(float newHeight, float newDist) : height(newHeight), dist(newDist)
	{
	}	
	RowsPrms()
	{
	}
	float		height;
	float		dist;
};

struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ColumnParameters : public ColumnsPrms
{
	ColumnParameters()
	{
	}
	float		begin;
	float		end;
	float		lineX;
	ed::string itemSet;
};

struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR RowParameters : public RowsPrms
{
	RowParameters()
	{
	}
	float		begin;
	float		end;
	float		lineY;
};

typedef ed::vector<ColumnParameters> ColumnsParameters;
typedef ed::vector<RowParameters> RowsParameters;

typedef ed::vector<Static*> Row;
typedef ed::vector<Row*> Table;

typedef ed::vector<int> RowAligns;
typedef ed::vector<RowAligns> Aligns;

typedef ed::list<ed::string> ItemsNames;

struct Marker
{
	int		xShift;
	int		yShift;
	bool	proportional;	// true - имеет размер строки за вычетом yShift, 
};							// false - фиксированный размер height
