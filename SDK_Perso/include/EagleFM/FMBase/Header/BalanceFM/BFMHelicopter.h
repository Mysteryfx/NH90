#ifndef HELICOPTER_H
#define HELICOPTER_H

#include "Base.h"
#include "BFMStructs.h"
//------------------------------------------------------------------------------
#include <memory>

namespace EagleFM
{
namespace BalanceFM
{

#ifndef ATM_T0
#define ATM_T0 288.15f
#endif
#ifndef ATM_P0
#define ATM_P0 760.f
#endif
#ifndef ATM_R0
#define ATM_R0 1.225f
#endif


#define GRAV 9.81f

#define MAX_COEFS_POINTS_COUNT 256
#define MAX_CALIB_POINTS_COUNT 64
// 
#define MAX_TIME_INTERVAL 0.05f
// 
// #define MI8_AUTOPILOT_KTETA  1.0f
// #define MI8_AUTOPILOT_ITETA  0.26f
// #define MI8_AUTOPILOT_IWZ    0.36f
// 	//#define MI8_AUTOPILOT_IWZ    0.32f
// 
// #define MI8_AUTOPILOT_KGAMMA 0.5f//0.33f//3.f//
// 	//#define MI8_AUTOPILOT_KGAMMA 0.3f
// #define MI8_AUTOPILOT_IGAMMA 0.15f
// 	//#define MI8_AUTOPILOT_IGAMMA 0.25f
// #define MI8_AUTOPILOT_IWX    0.15f
// 	//#define MI8_AUTOPILOT_IWX    0.25f
// #define MI8_AUTOPILOT_IPSI   0.21f
// #define MI8_AUTOPILOT_IWY    0.4f
// 
// #define MI8_CMD_TRIMMER       0x1
// #define MI8_CMD_FOOT          0x2
// #define MI8_CMD_AP_CHN_TETA   0x4
// #define MI8_CMD_AP_CHN_GAMMA  0x8
// #define MI8_CMD_AP_CHN_PSI    0x10
// #define MI8_CMD_AP_CHN_HEIGHT 0x20
// 
// #define MI8_RM_D    25.0f
// #define MI8_RM_A    0.002f
// #define MI8_RM_B    2.0f
// #define MI8_RM_C    0.125f
// #define MI8_BOOSTER_D    67.0f
// #define MI8_BOOSTER_A    0.01f
// #define MI8_BOOSTER_B    4.0f
// 
// #define MI8_MAIN_ROTOR_DELAY    0.08f
// 
// #define MI8_CHASSIS_RIGHT_X    -0.94f
// #define MI8_CHASSIS_RIGHT_Y    -2.25f
// #define MI8_CHASSIS_RIGHT_Z    2.225f
// 
// #define MI8_CHASSIS_LEFT_X     -0.94f
// #define MI8_CHASSIS_LEFT_Y     -2.25f
// #define MI8_CHASSIS_LEFT_Z     -2.225f
// 
// #define MI8_CHASSIS_FORWD_X    3.341f
// #define MI8_CHASSIS_FORWD_Y    -2.25f
// #define MI8_CHASSIS_FORWD_Z    0.0f

class FMBASE_API THelicopterData 
{
public:
	float	Autopilot_KTETA;
	float	Autopilot_ITETA;
	float	Autopilot_IWZ;
	float	Autopilot_KGAMMA;
	float	Autopilot_IGAMMA;
	float	Autopilot_IWX;
	float	Autopilot_IPSI;
	float	Autopilot_IWY;

	int		CMD_TRIMMER;
	int		CMD_FOOT;
	int		CMD_AP_CHN_TETA;
	int		CMD_AP_CHN_GAMMA;
	int		CMD_AP_CHN_PSI;
	int		CMD_AP_CHN_HEIGHT;

	float	RM_D;
	float	RM_A;
	float	RM_B;
	float	RM_C;

	float	BOOSTER_D;
	float	BOOSTER_A;
	float	BOOSTER_B;

	float	MAIN_ROTOR_DELAY;

	float	CHASSIS_RIGHT_X;
	float	CHASSIS_RIGHT_Y;
	float	CHASSIS_RIGHT_Z;

	float	CHASSIS_LEFT_X;
	float	CHASSIS_LEFT_Y;
	float	CHASSIS_LEFT_Z;

	float	CHASSIS_FORWD_X;
	float	CHASSIS_FORWD_Y;
	float	CHASSIS_FORWD_Z;
	
	virtual ~THelicopterData(){};
protected:
	THelicopterData();
	friend class	THelicopter;
};
//-----------------------------------------------------------
//                      вектора
// вектор управления от летчика
typedef struct TPilotVector
{
	float Xb;	// отклонение ручки продольное
	float Xc;	// отклонение рычага общего шага
	float Xk;	// отклонение ручки поперечное
	float Xn;	// отклонение педалей
	unsigned long Buttons; // кнопки
}
TPilotVector;

// вектор управления
typedef struct TControlVector
{
	float Xi;	// отклонение АП продольное
	float Fc;	// общий шаг
	float Nu;	// отклонение АП поперечное
	float Ft;	// шаг рулевого винта
}
TControlVector;

//-----------------------------------------------------------
//                      коэффициенты

// продольное движение
typedef struct TCoefLong
{
	float  X_Vy;
	float dX_Wz;
	float  X_Xi;
	float  X_Fc;
	float  Y_Vy;
	float dY_Wz;
	float  Y_Xi;
	float  Y_Fc;
	float  Mz_Vy;
	float  Mz_Wz;
	float  Mz_Xi;
	float  Mz_Fc;
}
TCoefLong;

// боковое движение
typedef struct TCoefLat
{
	float  Z_Vz;
	float dZ_Wx;
	float dZ_Wy;
	float  Z_Nu;
	float  Z_Ft;
	float  Z_Fc;
	float  Mx_Vz;
	float  Mx_Wx;
	float  Mx_Wy;
	float  Mx_Nu;
	float  Mx_Ft;
	float  Mx_Fc;
	float  My_Vz;
	float  My_Wx;
	float  My_Wy;
	float  My_Nu;
	float  My_Ft;
	float  My_Fc;
}
TCoefLat;

// влияние продольного движения на боковое
typedef struct TCoefLongToLat
{
	float  Z_Vy;
	float  Z_Wz;
	float  Z_Xi;
	float  Mx_Vy;
	float  Mx_Wz;
	float  Mx_Xi;
	float  My_Vy;
	float  My_Wz;
	float  My_Xi;
}
TCoefLongToLat;

// влияние бокового движения на продольное
typedef struct TCoefLatToLong
{
	float  X_Vz;
	float  X_Wx;
	float dX_Wy;
	float  X_Nu;
	float  X_Ft;
	float  Y_Vz;
	float dY_Wx;
	float  Y_Wy;
	float  Y_Nu;
	float  Y_Ft;
	float  Mz_Vz;
	float  Mz_Wx;
	float  Mz_Wy;
	float  Mz_Nu;
	float  Mz_Ft;
}
TCoefLatToLong;

// балансировки
typedef struct TBalance
{
	float Vy;
	float Xi;
	float Nu;
	float Fc;
	float Ft;
	float Teta;
	float Gamma;
}
TBalance;

// коэффициенты
typedef struct TCoefs
{
	float V;

	TCoefLong      	Long;
	TCoefLat       	Lat;
	TCoefLongToLat	Long2Lat;
	TCoefLatToLong	Lat2Long;
	TBalance        Bal;
}
TCoefs;

typedef struct TCoefBase
{
	TCoefLong Long[MAX_COEFS_POINTS_COUNT];
	float VLong[MAX_COEFS_POINTS_COUNT];
	int LongCount;

	TCoefLat Lat[MAX_COEFS_POINTS_COUNT];
	float VLat[MAX_COEFS_POINTS_COUNT];
	int LatCount;

	TCoefLongToLat Long2Lat[MAX_COEFS_POINTS_COUNT];
	float VLong2Lat[MAX_COEFS_POINTS_COUNT];
	int Long2LatCount;

	TCoefLatToLong Lat2Long[MAX_COEFS_POINTS_COUNT];
	float VLat2Long[MAX_COEFS_POINTS_COUNT];
	int Lat2LongCount;

	TBalance Bal2[MAX_COEFS_POINTS_COUNT][MAX_COEFS_POINTS_COUNT];
	float VxBal[MAX_COEFS_POINTS_COUNT];
	float VzBal[MAX_COEFS_POINTS_COUNT];
	int BalXCount,BalZCount;

}
TCoefBase;

//-----------------------------------------------------------
//       тарировки управления (АП, шаг РВ и НВ)

// АП, продольный канал
typedef struct TCalibLong
{
	float Xi,Xb;
}
TCalibLong;

// АП, поперечный канал
typedef struct TCalibLat
{
	float Nu,Xk;
}
TCalibLat;

// Рулевой Винт
typedef struct TCalibTailRotor
{
	float Ft,Xn;
}
TCalibTailRotor;

// Несущий Винт
typedef struct TCalibCollective
{
	float Fc,Xc;
}
TCalibCollective;

// Тарировки
typedef struct TCalibs
{
	TCalibLong       Long[MAX_CALIB_POINTS_COUNT];
	TCalibLat        Lat[MAX_CALIB_POINTS_COUNT];
	TCalibTailRotor  Tail[MAX_CALIB_POINTS_COUNT];
	TCalibCollective Coll[MAX_CALIB_POINTS_COUNT];
	unsigned short LongCount;
	unsigned short LatCount;
	unsigned short TailCount;
	unsigned short CollCount;
}
TCalibs;

//-----------------------------------------------------------
// вспомогательные структуры
// матрицы поворота
class TMatrix
{
	float xx, xy, xz;
	float yx, yy, yz;
	float zx, zy, zz;

public:
	TMatrix()
	{
		xx=yy=zz=1.0f;
		xy=xz=yx=yz=zx=zy=0.0f;
	};
	void Set(TSinus sin, TCosinus cos)
	{
		xx= cos.Teta *cos.Psi                              ;
		xy= sin.Gamma*sin.Psi - sin.Teta*cos.Gamma*cos.Psi ;
		xz= cos.Gamma*sin.Psi + sin.Teta*sin.Gamma*cos.Psi ;
		yx= sin.Teta                                       ;
		yy= cos.Teta*cos.Gamma                             ;
		yz= -cos.Teta*sin.Gamma                            ;
		zx= -cos.Teta*sin.Psi                              ;
		zy= sin.Gamma*cos.Psi + sin.Teta*cos.Gamma*sin.Psi ;
		zz= cos.Gamma*cos.Psi - sin.Teta*sin.Gamma*sin.Psi ;
	};
	void Turn(float  x_in , float  y_in , float  z_in,
		float* x_out, float* y_out, float* z_out)
	{
		if(x_out)
			*x_out= xx* x_in + xy* y_in + xz* z_in;
		if(y_out)
			*y_out= yx* x_in + yy* y_in + yz* z_in;
		if(z_out)
			*z_out= zx* x_in + zy* y_in + zz* z_in;
		return;
	};
	void TurnD(double  x_in , double  y_in , double  z_in,
		double* x_out, double* y_out, double* z_out)
	{
		if(x_out)
			*x_out= xx* x_in + xy* y_in + xz* z_in;
		if(y_out)
			*y_out= yx* x_in + yy* y_in + yz* z_in;
		if(z_out)
			*z_out= zx* x_in + zy* y_in + zz* z_in;
		return;
	};

};

//-----------------------------------------------------------
// автопилот
typedef struct TAutopilot
{
	char TetaChannel; // вкл. тангажного канала
	char GammaChannel;// вкл. кренового канала
	char PsiChannel;  // вкл. курсового канала
	char Trimmer;     // нажата кнопка триммирования
	float Xi;	  // зад. отклонение АП продольное
	float Nu;	  // зад. отклонение АП поперечное
	float Teta,Psi,Gamma;	// зад. углы тангажа, рысканья и крена
}
TAutopilot;

//-----------------------------------------------------------
// мех. параметры системы управления
typedef struct TControlParams
{
	//тангажный канал
	float lt,at,bt,ct,abt,dt;
	//креновый канал
	float lg,ag,bg,cg,abg,dg;
	//курсовой канал
	float lp,ap,bp,cp,abp,dp;
}
TControlParams;

//-----------------------------------------------------------
//                      атмосфера

// данные о состоянии атмосферы для модели динамики
class TCurrentAtmosphere
{
public:
	float T, P, Ro;
	float Vx, Vy, Vz;
	TCurrentAtmosphere()
	{
		T=ATM_T0; P=ATM_P0; Ro=ATM_R0;
		Vx=Vy=Vz=0.f;
	};
};



//**************************************************************************
// Система по предотвращению выхода за эксплуатационные ограничения

class CLimitsPrevention
{
public:
	double	T, K1, K2;
	double	Xmax, Xmin;
	double	XprevUp, XprevLo;
	double	IntegratorUp, IntegratorLo;
	//
	double	d0Up, d0Lo;
	//
	double	XupdateUp, XupdateLo;
	double	IntegratorLimUp, IntegratorLimLo;
	double	SumUp, SumLo;
	double	Up, Lo;
//	double	dt;
//	double	X, U;
	double	ULim;
	//
	void InitSys(double T_1, double K_1, double K_2, double LimUp, double LimLo)
	{
		T=T_1;
		K1=K_1;
		K2=K_2;
		Xmax=LimUp;
		Xmin=LimLo;
		XprevUp=0.0;
		XprevLo=0.0;
		IntegratorUp=0.0;
		IntegratorLo=0.0;
		d0Up=4.2/180.0*3.14159;
		d0Lo=-4.0/180.0*3.14159;
	}
	//
	void Run(double dt, double U, double X)
	{
		// определяем верхний предел
		XupdateUp=X+T/dt*(X-XprevUp);
		XprevUp=X;
		IntegratorLimUp=(U-IntegratorUp)*K1;
		IntegratorLimUp=( IntegratorLimUp < Xmin )?(-Xmax):(IntegratorLimUp);
		SumUp=IntegratorLimUp-XupdateUp;
		IntegratorUp+=SumUp*K2*dt;
		Up=d0Up-IntegratorUp;
		// определяем нижний предел
		XupdateLo=(-X)+T/dt*((-X)-XprevLo);
		XprevLo=-X;
		IntegratorLimLo=(U-IntegratorLo)*K1;
		IntegratorLimLo=( IntegratorLimLo < -Xmax )?(-Xmax):(IntegratorLimLo);
		SumLo=IntegratorLimLo-XupdateLo;
		IntegratorLo+=SumLo*K2*dt;
		Up=d0Up+IntegratorLo;
		// определение ограниченного сигнала
		ULim=(U>Up)?(Up):(U);
		ULim=(U<Lo)?(Lo):(U);
		// проверка ограниченного сигнала
		ULim=(U>d0Up)?(Up):(U);
		ULim=(U<d0Lo)?(Lo):(U);
	}
};

//**************************************************************************


//===========================================================
//            динамика одновинтового вертолета
class TShassy;

class FMBASE_API THelicopter abstract
{
	//вспомогательные переменные для автопилота
	float ap_db, ap_dk, ap_wb, ap_wk;
	float ap_Ft, ap_ipsi;

	float X1,Z1;

protected:
	std::auto_ptr<THelicopterData> m_HelData;
	TShassy* m_pShassy;
	
public://tmp
	float ap_Wdb, ap_wdb, ap_Wdk, ap_wdk;
	int ap_btn_b, ap_btn_k;

	TStateVector	S,	//вектор состояния вертолета
		D,      //производная вектора состояния
		Dold;
	TPilotVector	Up,	//управление от летчика (с ручки)
		Ubal;   //балансировка (выходной параметр) - добавлено 28.07.04
	bool IsBalanced, BalancedEvent; //признак сбалансированности орг. упр. - 06.08.04
	void Trim();                     //режим балансировки                   - 06.08.04
	bool IsCrashed, CrashEvent;     //признак аварии - 09.08.04

	TControlVector	U1,	//управления(в отклон. АП)
		U2,
		U3,
		U4,
		Uf;
	TCoefs    Cv;             //Коэффициенты для текущей скорости
	TCoefBase Cbase;          //массивы коэффициентов

	TCalibs        Calib;              //тарировки управления
	TAutopilot     A;                  //автопилот
	TControlParams Ctrl;      // мех. парам. сист. упр.
	TSinus		Sinus;	   // тригон. ф-ции углов
	TCosinus	Cosinus;   // тригон. ф-ции углов
	TMatrix        Matrix;


	// атмосфера
	TCurrentAtmosphere Atm;

	//ландшафт
	double Hr; //-высота над рельефом
	//...

	// двигатель
//	TEngine E;
	float KEngine;   

	// система ограничения фазовых координат
	CLimitsPrevention	RollLimPrev;	// по крену
	//

	THelicopter(THelicopterData* data = new THelicopterData());
	~THelicopter();

	THelicopter(const THelicopter& obj)
	{
		m_HelData.reset(new THelicopterData(*obj.m_HelData));
	}

	THelicopter& operator= (const THelicopter& obj)
	{
		if (this == &obj)
		{
			return *this;
		}
		m_HelData.reset(new THelicopterData(*obj.m_HelData));
		return *this;
	}
	bool Clear;

	virtual int Run(float dt_in);
	virtual void SetWheelBrake(double pos){};

	int LoadFromFileText(char *filename_in);

	int Control(double dt);
	int Autopilot(float dt_in);
	int Mashine(float dt_in);
	int Booster(float dt_in);
	int MainRotorDelay(float dt_in);

	int UpdateCoefs(void);
	//int Engine(float dt_in);
	int CoreEquations(void);

	int Stability(float dt_in);
	int AirCushion(void);
	int Surface(float dt_in);

	int Integrate(float dt_in);
	int Angles(void);
	int Check(void);

	TShassy* getShassy() { return m_pShassy; }

	bool TrimMode; //добавлено 05.08.2004
};

}
}

#endif
