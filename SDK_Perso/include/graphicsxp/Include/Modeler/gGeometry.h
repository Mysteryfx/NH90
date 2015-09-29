/*
//	Файл:				gGeometry.h
//	Подсистема:			Graphics (interface)
//
//	Опасание структур, предназначенных для хранения 
//	геометрии объектов.
//
*/

#ifndef __gGeometry_h__
#define __gGeometry_h__

#include	 <stddef.h>

// Константы определения типа примитивов

const unsigned char		gPrimitivePoint		= 1; // Точки
const unsigned char		gPrimitiveLine		= 2; // Линии
const unsigned char		gPrimitiveTriangle	= 3; // Треугольники
const unsigned char		gPrimitiveText2D	= 4; // Двухмерный текст
const unsigned char		gPrimitiveMarker	= 6; // Маркер (битовая маска)
const unsigned char		gPrimitiveRect2D	= 7; // Прямоугольник в СК экрана.
const unsigned char		gPrimitiveText2DTT	= 8; // Двухмерный текст True Type 
const unsigned char		gPrimitiveText2DTTn	= 9; // Массив Text2DTT

// флаги рисования части

const unsigned int		gDrawZTest			= 0x00000001;	// Устаревший
const unsigned int		gDrawLighting		= 0x00000002;	// Нужна освещенность
const unsigned int		gDrawBlend			= 0x00000004;	// Нужна прозрачность
const unsigned int		gDrawSprite			= 0x00000008;	// Нужен дополнительный поворот на камеру
const unsigned int		gDrawTexture		= 0x00000010;	// Устаревший
const unsigned int		gDrawForceTexture	= 0x00000020;	// Устаревший
const unsigned int		gDrawTextRight		= 0x00000040;	// Устаревший
const unsigned int		gDrawTextCenter		= 0x00000080;	// Устаревший
const unsigned int		gDrawFilterTexture	= 0x00000100;	// Нужна фильтрация текстур.
const unsigned int		gDrawBillBoard	    = 0x00000200;	// Ориентация оси X на камеру, вращением вокруг оси Y.
const unsigned int		gDrawMipmapping		= 0x00000400;	// Включение mipmapping-а.
const unsigned int		gDrawNoCulling		= 0x00000800;	// Отключение проверки ориентации.
const unsigned int		gDrawStripe			= 0x00001000;	// Идентификация Stripe.
const unsigned int		gDrawFan			= 0x00002000;	// Идентификация Fan.
const unsigned int		gDrawTCoords		= 0x00004000;	// Флаг преобразования текстурных координат.
const unsigned int		gDrawNoMipmapping	= 0x00008000;	// Выключение mipmapping-а.
const unsigned int		gDrawNoMelt			= 0x00010000;	// Нерастворяемые точки и линии.
const unsigned int		gDrawAlphaOp		= 0x00020000;	// Флаг альфа операции.

// Цвет
struct gColorRGB {			
	float		r;
	float		g;
	float		b;
};

// Метериал
struct gMaterial {			
	gColorRGB		ambientColor;
	gColorRGB		diffuseColor;
	gColorRGB		specularColor;
	float			alpha;
	unsigned char	textureCount;
	char			textureFileName[8][32];
	int				textureName[8];
};

// ************************************************************
// Flexible vertex format

// Типы вершин
typedef enum gVertexType {
	gv3D_T2F_N3F_V3F,
//	gv3D_T2F_N3S_V3F,
	gv3D_N3F_V3F,
//	gv3D_N3S_V3F,
	gv3D_V3F,
	gv3D_V2F,
	gv3D_T2F_V2F,
};

// Отдельный массив в структуре вершин
struct gStrideFVF {
	void			*data;
	unsigned int	stride;						// Должен быть равет 0
};

// Вершины
struct gVertexFVF {
	gStrideFVF		position;
	gStrideFVF		normal;
	gStrideFVF		diffuse;
	gStrideFVF		specular;
	gStrideFVF		textureCoords[8];
};

// Aditional structures for conversion
struct gCoords2f {
	float	x;
	float	y;
};

struct gNormal3f {
	float	nx;
	float	ny;
	float	nz;
};

struct gBox				//	sizeof(gBox) = 32
{						//
float	  x,y,z,X,Y,Z;	//	Габаритные координаты объекта.
float			    R;	//	Радиус описанной сферы.
float		 Distance;	//	Дистанция  вырождения объекта(проекция в СК экрана меньше минимальной).
};			
//	Определение дистанций:
//					      size_x
//	Distance = Rbox*--------------------
//				    2.*tan(A)*minPixels1
//
//					      size_x
//	Где:	Rbox		-	Радиус описанной окружности (БД).
//			size_x		-	Размер изображения в пикселах по оси X.
//			A			-	Половина угла обзора (22.5 градуса).
//			minPixels1	-	Минимальное количество отображаемых пикселов (==1).
//			minPixels2	-	Количество пикселов при котором велика вероятность
//							вырождения отдельных треугольников в точку (==20).

// ************************************************************

struct	gColor4f {float	r,g,b,a;	// Цвет.

gColor4f ()										{}
gColor4f (float  f)								{r=   f;g=   f;b=   f;a=   f;}
gColor4f (float _r,float _g,float _b,float _a)	{r=  _r;g=  _g;b=  _b;a=  _a;}

inline gColor4f& operator = (float							  f){r=   f;g=   f;b=   f;a=   f; return *this;}
inline gColor4f& operator = (gColor4f						  c){r=c. r;g=c. g;b=c. b;a=c. a; return *this;}
inline gColor4f& operator = (gColor4f*						  c){r=c->r;g=c->g;b=c->b;a=c->a; return *this;}
inline	void operator ()(float _r,float _g,float _b,float _a){r=  _r;g=  _g;b=  _b;a=  _a;}
};

struct	gPrimitive {};

struct	gLine : gPrimitive {			// Линия как массив индексов вершин
unsigned short	index[2];
};

struct	gRect2D : gPrimitive	//	Прямоугольник в СК экрана с текстурой.
{
char	 *FileName;	// Имя файла текстуры.
int			 index; // Индекс текстуры.
int			filter; // Флаг включения билинейной фильтрации.
gColor4f	colorD; // Diffuse	 - составляющая цвета материала.
float  x, y,  X, Y;	// Габаритные координаты прямоугольника.
float tx,ty, tX,tY;	// Соответствующие координаты тектуры.
};

struct	gLine2D : gPrimitive	//	Линии в СК экрана.
{
gColor4f	colorD; // Diffuse	 - составляющая цвета материала.
gCoords2f*	 coord; // Указатель на массив координат вершин в СК экрана.
gLine	 *	 index; // Указатель на массив индексов вершин линий.
int			 vnumb; // Количество вершин.
int			 pnumb; // Количество линий.
};

struct gMesh {
	__int32			drawingFlags;	// Флаги рисования части
	gMaterial		material;		// Указатель на описание материала части
	unsigned char	solidColor;
	unsigned int	dispList;		// Идентификатор Display List этой части
	gVertexType		type;			// Тип, в котором указаны точки
	unsigned short	nVertex;		// Количество вершин
	gVertexFVF		vertices;		// Массив вершин
	unsigned char	primitiveType;	// Тип используемых примитивов
	unsigned short	width;			// Толщина точек или линий в пикселах
	unsigned short	nPrimitives;	// Количество примитивов
	gPrimitive*		primitives;		// Массив примитивов
	gNormal3f*		faceNormals;	// Массив нормалей 
	gBox			box;
};

#endif
