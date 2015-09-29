#pragma once
#include "dPosition.h"
#include "Lua/Config.h"
//#include "Utilities/ccPolarPosition.h"

#ifdef _ED_HIDDEN_DEBUG 
#include "Renderer/DbgGraph.h"
#endif

namespace cockpit
{

    class avSensorEye
    {
    public:
		avSensorEye():fov(0.0),failure(false)
        {
#ifdef _ED_HIDDEN_DEBUG 
            dbg = 0;
#endif
        }
       ~avSensorEye()
       {
#ifdef _ED_HIDDEN_DEBUG 
           if (dbg)
           {
               delete dbg;
           }
#endif
       }
        dPosition     pos;
        double        fov;
        bool          failure;

		void		initialize(Lua::Config &conf);

        bool      inside(const dVector & point_in_carrier_axes) const
        {
            static  dVector pnt_in_eye;
            mult_transp(pnt_in_eye,pos,pos.p - point_in_carrier_axes);

            pnt_in_eye.z = sqrt(pnt_in_eye.z * pnt_in_eye.z +
                                pnt_in_eye.y * pnt_in_eye.y);

            return fabs(atan2(pnt_in_eye.z,pnt_in_eye.x)) < fov/2.0;
        }

#ifdef _ED_HIDDEN_DEBUG 
        void      parse(Graphics::RenderParser & parser ,const Position3 & pos_3)
        {
            if (!dbg)
            {
                dbg = new dbg_cone(4.0,fov);
                dbg->set_color(Vector3(1,1,0));
                dbg->get_pos() = Position3(toVec3(pos.x),
                                           toVec3(pos.y),
                                           toVec3(pos.z),
                                           toVec3(pos.p));
            }
            if (dbg)
            {
                dbg->parse(parser,pos_3);
            }
        }
        dbg_obj * dbg;
#endif
    };

}
