#ifndef lLandObject_H
#define lLandObject_H

#include "cPosition.h"
#include "viObjectFlatShape.h"
#include "ClipVolumes/ClipVolumeBox.h"
#include "wShapeTable.h"
#include "wsCollisionData.h"
#include "Notificator/Evoker.h"
#include "Mail/Stream.h"


class gShapeObject;
// динамические данные о наземном объекте
class lObjectDynData;
struct wShape;
class RegLandObjectStorage;
enum wcCoalitionName;

#define IID_lLandObject Common::identify<lLandObject>()

// блок отрисовки, в который входит домик
/*/
class EDOBJECTS_API lDrawBlock
{
public:
	virtual void Deconstruct() = 0;
};
/*/
class lLandObject;

class EDOBJECTS_API LandObjectDestroyNotifierBase : public LinkHost
{   
public:
	virtual void onLandObjectDestroy(lLandObject * obj) = 0;
};

typedef Link<LandObjectDestroyNotifierBase>  LandObjectDestroyNotifierPtr;

template <class T> 
class LandObjectDestroyNotifier : public LandObjectDestroyNotifierBase
{   
	T                    * owner_;
	void (T::*f_) (lLandObject * obj);
public:
	LandObjectDestroyNotifier(T * ptr_,void (T::*f)(lLandObject * obj))
	{
		owner_    = ptr_;
		f_		  = f;
	}
	void onLandObjectDestroy(lLandObject * obj)
	{
		if (f_ != NULL)
		   (owner_->*f_)(obj);
	}
};

class EDOBJECTS_API LandObjectDestroySignal  : public Evoker<LandObjectDestroyNotifierBase, LandObjectDestroyNotifierPtr >
{
public:
	void notifyDestroy(lLandObject * obj)
	{
		evoke(&LandObjectDestroyNotifierBase::onLandObjectDestroy,obj);
	}
};


struct StaticObjectData
{
    enum EObjectState
    {
        EDefault,           // простой объект
        ECargoCanBe,        // может быть грузом
        ECargoChoosing,     // кто-то выбрал в меню этот груз и пытается подцепить
        ECargoDelivering,   // кто-то подцепил и уже куда-то летит с грузом
        ECargoFlightDamage, // груз повержден в полете(подцепленный груз повредился)
        ECargoBroken,       // сломанный груз
        //если понадобятся другие стейты объекта, не относящихся к грузам, то все равно можно доьбавлять их сюда
        //есть еще одно дополнительное поле связанное с EObjectState - ObjectID uAdditionalObjId
    };
    StaticObjectData() :
    coalition((wcCoalitionName)-1), nObjState(EDefault), countryID(0), uAdditionalObjId(0), fMass(0.f), objectID(0),ropeLength(7.0f)
    {}
	StaticObjectData & operator = (const StaticObjectData & data)
    {
        coalition = data.coalition;
        countryID = data.countryID;
        typeName = data.typeName;
        shapeName = data.shapeName;
        name = data.name;
        missionID = data.missionID;
        liveryID = data.liveryID;
        pos = data.pos;
        dead = data.dead;
        nObjState = data.nObjState;
        uAdditionalObjId = data.uAdditionalObjId;
        fMass = data.fMass;
		ropeLength = data.ropeLength;
        objectID = data.objectID;
        return *this;
    }

	ObjectID		objectID;
	wcCoalitionName coalition;
    int             countryID;
    ed::string      typeName;
    ed::string      shapeName;
    ed::string      name;
	ed::string      missionID;
	ed::string	    liveryID;
	cPosition	    pos;
	bool		    dead;
    EObjectState    nObjState;
    ObjectID        uAdditionalObjId;
    float           fMass;
	float			ropeLength;
};



// Наземные объекты
class EDOBJECTS_API lLandObject : public viObjectFlatShape
{
    friend class lLandObjectMail;
	friend class RegLandObjectStorage;


	LandObjectDestroySignal signal_destroy;

private:
	double    scale;
	cPosition pos;
	Position3 pos3;
	wShape *type;
protected:
	lObjectDynData* dyndata;
	ClipVolumeBox flatshape;

	bool serialize_changes;

	// создание flatshape по позиции и gShapeObject
	virtual void ConstructFlatShape();

	virtual bool ForceID(viFlag _type, ObjectID _id);

public:
	static lLandObject* CreateObject();
	lLandObject();
	virtual ~lLandObject();

	/// Fuck, I need to override it to minitor changing of object type.
	virtual void viSetType(viFlag _type);

	virtual void Init(wShape *_type, const cPosition& _pos, viFlag status = viUnknown, void* context=NULL,double _scale = 1.0);

	virtual void Parse(Graphics::ModelParser& parser, const Position3& pos, Graphics::effectState* effects=0);

	// фабрика по созданию dyndata
	virtual void CreateDynamicData();
    inline virtual float getVisibility()const{return type?(type->fVisibility):1;};
	inline wShape *Type() const			{return type;}
	inline lObjectDynData *DynData()	{return dyndata;}
	virtual const gBox &LocalBox(wModelTime t);

	// сколько жизни осталось в текущей форме
	virtual double CurLife() const;
	
	// поддержка интерфейса ModelInstance
	virtual const Position3& GetPosition() {return pos3;}

	// поддержка интерфейса viObject
	virtual cPosition &Position(wModelTime t);
	virtual Graphics::Model *GetModel();
	virtual ClipVolume *FlatShape();
	
	// сколько жизни осталось во всех формах
	virtual float Life() const;
	virtual float Life0() const;

	// флаг уничтоженности объекта - для дебрифинга и целеуказания
	virtual bool IsDead() const;

	// далее - функции воздействия на объект

	// изменить положение объекта, вызовет перерегистрацию (невозможно для viLandObject)
	virtual void SetPos(const cPosition& _pos);

	// если объект получил достаточно повреждений, чтобы деградировать
    // silent - деградировать ли без спецэффектов (в случае инициализации по сети)
	virtual void Degrade(bool silent);

	// register fake damage by hitman and degrade
	void Degrade(bool silent, ObjectID hitman);
	
	// вызывается для повреждений (должна создать динамическую информацию)
	virtual void TakeDamage(double amount, ObjectID hitman);

	void destroy(); //тихо убрать

    // Серверный домик?
    bool IsNetServer() const;

	virtual void PostCollisionProcessing(wsCollisionData *info, ObjectID obj);

	// восстановить исходное состояние (если создавалась dyndata)
	virtual void ReinitState();

	void serializeChanges(bool on);	// если включено - сериализовать только изменение состояния
	void serialize(Serializer &);

	// блок отрисовки
//	void SetDrawBlock(lDrawBlock *block);
//	lDrawBlock *GetDrawBlock();
//	virtual bool AllowDrawBlocks();

    // связанные наземные объекты, специализированные классы могут ими пользоваться
    // изначально сделано для линий электропередач
    virtual int  getLinkedObjects(ObjectID *buff);
    virtual void setLinkedObjects(ObjectID *buff, int count);

    virtual void take_missionID(const ed::string & group_id,const ed::string & unit_id) {};

	virtual void setStaticData(const StaticObjectData & staticDataIn);
    const StaticObjectData & getStaticData() const { return staticData; };
    StaticObjectData & getStaticData() { return staticData; }


	const ed::string & getTypeName() const;
	wcCoalitionName Coalition() const;
	unsigned char Country() const;
	const ed::string & getName() const;
	const ed::string & getMissionID() const;
	
	void notifyOnDestroy(LandObjectDestroyNotifierBase * listener)
	{
		signal_destroy.add(listener);
	}

protected:
	StaticObjectData staticData;

private:
    void SetLife(double life);

    void removeFromDistrict();
public:
	model::IModel* getObjectType() const;
};

namespace Mail
{
    EDOBJECTS_API Stream& operator >> (Stream& stream, StaticObjectData & param);
}


#endif // lLandObject_H
