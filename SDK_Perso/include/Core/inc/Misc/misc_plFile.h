#pragma once


#include "misc/misc_mmf.h"

#include "misc/misc_mmf_wr.h"
#include "misc/misc_ptr.h"
#include "misc/misc_vector.h"
#include "misc/misc_string.h"

namespace misc
{
	const unsigned int pl_currentversion = 0;
	const char pl_current_filetype[4] = {'p', 'l', '2', ' '};

	struct PlPoint
	{
		float x, y, z;
	};
	struct Polyline
	{
		misc::vector<PlPoint> points;
		misc::ptr<Polyline> next;

		Polyline()
		{
			next = NULL;
		}
		template<class POINT> Polyline(const POINT& pt, misc::mmf_write& write)
		{
			next = NULL;
			points.resize(1, write);
			PlPoint& dst = points[0];
			dst.x = pt.x;			
			dst.y = pt.y;			
			dst.z = pt.z;			
		}
		const PlPoint& getPoint(int i) const 
		{
			return points[i];
		}
	};
	struct Shape
	{
		misc::string name;
		unsigned int color;
		misc::ptr<Polyline> lines;
		misc::ptr<Shape> next;

		Shape(){next=NULL;lines=NULL;color=0;}

		template<class POINT> void InsertPoint(const POINT& pt, misc::mmf_write& write)
		{
			Polyline* line = new (write) Polyline(pt, write);
			line->next = lines;
			lines = line;
		}
		Polyline* InsertLine(misc::mmf_write& write)
		{
			Polyline* line = new (write) Polyline();
			line->next = lines;
			lines = line;
			return line;
		}
	};
	class PlFile2
	{
	public:
		// Тип файла
		char filetype[4];
		// Версия 
		unsigned int version;
		// First shape
		misc::ptr<Shape> begin;

		PlFile2()
		{ 
			memcpy( filetype, pl_current_filetype, 4); 
			version = pl_currentversion;
			begin = NULL;
		}
		Shape* InsertShape(misc::mmf_write& write)
		{
			Shape* shape = new (write) Shape();
			shape->next = begin;
			begin = shape;
			return shape;
		}
		static PlFile2* Open(
			const char* filename, 
			misc::mmf& file)
		{
			if( !file.open(filename))
				return NULL;
			PlFile2* lroadfile;
			if( !file.get(0, lroadfile))
				return NULL;

			if( memcmp( lroadfile->filetype, pl_current_filetype, 4)!=0)
				return NULL;

			if( lroadfile->version != pl_currentversion)
				return NULL;

			return lroadfile;
		}
	};
}

namespace misc
{
	struct PlFileHeader
	{
		int numShape;
		// unsigned int [numShape]

		struct Point
		{
			float x, y, z;
		};

		struct Polyline
		{
			int numPoint;
			// Point[numPoint]
			Point& getPoint(int i) const 
			{
				return *((Point*)(this+1) + i);
			}
		};

		struct Shape
		{
			char name[128];
			int numPolyline;
			// unsigned int [numPolyline]

			Polyline* getPolyline(int i) const
			{
				unsigned int offset = *((unsigned int*)(this+1) + i);
				if( !offset) return 0;
				return (Polyline*)( (char*)this + offset);
			}
		};

		Shape* getShape(int i) const
		{
			unsigned int offset = *((unsigned int*)(this+1) + i);
			if( !offset) return 0;
			return (Shape*)( (char*)this + offset);
		}
	};

	class PlFile
	{
	protected:
		// Основной файл земли
		misc::mmf theMMF;
	public:
		// Открыть файл
		PlFileHeader* Open(const char* file)
		{
			PlFileHeader* header = NULL;
			try
			{
				if( !theMMF.open(file)) return false;
				theMMF.get(0, header);
			}
			catch(const misc::error&)
			{
			}
			return header;
		}
	};
}

