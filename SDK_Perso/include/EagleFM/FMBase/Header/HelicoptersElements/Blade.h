#ifndef __Blade_h__
#define __Blade_h__

#include "Base.h"
#include "FMMathematics/math.h"
#include "Optimizable.h"

namespace EagleFM
{

class DynamicBody;
class AerodynamicBody;
class RigidBody;

class ProbabilityChecker;

struct FMBASE_API BladeInit
{
	BladeInit()
	{
		Clockwise = false;
		ConstructionPhase = 0.0;
		Rmin = 0.0;
		Rmax = 1.0;
		RFlapHinge = 0.0;
		FlappingSpring = 0.0;
		FlappingDamper = 0.0;
		FlappingMin = 0.0;
		FlappingMax = 0.1;
		FlappingStop = 0.0;
		StopSupportRPM = -10.0;
		RHuntHinge = 0.0;
		HuntingSpring = 0.0;
		HuntingDamper = 0.0;
		HuntingMin = -0.174;
		HuntingMax = 0.174;
		mass = 1.0;
		Thickness = 50.0;
		K_PitchFlappReg = 0.84;
		mass_pos = Vec3(0.0);
		AxisPitch = 0.0f;
		AxisRoll = 0.0f;
		Rotor_pos = Vec3(0.0);
		K_ADForceLost = 0.95;
		K_ADForceIndLost = 0.93;
		SwirlingIndLim = 10.0;
		CushionParam[0] = 0.15;			//по умолчанию - параметры винта Ка-50
		CushionParam[1] = 2.7;
		VortexRingParam[0] = -1.18;    //по умолчанию - параметры винта Ка-50
		VortexRingParam[1] = -0.37;
		VortexRingParam[2] = 2.25;
		VortexRingParam[3] = 21.61638;
		VortexRingParam[4] = 1.0;
		VortexRingParam[5] = 0.775;
		VortexRingParam[6] = 2.0;
		AoA_Crit = 0.35;
		MachCrit = 0.95;
		AoA_CritK = 0.5;
		MachCritK = 0.7;
		Twist0 = 0.0;
		Twist1 = 0.0;
		Twist2 = 0.0;
		TwistMin = -100.0;
		TwistMax = 100.0;
		LimBlowParam[0] = LimBlowParam[1] = LimBlowParam[2] = 0.0;//!!!
		ForceMax = 100000.0;
		ForceDamage = 90000.0;
	}

	bool	Clockwise;
	double	ConstructionPhase;
	double	Rmin;					// Минимальный радиус лопасти отн. ВШ
	double	Rmax;					// Максимальный радиус лопасти отн. ВШ
	double	RFlapHinge;				// Вынос ГШ
	double	FlappingSpring;			// жесткость пружины ГШ
	double	FlappingDamper;			// дэмпфер ГШ
	double	FlappingMin;			// упоры взмаха
	double	FlappingMax;
	double	FlappingStop;			// центробежный упор
	double	StopSupportRPM;			// скорость ввода центробежного упора
	double	RHuntHinge;				// Вынос ВШ
	double	HuntingSpring;			// жесткость пружины ВШ
	double	HuntingDamper;			// дэмпфер ВШ
	double	HuntingMin;				// упоры опережения
	double	HuntingMax;
	double	mass;					// Масса
	double  Chord;					//
	double	Thickness;				// толщина [мм]
	double	K_PitchFlappReg;		// к-т регулятора взмаха (прибавка угла установки = K_PitchFlappReg*Flapping)
	Vec3	mass_pos;				// Координаты ц.т.
	float	AxisPitch;				// наклон оси винта вперед
	float	AxisRoll;				// наклон оси винта вбок (для класс.схемы)
	Vec3	Rotor_pos;				// центр винта
	double	K_ADForceLost;			// к-т потерь подъемной силы на концах (домножается на всю подъемную силу)
	double	K_ADForceIndLost;		// к-т потерь подъемной силы на концах при расчете индуктивной скорости (домножается на внешнем кольце)
	double	SwirlingIndLim;			// ограничение закрутки в индуктивной модели (при расчетах)
	double	CushionParam[2];		//параметры подушки
	double	VortexRingParam[7];		// параметры режима вихревого кольца
	double	AoA_Crit;				// закритика
	double	MachCrit;				// начало флаттера
	double	AoA_CritK;				// к-т усиления тряски от закритики
	double	MachCritK;				//к-т усиления тряски от флаттера
	//коэфф-ты полинома крутки:
	double	Twist0; // Twist = Twist2*R*R + Twist1*R + Twist0; R - отн.центра винта!
	double	Twist1;
	double	Twist2; //Не используется! Прописать если надо!
	double	TwistMin;
	double	TwistMax;
	//удары
	double	LimBlowParam[3];		//параметры ударов по упорам
	//разрушение
	double	ForceMax;				//разрушающая сила
	double	ForceDamage;			//деформирующая сила
};

class FMBASE_API Blade : public Optimizable
{
// ----------------------------- МЕТОДЫ ---------------------------------------
public:
	//Создание и инициализация
	Blade(DynamicBody*, AerodynamicBody*, RigidBody*);
	Blade(DynamicBody*, AerodynamicBody*, RigidBody*, const BladeInit&);

	virtual ~Blade();

	void			init(const BladeInit&);
	void			setRigidBodyElement(int RBElement) { Element = RBElement; } //сопоставление индексу в списке элементов твердого тела для повреждений
	void			setRigidContact(double Radius, double OmegaMax); //Radius - расстояние от оси вращения до точки контакта, OmegaMax - предельная разрушающая скорость вращения
	void			setIF(double); //Степень целостности. Происходит перерасчет характеристик!
	void			setVindSegmentsNum(int);	//разбить винт модели расчета индуктивной сокрости на N колец
	void			setADSegmentsNum(int);		//разбить АД часть лопасти на N сегментов
	void			setInductiveVelocity(const Vec3*, double, double); //инициализация индуктивной скорости (массива по кольцам, средней по всем кольцам и общей индуктивной тяги)
	void			setOmega(double omega)	{ Omega = omega; }
	void			setPitch(double pitch)	{ Pitch = pitch; }
	void			setUnderlyingSurface(double Alt, const Vec3 &Normal) { UnderlyingSurfaceAlt = Alt; UnderlyingSurfaceNormal = Normal; }
	void			setCirculation0(double _Circulation0)  { Circulation0 = __max(0.0, _Circulation0); }
	void			setVortexPhaseMax(double _Phase) { PhaseMaxCirculation = __max(0.0, __min(_Phase, Pi2)); }
	void			setDeicerRate(double Value) { DeicerRate = __max(0.0, Value); }
	void			resetMovement(double Kick = 0.0); //Kick - степень отдачи, отброс назад

	//Работа
private:
	virtual void	doSimulate();
public:

	//				Доступ
	double			getPhase()				const	{ return Phase; }
	double			getPitch()				const	{ return Pitch; }
	double			getFlapping()			const	{ return Flapping; }
	double			getRelativeFlapping()	const;
	double			getHunting()			const	{ return Hunting; }
	double			getRelativeHunting()	const;
	double			getDeflection()			const	{ return Deflection; }
	double			getDeltaLimFlapping()	const	{ return DeltaLimFlapping; }
	double			getShakeValue()			const	{ return ShakeAmpl; }
	double			getIF()					const	{ return IF; }
	double			getMOI_y()				const	{ return MOI_y; }


	Vec3			getForce_l()			const	{ return Force_l; }
	Vec3			getMoment_l()			const	{ return Moment_l; }
	double			getRotorMoment()		const	{ return RotorMoment; }
	double			getBladeForce()			const	{ return BladeForce; }
	Vec3			takeBlowForce_l();
	Vec3			takeBlowMoment_l();
	double			takeBlowRotorMoment();
	double			calcFlappingHeight(double Azimuth = -1.0) const; //расчет высоты взмаха в метрах
	double			getIceForInductive(int InductiveSegment)  const; //доступ к значению льда на сегменте индуктивной модели
	double			getCirculation()		const	{ return Circulation; }

	bool			HuntVibrCalcOn;					//такой грязный хак для получения земного резонанса - отключаем демпферы в ВШ и расчитываем поворот ц.б силы в ВШ

private:
	//				служебные методы
	void			calcMatrix();

// ----------------------------- ПОЛЯ ---------------------------------------

//	не хозяин:
	DynamicBody			*pDBody;					//сам вертолет
	AerodynamicBody		*pADBody;					//аэродинамика лопасти
	RigidBody			*pRBody;					//чтобы оторвать лопасть

//	хозяин:

	//					кинематика

	double				Phase;						//азимут
	double				Pitch;						//угол установки

	double				Omega;						//скорость вращения отн.оси винта
	double				LastOmega;					//для расчета раскрутки ротора

	double				Flapping;					//взмах
	double				dFlapping;					//угловая скорость взмаха
	double				d2Flapping;					//угловое ускорение взмаха
	double				Hunting;					//опережение (динамическое)
	double				dHunting;					//угловая скорость опережения

	double				Deflection;					//прогиб

	double				DeflectionInt;				//интегрируемая составляющая прогиба (для удобства расчета)
	double				DeltaLimFlapping;			//выход взмаха за пределы (прогиб) во время удара по упорам

	double				CushionParam[2];			//параметры подушки
	double				VortexRingParam[7];			//параметры режима вихревого кольца

	//служебные (индуктивная скорость)
	int					VindSegmentsNum;			//количество колец винта модели расчета индуктивной скорости
	Vec3				*Vind_a;
	Vec3				*Vind_p;
	double				*RotorR_Vind;				//радиусы, на которых даны индуктивные скорости (для интерполяции)
	double				VindAvr;					//средняя на всем винте индуктивная скорость
	double				FlowThrust;					//Тяга винта по индуктивной модели

	double				Circulation;				//вихревое кольцо
	double				Circulation0;				//установка (добавка) извне
	double				PhaseMaxCirculation;		//азимут макс.вихревого кольца

	double				UnderlyingSurfaceAlt;		// расстояние до подситалающей поверхности
	Vec3				UnderlyingSurfaceNormal;	// нормаль подстилающей поверхности


	//					динамика

	Vec3				Force_l;					//ветор результирующей силы на лопасти
	Vec3				Moment_l;					//вектор результирующего момента
	double				RotorMoment;				//момент отн. оси вала (реактивный)
	Vec3				BlowForce_l;				//ударная сила
	Vec3				BlowMoment_l;				//момент ударной силы
	double				BlowRotorMoment;			//момент ударной силы отн.оси вала (контакт с препятствием)

	double				BladeForce;					//сглаженное (тау = 1с) значение силы на лопасти для проверки разрушения

	//тряска
	Math::IIR_Filter	ShakeFilter;				//фильтр для генерации тряски угла установки лопасти
	double				ShakeAmpl;					//амплитуда тряски (идет в фильтр и в кабину на голову)
	Randomizer			Rnd;
	double				DeltaMachCrit;				//сглаженное (тау = 0.045с) значение превышения числа М крит на лопасти
	double				DeltaAoA_Crit;				//сглаженное (тау = 0.045с) значение превышения крит. угла атаки на лопасти

	double				AoA_Crit;					//закритика
	double				MachCrit;					//начало флаттера
	double				AoA_CritK;					//к-т усиления тряски от закритики
	double				MachCritK;					//к-т усиления тряски от флаттера



	bool				Turbulence;

	double				K_ADForceLost;				//к-т потерь подъемной силы на концах (домножается на всю подъемную силу)

	double				LimBlowParam[3];			//параметры ударов по упорам

	//разрушение
	int					Element;					//номер элемента в массиве повреждаемых элементов ЛА
	double				ForceMax;					//разрушающая сила
	double				ForceDamage;				//деформирующая сила
	ProbabilityChecker*	CheckFlatterDestroy;		//проверка разрушения по флаттеру


	//служебные (АД силы на сегментах)
	int					ADSegmentsNum;				//количество сегментов АД части лопасти
	Vec3				*ADForce;
	Vec3				*ADForce_pos;
	double				SegmentsLength;				//длина АД сегмента

	//лед
	double				*IceLayerThickness;			//толщина слоя льда на каждом АД участке [мм]
	double				*IceLayerThicknessInd;		//толщина слоя льда на каждом участке индуктивной модели (экспорт) [мм]
	double				DeicerRate;					//скорость таянья льда от искуственного подогрева [мм/мин]
	bool				IceFlag;					//флаг наличия льда (кэш)




	//					Геометрия и другие характеристики лопасти

	// ось			 ГШ			   ВШ         лопасть
	// [|]============O============[]===========|-----------/   /---------------------|
	//  		      			    			|___________\   \_____________________|
    //  |__RFlapHinge_|				|						/   /
	//				  |__RHuntHinge_|			|									  |
	//								|__Rmin_____|									  |
	//								|_____________________Rmax________________________|


	double				IF;					// Integrity Factor

	bool				Clockwise;			// true - вращается по часовой стрелке
	double				ConstructionPhase;
	double				Rmin;				// Минимальный радиус лопасти отн. ВШ
	double				Rmax;				// Максимальный радиус лопасти отн. ВШ
	double				mass;				// Масса
	Vec3				mass_pos;			// Координаты ц.т. //НАДО ЛИ???
	double				Thickness;			// толщина (для обледенения) [мм]
	double				RFlapHinge;			// Вынос ГШ
	double				FlappingMin;		// минимальный взмах
	double				FlappingMax;		// максимальный взмах
	double				FlappingStop;		// центробежный упор
	double				StopSupportRPM;		// скорость ввода центробежного упора
	double				FlappingSpring;		// жесткость пружины ГШ
	double				FlappingDamper;		// дэмпфер ГШ
	double				RHuntHinge;			// Вынос ВШ //НЕ НАДО?!
	double				HuntingMin;			// минимальное опережение
	double				HuntingMax;			// максимальное опережение
	double				HuntingSpring;		// жесткость пружины ВШ
	double				HuntingDamper;		// дэмпфер ВШ
	Matrix3				Rotor_or;			// наклон оси винта
	Vec3				Rotor_pos;			// центр винта
	double				Twist0;				// коэфф-ты полинома крутки:
	double				Twist1;
	double				TwistMin;
	double				TwistMax;

	double				m_to_l;				// плотность массы
	double				MOI_y;				// момент инерции лопасти отн. оси вала (для раскрутки винта)
	double				MOI_x;				// момент инерции лопасти отн. ГШ (для взмаха)

	//служебные характеристики:
	double				LblAD;				// длина АД части лопасти (постоянная)
	double				Lbl;				// длина лопасти от ВШ (разрушается)
	double				RotorRmax;			// максимальный радиус винта отн.центра (постоянный)
	double				RotorRh;			// Вынос ВШ отн.центра (постоянный)
	double				BladeRmax;			// макс. радиус лопасти отн.центра (разрушается)
	double				RotorArea;			// площадь винта (постоянный)



	//					Системы координат:

	// _w(мир.) => _h(верт.)   =>  _a(ось лоп.)  => _f(ГШ)  => _p(лопасть) => _e(элемент)
	//	      (or, pos)   (накл.оси,азимут)  (RFlapH,flap) (pitch)			(крутка)

	Matrix3				MRw_h;
	Matrix3				MRh_w;
	Matrix3				MRh_a;
	Matrix3				MRa_h;
	Matrix3				MRa_f;
	Matrix3				MRf_a;
	Matrix3				MRf_p;
	Matrix3				MRp_f;

//-------------------------DEBUG---------------------------------
#ifdef _ED_HIDDEN_DEBUG 
public:
	void			setPhase(double phase)	{ Phase = phase; }
	double			dbdtRy;
	double			OmegaRy;
	double			Vindy;
	double			Vy;
	double			_AoA;
	bool			ExternPhase;
	Vec3			Rmax_w;
	Vec3			Rmin_w;

	double			Time;
#endif
};

}

#endif
