#ifndef _wNetGroup_h_
#define _wNetGroup_h_

//Group of remote units

#include "wControl.h"

#include "CoalitionInfo.h"

#include "../../Frogfoot/include/Aircrafts/wNetPathData.h"

class IwcCountry;

class WORLDGENERAL_API wNetGroup : public wControl
{
public:
	wNetGroup(wClass groupClass, IwcCountry * coalition);
	~wNetGroup();
	virtual void    	control();
	virtual void    	destroy(bool immediately = false);
	
	virtual wClass		Class() const;
	virtual bool		isDead() const;
	virtual wcCoalitionName	Coalition() const;
	
	virtual Unit		getLeader() const;
	virtual Unit		getUnit(int number) const;
	virtual int			getSize() const;
	virtual cPoint 		getPos() const;
	
	void				setLeader(const Unit & leader);
	void				onMemberBirth(const Unit & member, int number);
	void				onMemberDeath(const Unit & member);

	const AI::NetAIPath &getNetRoute();
	void setNetRoute( AI::Route *pRoute );
	void setNetRouteIdx( int nIdx );
	AI::NetAIControl &getNetControl();

	virtual bool		isUncontrolledFlag() { return _bIsUncontrolled; } 
	virtual void		setNetUncontrolledFlag( bool bValue ) { _bIsUncontrolled = bValue; }

private:
	const wClass 		groupClass_;
	IwcCountry *		coalition_;
	Unit				leader_;
	Units				units_;

	AI::NetAIPath		_netAIPath;
	AI::NetAIControl	_netAIControl;
	bool			    _bIsUncontrolled;
};

#endif _wNetGroup_h_