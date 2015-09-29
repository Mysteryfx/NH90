#ifndef ED_OBJECTS_GRAPH_HELPER_H
#define ED_OBJECTS_GRAPH_HELPER_H

#include "Renderer/DbgGraph.h"
#include "viMovingObject.h"
#include "edObjects.h"

namespace Graphics{
	class Model;
}

struct gBox;

class vi_helper : public viMovingObject
{
public:
    EDOBJECTS_API static vi_helper * create(dbg_obj * obj_);
    EDOBJECTS_API vi_helper();
    EDOBJECTS_API virtual ~vi_helper();
    EDOBJECTS_API void                Parse(Graphics::ModelParser& parser,const Position3& pos,Graphics::effectState* effects);
    Graphics::Model  *  GetModel() { return 0; }
    EDOBJECTS_API const   gBox     &  LocalBox(wModelTime t);

    const Position3& GetPosition()
    {
        static Position3 p;
        p = pos;
        return p;
    }
    cPosition &         Position(wModelTime)
    {
        return pos;
    };
    EDOBJECTS_API void                set_obj(dbg_obj * new_obj);
    EDOBJECTS_API void                set_position(const cPosition & pos_);
private:
    dbg_obj             * obj;
    gBox                  box_;
    cPosition             pos;
};

typedef cPointerTemplate<vi_helper>	viHelperPointer;
#endif
