#pragma once

#include "Common.h"
#include "wTime.h"
#include "Utilities\ccSimpleOctTree.h"
#include "Utilities/avDrawCashedObj.h"
#include "CockpitBase.h"
#include "dPosition.h"

struct Waypoint;

namespace cockpit {

class Element;

class COCKPITBASE_API avMovingMapPoint : public SimpleOctTreeLoad,
                                         public avDrawCashedObj,
										 public Common::FakeUnknown<Serializable>
{
public:
	avMovingMapPoint();
	virtual ~avMovingMapPoint();

	virtual	void			serialize(Serializer &serializer);

    void                    setOrientedBox(const Box & bx) { OBB = bx;}        
	unsigned 				getType()       const { return     type;};
	unsigned 				getSubType()    const { return     sub_type;};
	const  ed::string &	getName()		const { return     name;};
	const  dVector&			getPoint()		const { return     position.p;};
    const  dPosition&       getPosition()   const { return     position;}
	virtual void			setName(const ed::string &new_name) { name     = new_name;};
	int						getNumber() const { return number; }
	void					setNameByNumber(int number_,const char * format = 0);
	void					setNumber(int number_) { number = number_;}
	void				    setPoint(const dVector &point);
    void                    setPosition(const dPosition & pos);
	void					setRotatePoint(const dVector &point, float angle);
	void					setRotate(float angle);

	bool               isSystem() const		{ return system;}
	void               setSystem(bool flag_in) { system   = flag_in;}
	void			   setType(unsigned type_) { type = type_;}
	void			   setSubType(unsigned sub) { sub_type = sub;}
	// Высота в метрах
	void			   setHeight(double HeightIn) {position.p.y = HeightIn;}
	double			   getHeight() {return position.p.y;}

protected:
	ed::string       name;
	unsigned          type;
	unsigned          sub_type;
	bool              system;
    dPosition         position;
	Box               OBB; //oriented bounding box
	int				  number;
};

}
