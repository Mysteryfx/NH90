#ifndef __A2G_RADAR_ENUMS__
#define __A2G_RADAR_ENUMS__

// ХИДЕР ИДЕКСОВ МАССИВА ПАРАМЕТРОВ СОСТОЯНИЯ РАДИОЛОКАТОРА A2G_RADAR
// !!!!  не редактировать этот файл 

//  READ ONLY HEADER FILE !!!!!!!!!!

enum TRadarMode { rmSAR = 0,  // картографирование синтезированием апертуры 
                  rmRBM    ,  // картографирование реальным лучем
                  rmMTD    ,  // селекция движущихся целей
                  rmSEA    ,  // режим МОРЕ
                  rmMeteo     // режим мертео
                } ;

enum TRadarScanMode  {
                  rsmSector ,
                  rsmAround 
                } ;

enum TMaterialParams {
                        mpsCam_H = 0              ,  // высота установки камеры
                        mpsBeamFi                 ,  // ширина луча диаграммы направленности
                        mpsAzimuthShft            ,
                        mpsGamma                  ,  // угловой размер зоны обзора
                        mpsScanSpeed              ,  // угловая скорость качания антенны
                        mpsScanRange              ,  // дальность обзора
                        mpsScanMode               ,  // режим сканипрвания антенны  0.0 - круговой , 1.0 - секторный
                        mpsRadarMode              ,  // режим радиолокатора
						                    
	                    //--- параметры передаваймые в шейдер O2R
                        mpsO2R_NoiseShiftX        ,
                        mpsO2R_NoiseShiftY        ,
                        mpsO2R_InColorAmplify     ,
                        mpsO2R_param0             ,
                        mpsO2R_param1             ,
                        mpsO2R_param2             ,
                        mpsO2R_param3             ,
                        mpsO2R_param4             ,
                        mpsO2R_param5             ,
                        mpsO2R_param6             ,
                        mpsO2R_param7             ,
                        mpsO2R_param8             ,
                        mpsO2R_param9             ,
                        mpsO2R_RliAmplify         ,  // коэф подавления мешающих отражений РЛИ
                        
                        //--- параметры передаваймые в шейдер BLUR
                        mpsBlur_BlurFactor        ,
                        mpsBlur_BlurMethod        ,
                        mpsBlur_reserve0          ,
                        mpsBlur_reserve1          ,

                        //--- параметры передаваймые в шейдер BEAM
                        mpsBeam_AzimuthPlusFi     ,
                        mpsBeam_Azimuth           ,
                        mpsBeam_AzimuthShift      ,
                        mpsBeam_Quantity          ,  // квантование
                        mpsBeam_Colormap          ,  // палитра
                        mpsBeam_Amplify           ,  // яркость изоьражения индикатора

						//--- параметры вычисляемые в визуализаторе
                        mpsRescanNeeded           ,  // флаг перерисовки зоны обзора
                        mpsRadarTargetDataPrt     ,  // адресс вектора параметров целей
                        
                        //--- число полей ---
                        mpsFieldCount                         
                     } ;

typedef struct 
{
  float s    ;  // относительная координата цели S
  float t    ;  // относительная координата цели T
  float size ;  //  
  float amp  ;  // ЭПР цели
} A2GRadarTargetMaterialParams ;

#endif /*__A2G_RADAR_ENUMS__*/