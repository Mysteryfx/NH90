#ifndef __SHELL_DESCRIPTORS_H__
#define __SHELL_DESCRIPTORS_H__

#include "WeaponsBase.h"
#include "descriptor_base.h"
#include "cx_interpolator.h"
#include "shell_coeffs.h"
#include "Ammunition/ammunition_name.h"

enum wcCoalitionName;

namespace Graphics { class Model; }
class wDatabaseLoader;
namespace Lua { class Loader; }
class wShell;
class wShellGroup;

#include "Math/Rotation3.h"

#include "Registry/Registered.h"

// Куда бы это запихнуть получше??
const ed::string cShellDefaultNamespace = "weapons.shells.";
const ed::string cShellAimingTableDefaultNamespace = "weapons.shells_aiming.";

class WEAPONSBASE_API wShellDescriptor : public wDescriptorBase, public wNamedAmmunitionDescriptor
{
public:

	wShellDescriptor(const ed::string& name) : wDescriptorBase(name) {}

	virtual void load(wDatabaseLoader* loader);

	Graphics::Model* getModel() const;

	wShell* launch(	wShellGroup* group, bool master, const Math::Vec3d& launcher_pos, const Math::Vec3d& launcher_dir, const Math::Vec3d& launcher_vel, ObjectID launcher_id,
					const wShellCoefficients & coefficients, wcCoalitionName coalition,unsigned char state, double initial_vel = 0, ObjectID forced_id = 0) const;

protected:
	template <bool B>
	wShell* launch_(wShellGroup* group,const Math::Vec3d& launcher_pos, const Math::Vec3d& launcher_dir, const Math::Vec3d& launcher_vel, ObjectID launcher_id,
					const wShellCoefficients & coefficients, wcCoalitionName coalition,unsigned char state, double initial_vel, ObjectID forced_id) const;


public:
	double getFullScaleTime() const;

public:
    struct ReboundCoefficients
    {
        void load(wDatabaseLoader* loader, const ed::string& table);

        // Два угла отсчитываются от нормали (angle0 > angle100)
        // 0 < нет рикошета < angle0 < вероятностный рикошет < angle100 < постоянный рикошет < 90
        double angle0, angle100;   
        double cosAngle0, cosAngle100;  // Это, естественно рассчитывается

        double deviationAngle;      // Угол в пределах которого отклоняется нормаль
        double velocityLossFactor;  // Потеря энергии (вычисляется как коэффициент*(1-cos(vel, n)))
        double cxFactor;            // На сколько возрастет cx
    };

public:
	// Fields
	double v0;

	double Dv0;
	double Da0;
	double Da1;

	double caliber;
	double AP_cap_caliber;
	bool   subcalibre;
	double mass;            // Масса снаряда
    double roundMass;       // Масса раунда
	double cartridgeMass;	// Масса гильзы
	int	   cartridgeID;
	double S;				// Характеристическая площадь
	CxInterpolator cx;		

	double life_time;

	double k1;				// Для старого прицела

	bool   scale_tracer;	// Надо ли масштабировать трассер в начале полета	
	double tracer_off;		// Когда трассер отключается
	double tracer_on;

    double explosive;       // Масса взрывчатки
    double damageFactor;    // Коэффициент "формы" - на него делится damage
    double piercingMass;    // Масса учитываемая при бронепробитии

    double cumulativeThickness; // Толщина пробиваемой по нормали брони
    double cumulativeMass;      // В кг взрывчатки (передается напрямую, минуя damageFactor)

    // Рикошет
    ReboundCoefficients reboundGround;      // на земле
    ReboundCoefficients reboundConcrete;    // на полосе/дороге
    ReboundCoefficients reboundWater;       // на воде
    ReboundCoefficients reboundObject;      // на объекте

    // Частота вращения вокруг своей оси
    double rotation_freq;

	double visual_effect_correction; //коррекция размера взрыва без изменения размера наносимого повреждения
	double visual_effect_correction_rebound; //коррекция размера взрыва без изменения размера наносимого повреждения
	double smoke_tail_life_time;
	float smoke_scale_;
	float smoke_opacity_;
	float smoke_particle_;

protected:
	ed::string model_name_;
	mutable Graphics::Model* model_;

	mutable double full_scale_time_;
};

class WEAPONSBASE_API wShellAimingTableDescriptor : public wDescriptorBase
{
protected:
	struct targeting_data_
	{
		double distance;    // дистанция

		double elevation;   // угол места
		double angle_hit;   // угол падения
		double fly_time;    // время полета
		double MO;          // максимальное превышение траектории
		double cwindY;      // коррекция ветра по азимуту
		double cwindX;      // коррекция ветра по дальности
		double celevation;  // коррекция угла места
		double alt_to_dist_correction; // коррекция дистанции в зависимости от высоты пушки над уровнем моря
	};

	struct charge_data_
	{
		double init_vel;    // начальная скорость при данном типе заряда
		ed::vector<targeting_data_> aiming_data;
	};

public:
	struct targeting_data : public targeting_data_
	{
        targeting_data():charge(0),init_vel(0){};
		int charge;	
		double init_vel;
	};

public:
	wShellAimingTableDescriptor(const ed::string& name) : wDescriptorBase(name) {};

	virtual void load(wDatabaseLoader* loader);

	bool getDirectAimingDataByDist(double distance, int charge, targeting_data& data_) const ;
    bool getDirectAimingDataByAngle(double angle, targeting_data& data_) const ;
	double getMaxDistance() const;

protected:
	// По количеству толкающих зарядов, по дистанции
	ed::vector<charge_data_> direct_aiming_data_;
};


#endif
