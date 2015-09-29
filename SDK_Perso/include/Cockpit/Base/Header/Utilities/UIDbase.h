#ifndef __UID_BASE_H__
#define __UID_BASE_H__	1

#include "CockpitBase.h"

class COCKPITBASE_API UID_base
{
public:
    UID_base()
    {
        min_uid                = 0; 
        min_free_UID           = 0;
        min_free_UID_is_actual = false;
    }
	void  set_min_uid(int min_uid_) {min_uid = min_uid_;}
    const int get_min_uid() const { return min_uid; }
        
    void  clear();
	int   take_UID();
	void  free_UID(int IUD);
	bool  is_busy(int UID) const;
	void  set_busy(int UID);
	int   get_busy_qty() const { return (int)base.size(); }
private:
	typedef ed::list<int> UID_list;
	int get_min_free_UID();
	UID_list base;
	int min_free_UID;
	bool min_free_UID_is_actual;
	int min_uid;
};

#endif __UID_BASE_H__