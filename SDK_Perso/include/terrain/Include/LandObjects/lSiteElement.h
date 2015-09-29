#pragma once

#include "Terrain.h"
#include "LandObjects\lStandartStructure.h"
#include "LandObjects\lLandVehicle.h"

//наземные объекты, объединенные в группы - сайты
//выполняет некоторые функции интерфейса viObjectNode у подвижных объектов

class lSiteElement;

class TERRAIN_API lSite
{
public:
	lSite();
	~lSite();
	virtual float get_param_value(unsigned int param_num) = 0;
	virtual void clear_site(bool full = false) {;}
	//virtual void on_element_enable(lSiteElement*) = 0;
	virtual void on_element_disable(lSiteElement*) = 0;
};

#define IID_lSiteElementBuilding	Common::identify<lSiteElementBuilding>()
#define IID_lSiteElementTech 		Common::identify<lSiteElementTech>()

class TERRAIN_API lSiteElement
{
public:
	lSiteElement();
	lSiteElement(lSite * owner_);
	~lSiteElement();
	void set_site(lSite * site_);
	void * get_obj_ptr() const {return obj_ptr;}
protected:	
	//void notify_birth();
	void notify_death();
	void * obj_ptr;
	lSite * site;	
};

class TERRAIN_API lSiteElementBuilding : public lStandartStructure,
										 public lSiteElement
{
protected:
	//Graphics::DParamList drawArgs;	
//	bool AllowDrawBlocks();
	virtual void Degrade(bool silent);
public:
	lSiteElementBuilding();	
	static lLandObject* CreateObject();
};

class TERRAIN_API lSiteElementTech : 	public lLandVehicle,
										public lSiteElement
{
protected:
	//Graphics::DParamList drawArgs;	
//	bool AllowDrawBlocks();
	virtual void Degrade(bool silent);
public:
	lSiteElementTech();
	static lLandObject* CreateObject();
};
