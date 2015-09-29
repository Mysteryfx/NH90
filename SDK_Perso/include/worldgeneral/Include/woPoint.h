//                    file woPoint.h
#pragma once

#include "WorldGeneral.h"
#include "wMovingObject.h"
#include "wActivityManager.h"


class lLandObject; 
class woPoint;
class woLandPoint;

#define IID_IPoint Common::identify<woPoint>() 
#define IID_ILandPoint Common::identify<woLandPoint>() 
#define IID_ITargetPoint Common::identify<woTargetPoint>() 

struct InitNetPoint;
struct StatusNetPoint;

class WORLDGENERAL_API woPoint: public MovingObject // Объекты - точки.
{         
public:
	enum Type
	{
		BasicPointType = 0,
		LandPointType,
		PathPointType,
		TargetPointType,
	};

	static woPoint* Create(const cVector& pos_, ObjectID id=0); // Создать объект-точку

	// network helpers

	woPoint();
    virtual				~woPoint();
	virtual bool		IsDead() const;
	int			        Class(void) {return woClass_Point;}
	Graphics::Model  *  GetModel();
	
  	virtual void		PostCollisionProcessing( wsCollisionData*, ObjectID);
	
	virtual	Type		type() const {return BasicPointType;};
	virtual void		setPosition(const cVector&);

	bool				netGetCreate(Mail::Message&) const;
	void				netDiscard();
    void                netDestroy();
	virtual void		netChangeStatus(const struct StatusNetPoint& str) {};
	void				kill();

protected:
	void				initialize();
	virtual void		destroy_();

private:
	virtual void		fillInitData(InitNetPoint& data) const {};

	friend struct PointMail;
};

ObjectID netCreatePoint(const InitNetPoint& data);

class WORLDGENERAL_API woLandPoint: public woPoint
{
public:
	static woPoint*		Create(lLandObject *lo, ObjectID id=0); // Создать объект карты-точку

	woLandPoint()       :m_uLandObjId(0){}
	virtual ~woLandPoint();

	Graphics::Model  *  GetModel();

	Type				type() const {return LandPointType;}

	lLandObject*		Get_pMap(void);

	static woLandPoint * getLandPoint(ObjectID landObjectID);

private:
	void                setLandObject(lLandObject *lo, ObjectID id);
	void				controlPoint(wModelTime& timeStep);
	void				fillInitData(InitNetPoint& data) const;

	wActivityManager	activities;
	ObjectID			m_uLandObjId;				// ИД объекта карты
	typedef ed::map<ObjectID,	woLandPoint*>		woLandPoints;
	static woLandPoints	landPoints;
};

class WORLDGENERAL_API woTargetPoint: public woPoint
{
public:
	static woTargetPoint* Create(const cVector& pos_, ObjectID columnId, wcCoalitionName coalition, ObjectID id=0, float radius = 0, ObjectID linkId = 0, ObjectID colorTrg = 0xFFFF0000);
	Type			type() const {return TargetPointType;}

	bool			isActive() const {return _isActive;};
	bool			Is_Target() {return isActive();}
	void			setActive(bool act);

	float			radius() const {return _radius;}
	void			setRadius(float r);
	void			setPosition(const cVector&);

	ObjectID		columnId() const {return _hostColumnId;};

	void			updateState();//Сетевое обновление информации о цели (жизнь, позиция, активность)
	virtual void	netChangeStatus(const struct StatusNetPoint& str);

	void			setLinkId( ObjectID objId );
	ObjectID		getLinkId() { return _linkId; }
    virtual dVector getAimPoint();
	void			setColorAttack( ObjectID color );
	ObjectID		getColorAttack() { return _colorTrg; }

private:
	void			fillInitData(InitNetPoint& data) const;
	void			controlPoint(wModelTime& timeStep);
	void			sendUpdateMsg();
	
	float _radius;
	wActivityManager _activities;
	ObjectID _hostColumnId;
	bool _isActive;
	ObjectID _linkId;
	ObjectID _colorTrg;
};