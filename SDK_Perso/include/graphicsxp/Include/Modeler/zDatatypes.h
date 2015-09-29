#ifndef __GEOMETRY_DATATYPES_H_
#define __GEOMETRY_DATATYPES_H_

#pragma pack(push,1)

typedef unsigned int  INSTANCE;
#define ENAMECSZ 28

struct ENAME {
	INSTANCE	instance;
	char	name[ENAMECSZ];
};

// Abstract vectors	///////////////////////////////////////////////////

struct  v3V{	float x,y,z;};
struct  n3V{	float nx,ny,nz; };
struct  fn3V{float fnx,fny,fnz;};
struct  tv2V{float s,t; };
struct  i3V{	__int16 i1,i2,i3;};
struct  i2V{	__int16 i1,i2;};
struct  cV{	char	R,G,B;};
struct t24V {char R,G,B;};
struct t32V {char R,G,B,A;};
struct t555V{__int16	rgb555;};
struct t565V{__int16	rgb565;};
typedef unsigned char t8V;
struct scV{ INSTANCE instance;};
struct LODV{ INSTANCE instance; unsigned int udistance;};

// these vectors are one per cache

struct boxV {
	float x,y,z,X,Y,Z,R,r;
};

const unsigned int M_IDENT		=0x1;
const unsigned int M_NOTRANS	=0x2;
const unsigned int M_NOROT		=0x4;
const unsigned int M_DESTROY	=0x8;
const unsigned int M_UNDEF		=0x10;

struct m16V {
	double	m[16];
	unsigned int mflags;
};

struct matV {
	float aR,aG,aB,aA,dR,dG,dB,dA,sR,sG,sB,sA;
	int SolidColor;
};


struct	thV {
	INSTANCE	pixels;
	char texname[32];
	int x;
	int	y;
	int startoffset;
	int	stride;
};

struct	ind_thV {
	unsigned int index;
};

struct all_thV {
	char filename[32];
};

struct strategyV {
	int strategy;
	int reserve[3];
};

struct layerV {
	int layer;
};

const unsigned int DW_Z		=0x00000001;
const unsigned int DW_LIGHT	=0x00000002;
const unsigned int DW_BLEND	=0x00000004;
const unsigned int DW_SPRITE	=0x00000008;
const unsigned int DW_TEXTURE	=0x00000010;
const unsigned int DW_M16		=0x00000020;;
const unsigned int DW_BOX		=0x00000040;

struct drawV {
	unsigned int drawflags;
};

struct semanticsV {
	unsigned int	iType;
};

enum TRANSFORM {TRA,ROT,SCA,NOP,BAD};
struct kinematicsV {
	TRANSFORM	transform;
	float X1, Y1, Z1; // Координаты точки - начала оси
	float X2, Y2, Z2; // Координаты точки - конца оси
	float RealMin, RealMax; // Границы изменения характеристики
	float ParamMin, ParamMax; //Границы применимости параметра (и видимости) 
	float reserved;
	__int16 ParamNo; // Номер кинематического параметра (по списку)
	__int16 ParamOption; // Как использовать параметр ( arg или 1-arg)
};

struct commandV {
	unsigned int CommandCode;
};

struct CollisionHeaderV {
	INSTANCE DataInstance; //Привязка данных
	__int16 TypeX, TypeY, TypeZ; //Размерность "грубого" массива столкновений
	float STEP_X, STEP_Y, STEP_Z;  //Шаг (размеры ячейки грубой сетки)
	__int16 nX, nY, nZ;	//Размерность точного массива столкновений
	float step_x, step_y, step_z; //Шаг (размеры ячейки точной сетки)
	float x,y,z,X,Y,Z; //Пространственный Вох модели, включаемый в расчеты
	// Не путать с обычным боксом - этот может быть меньше (нет парашютов и т.д.)
};

struct CollisionV {
	__int16 index;	//Индекс ячейки в "грубом" массиве столкновений
};

struct ChPointsV {
	float StartX, StartY, StartZ, EndX, EndY, EndZ; // Координаты характеристического отрезка
	float length; // Длина характерного отрезка
};

const unsigned int ARG_OPERATION_ALFA=0x0;

struct ArgDependentV {
	int type; //тип зависимости (blend, etc)
	int ArgNo; //номер аргумента, от к-рого зависит
	int reserved1;
	int reserved2;
};

struct LandVertexV
{
	float x,y,z;
	float nx,ny,nz;
	float u,v;
};

struct LandFacetV	// Triangle connection and type data
{
	// t1, t2, t3 - triangle side neighbours (indices in containing square)
	// s1, s2, s3 - relative neighbour square references:
	//		0 - current, 1 - North, 2 - East, 3 - South, 4 - West
	__int16 t1,s1,t2,s2,t3,s3;
	__int16 type;	// defines triangle base surface semantics (0 - land, 1 - water)
};

////////////////////////////////////////////////////////////////////////////////
// old-style properties
struct Scalar {
public:
	float aR,aG,aB,aA,dR,dG,dB,dA,sR,sG,sB,sA;
	int SolidColor;
	float x,y,z,X,Y,Z;
	float distance;
	char texname[32];
	int drawflag;
	int strategy;
	int layer;
	float subdist[4];		// LOD distances
	int	rawdata[4];
	int	reserve[1];			// reserved to SIZEOFSCALAR bytes boundary
};

struct oldscV{ char name[32]; float x,y,z,X,Y,Z; double m[16];};

////////////////////////////////////////////////////////////////////////////////
// Geometry data types

enum DATATYPE {STARTDATA=0, V=0, I3, NV, FN, TV, C, T24, T32, T555, T565, SC,
				LR, CE, I2,
				REF, IND_TH, ALL_TH, VL,
				I3L, FN2, FN3, FN4,
				I21, I22, I23, I24,
				OB, BX, MAT, TH, ST, LD,
				M16, DW, CMD, SEM, COL, COLD, CHP,
				ARG, STOPDATA};

const unsigned int typesize[]={ sizeof(v3V),sizeof(i3V),sizeof(n3V),
					sizeof(fn3V),sizeof(tv2V),sizeof(cV),sizeof(t24V),
					sizeof(t32V),sizeof(t555V),sizeof(t565V),sizeof(scV),
					sizeof(layerV),sizeof(kinematicsV),sizeof(i2V),
					sizeof(ENAME),sizeof(ind_thV),sizeof(all_thV),sizeof(LandVertexV),
					sizeof(LandFacetV),0,0,0,
					0,0,0,0,
					0,sizeof(boxV),sizeof(matV),sizeof(thV),sizeof(strategyV),
					sizeof(LODV),sizeof(m16V),sizeof(drawV), sizeof(commandV),
					sizeof(semanticsV), sizeof(CollisionHeaderV), sizeof(CollisionV),
					sizeof(ChPointsV), sizeof(ArgDependentV)};
////////////////////////////////////////////////////////////////////////////////

#pragma pack(pop)

#endif
