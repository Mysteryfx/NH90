#pragma once

// **************** 4 level ***************
// Free-fall air objects; parts
const unsigned char wsType_OBLOMOK_1    =   1;  // 1-й обломок самолета
const unsigned char wsType_OBLOMOK_2    =   2;  // 2-й обломок самолета
const unsigned char wsType_OBLOMOK_3    =   3;  // 3-й обломок самолета
const unsigned char wsType_OBLOMOK_4    =   4;  // 4-й обломок самолета
const unsigned char wsType_OBLOMOK_5    =   5;  // 5-й обломок самолета
const unsigned char wsType_OBLOMOK_6    =   6;  // 6-й обломок самолета
const unsigned char OBLOMOK_OBSHIWKI_1  =   7;  // Осколок обшивки
const unsigned char OBLOMOK_OBSHIWKI_2  =   8;  // Осколок обшивки
const unsigned char PILOT_K36           =   9;  // Пилот в кресле K36 при катапультиров.
const unsigned char PILOT_K37           =  10;  // Пилот в кресле K37 при катапультиров.
const unsigned char PILOT_PARASHUT      =  11;  // Пилот RUS под парашютом
const unsigned char FONAR_OTK           =  12;  // Сброшенный фонарь
// Free-fall air objects - snars
const unsigned char wsType_Chaff        =  13;  // радио-ловушка
const unsigned char wsType_Flare        =  14;  // тепловая ловушка
const unsigned char wsType_ShortMTail   =  15;  // короткий хвост ракет
const unsigned char wsType_SmallBomb    =  16;  // бомба из кассеты
const unsigned char PILOT_ACER          =  17;  // Пилот в кресле ACER при катапультиров.
const unsigned char PILOT_F14_SEAT      =  18;  // Пилот  F14 в кресле при катапультиров.
const unsigned char PILOT_PARASHUT_US   =  19;  // Пилот US под парашютом
const unsigned char A_10_FONAR          =  20;  // Сброшенный фонарь самолета А-10
const unsigned char F_14A_FONAR         =  21;  // Сброшенный фонарь самолета F-14A         
const unsigned char F_15_FONAR          =  22;  // Сброшенный фонарь самолета F-15
const unsigned char F_16_FONAR          =  23;  // Сброшенный фонарь самолета F-16
const unsigned char F_18C_FONAR         =  24;  // Сброшенный фонарь самолета F-18C
const unsigned char MIG_23_FONAR        =  25;  // Сброшенный фонарь самолета МиГ-23
const unsigned char MIG_25_FONAR        =  26;  // Сброшенный фонарь самолета МиГ-25
const unsigned char MIG_27_FONAR        =  27;  // Сброшенный фонарь самолета МиГ-27
const unsigned char MIG_29_FONAR        =  28;  // Сброшенный фонарь самолета МиГ-29
const unsigned char MIG_29K_FONAR       =  29;  // Сброшенный фонарь самолета МиГ-29К
const unsigned char MIG_31_FONAR_P      =  30;  // Сброшенный фонарь самолета МиГ-31 (передний летчик)
const unsigned char MIG_31_FONAR_Z      =  31;  // Сброшенный фонарь самолета МиГ-31 (задний летчик)
const unsigned char SU_24_FONAR_L       =  32;  // Сброшенный фонарь самолета Су-24 (левый летчик)
const unsigned char SU_24_FONAR_R       =  33;  // Сброшенный фонарь самолета Су-24 (правый летчик)
const unsigned char SU_25_FONAR         =  34;  // Сброшенный фонарь самолета Су-25
const unsigned char SU_27_FONAR         =  35;  // Сброшенный фонарь самолета Су-27
const unsigned char SU_30_FONAR         =  36;  // Сброшенный фонарь самолета Су-30
const unsigned char SU_33_FONAR         =  37;  // Сброшенный фонарь самолета Су-33
const unsigned char SU_39_FONAR         =  38;  // Сброшенный фонарь самолета Су-39

const unsigned char TORNADO_FONAR       =  39;  // Сброшенный фонарь самолета TORNADO
const unsigned char Mirage_FONAR        =  40;  // Сброшенный фонарь самолета Mirage
const unsigned char F_4_FONAR_P         =  41;  // Сброшенный фонарь самолета F-4 передний
const unsigned char F_4_FONAR_Z         =  42;  // Сброшенный фонарь самолета F-4 задний
const unsigned char F_5_FONAR           =  43;  // Сброшенный фонарь самолета F-5
const unsigned char SU_17_FONAR         =  44;  // Сброшенный фонарь самолета Cу-17
const unsigned char SU_34_FONAR_L       =  45;  // Сброшенный фонарь самолета Су-34 (левый летчик)
const unsigned char SU_34_FONAR_R       =  46;  // Сброшенный фонарь самолета Су-34 (правый летчик)
const unsigned char MIG_29C_FONAR       =  47;  // Сброшенный фонарь самолета МиГ-29
const unsigned char MIG_29G_FONAR       =  48;  // Сброшенный фонарь самолета МиГ-29
const unsigned char SU_25T_FONAR        =  49;  // Сброшенный фонарь самолета Су-25Т
const unsigned char wsType_Flare_GREEN  =  50;// сигнальная ракета ЗЕЛЕНАЯ
const unsigned char wsType_Flare_RED    =  51;// сигнальная ракета КРАСНАЯ
const unsigned char wsType_Flare_WHITE  =  52;// сигнальная ракета БЕЛАЯ
const unsigned char wsType_Flare_YELLOW =  53;// сигнальная ракета ЖЕЛТАЯ

const unsigned char PILOT_DEAD          =  54;// труп пилота
const unsigned char PARACHUTE_ON_GROUND =  55;// погасший купол
const unsigned char PILOT_PARASHUT_P51	=  56;// погасший купол
const unsigned char PILOT_F15_WITH_CHAIR=  58;// 


const unsigned char wsType_Point        =  201; // woPoint
const unsigned char wsType_PathPoint        =  202; // точка пути в виде F10


// Взрывы
const unsigned char GROUND_EXP          = 1;  // Наземный  взрыв 1  
//////////////////////////////////////////////////////////////////////////

const unsigned char Heliport_standart   = 1; // вертолётодром

const unsigned char wsType_RunWay       = 100;

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//        Airplanes list.       Список самолетов
/////////////////////////////////////////////////////////////////////////
// !!! ПРИ ДОБАВЛЕНИИ ТИПОВ САМОЛЕТОВ ПЕРЕОПРЕДЕЛИТЬ LastPlaneType !!! //
// !!! Добавить уникальные коды в wSADL_parser::getStypeAndPlatform()!!//
/////////////////////////////////////////////////////////////////////////
//  Истребители и многоцелевые самолеты.
const unsigned char MiG_23  =  1;   // MiG-23 Flogger   
const unsigned char MiG_29  =  2;   // MiG-29 Fulcrum   
const unsigned char Su_27   =  3;   // Su-27 Flanker    
const unsigned char Su_33   =  4;   // Su-33                        
const unsigned char F_14    =  5;   // F-14 Tomcat                          
const unsigned char F_15    =  6;   // F-15 Eagle       
const unsigned char F_16    =  7;   // F-16 Fighting Falcon
// Перехватчики.
const unsigned char MiG_25  =  8;   // MiG-25 Foxbat    
const unsigned char MiG_31  =  9;   // MiG-31 Foxhound  
const unsigned char F_2     =  10;  // Tornado F.2 IDS GR.1A    
// Истребители-бомбaрдировщики.
const unsigned char MiG_27  =  11;  // MiG-27 Flogger-D 
const unsigned char Su_24   =  12;  // Su-24 Fencer     
const unsigned char Su_30   =  13;  // Su-30                        
const unsigned char FA_18   =  14;  // F/A-18A Hornet                       
const unsigned char F_111   =  15;  // F-111    
// Штурмовики.
const unsigned char Su_25   =  16;  // Su-25 Frogfoot   
const unsigned char A_10A    =  17;  // A-10 Thunderbolt II
// Стратегические бомбардировщики.
const unsigned char Tu_160  =  18;  // Tu-160 Blackjack                 
const unsigned char B_1     =  19;  // B-1 Lancer                       
const unsigned char Su_34   =  20;  // Cу-34 Истребитель-бомбaрдировщик
const unsigned char Tu_95   =  21;  // Tu-95 Bear   
const unsigned char Tu_142  =  22;  // Tu-142 Bear  
const unsigned char B_52    =  23;  // B-52 Stratofortress                  
// Бомбардировщики.
const unsigned char MiG_25P =  24;  // MiG_25P // Перехватчик
const unsigned char Tu_22M3 =  25;  // Tu-22M3 Backfire 
// АВАКС.
const unsigned char A_50    =  26;  // А-50 Mainstay
const unsigned char E_3     =  27;  // E-3 Sentry                       
// Самолеты заправщики
const unsigned char IL_78   =  28;  // IL-78 Midas                      
const unsigned char KC_10   =  29;  // KC-10 Extender                       
// Транспортные самолеты.
const unsigned char IL_76   =  30;  // IL-76 Candid 
const unsigned char C_130   =  31;  // C-130 Hercules
const unsigned char MIG_29K =  32;  // MiG-29K - истребитель                        
const unsigned char S_3R    =  33;  // S-3A VICING, палубный, заправщик                     
const unsigned char Mirage  =  34;  // Mirage 2000 - истребитель                        
// Беспилотные самолеты-разведчики.
const unsigned char Tu_143  =  35;  // Tu-143 (ВР-3) "Рейс"                     
const unsigned char Tu_141  =  36;  // Tu-141 (ВР-2) "Стриж"

const unsigned char F_117   =  37;  // F-117 штурмовик                          
const unsigned char Su_39   =  38;  // SU-39 штурмовик                          
const unsigned char AN_26B  =  39;  // AN-26B транспортный                          
const unsigned char AN_30M  =  40;  // AN-30M транспортный                              
const unsigned char E_2C    =  41;  // E-2C Hawkeye палубный AWACS как AN_26B                           
const unsigned char S_3A    =  42;  // S-3A Viking палубный штурмовик                           
const unsigned char AV_8B   =  43;  // AV-8B Harrier истребитель                            
const unsigned char EA_6B   =  44;  // EA-6B Prowler радиопротиводействия штурмовик                         
const unsigned char F_4E    =  45;  // F-4E Phantom II истребитель                          
const unsigned char F_5E    =  46;  // F-5E Tiger истребитель                           
const unsigned char C_17    =  47;  // C-17 транспортный USA                            
const unsigned char SU_17M4 =  48;  // Cy-17M4 истребитель                          
const unsigned char MiG_29G  = 49;  // MiG-29 Fulcrum германский    
const unsigned char MiG_29C  = 50;  // MiG-29 Fulcrum германский    
const unsigned char Su_24MR  = 51;  // Su-24MR Fencer       
const unsigned char F_16A    = 52;  // F-16A Fighting Falcon        
const unsigned char FA_18C   = 53;  // F/A-18C Hornet           
const unsigned char Su_25T   = 54;  // Su-25 Frogfoot   
const unsigned char RQ_1A_Predator = 55;// Predator UAV
const unsigned char TORNADO_IDS = 56;// Predator UAV
const unsigned char Yak_40   =  57;  // JAK_40 транспортный                
const unsigned char A_10C    =  58;  // A-10C
const unsigned char F_15E	 =  59;  // F-15E        
const unsigned char KC_135   =  60;  // KC-135
const unsigned char L_39ZA	 =  61;  // Л-39ЗА учебно-тренировочный с пушкой
const unsigned char P_51B    =  62;  // P-51B
const unsigned char P_51D    =  63;  // P-51D
const unsigned char TF_51D    =  65;  // TF_51D

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
const unsigned char LastPlaneType = 150;//выделяем диапазон под самолеты
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//        Helicopters list.       Список вертолетов
/////////////////////////////////////////////////////////////////////////
const unsigned char MI_8MT  =  1 + LastPlaneType;       // Mи-8МТ. Россия/Украина   
const unsigned char MI_24W  =  2 + LastPlaneType;       // Mи-24B. Россия/Украина   
const unsigned char MI_26   =  3 + LastPlaneType;       // Mи-26.  Россия/Украина   
const unsigned char KA_27   =  4 + LastPlaneType;       // Ka-27.  Россия/Украина                       
const unsigned char KA_50   =  5 + LastPlaneType;       // Ka-50.  Россия                       
const unsigned char KA_52   =  6 + LastPlaneType;       // Ka-52.  Россия                       
const unsigned char AH_64A  =  7 + LastPlaneType;       // AH-64A Apache.         NATO                          
const unsigned char AH_64D  =  8 + LastPlaneType;       // AH-64D Apache Longbow. NATO                          
const unsigned char CH_47D  =  9 + LastPlaneType;       // CH-47D Chinook.        NATO                          
const unsigned char CH_53E  = 10 + LastPlaneType;       // CH-53E Super Stallion. NATO                          
const unsigned char SH_60B  = 11 + LastPlaneType;       // SH-60B Sea Hawk.       NATO                          
const unsigned char UH_60A  = 12 + LastPlaneType;       // UH-60A Night Hawk.     NATO                          
const unsigned char AH_1W   = 13 + LastPlaneType;       // AH-1W  Super Cobra.    NATO                          
const unsigned char SH_3H   = 14 + LastPlaneType;       // SH-3W  Sea King.       NATO                          
const unsigned char Sea_Lynx= 15 + LastPlaneType;       // Sea Lynx Mk88.         NATO                          
const unsigned char AB_212  = 16 + LastPlaneType;       // AB-212ASW.             NATO      
const unsigned char MI_28N  = 17 + LastPlaneType;       // Ми-28Н
const unsigned char OH_58D  = 18 + LastPlaneType;       // OH-58D Kiowa
// Константа, определяющая номер последнего вертолетa
const unsigned char LastHelicopterType = 255; ////выделяем диапазон под вертолеты
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// Air-to-Air Missiles.  Ракеты воздух-воздух.
//                          0 - пусто empty 
const unsigned char R_550  =  1;   // R550 Magic 2                              
const unsigned char MICA_T =  2;   // MICA, IR                              
const unsigned char MICA_R =  3;   // MICA, AR  
const unsigned char Super_530F =  4;   // Super 530F                                
const unsigned char P_98   =  5;   // нет такой P-98 == P-98R (AA-3  “Anab”)    
const unsigned char P_4R   =  6;   // нет такой P-4R      (AA-5  'Ash')         
const unsigned char P_40R  =  7;   // P-40R   (AA-6  'Acrid')       
const unsigned char P_23R  =  8;   // P-23R   (AA-7  'Apex')        
const unsigned char P_24R  =  9;   // нет такой P-24R   (AA-7  'Apex')      
const unsigned char P_60   = 10;   // нет такой P-60      (AA-8  'Aphid')бл. боя        
const unsigned char P_33E  = 11;   // P-33Э  (AA-9  'Amos') бол.дальности   
const unsigned char P_27AE = 12;   // P-27AЭ (AA-10 'Alamo')cр.дальности        
const unsigned char P_27P  = 13;   // P-27P  (AA-10 'Alamo')cр.дальности        
const unsigned char P_27PE = 14;   // P-27PЭ (AA-10 'Alamo')cр.дальности        
const unsigned char P_27T  = 15;   // P-27T  (AA-10 'Alamo')cр.дальности        
const unsigned char P_27TE = 16;   // P-27TЭ (AA-10 'Alamo')cр.дальности        
const unsigned char P_27EM = 17;   // P-27ЭM (AA-10 'Alamo')cр.дальности        
const unsigned char P_73   = 18;   // P-73   (AA-11 'Archer')бл. боя        
const unsigned char P_77   = 19;   // P-77 (PBB-AE) (AA-12)cр.дальности                 
const unsigned char P_37   = 20;   // P-37                      

const unsigned char AIM_7  = 21;        // AIM-7   'Sparrow'            
const unsigned char AIM_9  = 22;        // AIM-9   'Sidewinder'     
const unsigned char AIM_54 = 23;        // AIM-54  'Phoenix'        
const unsigned char AIM_120= 24;        // AIM-120 'AMRAAM'     

const unsigned char P_23T  = 25;        // P-23T   (AA-7  'Apex')       
const unsigned char P_24T  = 26;        // нет такой P-24T   (AA-7  'Apex')     
const unsigned char P_40T  = 27;        // P-40T   (AA-6  'Acrid')      
const unsigned char SeaSparrow = 28;    // Sea Sparrow 


// Unguided rockets.   Неуправляемые реактивные снаряды.
const unsigned char C_8CM  = 30;        // C-8CM (с цветным дымом)       
const unsigned char C_5    = 31;        // C-5       
const unsigned char C_8    = 32;        // C-8       
const unsigned char C_13   = 33;        // C-13       
const unsigned char C_24   = 34;        // C-24       
const unsigned char C_25   = 35;        // C-25       
const unsigned char HYDRA_70 = 36;      // HYDRA-70       
const unsigned char Zuni_127 = 37;      // Zuni-127       
const unsigned char Zuni_127CM = 38;    // Zuni-127CM (с цветным дымом)       
// wsType для новых гидр
const unsigned char HYDRA_70_MK1 = 144;
const unsigned char HYDRA_70_MK5 = 145;
const unsigned char HYDRA_70_MK61 = 146;
const unsigned char HYDRA_70_M151 = 147;
const unsigned char HYDRA_70_M156 = 148;
const unsigned char HYDRA_70_WTU1B = 149;
const unsigned char HYDRA_70_M274 = 150;
const unsigned char HYDRA_70_M257 = 151;
const unsigned char HYDRA_70_M278 = 152;
const unsigned char HYDRA_70WP    = 154;
const unsigned char C_8OFP2       = 155;
const unsigned char C_8OM         = 158;
const unsigned char HVAR 		  = 159;


// Air-to-Surface Missiles. Ракеты воздух-поверхность.  

const unsigned char AGM_114K = 39; // как AGM-114       
const unsigned char AGM_119= 40; // Penguin, Norway     
const unsigned char X_22   = 41; // X-22 Burya wing,anti-ship (AS-4'Kitchen')       
const unsigned char X_29TE = 42; // X_29TE (AS-14  'Kedge' экспорт)         
const unsigned char X_23   = 43; // X-23 Grom anti-radar(AS-7   'Kerry')        
const unsigned char X_28   = 44; // X-28 anti-radar(AS-9   'Kyle')      
const unsigned char X_25ML = 45; // X-25ML laser   (AS-10  'Karen')     
const unsigned char X_58   = 46; // X-58  anti-radar(AS-11  'Kilter')       
const unsigned char X_25MP = 47; // X-25MP anti-radar(AS-12  'Kegler')      
const unsigned char AT_6   = 48; // AT_6 (9M114) radio-command anti-tank, STURM     
const unsigned char X_29L  = 49; // X-29L laser    (AS-14  'Kedge')     
const unsigned char X_55   = 50; // X-55  wing     (AS-15A 'Kent')      
const unsigned char X_65   = 51; // X-65  wing     (AS-15B 'Kent')      
const unsigned char X_15   = 52; // X-15 аnti-ship,ballistic(AS-16  'Kickback') 
const unsigned char X_31A  = 53; // X-31A аnti-ship(AS-17  'Krypton')       
const unsigned char X_59M  = 54; // X-59M Ovod     (AS-18   Kazoo)      
const unsigned char X_35   = 55; // X-35           (AS-20  'Kayak') 
const unsigned char X_41   = 56; // X-41 (unknown) аnti-ship        
const unsigned char Vikhr  = 57; // 9M120 Vikhr laser (AT-12) {air-tank}        
const unsigned char Vikhr_M= 58; // 9M120M  Vikhr-M (AT-X-16)laser{air-tank}    
const unsigned char AGM_114= 59; // AGM-114 Hellfire laser                      
const unsigned char AGM_45 = 60; // AGM-45  'Shrike'    anti-radar          
const unsigned char AGM_65K= 61; // AGM-65K 'Maverick'          
const unsigned char AGM_84A= 62; // AGM-84  'Harpoon'           
const unsigned char AGM_84E= 63; // AGM-84E 'SLAM'      
const unsigned char AGM_86 = 64; // AGM-86  'CALCM'    wing     
const unsigned char AGM_88 = 65; // AGM-88  'HARM'      anti-radar
const unsigned char Sea_Eagle = 66; // Sea_Eagle    
const unsigned char SAV611 = 67; // 4K60 корабельного комплеса "Шторм"          нет такой           
const unsigned char AGM_122= 68; // AGM-122 'Sidearm'   anti-radar  
const unsigned char AGM_123= 69; // AGM-123 'Skipper'   laser   
const unsigned char AGM_65E= 70; // AGM-65E 
const unsigned char AGM_130= 71; // AGM-130         wing        
const unsigned char ALARM  = 72; // ALARM
const unsigned char X_23L  = 73; // X-23L Grom laser (AS-7   'Kerry')       
const unsigned char X_25MR = 74; // X-25MR TV     (AS-12  'Kegler')     
const unsigned char X_29T  = 75; // X-29T  TV     (AS-14  'Kedge')      
const unsigned char X_31P  = 76; // X-31П аnti-radar(AS-17  'Krypton')
const unsigned char AGM_65D= 77; // AGM-65D  'Maverick'         
const unsigned char Kormoran=78; // Kormoran AS 34         

// Surface-to-Air Missiles and others. Ракеты поверхность-воздух и прочие.

const unsigned char SM_2   = 79; // SM-2    для TICONDEROGA
const unsigned char SA5B55 = 80; // 5B55    Комплекса С-300ПМУ
const unsigned char SA48H6E2=81; // 48Н6Е2  Комплекса С-300ПМУ
const unsigned char SA9M82 = 82; // 9М82    Комплекса С-300В
const unsigned char SA9M83 = 83; // 9М83    Комплекса С-300В
const unsigned char SA3M9M = 84; // 3М9М    Комплекса 2К12 Куб
const unsigned char SA9M33 = 85; // 9М33    Комплекса 9К33 Оса
const unsigned char SA9M31 = 86; // 9М31    Комплекса 9К31 Стрела-1
const unsigned char SA9M38M1=87; // 9М38М1  Комплекса 9К37 Бук
const unsigned char SA9M333= 88; // 9М333   Комплекса 9К35 Стрела-10
const unsigned char SA9M330= 89; // 9М330   Комплекса Тор М1
const unsigned char SA9M311= 90; // 9М311   Комплекса 2C6М Тунгуска
const unsigned char Igla_1E= 91; // Igla-1E Комплекса ПЗРК Игла
const unsigned char MIM_104= 92; // MIM-104 Комплекса Patriot
const unsigned char FIM_92C= 93; // FIM_92C STINGER Комплекса Avenger и ручная.
//------------------------------------------------------------------------------
const unsigned char GRAD_RAKETA  = 94; // GRAD-RAKETA   РСЗО Град ракета
const unsigned char URAGAN_RAKETA  = 179; // URAGAN-RAKETA   РСЗО Ураган ракета
const unsigned char SMERCH_RAKETA= 95; // SMERCH-RAKETA РСЗО Смерч ракета   
const unsigned char SA5B27       = 97; // Ракета З-В 5B27 для "Грозного"
const unsigned char HAWK_RAKETA  = 98; // Ракета для Hawk
const unsigned char ROLAND_R     = 99; // Ракета ROLAND

const unsigned char AIM_120C            = 106;
//------------------------------------------------------------------------------
// Surface-to-Surface Missiles. Ракеты поверхность-поверхность (для кораблей).  
const unsigned char P_35   = 118; // Ракета корабельного комплекса Прогресс-М
const unsigned char P_500  = 119; // Ракета корабельного комплекса Базальт
const unsigned char P_700  = 120; // Ракета корабельного комплекса Гранит
const unsigned char P_15U  = 121; // Ракета корабельного комплекса Термит
const unsigned char P_120  = 122; // Ракета корабельного комплекса Малахит
const unsigned char R_85   = 123; // Ракета-торпеда корабельного комплекса Метель
const unsigned char R_85U  = 124; // Ракета-торпеда корабельного комплекса Раструб  
const unsigned char BGM_109B  = 125; // Ракета корабельного комплекса Томагавк
const unsigned char AGM_84S   = 126; // Ракета корабельного комплекса Гарпун
const unsigned char TOW       = 130; // вертолетный TOW
const unsigned char M26_RAKETA= 131; // Нурс к MLRS
const unsigned char AGM_154   = 132; // AGM-154 JSOW
const unsigned char S_25L     = 133; // S-25L
const unsigned char SCUD_RAKETA = 96; // 

//------------------------------------------------------------------------------
const unsigned char wsType_Shell_SPPU = 134; //  снаряд авиационный. 20 мм 
//------------------------------------------------------------------------------
//  Added missiles
//Air-to-Air
const unsigned char AIM_9P = 135;       // AIM-9P   'Sidewinder' rear_aspect IR missile 
const unsigned char AIM_9X = 136;       // AIM-9X   'Sidewinder' perspective all aspect IR/UV missile   

const unsigned char MIM_72G  = 137;     // M48 Chaparral missile, sidewinder-like
const unsigned char AGM_65H  = 138; // new mavericks        
const unsigned char AGM_65G  = 139;
const unsigned char TGM_65G  = 140; // training
const unsigned char TGM_65D  = 141; // training
const unsigned char CATM_65K = 142; // training

// Surface-to-Surface
const unsigned char MALUTKA     = 127; // Ракета Малютка БМД-1 и БМП-1
const unsigned char KONKURS     = 128; // Ракета Конкурс БМП-2
const unsigned char P_9M117     = 129; // Ракета к БМП-3
const unsigned char ATGM_Kornet = 153; // 9M133
const unsigned char ATGM_Reflex = 156; // 9M119
const unsigned char ATGM_Svir   = 157; // 9M119
const unsigned char TOW2        = 160; // правильный TOW для наземки

// Air-to-air
const unsigned char CATM_9 = 143;       //
const unsigned char TGM_65H = 154;

const unsigned char TGM_65E  = 155; // training
const unsigned char wsType_Gun_Invalid = 141;

// Bombs. Бомбы.    
const unsigned char BetAB_150DS =  1;  // BetAB-150DS,
const unsigned char BetAB_250   =  2;  // BetAB-250,
const unsigned char BetAB_500   =  3;  // BetAB-500,
const unsigned char BetAB_500ShP=  4;  // BetAB-500ShP          
const unsigned char FAB_100     =  5;  // FAB-100,
const unsigned char FAB_250     =  6;  // FAB-250,
const unsigned char FAB_500     =  7;  // FAB-500
const unsigned char FAB_1000    =  8;  // FAB-1000
const unsigned char FAB_1500    =  9;  // FAB-1500
const unsigned char FAB_5000    = 10;  // FAB-5000 нет такой        
const unsigned char KAB_500     = 11;  // KAB-500       
const unsigned char KAB_500Kr   = 12;  // KAB-500Kr,
const unsigned char KAB_500KrOD = 13;  // KAB-500Kr-OD, 
const unsigned char KAB_1500Kr  = 14;  // KAB-1500Kr    
const unsigned char ODAB_500PM  = 16;  // ODAB-500 PM   
const unsigned char ODAB_250    = 17;  // ODAB-250  нет такой
const unsigned char RBK_250     = 18;  // RBK-250,
const unsigned char RBK_250S    = 19;  // RBK-250S,
const unsigned char RBK_500AO   = 20;  // RBK-500AO,
const unsigned char RBK_500SOAB = 21;  // RBK-500SOAB   
const unsigned char Puma        = 22;  // Puma
const unsigned char BL_755      = 23;  // BL_755    
const unsigned char GluB        = 24;  // Glubinaya Bomba                           
const unsigned char NB_1        = 25;  // Nuclear Bomb - 1 нет такой                            
const unsigned char NB_2        = 26;  // Nuclear Bomb - 2                          
const unsigned char Torpedo     = 27;  // Torpedo                           
const unsigned char XZAB_250    = 28;  // XZAB-250,
const unsigned char XZAB_500    = 29;  // XZAB-500      
const unsigned char Mk_81       = 30;  // Mk-81
const unsigned char Mk_82       = 31;  // Mk-82
const unsigned char Mk_83       = 32;  // Mk-83
const unsigned char Mk_84       = 33;  // Mk-84 
const unsigned char M_117       = 34;  // M-117             
const unsigned char CBU_97      = 35;  // CBU-97                
const unsigned char GBU_10      = 36;  // GBU-10
const unsigned char GBU_11      = 37;  // GBU-11
const unsigned char GBU_12      = 38;  // GBU-12
const unsigned char GBU_16      = 39;  // GBU-16
const unsigned char GBU_17      = 40;  // GBU-40
const unsigned char GBU_24      = 41;  // GBU-41
const unsigned char GBU_15      = 42;  // GBU-15        
const unsigned char GBU_27      = 43;  // GBU-27
const unsigned char GBU_22      = 44;  // GBU-22
const unsigned char ROCKEYE     = 45;  // Бомбовый контейнер

const unsigned char AGM_62      = 47;  // AGM-62 Walley
const unsigned char GBU_28      = 48;  // GBU-28            
const unsigned char GBU_29      = 49;  // GBU-29
const unsigned char GBU_30      = 50;  // GBU-30 JDAM

const unsigned char FAB_100P    = 53;  // FAB-100П
const unsigned char FAB_250P    = 54;  // FAB-250П
const unsigned char FAB_500P    = 55;  // FAB-500П
const unsigned char FAB_500_3   = 56;  // FAB-500 - 3 
const unsigned char MW_1        = 57;  // MW-1 dispenser
const unsigned char PB_250      = 58;  // PB-250    c парашютом
const unsigned char Z_BAK_3     = 59;  // зажигательный бак - 3 тип 

const unsigned char Mk_50_Torpedo = 61;// торпеда Mk-60 
const unsigned char Durandal    = 62;  // BLU_107 Durandal
const unsigned char SAB_100     = 63;  // Осветительная авиабомба
const unsigned char LUU_2B      = 64;  // Осветительная авиабомба из кассеты SUU_25
const unsigned char AO_2_5RT    = 65;  // Суббоеприпас для КМГУ-2
const unsigned char PTAB_2_5KO  = 66;  // Суббоеприпас для КМГУ-2

// 67 и 68 - PTAB_2_5RT_void и AO_2_5RT_void

const unsigned char BDU_33      = 69;
const unsigned char BDU_50LD    = 70;
const unsigned char BDU_50HD    = 71;
const unsigned char BDU_50LGB   = 72;
const unsigned char BDU_56LD    = 73;
const unsigned char BDU_56LGB   = 74;
const unsigned char MK_82AIR    = 75;
const unsigned char LUU_19      = 76;   // IR-осветительная бомба
const unsigned char CBU_87      = 77;
const unsigned char CBU_89      = 78;
const unsigned char MK_82SNAKEYE = 79;
const unsigned char LUU_2AB     = 80;
const unsigned char LUU_2BB     = 81;
const unsigned char LUU_1       = 82;
const unsigned char LUU_5       = 83;
const unsigned char LUU_6       = 84;

// Phase 2, JDAM
const unsigned char GBU_31      = 85;
const unsigned char GBU_38      = 86;
const unsigned char CBU_105     = 87;
const unsigned char CBU_103     = 88;
const unsigned char CBU_104     = 89;

//P-54D bomb weapon
const unsigned char AN_M64     = 90;
const unsigned char RBK_500U   = 91;

const unsigned char GBU_31_V_3B = 92;

//       Контейнеры и баки. Балочные держатели.
const unsigned char UB_13       = 1;    // контейнер НУРС-ов
const unsigned char UB_32_1     = 2;    // контейнер НУРС-ов
const unsigned char MBD_3       = 3;    // балочный держатель на 3 бомбы
const unsigned char LAU_88      = 4;    // балочный держатель на 3 MAVERICK
const unsigned char PTB_3000    = 5;    // Бак самолета Cу-24 на 3000 кг топлива
const unsigned char B_20        = 6;    // контейнер НУРС-ов
const unsigned char S_25_C      = 7;    // контейнер C-25 c ракетой 
const unsigned char LAU_10      = 8;    // контейнер НУРС-ов
const unsigned char LAU_61      = 9;    // контейнер НУРС-ов
const unsigned char F_15_PTB    =10;    // Бак самолета F-15C    1600 кг топлива
const unsigned char F_16_PTB_N1 =11;    // Бак 1 самолета F-16   1120 кг топлива
const unsigned char F_16_PTB_N2 =12;    // Бак 2 самолета F-16    900 кг топлива
const unsigned char F_18_PTB    =13;    // Бак самолета F-18     1000 кг топлива
const unsigned char MIG_23_PTB  =14;    // Бак самолета MIG-23    640 кг топлива
const unsigned char MIG_25_PTB  =15;    // Бак самолета MIG-25   4370 кг топлива
const unsigned char PTB_1150    =16;    // Бак 1 самолета MIG-29  900 кг топлива
const unsigned char PTB_1500    =17;    // Бак 2 самолета MIG-29 1200 кг топлива
const unsigned char SPPU_22     =18;    // Пушечный контейнер СППУ-22
const unsigned char KINGAL      =19;    // Управляющий контейнер Су-39
const unsigned char UPK_23_25   =20;    // Пушечный контейнер УПК-23-25
const unsigned char MBD         =21;    // Балочный держатель бомб 6-точечный 
const unsigned char MBD_4       =22;    // Балочный держатель бомб 4-точечный 
const unsigned char TU_22_MBD   =23;    // Балочный держатель бомб для Ту-22М3 
const unsigned char S_25_PU     =24;    // контейнер C-25 без ракеты
const unsigned char ALQ_131     =25;    // Контейнер РЭП (США)
const unsigned char LANTIRN     =26;    // Контейнер управл.F-16
const unsigned char LANTIRN_F18 =27;    // Контейнер управл. F-18   
const unsigned char PAVETACK    =28;    // Контейнер управл. F-111  
const unsigned char SORBCIJA    =29;    // Контейнер РЭП (Су-27)    
const unsigned char SPS_141     =30;    // Контейнер РЭП (Россия)   
const unsigned char PTB_B_1B    =31;    // Бак самолета B-1B на 14915 кг топлива    
const unsigned char MBD_3_LAU_10=32;    // MDB-3 & 3 LAU_10
const unsigned char MBD_3_LAU_61=33;    // контейнер НУРС-ов MDB-3 & 3 LAU_61
const unsigned char AT_9M120    =34;    // Vikhr (AT-12) {air-tank}     
const unsigned char AT_9M120M   =35;    // Vikhr-M (AT-X-16) {air-tank} 
const unsigned char F_5_PTB     =36;    // Бак самолета F-5 1000 л топлива
const unsigned char Sky_Shadow  =37;    // Контейнер РЭП, Tornado
const unsigned char PTB_F2_1500 =38;    // Бак самолета Tornado на 1500 кг топлива
const unsigned char M2000_PTB   =39;    // Бак самолета Mirage  на 1000 кг топлива
const unsigned char MBD_A10_2   =40;    // Балочный держатель 2х ракет для A-10
const unsigned char F4_PTB_WING =41;    // Бак самолета F-4 крыльевой
const unsigned char F4_PTB_FUZ  =42;    // Бак самолета F-4 фюзеляжный
const unsigned char AGM_114_Pilon = 43; // балочный держатель 4х точечный
const unsigned char AT_6_9M114  =44;    // SHTURM контейнер типа Vikhr-M    
const unsigned char F4_PILON    =45;    // Балочный держатель 2х ракет для F4
const unsigned char MER_L_P_60  = 46;   // Балочный держатель 2х ракет P_60 левый
const unsigned char MER_R_P_60  = 47;   // Балочный держатель 2х ракет P_60 правый
const unsigned char MER_TOW     = 48;   // Балочный держатель на 4 TOW
const unsigned char MER_2_F_18  = 49;   // Балочный держатель на 2, F-18
const unsigned char MER_9_B52   = 50;   // Балочный держатель на 9, B-52
const unsigned char MER_12_B52  = 51;   // Балочный держатель на 12, B-52
const unsigned char MBD_6_B52   = 52;   // Балочный держатель на 6, B-52
const unsigned char PTB_2000    = 53;   // Топливный бак на 2000 л для Су-34
const unsigned char PTB_800L_Wing = 54; // Топливный бак на 800 л для Mig-23
const unsigned char PTB_S_3     = 55;   // Топливный бак для S-3
const unsigned char PTB_367GAL  = 56;   // Топливный бак на 367 gal для F-14
const unsigned char SKY_SHADOW  = 57;   // Контейнер РЭП 
const unsigned char BOZ_100     = 58;   // Контейнер c ловушками
const unsigned char LANTIRN_F14 = 59;   // Контейнер управл. F-14
const unsigned char FLIR_POD    = 60;   // Контейнер управл. F-18
const unsigned char PTB_1150_29 = 61;   // Топливный бак на 1150 l для MiG-29
const unsigned char TANGAZH     = 62;   // Контейнер для разведки 
const unsigned char EPHIR       = 63;   // Контейнер для разведки 
const unsigned char SHPIL       = 64;   // Контейнер для разведки 
const unsigned char FANTASM     = 65;   // Контейнер управл.
const unsigned char SmokeGenerator_red = 66;// Дымовой контейнер (красный).
const unsigned char SmokeGeneratorAIM_red = 67;// Дымовой контейнер (красный). AIM-9S
const unsigned char B_20CM       = 68;  // контейнер маркерных НУРС-ов аналог B-20
const unsigned char Whiskey_Pete = 69;  // контейнер маркерных НУРС-ов 2.75
const unsigned char SUU_25       = 70;  // контейнер (балка) с осветительными бомбами   
//71,72 is free, use if you want (duplicated F-18 fuel tanks removed)
const unsigned char AN_AAS_38_FLIR = 74; // AN/AAS-38 FLIR
const unsigned char MBD_2_67U    = 76;  // МБД-2-67У
const unsigned char S_25L_AND_PU = 77;  // С-25Л + ПУ (болванка для сброса)
const unsigned char AN_ASQ_173_LST_CAM = 78; // AN/ASQ-173 LST/CAM
const unsigned char AKU_58       = 80;  // AKU_58
const unsigned char KMGU_2       = 81;  // KMGU-2
const unsigned char SmokeGenerator_green  = 82; // Дымовой контейнер (зеленый).
const unsigned char SmokeGenerator_blue   = 83; // Дымовой контейнер (синий).
const unsigned char SmokeGenerator_white  = 84; // Дымовой контейнер (белый).
const unsigned char SmokeGenerator_yellow = 85; // Дымовой контейнер (желтый).
const unsigned char SmokeGenerator_orange = 86; // Дымовой контейнер (оранжевый).
const unsigned char SmokeGeneratorAIM_green  = 87; // Дымовой контейнер (зеленый). AIM-9S
const unsigned char SmokeGeneratorAIM_blue   = 88; // Дымовой контейнер (синий). AIM-9S
const unsigned char SmokeGeneratorAIM_white  = 89; // Дымовой контейнер (белый). AIM-9S
const unsigned char SmokeGeneratorAIM_yellow = 90; // Дымовой контейнер (желтый). AIM-9S
const unsigned char SmokeGeneratorAIM_orange = 91; // Дымовой контейнер (оранжевый). AIM-9S
const unsigned char APU_68       = 92;  // APU-68
const unsigned char APU_73       = 93;  // APU-73
const unsigned char MPS_410      = 94;  // MPS_410 Малогабаритная помеховая станция
const unsigned char Kopyo        = 95;  // Радар "Копье" в подвесном контейнере
const unsigned char APU_170      = 96;  // АПУ-170
const unsigned char LAU_117      = 97;  // LAU_117
const unsigned char PTB_KA_50    = 99;  // PTB KA-50
const unsigned char Shturm_9K114 = 100;
const unsigned char AN_AAQ_28_LITENING  = 101; // AN/AAQ-28 LITENING
const unsigned char TER_LS       = 102; // Triple ejector rack
const unsigned char TK600        = 103; // Топливный бак
const unsigned char TRAVEL_POD   = 104;

// Гидры
// На LAU_68
const unsigned char LAU_68_HYDRA_70_MK1     = 105;
const unsigned char LAU_68_HYDRA_70_MK5     = 106;
const unsigned char LAU_68_HYDRA_70_MK61    = 107;
const unsigned char LAU_68_HYDRA_70_M151    = 108;
const unsigned char LAU_68_HYDRA_70_M156    = 109;
const unsigned char LAU_68_HYDRA_70_WTU1B   = 110;
const unsigned char LAU_68_HYDRA_70_M274    = 111;
const unsigned char LAU_68_HYDRA_70_M257    = 112;
const unsigned char LAU_68_HYDRA_70_M278    = 113;
// На LAU_131
const unsigned char LAU_131_HYDRA_70_MK1    = 114;
const unsigned char LAU_131_HYDRA_70_MK5    = 115;
const unsigned char LAU_131_HYDRA_70_MK61   = 116;
const unsigned char LAU_131_HYDRA_70_M151   = 117;
const unsigned char LAU_131_HYDRA_70_M156   = 118;
const unsigned char LAU_131_HYDRA_70_WTU1B  = 119;
const unsigned char LAU_131_HYDRA_70_M274   = 120;
const unsigned char LAU_131_HYDRA_70_M257   = 121;
const unsigned char LAU_131_HYDRA_70_M278   = 122;

// 3* SUU_25 (на TER)
const unsigned char SUU_25x3                = 123;

// На тройном LAU_68 (на TER)
const unsigned char LAU_68x3_HYDRA_70_MK1   = 124;
const unsigned char LAU_68x3_HYDRA_70_MK5   = 125;
const unsigned char LAU_68x3_HYDRA_70_MK61  = 126;
const unsigned char LAU_68x3_HYDRA_70_M151  = 127;
const unsigned char LAU_68x3_HYDRA_70_M156  = 128;
const unsigned char LAU_68x3_HYDRA_70_WTU1B = 129;
const unsigned char LAU_68x3_HYDRA_70_M274  = 130;
const unsigned char LAU_68x3_HYDRA_70_M257  = 131;
const unsigned char LAU_68x3_HYDRA_70_M278  = 132;

// На тройном LAU_131 (на TER)
const unsigned char LAU_131x3_HYDRA_70_MK1  = 133;
const unsigned char LAU_131x3_HYDRA_70_MK5  = 134;
const unsigned char LAU_131x3_HYDRA_70_MK61 = 135;
const unsigned char LAU_131x3_HYDRA_70_M151 = 136;
const unsigned char LAU_131x3_HYDRA_70_M156 = 137;
const unsigned char LAU_131x3_HYDRA_70_WTU1B= 138;
const unsigned char LAU_131x3_HYDRA_70_M274 = 139;
const unsigned char LAU_131x3_HYDRA_70_M257 = 140;
const unsigned char LAU_131x3_HYDRA_70_M278 = 141;

const unsigned char ALQ_184                 = 142;
const unsigned char LAU_68                  = 143;
const unsigned char LAU_131                 = 144;

const unsigned char OH_58_BRAUNING          = 145;

const unsigned char LAU_131WP               = 146;
const unsigned char M279_AGM114             = 147;  // two-round launcher
const unsigned char B_8V20A_KOM				= 98;  // B_8V20A C-8KOM
const unsigned char B_8V20A_WP              = 148;  // С-8ЦМ
const unsigned char B_8V20A_OFP2            = 149;  // С-8М
const unsigned char B_8V20A_OM              = 150;  // С-8ОМ
const unsigned char B_8M1_OFP2              = 151;  // B-8M1 С-8OFP2

const unsigned char M134_L		 = 160;
const unsigned char M134_R		 = 161;

const unsigned char XM_158_HYDRA_70_MK1		= 162;
const unsigned char XM_158_HYDRA_70_MK5		= 163;
const unsigned char XM_158_HYDRA_70_M151	= 164;
const unsigned char XM_158_HYDRA_70_M156	= 165;
const unsigned char XM_158_HYDRA_70_M274	= 166;
const unsigned char XM_158_HYDRA_70_M257	= 167;
const unsigned char M261_HYDRA_70_M151		= 168;
const unsigned char M261_HYDRA_70_M156		= 169;

const unsigned char GUV_YakB_GSHP		= 170;  //Гондола универсальная вертолётная
const unsigned char GUV_VOG				= 171;  //Гондола универсальная вертолётная
const unsigned char HWAR_SMOKE_GENERATOR = 172;

const unsigned char M134_SIDE_L		 = 174;
const unsigned char M134_SIDE_R		 = 175;
const unsigned char M60_SIDE_L		 = 176;
const unsigned char M60_SIDE_R		 = 177;

const unsigned char wsType_Test4        = 200;  // точка на земле

//*************************************************
// Константы для сообщений об облучении !!! не входят в Level 1,2,3,4//*
const unsigned char  wsRadarAir           = 101;//
const unsigned char  wsRadarLongRange     = 102;//
const unsigned char  wsRadarMidRange      = 103;//
const unsigned char  wsRadarShrtRange     = 104;//
const unsigned char  wsRadarEWS           = 105;//
const unsigned char  wsRadarEngagement    = 106;//
const unsigned char  wsTypeLauncher       = 107;
const unsigned char  wsTypeComandPost     = 108;
const unsigned char  wsRadarOptical       = 109;
const unsigned char  wsRadarAWACS         = 110;
const unsigned char  wsRadarActiveHoming  = 111;

//      Пушки
const unsigned char Gun__ = 241;        // передняя авиационная пушка
const unsigned char Gun__1= 242;        // кормовая авиационная пушка
//      Балочные держатели с подвесками 
//      wsType_Weapon, wsType_Missile(Bomb), wsType_Container
const unsigned char MBD_FAB_100       = 1;  // MDB & 6 FAB-100
const unsigned char MBD_FAB_250       = 2;  // MDB & 6 FAB-250
const unsigned char MBD_FAB_500       = 3;  // MDB & 6 FAB-500
const unsigned char MBD_BETAB_250     = 4;  // MDB & 6 BETAB-250
const unsigned char MBD_ODAB          = 5;  // MDB & 6 ODAB
const unsigned char MBD_PB_250        = 6;  // MDB & 6 PB-250
const unsigned char MBD_RBK_250       = 7;  // MDB & 6 RBK-250
const unsigned char MBD_RBK_500AO     = 8;  // MDB & 6 RBK-500AO
const unsigned char MBD_RBK_500SOAB   = 9;  // MDB & 6 RBK-500SOAB
const unsigned char MBD_ZAB           = 10; // MDB & 6 ZAB
const unsigned char MBD_CBU_97        = 11; // MDB & 6 CBU-97
const unsigned char MBD_M_117         = 12; // MDB & 6 M-117AB
const unsigned char MBD_MK_81         = 13; // MDB & 6 MK-81
const unsigned char MBD_MK_82         = 14; // MDB & 6 MK-84
const unsigned char MBD_Rockeye       = 15; // MDB & 6 Rockeye
const unsigned char MBD_3_FAB_100     = 16; // MDB-3 & 3 FAB-100
const unsigned char MBD_3_FAB_250     = 17; // MDB-3 & 3 FAB-250
const unsigned char MBD_3_FAB_500     = 18; // MDB-3 & 3 FAB-500
const unsigned char MBD_3_BETAB_250   = 19; // MDB-3 & 3 BETAB-250
const unsigned char MBD_3_ODAB        = 20; // MDB-3 & 3 ODAB
const unsigned char MBD_3_PB_250      = 21; // MDB-3 & 3 PB-250
const unsigned char MBD_3_RBK_250     = 22; // MDB-3 & 3 RBK-250
const unsigned char MBD_3_RBK_500AO   = 23; // MDB-3 & 3 RBK-500AO
const unsigned char MBD_3_RBK_500SOAB = 24; // MDB-3 & 3 RBK-500SOAB
const unsigned char MBD_3_ZAB         = 25; // MDB-3 & 3 ZAB
const unsigned char MBD_3_CBU_97      = 26; // MDB-3 & 3 CBU-97
const unsigned char MBD_3_M_117       = 27; // MDB-3 & 3 M-117AB
const unsigned char MBD_3_MK_81       = 28; // MDB-3 & 3 MK-81
const unsigned char MBD_3_MK_82       = 29; // MDB-3 & 3 MK-84
const unsigned char MBD_3_Rockeye     = 30; // MDB-3 & 3 Rockeye
const unsigned char MBD_3_GBU_16      = 31; // MDB-3 & 3 GBU_16
const unsigned char MBD_4_FAB_100     = 32; // MDB-4 & 4 FAB-100
const unsigned char LAU_88_AGM_65K    = 33; // LAU_88 & 3 AGM-65K
const unsigned char TU_22_FAB_100     = 34; // TU-22M3-MBD & 9 FAB-100
const unsigned char TU_22_FAB_250     = 35; // TU-22M3-MBD & 9 FAB-250
const unsigned char TU_22_BETAB_250   = 36; // TU-22M3-MBD & 9 BETAB-250
const unsigned char TU_22_PB_250      = 37; // TU-22M3-MBD & 9 PB-250
const unsigned char TU_22_RBK_250     = 38; // TU-22M3-MBD & 9 RBK_250
const unsigned char TU_22_FAB_500     = 39; // TU-22M3-MBD & 6 FAB-500
const unsigned char TU_22_BETAB_500   = 40; // TU-22M3-MBD & 6 BETAB_500
const unsigned char TU_22_BETAB_500SP = 41; // TU-22M3-MBD & 6 BETAB_500SP
const unsigned char TU_22_ODAB        = 42; // TU-22M3-MBD & 6 ODAB
const unsigned char TU_22_RBK_500AO   = 43; // TU-22M3-MBD & 6 RBK_500AO
const unsigned char TU_22_RBK_500SOAB = 44; // TU-22M3-MBD & 6 RBK_500SOAB
const unsigned char TU_22_ZAB         = 45; // TU-22M3-MBD & 6 ZAB
const unsigned char VICHR             = 46; // Vikhr (AT-12) {air-tank}     
const unsigned char VICHR_M           = 47; // Vikhr-M (AT-X-16) {air-tank} 
const unsigned char LAU_88_AGM_65D    = 48; // LAU_88 & 3 AGM-65D
const unsigned char LAU_88_AGM_65E    = 49; // LAU_88 & 3 AGM-65E
const unsigned char MBD_A10_2_AIM_9   = 50; // MBD_A10_2 & 2 AIM_9
const unsigned char MBD_F2_2_Mk_82    = 51; // MBD F2_2  & 2 Mk_82
const unsigned char MBD_F2_2_Mk_83    = 52; // MBD F2_2  & 2 Mk_83
const unsigned char MBD_F2_2_Puma     = 53; // MBD F2_2  & 2 Puma
const unsigned char MBD_F2_2_BL_755   = 54; // MBD F2_2  & 2 BL_755
const unsigned char MBD_F2_2_ROCKEYE  = 55; // MBD F2_2  & 2 ROCKEYE
const unsigned char MBD_F2_2_ALARM    = 56; // MBD F2_2  & 2 ALARM
const unsigned char MBD_3_GBU_22      = 57; // MDB-3 & 3 GBU_22
const unsigned char AGM_114_Pilon_4   = 58; // AGM_114_Pilon & 4 AGM_114
const unsigned char AGM_114K_Pilon_4  = 59; // AGM_114_Pilon & 4 AGM_114K
const unsigned char SHTURM            = 60; // SHTURM   
const unsigned char MER_AIM_9_2       = 61; // F4_PILON & 2 AIM-9
const unsigned char MER_L_P_60_2      = 62; // MER_L_P_60 & 2 P-60
const unsigned char MER_R_P_60_2      = 63; // MER_R_P_60 & 2 P-60
const unsigned char MER_TOW_4         = 64; // MER_TOW & 4 TOW
const unsigned char LAU_88_AGM_65K_2_LEFT = 65; // LAU_88 & 2(6) AGM_65K (left)
const unsigned char LAU_88_AGM_65D_2_LEFT = 66; // LAU_88 & 2(6) AGM_65D (left)
const unsigned char MER_6_AGM_86C     = 67; // MBD_B52_6 & 6 AGM_86C
const unsigned char MBD_FAB_250_2     = 68; // MDB & 2(6) FAB_250
const unsigned char MBD_RBK_250_2     = 69; // MDB & 2(6) RBK_250
const unsigned char MBD_BetAB_250_2   = 70; // MDB & 2(6) BetAB_250
const unsigned char TU_22_FAB_1500_2  = 71; // TU-22M3-MBD & 2(9) FAB-1500
const unsigned char MBD_3_BetAB_500   = 72; // MDB-3 & 3 BetAB_500
const unsigned char MER_2_F_18_CBU_97 = 73; // MER_2_F_18 & 2 CBU_97
const unsigned char MER_12_B52_Mk_82  = 74; // MER_12_B52 & 12 Mk_82
const unsigned char MER_6_BLU_107     = 75; // MDB & 6 BLU-107
const unsigned char MER_12_B52_M_117  = 76; // MER_12_B52 & 12 M_117
const unsigned char MER_9_B52_CBU_97  = 77; // MER_9_B52 & 9 CBU_97
const unsigned char MER_9_B52_Mk_84   = 78; // MER_9_B52 & 9 Mk_84
const unsigned char MER_9_B52_Rockeye = 79; // MER_9_B52 & 9 Rockeye
const unsigned char MBD_4_RBK_250     = 80; // MDB-4 & 4 RBK_250
const unsigned char MBD_4_FAB_250     = 81; // MDB-4 & 4 FAB_250
const unsigned char MER_6_2_PB_250    = 82; // MBD & 2 PB_250
const unsigned char MER_6_4_PB_250    = 83; // MBD & 4 PB_250
const unsigned char MER_6_4_FAB_250   = 84; // MBD & 4 FAB_250
const unsigned char SUU_25_8_LUU_2    = 85; // SUU_25 & 8 LUU_2B
const unsigned char APU_6_VICHR_M     = 86; // APU_6_VIKHR & 6 Vikhr_M
const unsigned char S_25L_PU          = 87; // S_25L & PU
const unsigned char MBD_2_67U_FAB_100 = 88; // MBD_2_67U & FAB_100
const unsigned char AKU_58_X_58       = 91; // AKU_58 & X_58
const unsigned char AKU_58_X_29T      = 92; // AKU_58 & X_29T
const unsigned char AKU_58_X_29L      = 93; // AKU_58 & X_29L
const unsigned char KMGU_2_AO_2_5RT   = 94; // KMGU-2 & AO_2_5RT
const unsigned char KMGU_2_PTAB_2_5KO = 95; // KMGU-2 & PTAB_2_5KO
const unsigned char AKU_58_X_31A      = 96; // AKU-58 & X-31A
const unsigned char AKU_58_X_31P      = 97; // AKU-58 & X-31P
const unsigned char AKU_58_X_35       = 98; // AKU-58 & X-35
const unsigned char APU_68_X_25ML     = 99; // APU-68 & X-25ML
const unsigned char APU_68_X_25MP     = 100; // APU-68 & X-25MP
const unsigned char APU_68_C_24       = 101; // APU-68 & C-24
const unsigned char APU_73_P_73       = 102; // APU-73 & R-73
const unsigned char APU_170_P_77      = 103; // APU-170 & R-77
const unsigned char LAU_88_AGM_65E_2_LEFT  = 104; // LAU_88 & 2(6) AGM_65E (left)
const unsigned char LAU_88_AGM_65K_2_RIGHT = 105; // LAU_88 & 2(6) AGM_65K (right)
const unsigned char LAU_88_AGM_65D_2_RIGHT = 106; // LAU_88 & 2(6) AGM_65D (right)
const unsigned char LAU_88_AGM_65E_2_RIGHT = 107; // LAU_88 & 2(6) AGM_65E (right)
const unsigned char LAU_117_AGM_65K   = 108; // LAU_117 & AGM_65K
const unsigned char LAU_117_AGM_65D   = 109; // LAU_117 & AGM_65D
const unsigned char LAU_117_AGM_65E   = 110; // LAU_117 & AGM_65E
// A-10C
const unsigned char TER_3_BDU_33      = 114;
//const unsigned char TER_3_BDU_50LD    = 115;
//const unsigned char TER_3_BDU_50HD    = 116;
//const unsigned char TER_3_BDU_50LGB   = 117;



/*
const unsigned char LAU_88_AGM_65H    = 119; // LAU_88 & 3 AGM-65H
const unsigned char LAU_88_AGM_65G    = 120; // LAU_88 & 3 AGM-65G
const unsigned char LAU_88_AGM_65H_2_LEFT = 121; // LAU_88 & 2(6) AGM_65H (left)
const unsigned char LAU_88_AGM_65G_2_LEFT = 122; // LAU_88 & 2(6) AGM_65G (left)
const unsigned char LAU_88_AGM_65D_2_RIGHT = 123; // LAU_88 & 2(6) AGM_65D (right)
const unsigned char LAU_88_AGM_65E_2_RIGHT = 124; // LAU_88 & 2(6) AGM_65E (right)
*/
const unsigned char LAU_117_AGM_65H   = 125; // LAU_117 & AGM_65H
const unsigned char LAU_117_AGM_65G   = 126; // LAU_117 & AGM_65G

const unsigned char TER_3_GBU_12      = 127; // TER 3*GBU_12
const unsigned char LAU_105_1_AIM_9M_L  = 128 ;
const unsigned char LAU_105_2_CATM_9M  = 129 ;
const unsigned char LAU_105_1_CATM_9M_L  = 130 ;
const unsigned char TER_3_MK82AIR      = 131 ;
const unsigned char TER_FREE           = 132 ;
const unsigned char LAU_105_FREE       = 133 ;
const unsigned char LAU_88_AGM_65H     = 134 ;
const unsigned char LAU_88_AGM_65H_2_L = 135 ;
const unsigned char LAU_88_AGM_65H_2_R = 136 ;
const unsigned char LAU_88_AGM_65H_3   = 137 ;
const unsigned char LAU_117_TGM_65D    = 138 ;
const unsigned char LAU_117_TGM_65G    = 139 ;
const unsigned char LAU_117_TGM_65H    = 140 ;
const unsigned char LAU_117_CATM_65K   = 141 ;
const unsigned char LAU_105_1_AIM_9M_R  = 142 ;
const unsigned char LAU_105_1_CATM_9M_R  = 143 ;
const unsigned char TER_3_SUU_25_8_LUU_2  = 144 ;
const unsigned char LAU_88_AGM_65D_ONE  = 145 ;

