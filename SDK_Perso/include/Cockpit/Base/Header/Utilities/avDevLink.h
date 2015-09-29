#ifndef _avDevLink_h
#define _avDevLink_h

#include <assert.h>
#include "ed/unordered_map.h"

namespace cockpit {

class avDevice;
class ccDrawable;

class ILinkVar
{
public:
	virtual ~ILinkVar() {}
	virtual void set(avDevice* pDeviceIn) = 0;
	
	ed::string Name;
};


typedef int link_key_type;

#define INVALID_DEV_LINK -1

struct device_link_data
{
	device_link_data() : target(nullptr), source(INVALID_DEV_LINK), isSoft(false) {}
	ILinkVar		* target;
	link_key_type	  source;
	bool              isSoft;
};

typedef ed::unordered_map<ed::string,device_link_data>		dev_links;


template<class Td, class To> class DevLinkVar : public ILinkVar
{
public:
	DevLinkVar() : pRawDevice_(0), pTypedDevice_(0), converted_(false), pOwner(0), pLinkVarsSpace(0) {}
	virtual ~DevLinkVar() {unregFromLinkVarsSpace(); }
	virtual void set(avDevice* pDeviceIn) 
    { 
        pRawDevice_     = pDeviceIn;
        pTypedDevice_   = 0;
        converted_      = false;
    }

	inline void setTd(Td* pDeviceIn)
    {
        converted_		= true;
        pRawDevice_		= pDeviceIn;
        pTypedDevice_	= pDeviceIn;
    }

    inline Td* get()
    { 
        if (!converted_)
        {
            if (pRawDevice_)
            {
                pTypedDevice_ = dynamic_cast<Td*>(pRawDevice_);
                assert(pTypedDevice_);
            }
            converted_ = true;
        }

        return pTypedDevice_;
    }

	void regInLinkVarsSpace(const char* NameIn, To* pOwnerIn, ed::map< To*, dev_links >* pLinkVarsSpaceIn)
	{
        pOwner = pOwnerIn;
		if (!pOwner)
			return;

		pLinkVarsSpace = pLinkVarsSpaceIn;
		Name = NameIn;


		device_link_data new_link;
		new_link.target = this;
		new_link.source = INVALID_DEV_LINK;

		ed::map< To*, dev_links >::iterator it_g = (*pLinkVarsSpace).find(pOwner);
		if (it_g == (*pLinkVarsSpace).end())
		{
			dev_links		  new_links;
			new_links[Name] = new_link;
			(*pLinkVarsSpace)[pOwner] = new_links;
		}
		else
		{
			dev_links & Links = it_g->second;
			Links[Name]		  = new_link;
		}
	}
	
	void unregFromLinkVarsSpace()
	{
		if (pOwner && pLinkVarsSpace && Name.size() > 0)
		{
			ed::map< To*, dev_links >::iterator it_g = (*pLinkVarsSpace).find(pOwner);
			if (it_g != (*pLinkVarsSpace).end())
			{
				dev_links & Links = it_g->second;
				dev_links::iterator it_l = Links.find(Name);
				if (it_l != Links.end())
				{
					Links.erase(it_l);
					pOwner = 0;
				}
				if (!Links.size())
					(*pLinkVarsSpace).erase(it_g);
			}
		}
	}

protected:
    avDevice* pRawDevice_;
    Td* pTypedDevice_;
    bool converted_;

	To* pOwner;
	ed::map<To*,dev_links> * pLinkVarsSpace;
};

template<class Td, class To> class LinkPtr
{
public:
	LinkPtr() {Ptr = new DevLinkVar<Td, To>;}
	~LinkPtr() {delete Ptr;}
	inline operator Td*() const {return Ptr ? Ptr->get() : 0;}
	inline Td* operator->() const {return Ptr ? Ptr->get() : 0;}
	inline LinkPtr& operator = (Td* pDeviceIn) {if (Ptr) {Ptr->setTd(pDeviceIn); } return *this;}

protected:
	DevLinkVar<Td, To>* Ptr;
};

#define DECLARE_LINKPTR(MType, MName, MOwnerType, MOwner, MLinkVarsSpace) \
    template <int dummy = 0> \
	class LinkPtr_##MName : public LinkPtr<MType, MOwnerType> \
	{ \
        typedef LinkPtr<MType, MOwnerType> base; \
        typedef LinkPtr_##MName self; \
	public: \
		LinkPtr_##MName() {Ptr->regInLinkVarsSpace(#MName, MOwner, MLinkVarsSpace);} \
		inline LinkPtr_##MName& operator = (MType* pDeviceIn) { return static_cast<self&>(base::operator =(pDeviceIn)); } \
	}; \
	LinkPtr_##MName<> MName;

// USAGE:
// DECLARE_DEV_LINKPTR(class, name);
// DECLARE_IND_LINKPTR(class, name);

#define DECLARE_DEV_LINKPTR(MType, MName) \
	DECLARE_LINKPTR(MType, MName, avDevice, avDevice::LastCreatedDevice, &avDevice::LinkVarsSpace);

#define DECLARE_IND_LINKPTR(MType, MName) \
	DECLARE_LINKPTR(MType, MName, ccDrawable, ccDrawable::LastCreatedIndicator, &ccDrawable::LinkVarsSpace);

}

#endif // _avDevLink_h
