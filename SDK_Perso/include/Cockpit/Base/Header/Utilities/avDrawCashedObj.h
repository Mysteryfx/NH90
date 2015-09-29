#pragma once
#include "CockpitBase.h"
#include <ed/vector.h>

namespace cockpit
{
    class Element;
    class avDrawCashedObj
    {
    public:
        COCKPITBASE_API avDrawCashedObj();
        COCKPITBASE_API virtual  ~avDrawCashedObj();
        Element * get_draw_object(unsigned cash)  const   {   return (cash < draw_objects.size()) ? draw_objects[cash] : 0;  }
        COCKPITBASE_API void	  set_draw_object(Element *element,unsigned cash) const;
        COCKPITBASE_API void 	  reset_draw_objects() const;
    private:
        mutable ed::vector<Element*>  draw_objects;
    };

	template<class T> T* void_cast(void * ptr)
	{
		return static_cast<T*>((avDrawCashedObj*)(ptr));
	}

}