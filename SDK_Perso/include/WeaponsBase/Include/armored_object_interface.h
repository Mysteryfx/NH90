#ifndef __ARMORED_TARGET_INTERFACE_H__
#define __ARMORED_TARGET_INTERFACE_H__

#include "Math/Vector.h"

class wIArmoredTarget
{
public:
    struct ArmorData
    {
        ArmorData(double width_, double K_, double rho_):width(width_),K(K_),rho(rho_){};

        double width;       // Толщина в метрах
        double K;           // Коэффициент прочности, 2100 по умолчанию
        double rho;         // плотность в кг/м^3
    };

public:
    // Получить параметры брони в данной точке
    // Точка задана в локальных координатах объекта
    virtual ArmorData getArmorData(const Math::Vec3d& point) const = 0;
	virtual ArmorData getArmorData(const Math::Vec3d& point, bool is_chassis) const = 0;
    virtual wIArmoredTarget::ArmorData getAverageArmor() const = 0;
};

#endif
