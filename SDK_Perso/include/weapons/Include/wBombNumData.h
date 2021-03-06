//            file wBombNumData.h   Численная информация о бомбах. 
#ifndef __wBombNumData_h__    
#define __wBombNumData_h__

struct Bomb_Const Bombs[ ] ={   
// Bomb_Name_;Bomb_Type_;Bomb_Massa_;Expl_Massa_;H_min_;H_max_,C_x_,VyHold_;Ag_;
/*{ BetAB_150DS, 1,  150.0f,   45.0f,   50.0f, 25000.0f, 0.0004f ,  -50.0f,-10.0f}, // c парашютом    
{ BetAB_250  , 1,  250.0f,  150.0f,   50.0f, 25000.0f, 0.00074f,  -50.0f,-10.0f}, // c парашютом, Масса ВВ в оригинале 80 кг, сделано 150 для увеличения воздействия на укрепления.
{ BetAB_500  , 1,  478.0f,  250.8f,   50.0f, 25000.0f, 0.00148f,  -50.0f,-10.0f}, // c парашютом, Масса ВВ в оригинале 75 кг, сделано 250 для увеличения воздействия на укрепления.
{BetAB_500ShP, 1,  380.0f,  300.0f,   50.0f,  3000.0f, 0.00148f,  -50.0f,-20.0f}, // c парашютом, Масса ВВ в оригинале 77 кг, сделано 300 для увеличения воздействия на укрепления.
{ FAB_100    , 0,  100.0f,   45.0f,  100.0f, 20000.0f, 0.0003f , -100.0f,-1.23f}, //
{ FAB_250    , 0,  250.0f,  120.0f,  200.0f, 20000.0f, 0.00074f, -100.0f,-1.23f}, //
{ FAB_500    , 0,  500.0f,  240.0f,  300.0f, 20000.0f, 0.00148f, -100.0f,-1.23f}, //
{ FAB_1000   , 0, 1000.0f,  600.0f,  500.0f, 20000.0f, 0.00293f, -100.0f,-1.23f}, // 
{ FAB_1500	 , 0, 1500.0f, 1100.0f,  500.0f, 20000.0f, 0.00322f, -100.0f,-1.23f}, // 
{ FAB_5000	 , 0, 5000.0f, 4500.0f,  500.0f, 20000.0f, 0.00488f, -120.0f,-1.23f}, // 		
{ KAB_500    , 4,  534.0f,  195.0f, 1000.0f, 10000.0f, 0.00168f, -60.0f,-1.0f }, // лазер упр 		
{ KAB_500Kr	 , 3,  560.0f,  300.0f, 1000.0f, 10000.0f, 0.0017f , -60.0f,-1.0f }, // TV, Масса ВВ в оригинале 195 кг, сделано 300 для увеличения воздействия на укрепления.
{ KAB_500KrOD, 3,  460.0f,  280.0f, 1000.0f, 10000.0f, 0.00137f, -60.0f,-1.0f }, // 
{ KAB_1500Kr , 4, 1500.0f, 1100.0f, 1000.0f, 15000.0f, 0.00436f, -80.0f,-1.0f }, // 	
{ ODAB_500PM , 2,  520.0f,  193.0f,   50.0f,  7000.0f, 0.00167f,  -50.0f,-8.0f}, // напалм	c парашютом
{ ODAB_250   , 2,  275.0f,   94.0f,   50.0f,  7000.0f, 0.0008f ,  -50.0f,-8.0f}, // 	c парашютом
{ RBK_250    , 5,  275.0f,   94.0f,  200.0f, 25000.0f, 0.0008f , -100.0f,-1.23f}, // кассетная бомба	
{ RBK_250S	 , 5,  275.0f,   94.0f,  200.0f, 25000.0f, 0.0008f , -100.0f,-1.23f}, // кассетная бомба	
{ RBK_500AO	 , 5,  500.0f,  210.0f,  200.0f, 25000.0f, 0.00148f, -100.0f,-1.23f}, // осколочная кассетная бомба	
{ RBK_500SOAB, 5,  504.0f,  210.0f,  200.0f, 25000.0f, 0.00148f, -100.0f,-1.23f}, // противопехотная кассетная бомба		
{ Puma    ,	   0,  250.0f,  100.0f,  200.0f, 12000.0f, 0.00074f, -100.0f,-1.23f}, // зажигательная  
{ GluB       , 0,  250.0f,  100.0f,  500.0f, 12000.0f, 0.00074f, -100.0f,-1.23f}, // глубинная бомба							
{ NB_1       , 0,  347.0f,    1e9f,  500.0f, 12000.0f, 0.001f  , -100.0f,-1.23f}, // B61 nuclear bomb							
{ NB_2       , 0, 1088.0f,    2e9f,  500.0f, 12000.0f, 0.00322f, -100.0f,-1.23f}, // B83 nuclear bomb							
{ Torpedo	 , 0,  500.0f,  240.0f,  500.0f, 12000.0f, 0.00148f, -100.0f,-1.23f}, // какая-то торпеда							
{ XZAB_250   , 0,  250.0f,  100.0f,  300.0f, 12000.0f, 0.00074f, -100.0f,-1.23f}, // зажигательная 
{ XZAB_500   , 0,  375.0f,  240.0f,  300.0f, 12000.0f, 0.00148f, -100.0f,-1.23f}, // зажигательная 		
{ Mk_81      , 0,  118.0f,   45.0f,  100.0f, 12000.0f, 0.00035f, -100.0f,-1.23f}, // 
{ Mk_82      , 0,  241.0f,   89.0f,  100.0f, 12000.0f, 0.00074f, -100.0f,-1.23f}, // 
{ Mk_83      , 0,  447.0f,  202.0f,  200.0f, 12000.0f, 0.00136f, -100.0f,-1.23f}, // 
{ Mk_84      , 0,  894.0f,  428.0f,  300.0f, 12000.0f, 0.00264f, -100.0f,-1.23f}, // 	
{ M_117      , 0,  340.0f,  175.0f,  300.0f, 12000.0f, 0.001f  , -100.0f,-1.23f}, // 				
{ CBU_87     , 5,  450.0f,  200.0f,   61.0f,  6100.0f, 0.00136f, -100.0f,-1.23f}, // anti-tank cluster bomb				
{ CBU_89     , 5,  450.0f,  200.0f,   61.0f,  6100.0f, 0.00136f, -100.0f,-1.23f}, // anti-tank cluster bomb				
{ CBU_97     , 5,  450.0f,  200.0f,   61.0f,  6100.0f, 0.00136f, -100.0f,-1.23f}, // anti-tank cluster bomb				
{ GBU_10     , 4,  900.0f,  428.0f, 1000.0f, 40000.0f, 0.00264f, -100.0f,-2.0f }, // 
{ GBU_11     , 4,  900.0f,  428.0f, 1000.0f, 40000.0f, 0.00264f, -100.0f,-2.0f }, //
{ GBU_12     , 4,  225.0f,   89.0f, 1000.0f, 40000.0f, 0.00067f, -100.0f,-2.0f }, //
{ GBU_22     , 4,  225.0f,   89.0f, 1000.0f, 40000.0f, 0.00067f, -100.0f,-2.0f }, //
{ GBU_16     , 4,  454.0f,  202.0f, 1000.0f, 40000.0f, 0.00136f, -100.0f,-2.0f }, //
{ GBU_17     , 4,  500.0f,  240.0f, 1000.0f, 40000.0f, 0.00148f, -100.0f,-2.0f }, //
{ GBU_24     , 4,  900.0f,  428.0f, 1000.0f, 40000.0f, 0.00264f, -100.0f,-2.0f }, //
{ GBU_15	 , 3, 1140.0f,  428.0f,  500.0f,  7000.0f, 0.00336f, -100.0f,-2.0f }, // TV or IRS-guided		
{ GBU_27     , 4,  984.0f,  240.0f, 1000.0f, 40000.0f, 0.00292f, -100.0f,-2.0f }, //    
{ GBU_28	 , 4, 2130.0f,  306.0f, 1000.0f, 40000.0f, 0.00488f, -100.0f,-2.0f }, //			
{ GBU_29     , 3,  225.0f,   60.0f, 1000.0f, 40000.0f, 0.00067f, -100.0f,-2.0f }, //
{ GBU_30     , 3,  225.0f,   60.0f, 1000.0f, 40000.0f, 0.00067f, -100.0f,-2.0f }, //
{ AGM_62     , 3, 1088.0f,  424.0f,   75.0f, 12000.0f, 0.00322f, -100.0f,-2.0f }, // TV Walleye 2				
{ FAB_100P   , 0,  100.0f,   40.0f,  100.0f,  5000.0f, 0.0003f , -100.0f,-1.23f}, // 
{ FAB_250P   , 0,  250.0f,  100.0f,  200.0f,  5000.0f, 0.00074f, -100.0f,-1.23f}, // 
{ FAB_500P   , 0,  500.0f,  240.0f,  300.0f,  5000.0f, 0.00148f, -100.0f,-1.23f}, // 
{ FAB_500_3  , 0,  500.0f,  340.0f,  300.0f, 12000.0f, 0.00148f, -100.0f,-1.23f}, // FAB-500 - 3 
{ PB_250	 , 2,  250.0f,  150.0f,   50.0f,  5000.0f, 0.00074f,  -50.0f,-8.0f}, // 	c парашютом
{ Z_BAK_3    , 0,  500.0f,  240.0f,  500.0f, 12000.0f, 0.00148f, -100.0f,-1.23f}, // зажигательная 
{ ROCKEYE	 , 5,  222.0f,   50.0f,   90.0f,  5000.0f, 0.00065f, -100.0f,-1.23f}, // кассетная
{ Puma	     , 0,  249.0f,  120.0f,  200.0f, 12000.0f, 0.00074f, -100.0f,-1.23f}, //
{ BL_755     , 5,  277.0f,  132.3f,  100.0f, 25000.0f, 0.0008f,  -100.0f,-1.23f}, // кассетная бомба	
{ MW_1       , 5,  525.0f,  240.0f,  100.0f,  1000.0f, 0.00167f, -100.0f,-1.23f}, // кассетная бомба	
{ Durandal	 , 1,  185.0f,   15.0f,   50.0f,  5000.0f, 0.00148f,  -50.0f,-5.0f}, //
{ SAB_100	 , 0,  100.0f,   0.0f,   400.0f,  7000.0f, 0.0003f,   -50.0f,-1.23f}, // c осветительными бомбами LUU_2B
{ LUU_2B	 , 2,   13.6f,   0.0f,   400.0f,  7000.0f, 0.0001f,   -10.0f,-5.0f}, // 64 Осветительная авиабомба c парашютом
{ LUU_2AB	 , 2,   13.6f,   0.0f,   400.0f,  7000.0f, 0.0001f,   -10.0f,-5.0f}, // 64 Осветительная авиабомба c парашютом
{ LUU_2BB	 , 2,   13.6f,   0.0f,   400.0f,  7000.0f, 0.0001f,   -10.0f,-5.0f}, // 64 Осветительная авиабомба c парашютом
{ LUU_19	 , 2,   13.6f,   0.0f,   400.0f,  7000.0f, 0.0001f,   -10.0f,-5.0f}, // 64 Осветительная авиабомба c парашютом
{ AO_2_5RT   , 5,   3.3f,   50.0f,    50.0f,  3000.0f, 0.0001f,  -100.0f,-1.23f}, // 65 Суббоеприпас для КМГУ-2
{ PTAB_2_5KO , 5,   3.0f,   40.0f,    50.0f,  3000.0f, 0.0001f,  -100.0f,-1.23f}, // 66 Суббоеприпас для КМГУ-2

// Для A-10C
{ BDU_33     , 0,  100.0f,   45.0f,  100.0f, 20000.0f, 0.0003f , -100.0f,-1.23f}, // 
{ BDU_50LD   , 0,  100.0f,   45.0f,  100.0f, 20000.0f, 0.0003f , -100.0f,-1.23f}, // 
{ BDU_50HD   , 2,  100.0f,   45.0f,  100.0f, 20000.0f, 0.0003f , -100.0f,-10.0f}, // 
{ BDU_50LGB	 , 4,  560.0f,  300.0f, 1000.0f, 10000.0f, 0.0017f , -60.0f,-1.0f }, // 
{ BDU_56LD   , 0,  100.0f,   45.0f,  100.0f, 20000.0f, 0.0003f , -100.0f,-1.23f}, // 
{ BDU_56LGB	 , 4,  560.0f,  300.0f, 1000.0f, 10000.0f, 0.0017f , -60.0f,-1.0f }, // 
{ MK_82AIR   , 2,  100.0f,   45.0f,  100.0f, 20000.0f, 0.0003f , -100.0f,-10.0f}, //
{ MK_82SNAKEYE, 2,  100.0f,   45.0f,  100.0f, 20000.0f, 0.0003f , -100.0f,-10.0f}, //*/

// нулевая бомба
{0,0,0.0f,0.0f,0.0f, -50.0f, -1.225f}   
};
// 	dcx = (sqrt(6724.0f+0.432f*Bomb_Massa_)-82.0f)/0.216f;
//	C_x_ = MIN_MAX(1.0f, dcx, 20.0f)/4096
// 
//	if( bomb_data->Bomb_Type_ == 1)	Vy_Hold = -35.0f;
//	else	Vy_Hold = -50.0f;
//	if( bomb_data->Bomb_Type_ == 1)	Ag = 2.0f*g;
//	else if (bomb_data->Bomb_Type_==2)	Ag = 4.0f*g;
//	else Ag = g/8.0f;

#endif