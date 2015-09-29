//                    file woShipNum.h
#ifndef __woShipNum_h__
#define __woShipNum_h__
#include "cLinear.h"

         //    Информация о кораблях.

// ***************** "Адмирал Кузнецов" ******************************

//   Взлетные и посадочные полосы "Адмирала Кузнецова" в его осях

struct RunWay RunWays_K[] = 
{     // Посадочная полоса задается координатами центра
 //Name_Vpp;         R_Vpp;                        Psi_Vpp(град);Length_Vpp;Width_Vpp; 
	{1,  1, 0, cVector(-15.55f, 16.257f, -14.42f), 7.72964f,     201.84f,  25.0f },
// Взлетные полосы: задаются точкой установки самолета для старта
	{2,  2, 0, cVector(76.2f,  16.257f,   11.93f),   7.0f,      102.368f,  25.0f },
	{3,  3, 0, cVector(76.2f,  16.257f,  -11.93f), 353.0f,      102.368f,  25.0f },
	{4,  4, 0, cVector(32.5f,  16.257f,  -16.78f), 353.0f,      146.118f,  25.0f },
	{5,  5, 0, cVector(-11.3f,  16.257f, -21.63f), 353.0f,      189.868f,  25.0f }
};

// Маршруты рулежки и стоянки в осях авианосца

struct TaxiPoint TRoute_0K[] = {
//    x        y        z     V_target      pNext
{   65.0f,   16.257f,    -25.0f, 3.0f,   &TRoute_0K[1]  },
{   70.0f,   16.257f,      0.0f, 2.0f,   &TRoute_0K[2]  },
{  -10.0f,   16.257f,      7.0f, 2.0f,   &TRoute_0K[3]  },
{  -92.0f,   16.257f,     21.0f, 2.0f,}
};

struct TaxiPoint TRoute_1K[] = {
//    x        y        z     V_target      pNext
{   65.0f,   16.257f,    -25.0f, 3.0f,   &TRoute_1K[1]  },
{   70.0f,   16.257f,      0.0f, 2.0f,   &TRoute_1K[2]  },
{  -10.0f,   16.257f,      7.0f, 2.0f,   &TRoute_1K[3]  },
{  -70.0f,   16.257f,     20.0f, 2.0f,}
};

struct TaxiPoint TRoute_2K[] = {
//    x        y        z     V_target      pNext
{   65.0f,   16.257f,    -25.0f, 3.0f,   &TRoute_2K[1]  },
{   70.0f,   16.257f,      0.0f, 2.0f,   &TRoute_2K[2]  },
{  -10.0f,   16.257f,      6.0f, 2.0f,   &TRoute_2K[3]  },
{  -48.0f,   16.257f,     18.0f, 2.0f,}
};

struct TaxiPoint TRoute_3K[] = {
//    x        y        z     V_target      pNext
{   65.0f,   16.257f,    -25.0f, 3.0f,   &TRoute_3K[1]  },
{   70.0f,   16.257f,      0.0f, 2.0f,   &TRoute_3K[2]  },
{  -12.0f,   16.257f,      7.0f, 2.0f,   &TRoute_3K[3]  },
{  -26.0f,   16.257f,     13.0f, 2.0f,}
};

struct TaxiPoint TRoute_4K[] = {
//    x        y        z     V_target      pNext
{   65.0f,   16.257f,    -25.0f, 3.0f,   &TRoute_4K[1]  },
{   70.0f,   16.257f,      0.0f, 2.0f,   &TRoute_4K[2]  },
{   42.0f,   16.257f,      5.0f, 2.0f,   &TRoute_4K[3]  },
{   -3.0f,   16.257f,      7.0f, 2.0f,}
};

struct TaxiPoint TRoute_5K[] = {
//    x        y        z     V_target      pNext
{   65.0f,   16.257f,    -25.0f, 3.0f,   &TRoute_5K[1]  },
{   70.0f,   16.257f,      0.0f, 2.0f,   &TRoute_5K[2]  },
{   20.0f,   16.257f,      7.0f, 2.0f,}
};

struct TaxiPoint TRoute_6K[] = {
//    x        y        z     V_target      pNext
{   65.0f,   16.257f,    -25.0f, 3.0f,   &TRoute_6K[1]  },
{   70.0f,   16.257f,      0.0f, 2.0f,   &TRoute_6K[2]  },
{   42.0f,   16.257f,      5.0f, 2.0f,}
};

struct TaxiPoint TRoute_7K[] = {
//    x        y        z     V_target      pNext
{   65.0f,   16.257f,    -25.0f, 3.0f,   &TRoute_7K[1]  },
{   70.0f,   16.257f,      0.0f, 2.0f,   &TRoute_7K[2]  },
{   55.0f,   16.257f,     18.0f, 2.0f,}
};

struct TaxiPoint TRoute_8K[] = {
//    x        y        z     V_target      pNext
{   65.0f,   16.257f,    -25.0f, 3.0f,   &TRoute_8K[1]  },
{   70.0f,   16.257f,      5.0f, 2.0f,}
};


struct TaxiRoute TRoutes_K[] = {  // Маршруты рулежки до места стоянки.
	{ 0, TRoute_0K},
	{ 1, TRoute_1K},
	{ 2, TRoute_2K},
	{ 3, TRoute_3K},
	{ 4, TRoute_4K},
	{ 5, TRoute_5K},
	{ 6, TRoute_6K},
	{ 7, TRoute_7K},
	{ 8, TRoute_8K}
};
  

// ****************** "VINSON" ****************************************                         
//   Взлетные и посадочные полосы "VINSON" в его осях
struct RunWay RunWays_V[] = 
{  // Посадочная полоса задается координатами центра 
	//Name_Vpp;         R_Vpp[3]; Psi_Vpp(град);Length_Vpp;Width_Vpp;
	                                                                        //lowGlidePath,slightlyLowGlidePath,onLowerGlidePath,onUpperGlidePath,slightlyHighGlidePath,highGlidePath    
	{1,  1, 0, cVector( -44.54f, 19.6f, -10.49f ),  9.1359f, 240.0f,  25.0f, 2.5,2.8,3.0,3.0,3.2,3.5},
 // Взлетные полосы: задаются точкой установки самолета для старта
	{2,  2, 0, cVector(  52.48f, 19.6f,  17.04f ),  5.9814f, 113.0f,  25.0f, 2.5,2.8,3.0,3.0,3.2,3.5},
	{3,  3, 0, cVector(  47.87f, 19.6f,  -4.27f ),  2.105f,  117.4f,  25.0f, 2.5,2.8,3.0,3.0,3.2,3.5},
	{4,  4, 0, cVector( -43.13f, 19.6f, -20.56f),   4.411f,  137.0f,  25.0f, 2.5,2.8,3.0,3.0,3.2,3.5},
	{5,  5, 0, cVector( -61.30f, 19.6f, -31.79f),   0.0f,    132.52f, 25.0f, 2.5,2.8,3.0,3.0,3.2,3.5}
};
// Маршруты рулежки и стоянки в осях авианосца

struct TaxiPoint TRoute_0V[] = {
//    x        y        z     V_target      pNext
{   23.0f,   19.6f,    -20.0f,  3.0f,   &TRoute_0V[1]  },
{   23.89f,  19.6f,     25.0f, 2.0f,}
};

struct TaxiPoint TRoute_1V[] = {
//    x        y        z     V_target      pNext
{   23.0f,   19.6f,    -20.0f,  3.0f,   &TRoute_1V[1]  },
{   10.0f,   19.6f,     10.0f,  2.0f,   &TRoute_1V[2]  },
{  -60.0f,   19.6f,     15.0f,  2.0f,   &TRoute_1V[3]  },
{ -130.0f,   19.6f,     26.0f, 2.0f,}
};

struct TaxiPoint TRoute_2V[] = {
//    x        y        z     V_target      pNext
{   23.0f,   19.6f,    -20.0f,  3.0f,   &TRoute_2V[1]  },
{   10.0f,   19.6f,     10.0f,  2.0f,   &TRoute_2V[2]  },
{  -60.0f,   19.6f,     15.0f,  2.0f,   &TRoute_2V[3]  },
{ -110.0f,   19.6f,     25.0f, 2.0f,}
};

struct TaxiPoint TRoute_3V[] = {
//    x        y        z     V_target      pNext
{   23.0f,   19.6f,    -20.0f,  3.0f,   &TRoute_3V[1]  },
{   10.0f,   19.6f,     10.0f,  2.0f,   &TRoute_3V[2]  },
{  -60.0f,   19.6f,     15.0f,  2.0f,   &TRoute_3V[3]  },
{  -90.0f,   19.6f,     20.0f, 2.0f,}
};

struct TaxiPoint TRoute_4V[] = {
//    x        y        z     V_target      pNext
{   23.0f,   19.6f,    -20.0f,  3.0f,   &TRoute_4V[1]  },
{   10.0f,   19.6f,     10.0f,  2.0f,   &TRoute_4V[2]  },
{  -70.0f,   19.6f,     16.0f, 2.0f,}
};

struct TaxiPoint TRoute_5V[] = {
//    x        y        z     V_target      pNext
{   23.0f,   19.6f,    -20.0f,  3.0f,   &TRoute_5V[1]  },
{   10.0f,   19.6f,     10.0f,  2.0f,   &TRoute_5V[2]  },
{  -50.0f,   19.6f,     14.0f, 2.0f,}
};

struct TaxiPoint TRoute_6V[] = {
//    x        y        z     V_target      pNext
{   23.0f,   19.6f,    -20.0f,  3.0f,   &TRoute_6V[1]  },
{   10.0f,   19.6f,     10.0f,  2.0f,   &TRoute_6V[2]  },
{  -25.0f,   19.6f,      8.0f,  2.0f,   &TRoute_6V[3]  },
{  -30.0f,   19.6f,     25.0f, 2.0f,}
};

struct TaxiPoint TRoute_7V[] = {
//    x        y        z     V_target      pNext
{   23.0f,   19.6f,    -20.0f,  3.0f,   &TRoute_7V[1]  },
{   10.0f,   19.6f,     10.0f,  2.0f,   &TRoute_7V[2]  },
{  -30.0f,   19.6f,      8.0f,  2.0f,}
};

struct TaxiPoint TRoute_8V[] = {
//    x        y        z     V_target      pNext
{   23.0f,   19.6f,    -20.0f,  3.0f,   &TRoute_8V[1]  },
{   10.0f,   19.6f,     10.0f,  2.0f,   &TRoute_8V[2]  },
{  -10.0f,   19.6f,      6.0f,  2.0f,   &TRoute_8V[3]  },
{  -10.0f,   19.6f,     25.0f, 2.0f,}
};

struct TaxiPoint TRoute_9V[] = {
//    x        y        z     V_target      pNext
{   23.0f,   19.6f,    -20.0f,  3.0f,   &TRoute_9V[1]  },
{   10.0f,   19.6f,     10.0f,  2.0f,   &TRoute_9V[2]  },
{  -10.0f,   19.6f,      6.0f,  2.0f,}
};

struct TaxiPoint TRoute_10V[] = {
//    x        y        z     V_target      pNext
{   23.0f,   19.6f,    -20.0f,  3.0f,   &TRoute_10V[1] },
{   10.0f,   19.6f,      8.0f,  2.0f,   &TRoute_10V[2] },
{   10.0f,   19.6f,     25.0f,  2.0f,}
};

struct TaxiPoint TRoute_11V[] = {
//    x        y        z     V_target      pNext
{   23.0f,   19.6f,    -20.0f,  3.0f,   &TRoute_11V[1] },
{   10.0f,   19.6f,     10.0f,  2.0f,}
};

struct TaxiPoint TRoute_12V[] = {
//    x        y        z     V_target      pNext
{   23.0f,   19.6f,    -20.0f,  3.0f,   &TRoute_12V[1] },
{   30.0f,   19.6f,     10.0f,  2.0f,}
};


struct TaxiRoute TRoutes_V[] = {  // Маршруты рулежки до места стоянки.
	{ 0, TRoute_0V},
	{ 1, TRoute_1V},
	{ 2, TRoute_2V},
	{ 3, TRoute_3V},
	{ 4, TRoute_4V},
	{ 5, TRoute_5V},
	{ 6, TRoute_6V},
	{ 7, TRoute_7V},
	{ 8, TRoute_8V},
	{ 9, TRoute_9V},
	{10, TRoute_10V},
	{11, TRoute_11V},
	{12, TRoute_12V}
};

//*********************************************************************                         

RunWay* CarierRunWays[] =  // ВПП авианосцев в их осях
{ 
	RunWays_K,        // 0
	RunWays_K,        // 1  "Адмирал Кузнецов"
	RunWays_V,        // 2  "VINSON"
	RunWays_V,        // 3  
	RunWays_V,        // 4  
	RunWays_V,        // 5  
	RunWays_V,        // 6  
	RunWays_V,        // 7  
	RunWays_V,        // 8  
	RunWays_V,        // 9  
	RunWays_V,        // 10  
	RunWays_V,        // 11  
	RunWays_V,        // 12  
	RunWays_V,        // 13  
	RunWays_V,        // 14  
	RunWays_V,        // 15  
	RunWays_V,        // 16  
	RunWays_V,        // 17  
	RunWays_V,        // 18  
	RunWays_V,        // 19  
	RunWays_V,        // 20  
	RunWays_V,        // 21  
	RunWays_V,        // 22  
	RunWays_V,        // 23  
	RunWays_V,        // 24  
	RunWays_V,        // 25  
	RunWays_V,        // 26  
	RunWays_V,        // 27  
	RunWays_V         // 28  
};

TaxiRoute* CarierTaxiRoutes[] = // Стоянки и маршруты рулежки 
{                               // авианосцев в их осях.
	TRoutes_K,        // 0
	TRoutes_K,        // 1
	TRoutes_V,        // 2
	TRoutes_K,        // 3
	TRoutes_K,        // 4
	TRoutes_K,        // 5
	TRoutes_K,        // 7
	TRoutes_K,        // 8
	TRoutes_K,        // 9
	TRoutes_K,        // 10
	TRoutes_K,        // 11
	TRoutes_K,        // 12
	TRoutes_K,        // 13
	TRoutes_K,        // 14
	TRoutes_K,        // 15
	TRoutes_K,        // 16
	TRoutes_K,        // 17
	TRoutes_K,        // 18
	TRoutes_K,        // 19
	TRoutes_K,        // 20
	TRoutes_K,        // 21
	TRoutes_K,        // 22
	TRoutes_K,        // 23
	TRoutes_K,        // 24
	TRoutes_K,        // 25
	TRoutes_K,        // 26
	TRoutes_K,        // 27
	TRoutes_K         // 28
};

#endif
