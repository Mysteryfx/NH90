#ifndef __LANDSCAPE3_LSURFACEFILE_H
#define __LANDSCAPE3_LSURFACEFILE_H
#include "../landscape3_config.h"

#include <osg/Vec2i>
#include "edTerrain.h"

#include "lTerraBasic.h"
#include "landscape3/lGeometry.h"
#include <stdarg.h>
#include <stdio.h>


namespace surface3
{
class surface_error;
// Текущая версия
const unsigned int landscape_currentversion = 1;

// Описание квадрата
struct Square : public geometry3::GeometrySource
{
	// части
	struct Part : public geometry3::GeometryItem
	{
		Part& operator =(const geometry3::GeometryItem& item)
		{
			*(geometry3::GeometryItem*)(this) = item;
			return *this;
		}
	};
	// Информация для постройки SideBounds
	struct sidefacedata
	{
		uint16_t* faceref;
		uint16_t face;
	};
	typedef ed::map<float, sidefacedata> tag_sidefacedatalist;

	// смежные тр-ки
	// side:
	// 0 - WEST (-z)
	// 1 - EAST (+z)
	// 2 - NORD (-x)
	// 3 - SOUTH (+x)
	struct FaceNeighbour
	{
		struct faceref
		{
			uint16_t side;		// Индексы сторон начинаются с 1-цы!!!
			// Если side==0 face задает индекс тр-ка
			// иначе номер тр-ка в списке наружних тр-ков
			uint16_t face;

			faceref()
			{
				side=(unsigned short)-1;
				face=0;
			}
			bool isOwnFace()
			{
				return side==0;
			};
			faceref& operator =(unsigned short& arg)
			{
				face = arg;
				side=0;
				return *this;
			};

			// Построить
			EDTERRAIN_API bool FindNeighbour(
			    int level,
			    uint16_t v1, uint16_t v2,
			    li3V* faces, int fcount, int exclude,
			    lv3V* verts, tag_sidefacedatalist* sides,
			    ed::list< surface3::surface_error>& errors);
		};
		faceref t12, t23, t31;	// Индексы смежных тр-ков
		faceref& getfaceref(int edge)
		{
			return *(&t12 + edge);
		}
		bool operator ==(const FaceNeighbour& arg) const
		{
			return memcmp(this, &arg, sizeof(arg))==0;
		}
		bool operator !=(const FaceNeighbour& arg) const
		{
			return !(*this==arg);
		}

		// какое ребро прилегает к границе
		int getEdgeOfSide(int side)
		{
			side++;
			if(t12.side == side) return 0;
			if(t23.side == side) return 1;
			if(t31.side == side) return 2;
			return -1;
		}
	};

	// Запись о тр-ках на границе
	struct SideBoundFaces
	{
		// Тр-ки отсортированные по возр. соответствующей коорд-ты
		io::mmfvector<uint32_t> boundfaces;

		unsigned int getFace(uint32_t i)
		{
			if(i < boundfaces.size())
				return boundfaces[i];
			else
				return UINT_MAX;
		}
	};
	// Исходный файл (чтоб ошибки искать)
	ed::string sourcefilename;

	// Баунд квадрата
	geometry3::BoundData bound;
	// индекс центрального тр-ка
	uint32_t centerFace;

	// Части
	ed::vector<Part> parts;
	// смежные тр-ки
	io::mmfvector<FaceNeighbour> faceNeighbours;
	// смещение на массив исходных нормалей вершин n3V*
	io::mmfvector<ln3V>  sourcenorms;
	// Далее записи о тр-ках на границе
	// 0 - WEST (-z)
	// 1 - EAST (+z)
	// 2 - NORD (-x)
	// 3 - SOUTH (+x)
	SideBoundFaces SideBounds[4];
	// Индексы частей тр-ков
	io::mmfvector<uint32_t> facetypes;
	// Семантики треугольников (если есть этот массив семантики прописаные в Part игнорируются)
	io::mmfvector<uint16_t> facesemantics;	// индекс в массиве semanticlist
	ed::vector<ed::string> semanticlist;

	//////////////////////////////////////
	// Ф-ции
public:
	int getNumParts()
	{
		return (int)parts.size();
	}
	int getNumFaces(int nPart)
	{
		return parts[nPart].nFaces;
	};
	int getNumVrts(int nPart)
	{
		return parts[nPart].nVerts;
	};
	//		lv3V getVert(int nPart, int vert){ return parts[nPart].verts[vert];};
	//		li3V getFaceVert(int nPart, int face){ return parts[nPart].faces[face];};
	//		ltv2V getTVert(int nPart, int vert){ return parts[nPart].tverts[0][vert];};
	//		ln3V getNorm(int nPart, int vert){ return parts[nPart].norms[vert];};

	// Геометрия части квадрата
	Part* getPart(int i)
	{
		return &parts[i];
	}

	int getNumFaces()
	{
		return (int)faces.size();
	};
	li3V* getFaces()
	{
		return faces.begin();
	};
	lv3V* getVrts()
	{
		return verts.begin();
	};
	ltv2V* getTVerts()
	{
		return tverts[0].begin();
	};
	ln3V* getNorms()
	{
		return norms.begin();
	};
	//		ln3V* getSourceNorms(){return sourcenorms.begin();};

	FaceNeighbour* getNeighbour()
	{
		return faceNeighbours.begin();
	};
	//		unsigned int* getTypes(){ return facetypes.begin();};

	// side = 0, 1, 2, 3
	SideBoundFaces* getBoundFaces(int side)
	{
		return &SideBounds[side];
	}

	EDTERRAIN_API unsigned int getFace(FaceNeighbour::faceref& ref);

	// Семантика тр-ка
	EDTERRAIN_API const char* getFaceSemantic(
	    int face);
	// Поиск тр-ка в который попадает точка
	EDTERRAIN_API unsigned int findTrg(
	    unsigned int iCur,
	    float x, float z,
	    const cPoint& p = cPoint());	// debug: square pos

	// Поиск тр-ка в который попадает точка
	EDTERRAIN_API unsigned int findTrg(float x, float z,
	                                   const cPoint& p = cPoint());	// debug: square pos

	// Точка в тр-ке?
	EDTERRAIN_API bool isInTrg(float x, float z, unsigned int face);

	// Получить высоту по координате
	EDTERRAIN_API float getHeight(unsigned int face, float x, float z);
	EDTERRAIN_API float getHeight(float x, float z, const cPoint& p = cPoint());	// debug: square pos

	// Получить параметры треугольника
	EDTERRAIN_API void getFaceSpecs(unsigned int face, cPoint& a, cPoint& b, cPoint& c, cPoint& n);
#ifndef EDGE
	void getFaceSpecs(unsigned int face, osg::Vec3f& a, osg::Vec3f& b, osg::Vec3f& c, osg::Vec3f& n)
	{
		getFaceSpecs(face, *(cPoint*)&a, *(cPoint*)&b, *(cPoint*)&c, *(cPoint*)&n);
	}
#endif
	// Поиск стороны, через которую отрезок выходит из треугольника (в 2D) (1=t12, 2=t23, 3=t31)
	EDTERRAIN_API short segmentSideOut(const cPoint& a, const cPoint& b, unsigned int face, cPoint* pos = 0);

	// Отрезок пересекает треугольник (в 3D)?
	EDTERRAIN_API bool segmentFaceIntersect(const cPoint& a, const cPoint& b, unsigned int face, cPoint* pos = 0);

	// Найти точку пересечения отрезка с квадратом
	EDTERRAIN_API bool segmentSquareIntersect(const cPoint& a, const cPoint& b, FaceNeighbour::faceref& next, cPoint* pos = 0);
#ifndef EDGE
	bool segmentSquareIntersect(const osg::Vec3f& a, const osg::Vec3f& b, FaceNeighbour::faceref& next, osg::Vec3f* pos)
	{
		return segmentSquareIntersect( *(const cPoint*)&a, *(const cPoint*)&b, next, (cPoint*)pos);
	}
#endif
	// Построить разрез поверхности вдоль отрезка, возвращает количество точек в env
	EDTERRAIN_API int envelopeSquare(const cPoint& a, const cPoint& b, cPoint* env, int size, FaceNeighbour::faceref& next);

	// Ф-ции создания
public:
	/*/
	// Копировать квадрат из старого формата
	Square(
		landscape_old::LandscapeSquareSection::Square& src,
		misc::mmf_write& file,
		int level);
	/*/
	EDTERRAIN_API Square();
	EDTERRAIN_API ~Square();
	// Копировать квадрат
	EDTERRAIN_API void Init(
	    Square& src);
	// создать квадрат
	EDTERRAIN_API bool Init(
	    int level,		// Уровень детал.
	    ed::list< geometry3::GeometrySource*>& sources,
	    ed::list< geometry3::GeometryItem*>& Items,
	    ed::list< surface3::surface_error>& errors
	);

	//		void setSourceNorms(ln3V* src, int count, misc::mmf_write* file);
	EDTERRAIN_API void setFaceTypes(unsigned int* src, int count);

	// Сравнить
	EDTERRAIN_API bool match(Square& arg);

	// Построить смежные тр-ки
	// faceNeighbours и SideBounds
	EDTERRAIN_API bool CreateNeighbourInfo(
	    int level,
	    float eps,
	    float minheigth,	// для проверки сингулярности
	    ed::list< surface3::surface_error>& errors);
	// Загрузка
	EDTERRAIN_API void preload();

	// Инициализация после загрузки
	EDTERRAIN_API void InitAfterLoad(
	    material3::Material* materials	// Материалы
	);

};

/*/
// Индекс квадрата
struct SquareKey
{
	// Индекс
	intpoint sqind;
	// Уровень детализации
	int32_t level;
	// Размер (0-10000, 1-20000, 2-40000, ...) 10000*2^(size+1)
	int32_t size;

//		SquareKey():sqind(0, 0){level=0; size=0;}
	bool operator <(const SquareKey& arg) const
	{
		if( sqind < arg.sqind) return true;
		if( arg.sqind < sqind) return false;
		if( level < arg.level) return true;
		if( arg.level < level) return false;
		return (size < arg.size);
	}

};
/*/

////////////////////////////////////////////////
// Файл описания обитаемой области
struct EDTERRAIN_API lSurfaceFile
{
	// Границы обитаемой области в номерах квадратов 10x10 (кратны 2)
	int32_t xMin, xMax;
	int32_t zMin, zMax;

	// Материалы
	ed::vector<material3::Material> materials;

	// Замещающие квадраты
	ed::vector<Square> pOffsideSquares;
	// Массивы квадратов
	struct Level
	{
		// Уровень детализации
		int level;
		// Размер (0-10000, 1-20000, 2-40000, ...) 10000*2^(size+1)
		int size;
		// Границы
		intpoint sqmin, sqmax;
		// Массив квадратов (указателей)
		ed::vector< Square > squarepool;
		// Массив квадратов размерность X*Z (лежит номер в куче squarepool)
		ed::vector< int32_t > pSquares;

		EDTERRAIN_API Level();

		void Init(
		    int level, int size,
		    lSurfaceFile* pSurfaceFile
		);

		// Получить квадрат по индексу
		Square* getSquare(intpoint sqindex)   // deprecated - use osg version instead
		{
			if(sqindex.x < sqmin.x || sqindex.x >= sqmax.x)
				return NULL;
			if(sqindex.z < sqmin.z || sqindex.z >= sqmax.z)
				return NULL;

			int i = (sqindex.x - sqmin.x) * (sqmax.z - sqmin.z) + (sqindex.z - sqmin.z);
			int32_t index = pSquares[i];
			if(index < 0 || index >= (int)squarepool.size())
				return NULL;

			return &squarepool[index];
		}

		Square* getSquare(const osg::Vec2i& squareIndex)
		{
			if(squareIndex.x() < sqmin.x || squareIndex.x() >= sqmax.x)
				return NULL;
			if(squareIndex.y() < sqmin.z || squareIndex.y() >= sqmax.z)
				return NULL;

			int i = (squareIndex.x() - sqmin.x) * (sqmax.z - sqmin.z) + (squareIndex.y() - sqmin.z);
			int32_t index = pSquares[i];

			if(index < 0 || index >= (int)squarepool.size())
				return NULL;

			return &squarepool[index];
		}
	};
	// Уровени детализации
	ed::vector<Level> pLevels;

	// Высоты квадратов
	struct Heigth
	{
		// Границы
		intpoint sqmin, sqmax;
		// Высоты квадратов
		io::mmfvector<float> pHeigths;

		void Init(
		    int size,
		    lSurfaceFile* pSurfaceFile
		);

		float* getHeigth(intpoint sqindex)
		{
			if(sqindex.x < sqmin.x || sqindex.x >= sqmax.x)
				return NULL;
			if(sqindex.z < sqmin.z || sqindex.z >= sqmax.z)
				return NULL;

			int i = (sqindex.x - sqmin.x) * (sqmax.z - sqmin.z) + (sqindex.z - sqmin.z);
			return &pHeigths[i];
		}

		float* getHeight(const osg::Vec2i& squareIndex)
		{
			if(squareIndex.x() < sqmin.x || squareIndex.x() >= sqmax.x)
				return NULL;
			if(squareIndex.y() < sqmin.z || squareIndex.y() >= sqmax.z)
				return NULL;

			int i = (squareIndex.x() - sqmin.x) * (sqmax.z - sqmin.z) + (squareIndex.y() - sqmin.z);
			return &pHeigths[i];
		}

		void Build(Heigth* prev, Level* data);
	};
	ed::vector<Heigth> pLevelHeights;

	// Функции доступа
public:
	// Открыть файл
	bool Load(
	    io::MmfStream& mmfStream
	);
	bool Save(
	    const char* filename
	);
	bool Serialize(io::Stream& file);

	void InitAfterLoad();

	// Границы области
	intpoint getMinSquare();
	intpoint getMaxSquare();

	/**
	 * getSquare
	 *
	 * Возвращает поинтер на структуру квадрата
	 * @param squareIndex индекс квадрата
	 * @param level       лод квадрата
	 * @return            поинтер на квадрат
	 */
	Square* getSquare(intpoint sqind, int level); // deprecated - use osg version instead
	Square* getSquare(const osg::Vec2i& squareIndex, int level);

	/**
	 * getSquareHeight
	 *
	 * Получить высоту квадрата
	 * @param squareIndex индекс квадрата
	 * @param size        размер квадрата. НЕ ЛОД! size = GetSquareSize(level)
	 * @return            высота квадрата
	 */
	float getSquareHeight(intpoint sqind, int size); // deprecated - use osg version instead
	float getSquareHeight(const osg::Vec2i& squareIndex, int size);

	// Запрос замещающего квадрата
	Square* getOffsideSquare(int level);

	// Есть ли море в заданом квадрате
	bool IsSea(const Box& testbox);

	// Функции редактирования
public:
	lSurfaceFile();

public:
	// Построить высоты квадратов
	void BuildHeigths(
	    int maxsize
	);
	// Проверить граничные треугольники
	bool CheckFaceBounding(
	    intpoint sqindex,
	    ed::list< surface3::surface_error>& errors
	);
protected:
	bool CheckFaceBounding(
	    Square* square,
	    Square* matchsquare,
	    int side,
	    ed::list< surface3::surface_error>& errors
	);
};

// Класс ошибки при выгрузке пов-ти
class surface_error
{
public:
	cVector pos;
	char str[256];
public:
	surface_error() {};
	surface_error(const char* arg, ...)
	{
		this->pos = cVector(0.0f, 0.0f, 0.0f);
		va_list argList; va_start(argList, arg);
		sprintf(str, arg, argList);
		va_end(argList);
	}
	surface_error(cVector pos, const char* arg, ...)
	{
		this->pos = pos;
		va_list argList; va_start(argList, arg);
		vsprintf(str, arg, argList);
		va_end(argList);
	}
};

}

//#include "landscape3/lSurfaceFile.inl"
#endif