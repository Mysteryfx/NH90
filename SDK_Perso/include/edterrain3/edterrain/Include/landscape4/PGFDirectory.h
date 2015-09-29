#ifndef PGFDIRECTORY_H__
#define PGFDIRECTORY_H__
#include "../landscape3_config.h"

#include "edTerrain.h"

#include "PGF/PGFimage.h"
#include "osg/RectQuadtree.h"
#include "osg/Vec2i"

namespace Lua
{
class Config;
}

class EDTERRAIN_API PGFDirectory
{
public: 
	PGFDirectory();
	~PGFDirectory();

// Методы 
public: 
	// Инициализация
	bool init(Lua::Config& config);
	// 
	bool exit();

	osg::Vec3d getOrigin();
	float getPixelSize();
	bool loadData(void* dst, int dstsize, int dstbpp, const osg::Rect& rect, int level);
	
// inhouse data
protected:
	// Размер пикселя 0-го уровня
	double pixelsizeOfLevel0;
	// Начало координат
	osg::Vec3d origin;

	// список открытых pgf файлов
	struct PGFitem
	{
		PGFitem();
		~PGFitem();
	public:
		ed::string filename;

		HANDLE fd;
		CPGFImage pgf;
		CPGFFileStream stream;

		// данные из файла
		double pixelsize;
		// регион текстуры минимального уровня
		osg::Rect rect;
		// число слоев
		int levelcount;

		// минимальный уровень. расчитывается в buildPgfLevels()
		int minlevel;
	};
	ed::list<PGFitem> pgfs;

	// tree item
	struct PGFtreeitem
	{
		PGFitem* pgfitem;
		// уровень
		int level;
		// rect
		osg::Rect rect;
	public:
		const osg::Rect& getRect()const{return rect;};
	};
	// индекс для поиска файлов попадающих в регион требуемого уровня
	struct PGFLevel
	{
		// индекс файлов этого уровня
		ed::RectQuadtree<PGFtreeitem> pgfs;
	};
	ed::vector<PGFLevel> pgflevels;
	

protected:
	// Открыть Pgf
	bool AddPgfFile(const char* filename);

	// Построить индексы после загрузки всех файлов
	void buildPgfLevels();
};

#endif