#ifndef __RigidBody_h__
#define __RigidBody_h__

#include "FMEngineeringStructure/TouchPoint.h"

#include <ed/vector.h>

namespace EagleFM
{

struct RigidContact //точка контакта
{
	RigidContact() 
	{
		Element = 0;
		TP = 0;
		SparkTime = 0.0;
	}
	int			Element;	//номер элемента (см.структуру модели повреждения в Damage.lua)
	TouchPoint	*TP;
	double		SparkTime;			//для создания эффекта искр
};

typedef ed::vector<RigidContact> ContactVector;

struct DamageContact //поврежденный контакт
{
	DamageContact()
	{
		Element = 0;
		Value = 0;
	}
	int Element;	//номер элемента
    double Value;	//степень повреждения (0...1)
};

struct ElementProp //свойства элемента
{
	ElementProp()
	{
		ForceMax				= 1000000.0;
		ForceDamage				= 500000.0;
		SpringForceFactor		= 2400000.0;
		DirectDamperForceFactor	= 120000.0;
		BackDamperForceFactor	= 200000.0;
		ContactRadius			= 0.1;
		IntegrityFactor			= 1.0;

		initForceMax			= ForceMax;
		initForceDamage			= ForceDamage;
	}
	double ForceMax;	//разрушающая сила
	double ForceDamage;	//пороговая сила, наносящая ущерб (damage (0...1) = (Force - ForceDamage)/(ForceMax - ForceDamage)
	double SpringForceFactor;	//сила, создаваемая элементом (свойства амортизатора)
	double DirectDamperForceFactor;
	double BackDamperForceFactor;
	double ContactRadius;	//ход амортизатора
	double IntegrityFactor;

	double initForceMax;
	double initForceDamage;
};

typedef ed::vector<DamageContact> DamageContactVector;
typedef ed::vector<ElementProp>   ElementPropVector;


class EffectManager;

class FMBASE_API RigidBody //Класс, являющий динамическим хранилищем жестких контактов. 
{
public:
	RigidBody(DynamicBody*, EffectManager*);
	~RigidBody();

	void		setElementProp(int Element, const ElementProp&); //инструмент для настройки прочночти и упругости элементов
	void		setContactMaxNum(int MaxNum)	{ ContactMaxNum = MaxNum; Contacts.reserve(ContactMaxNum); } 
	void		setContactMinDist(double MinDist) { ContactMinDist = MinDist; }

	void		setNewContact(int Element, double Length, double SurfaceNoise, const Vec3& p_w, const Vec3& Normal_w, Vec3 V_w, int SurfaceType, double dt);
	void		addDamageElement(int Element, double value);

	void		simulate(double dt);

	void		applyForces();
	void		takeDamageContacts(DamageContactVector&);
	Vec3		getForce_l() const { return Force_l; }
	int			getContactsNum() const { return (int)Contacts.size(); }

	Vec3		getElementForce(int Element) const;
	
	const ElementProp &	getElementProp(int Element) const;
	ElementProp		  &	getElementProp(int Element);


	bool				FlagImmortal;

	void				repair();


	void				resetContacts(); // flush contacts 
private:
	DynamicBody			*pDBody;	//не хозяин    
	EffectManager		*pEManager;	//не хозяин    

	ContactVector		Contacts;
	DamageContactVector	DamageContacts;

	Vec3				Force_l;
	Vec3				Moment_l;	

	//Характеристики
	ElementPropVector   ElementProps;
	int					ContactMaxNum;		//максимальное количество контактных точек
	double				ContactMinDist;		//минимальное расстояние между ближайшими контактными точками

	double				DustTime;			//для создания эффекта пыли
	double				PiecesTime;			//для создания эффекта обломков
};

}

#endif