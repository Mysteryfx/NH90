#ifndef ENGINE_H
#define ENGINE_H


#include "FMMathematics/Constants.h"

namespace EagleFM
{
namespace BalanceFM
{

// атмосфера
#ifndef ATM_T0
#define ATM_T0 288.15f
#endif
#ifndef ATM_P0
#define ATM_P0 760.f
#endif
#ifndef ATM_R0
#define ATM_R0 1.225f
#endif

//режимы работы двигателя
#ifndef ENGINE_MODE_RUN_AUTO
#define ENGINE_MODE_RUN_AUTO 1
#endif
#ifndef ENGINE_MODE_RUN_TC
#define ENGINE_MODE_RUN_TC   (1<<1)
#endif

//номинальная частота вращения НВ
#ifndef ENGINE_WROT_NORMAL
#define ENGINE_WROT_NORMAL 20.0f
#endif
//радиус НВ
#ifndef MAIN_ROTOR_RADIUS
#define MAIN_ROTOR_RADIUS 10.647f
#endif
//момент инерции НВ
#ifndef MAIN_ROTOR_MOMENT
#define MAIN_ROTOR_MOMENT (1290.f*9.81f)
#endif
//коэф-т заполнения НВ
#ifndef MAIN_ROTOR_SIGMA
#define MAIN_ROTOR_SIGMA 0.0777f
#endif
//угол заклинения НВ (продольный)
#ifndef MAIN_ROTOR_ALPHA
#define MAIN_ROTOR_ALPHA (4.5f/57.3f)
#endif

//переход от % к оборотам в минуту для турбокомпрессора
#ifndef ENGINE_TC_PERCENT_TO_RPM
#define ENGINE_TC_PERCENT_TO_RPM 195.3748f
#endif
//переход от % к оборотам в минуту для свободной турбины
#ifndef ENGINE_FT_PERCENT_TO_RPM
#define ENGINE_FT_PERCENT_TO_RPM 2.01f
#endif
//переход от л.с. к Вт
#ifndef ENGINE_TC_HP_TO_WT
#define ENGINE_TC_HP_TO_WT 750.f
#endif

//коэф-т редукции привода регулятора ТК
#ifndef ENGINE_GOVR_I_TC
#define ENGINE_GOVR_I_TC (40.24f/ENGINE_TC_PERCENT_TO_RPM)
#endif
//коэф-т редукции основного редуктора
#ifndef ENGINE_GOVR_I_MAIN
#define ENGINE_GOVR_I_MAIN 62.5f
#endif
//коэф-т редукции привода регулятора СТ
#ifndef ENGINE_GOVR_I_FT
#define ENGINE_GOVR_I_FT 3.75f
#endif


//ограничения производной оборотов турбокомпрессора:
//приемистость полная
#ifndef ENGINE_TC_UP_TIME_FULL
#define ENGINE_TC_UP_TIME_FULL 9.f
#endif
//приемистость частичная
#ifndef ENGINE_TC_UP_TIME_PART
#define ENGINE_TC_UP_TIME_PART 4.f
#endif
//скорость сброса газа
#ifndef ENGINE_TC_DOWN_TIME
#define ENGINE_TC_DOWN_TIME 5.f
#endif

//ограничения расхода
#ifndef ENGINE_FLOW_MAX
#define ENGINE_FLOW_MAX 550.f
#endif
#ifndef ENGINE_FLOW_MIN
#define ENGINE_FLOW_MIN 90.f
#endif


#ifndef ENGINE_ROT_UP
#define ENGINE_ROT_UP    1
#endif
#ifndef ENGINE_ROT_DOWN
#define ENGINE_ROT_DOWN (1<<1)
#endif

#define TECTRLDATA


// вектор управлений двигателем
struct TECtrlData
{
	float Xcr;	// коррекция обор. РОШ
	float Xrr;	// РУД правый
	float Xrl;	// РУД левый
	unsigned long Buttons; // кнопки
};

//------------------------------------------------------------------------------
//                       модель двигателя

//------------------------------------------------------------------------------
// база параметров для интерполяции

#ifndef ENGINE_MAX_PARAMS_COUNT
#define ENGINE_MAX_PARAMS_COUNT 32
#endif
struct TEBaseItem
{
	float Value, Arg;
};
class TEBaseList
{
public:
	TEBaseItem Items[ENGINE_MAX_PARAMS_COUNT];
	int Count;
	TEBaseList(){Count=0;}
	float Value(float arg);
};
struct TEGovernorBaseList
{//changed
	TEBaseList T1, T2;
	TEBaseList K1, K2, K3, K4, K5, K6, K7, K8, K9, K10;
};
//changed
struct TEGovernorCommonBaseList
{
	TEBaseList Nrot, Ntc, Gtdmax, Gtdmin;
};
struct TERotorBaseList
{
	TEBaseList Kdn, KNe, NeStat, GtStat;
};
struct TETorqueBaseList
{
	TEBaseList MkStat, MkReqV, AStat, VStat, TyStat, Cx, Kloss;
};

struct TEngineBase
{//changed
	TEGovernorBaseList	GovrTC;
	TEGovernorBaseList	GovrFT;
	TEGovernorCommonBaseList GovrCommon;
	TERotorBaseList     	Rotr;
	TETorqueBaseList	Torq;
};

//------------------------------------------------------------------------------
// параметры, получаемые путем интерполяции

struct TEGovernorParams
{//changed
	float T1, T2;
	float K1, K2, K3, K4, K5, K6, K7, K8, K9, K10;
};
//changed
struct TEGovernorCommonParams
{
	float Nrot, Ntc, Gtdmax, Gtdmin;
};

struct TERotorParams
{
	float Kdn, KNe, NeStat, GtStat;
};
struct TETorqueReqParams
{
	float MkStat, MkStat0, MkReqV, AStat, VStat, TyStat, Cx, Kloss;
};

struct TEngineParams
{//changed
	TEGovernorParams	GovrTC;
	TEGovernorParams	GovrFT;
	TEGovernorCommonParams GovrCommon;
	TERotorParams		Rotr;
	TETorqueReqParams	Torq;
};

//==============================================================================
//                        модель двигателя

class TEngine
{

	//база параметров
	TEngineBase Base;
	//текущие параметры
	TEngineParams Params;
	//внутр.
	float N,Nr;            //мощность и приведенная мощность
	float Ntcr;            //приведенные обороты
	float Gtr;             //приведенный расход топлива
	float DN;              //отклонение оборотов винта от настр.
	float Vrel;       //относ. скорость
	float dXtc, dXft; //производн. перемещений доз. иглы
	float Xtc,  Xft;  //перемещения доз. иглы
	float Gt0;

	//интерполяция параметров
	int UpdateParams( float dt_in );
	// модель регулятора
	int Governor( float dt_in);
	// расчет оборотов
	int Rotor   ( float dt_in);
	// расчет потребного момента
	int Torque  ( float dt_in);
	// приведение к сбалансированному режиму
	int Balance();

public:
	bool Clear;

	//вход
	float Alpha; //угол атаки
	float AlphaRot; //угол атаки НВ
	float Arud;  //угол повор. РУД
	float Fc, Fcbal;    //общий шаг и балансировочный общий шаг 
	float V, Vx;     //скорости
	float Wx,Wy,Wz, dWx,dWy,dWz;   //угл. скорости и их производные
	float Th, Ph, Roh; //атмосф. условия
	float Vwx, Vwy, Vwz;  //ветер
	unsigned long Fails;   //сбои
	unsigned long Mode;    //режим работы двигателя
	TECtrlData Ctrl;
	//выход
	float Wrot;            //частота вращения НВ
	float Ntc;	        //обороты турбокомпрессора
	float Gt;		//расход топлива (1 двигатель)
	float KW;		//[(Wнв - Wy)/Wнв ном]^2

	float out0;
	float out1;
	float out2;
	//инициализация параметров непосредственно в коде
	TEngine();
	~TEngine();
	int Run( float dt_in);

};


}
}

//------------------------------------------------------------------------------
#endif
