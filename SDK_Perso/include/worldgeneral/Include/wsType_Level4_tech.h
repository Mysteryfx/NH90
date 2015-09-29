#pragma once

// **************** 4 level ***************

///////////////////////////////////////////////////////////////////////
// СПИСОК НАЗЕМНЫХ ОБЪЕКТОВ  
///////////////////////////////////////////////////////////////////////
// ОБЪЕКТЫ ПВО
///////////////////////////////////////////////////////////////////////

const unsigned char EWR_1L13            = 1;    // Радар дальнего обнаружения 1L13
const unsigned char EWR_55G6            = 2;    // Радар дальнего обнаружения 55Ж6 
//const unsigned char KP_54K6             = 3;    // КП 54K6 командный пункт
const unsigned char V_40B6M             = 4;    // track radar                   
const unsigned char V_40B6MD            = 5;    // search radar
const unsigned char RLS_5H63C           = 6;    // track radar 
const unsigned char RLO_64H6E           = 7;    // search radar
const unsigned char PU_5P85C            = 8;    // launcher
const unsigned char PU_5P85D            = 9;    // launcher
const unsigned char RLS_9C32_1          = 10;   // track radar 
const unsigned char RLO_9C15MT          = 11;   // search radar
const unsigned char RLO_9C19M2          = 12;   // search radar
const unsigned char S300V_9A82          = 13;   // launcher
const unsigned char S300V_9A83          = 14;   // launcher
const unsigned char S300V_9A84          = 15;   // launcher
const unsigned char S300V_9A85          = 16;   // launcher
//const unsigned char BUK_9C470M1         = 17;   // КП
const unsigned char BUK_9C18M1          = 18;   // search radar
const unsigned char BUK_PU              = 19;   // launcher 
//const unsigned char BUK_LL              = 20;   // loader/launcher 
const unsigned char KUB_1C91            = 21;   // search and track radar (str)
const unsigned char KUB_2P25            = 22;   // launcher 
const unsigned char OSA_9A33BM3         = 23;   // search radar 
const unsigned char OSA_9T217           = 24;   // loader 
const unsigned char Strela_9K31         = 25;   // launcher 
const unsigned char Strela_9K35         = 26;   // launcher (ln)
const unsigned char Radar_Dog_Ear       = 27;   // search radar  
const unsigned char Tor_                = 28;   // str, ln 
const unsigned char Tunguska_           = 29;   // str, ln
const unsigned char Shilka_             = 30;   // str, ln
const unsigned char Roland_             = 31;   // str, ln  
const unsigned char Roland_Search_Radar = 32;   // search radar
const unsigned char Avenger_            = 33;   // ln
const unsigned char Patr_AN_MPQ_53_P    = 34;   // str
const unsigned char Patr_KP             = 36;   // Командный пункт для "Патриота"
const unsigned char Patriot_            = 37;   // ln
const unsigned char Gepard_             = 38;   // str, ln 
const unsigned char Hawk_Search_Radar   = 39;   // search radar
const unsigned char Hawk_Track_Radar    = 40;   // track radar 
const unsigned char Hawk_               = 41;   // ln
// const unsigned char Dzhigit_            = 42;   // ln
// const unsigned char Igla_               = 43;   // ln
// const unsigned char Stinger_            = 44;   // ln
//const unsigned char KP_S300V_9C457      = 45;   // Пункт боевого управления системы С300 В
const unsigned char wsTypeVulkan        = 46;   // ln
// const unsigned char ZU_23               = 47;   // голая неподвижная ЗУ-23
// const unsigned char ZU_23_OKOP          = 48;   // неподвижная ЗУ-23 с обваловкой
// const unsigned char ZU_23_URAL          = 49;   // подвижная ЗУ-23 на Урале
// const unsigned char M48_Chaparral       = 50;   // аналог Стрела-1
// const unsigned char M6Linebacker        = 51;   // M6 Linebacker

// const unsigned char HumanTypeStart      = 52;   // старт кодов для мужиков с базуками
// const unsigned char IglaRUS_1           = 52;   // славяне 
// const unsigned char IglaRUS_2           = 53;   // славяне пара
// const unsigned char IglaGRG_1           = 54;   // грузины
// const unsigned char IglaGRG_2           = 55;   // грузины пара
// const unsigned char StingerUSA_1        = 56;   // америка
// const unsigned char StingerUSA_2        = 57;   // америка пара
// const unsigned char StingerIZR_1        = 58;   // израильтяне
// const unsigned char StingerIZR_2        = 59;   // израильтяне пара
// const unsigned char HumanTypeEnd        = 59;   // конец кодов для мужиков с базуками

// const unsigned char ZU_23_insurgent     = 70;   // ZU-23 insurgent
// const unsigned char ZU_23_insurgent_okop = 71;   // ZU-23 insurgent
// const unsigned char ZU_23_insurgent_ural = 72;   // ZU-23 insurgent
// const unsigned char SA2_TR              = 73;   // S-125
// const unsigned char SA2_LN              = 74;   // S-125
// const unsigned char SA2_SR              = 75;   // S-125

// // При добавлении техники переопределить LastSAMType !!!
// const unsigned char LastSAMType         = SA2_SR; 
// 
// ///////////////////////////////////////////////////////////////////////
// // Наземная ТЕХНИКА
// ///////////////////////////////////////////////////////////////////////
// 
// const unsigned char wsTypeSAUmsta       = 1;    
// const unsigned char wsTypeSAUakacia     = 2;    
// const unsigned char wsTypeSAU_2C9       = 3;    
const unsigned char wsTypeTMZ5          = 4;    
const unsigned char wsTypeTZ10          = 5;    
// const unsigned char wsTypeUralApa       = 6;    
// const unsigned char wsTypeBMD1          = 7;    
// const unsigned char wsTypeBMP1          = 8;    
// const unsigned char wsTypeBMP2          = 9;    
// const unsigned char wsTypeBRDM2         = 10;   
// const unsigned char wsTypeBTR70         = 11;   
// const unsigned char wsTypeGrad          = 12;    
// const unsigned char wsTypeM818          = 13;   // Тягач M-818
// const unsigned char wsTypeHummer        = 14;   
// const unsigned char wsTypeLAV25         = 15;   
// const unsigned char wsTypeLeopard2      = 16;   
// const unsigned char wsTypeLVTP7         = 17;   
// const unsigned char wsTypeM109          = 18;   
// const unsigned char wsTypeM113          = 19;   
// const unsigned char wsTypeM113G         = 20;   
// const unsigned char wsTypeAbrams        = 21;   
// const unsigned char wsTypeMarder        = 22;   
// const unsigned char wsTypeBeregSAU      = 23;   
// const unsigned char wsTypeBeregRLS      = 24;   
const unsigned char wsTypeMAZelektro    = 25;   
const unsigned char wsTypeMAZkaraul     = 26;   
const unsigned char wsTypeMAZobsch      = 27;   
const unsigned char wsTypeMAZstol       = 28;   
// const unsigned char wsTypeSmerch        = 29;   
// const unsigned char wsTypeATZ60         = 30;   
//const unsigned char wsTypeRLS_PRW11     = 31;   
const unsigned char wsTypeRLS_RSP7      = 32;   
// const unsigned char wsTypeReis          = 33;   // Развед комплекс Рейс 
const unsigned char wsTypeRLS37         = 34;   // РЛС-37
// const unsigned char wsTypeT80           = 35;   
//const unsigned char wsTypeTiagach       = 36;   // Тягач для Кузнецова  
// const unsigned char wsTypeTPZ           = 37;   
const unsigned char wsTypeUAZ469        = 38;   
// const unsigned char wsTypeUragan        = 39;   
const unsigned char wsTypeUral375       = 40;   
const unsigned char wsTypeUral375PBU    = 41;   
// const unsigned char wsTypeZapros        = 43;   
const unsigned char wsTypeZIL135        = 44;   
const unsigned char wsTypeElektrovoz    = 45;   // Электровоз
const unsigned char wsTypeBus           = 46;   // Автобус
const unsigned char wsTypeVAZ           = 47;   // Легковой автомобиль
const unsigned char wsTypeTeplowoz      = 48;   // Тепловоз     
const unsigned char wsTypeTrolebus      = 49;   // Троллейбус       
const unsigned char wsTypeWCisterna     = 50;   // Вагон-цистерна       
const unsigned char wsTypeWGruz         = 51;   // Вагон грузовой       
const unsigned char wsTypeWGruzOtkr     = 52;   // Вагон грузовой       
const unsigned char wsTypeWagonPlatforma= 53;   // Вагон-платформа      
const unsigned char wsTypeWagonPass     = 54;   // Вагон пассажирский
//const unsigned char wsTypeRadioSt       = 55;   // Курсовой радио маяк
const unsigned char wsTypeKAMAZ_Fire    = 56;   // Курсовой радио маяк
const unsigned char wsTypeKAMAZ_Tent    = 57;   // Курсовой радио маяк
const unsigned char wsTypeLAZ_695       = 58;   // Курсовой радио маяк
// const unsigned char wsType2C1           = 59;   // CАУ
// const unsigned char wsTypeBMP3          = 60;   // БМП-3
// const unsigned char wsTypeM2Bradley     = 61;   // БМП
// const unsigned char wsTypeMCV80         = 62;   // БМП
// const unsigned char wsTypeMLRS          = 63;   // РСЗО
// const unsigned char wsTypeM_60          = 64;   // Танк М-60
// const unsigned char wsTypeLeo1          = 65;   // Танк leo1
// const unsigned char wsTypeBTR_D         = 66;   // БТР-Д
const unsigned char wsTypeGAZ_66        = 67;   // GAZ-66
const unsigned char wsTypeGAZ_3307      = 68;   // GAZ-3307
const unsigned char wsTypeGAZ_3308      = 69;   // GAZ-3308
const unsigned char wsTypeMAZ_6303      = 70;   // MAZ-6303
const unsigned char wsTypeZIL_4334      = 71;   // ZIL-4334
// const unsigned char wsTypePredator_GCS  = 72;   //КП для UAV Predator
// const unsigned char wsTypePredator_TrojanSpirit  = 73;//Станция связи для UAV Predator
// const unsigned char wsTypeZIL_SKP_11    = 74;   //СКП-11 Передвижной КП для вертолетных площадок и малых аэродромов
const unsigned char wsTypeURAL_4320T    = 75;   //Грузовик Урал-4320 с тентованным кузовом
// const unsigned char wsTypeURAL_4320_31  = 76;   //Грузовик Урал-4320-31 с бронировванной кабиной
// const unsigned char wsTypeURAL_ATsP_6   = 77;   //Пожарная машина Урал АЦП-6
// const unsigned char wsTypeZIL_APA_80    = 78;   //Аэродромный пусковой агрегат ЗиЛ АПА-80
const unsigned char wsTypeZIL_131_KUNG  = 79;   //Грузовик ЗиЛ-131 КУНГ
// const unsigned char wsTypeM1126_Stryker_ICV  = 80;  //БТР M1126 Stryker ICV
// const unsigned char wsTypeM1128_Stryker_MGS  = 81;  //БТР M1128 Stryker MGS
// const unsigned char wsTypeM1134_Stryker_ATGM  = 82; //БТР M1134 Stryker ATGM
// const unsigned char wsTypeMTLB  = 83;
// const unsigned char wsTypeBTR80 = 84;
// const unsigned char wsTypeT72 = 85;
// const unsigned char wsTypeT55 = 86;
// const unsigned char wsTypeM1043 = 87; // хаммер + M2
// const unsigned char wsTypeM1045 = 88; // хаммер + TOW
// const unsigned char wsTypeLeClerc = 89; 
// const unsigned char wsTypeRPG = 90; 
// const unsigned char wsTypeAutogun = 91; 
// const unsigned char wsTypeHEMTT_M978 = 92; 
// const unsigned char wsTypeSoldier_AK    = 93;   // 
// const unsigned char wsTypeSoldier_RPG   = 94;   // 
// const unsigned char wsTypeBOMAN   = 95;   // 
// const unsigned char wsTypeSandbox   = 96;   // 
// const unsigned char wsTypeBunker   = 97;   // 

// При добавлении техники переопределить LastTankType
//const unsigned char LastTankType = wsTypeBunker;//  номер последнего Tank-а

///////////////////////////////////////////////////////////////////////
// СПИСОК НАЗЕМНЫХ ОБЪЕКТОВ (КОНЕЦ) 
///////////////////////////////////////////////////////////////////////
