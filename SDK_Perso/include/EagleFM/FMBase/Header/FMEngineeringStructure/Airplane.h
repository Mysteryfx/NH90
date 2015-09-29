#ifndef __Aerplane_h__
#define __Aerplane_h__

#include "FMDynamics/DynamicBody.h"
#include "FMEngineeringStructure/ADElement.h"
#include "Optimizable.h"
#include <ed/vector.h>
#include <ed/list.h>

namespace EagleFM
{

typedef ed::vector<ADElement*> ADVector;

struct PlanePart
{
	ADElement		*Elem;
	double			IntegrityFactor;
	double			NormalHeight;
};

typedef ed::list<PlanePart> PlanePartList;


//Класс Аэроплан, савокупирующий все аэродинамические элементы самолета и выполняющий аэродинамический расчет
class FMBASE_API Airplane : public Optimizable
{
public:
	Airplane(DynamicBody*);
    
	void			initCentreSection(double Area, double Span);							//описать гемотрию центроплана: размах и площадь
	void			initWindTunneledChar(double Area, double MeanAerodChord, double Span);	//характерные величины для нормализации при продувке

	void			setADElement(ADElement*);		//добавить АД элемент
	void			setPlanePart(ADElement*);		//добавить часть несущей поверхности
	void			cleanADElements();				//удалить все элементы


	void			setUnderlyingSurface(const Vec3 &Point_w, const Vec3 &Normal_w) 
						{ UnderlyingSurfacePoint_w = Point_w; UnderlyingSurfaceNormal_w = Normal_w; }
	
private:
	virtual void	doSimulate();					//расчитать АД силы и моменты у всех элментов

public:
	void			applyForces();					//приложить АД силы и моменты к динамическому телу

	void			doWindTunneled(double Mach = 0.0);//АД продувка на заданном числе М для тестирования


	void			getUnderlyingSurface(Vec3 &Point_w, Vec3 &Normal_w) 
						{ Point_w = UnderlyingSurfacePoint_w; Normal_w = UnderlyingSurfaceNormal_w; }

	Vec3			getForce_l()	const { return ADForce_l; }  //log
	Vec3			getMoment_l()	const { return ADMoment_l; } //log

private:
	Vec3			ADForce_l;
	Vec3			ADMoment_l;	

	Vec3			UnderlyingSurfacePoint_w;	//точка на подситалающей поверхности
	Vec3			UnderlyingSurfaceNormal_w;	//нормаль к точке на подстилающей поверхности

	//не хозяин:
	DynamicBody*	pDBody;						//сюда будем прикладывать АД силы
	ADVector		ADShape;					//АД форма: набор АД элементов, дающих АД силу 
	PlanePartList	PlaneParts;					//элементы несущей поверхности для пересчета удлинения поверхности 
												//в случае его повреждения (ВНИМАНИЕ! пока представлена только одна поверхность! При необходимости сделать массив или лист с индексами)

	double			PlaneSpan;
	double			PlaneArea;					
	double			CenterSectionSpan;
	double			CenterSectionArea;

	double			WindTunArea;				//характерные величины для продувки
	double			WindTunChord;
	double			WindTunSpan;
};

}

#endif