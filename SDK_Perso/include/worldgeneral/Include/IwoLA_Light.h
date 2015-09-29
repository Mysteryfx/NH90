#pragma once

#include "WorldGeneral.h"
#include <ed/vector.h>
#include "Renderer/VolumeEffect.h"

namespace Graphics
{
    class ModelParser;
};
namespace Lua
{ 
    class Config;
};
class Position3;
class woLA;

namespace plane_lights
{
    class WORLDGENERAL_API IwoLA_Light 
    {
    public:
        IwoLA_Light(){}
        virtual ~IwoLA_Light(){}
        virtual void   update(double dt) = 0;
        virtual void   parse(Graphics::ModelParser & parser,const Position3 & pos,Graphics::effectState * effects) {};// для обеспечения различных эффектов связаннных с точкой взгляда
        virtual void   set_power(double power) = 0;
        virtual double get_power() const = 0;

		virtual	const IwoLA_Light * get_child(unsigned c) const { return nullptr;}
		virtual IwoLA_Light       * get_child(unsigned c) { return nullptr;}

		virtual void   set_child_power(unsigned c,double power) { set_power(power); } 
		virtual double get_child_power(unsigned c) const { return get_power(); } 
    };
    typedef ed::vector<IwoLA_Light*> lights_ex_vector;

    class WORLDGENERAL_API woLA_LightCollection : public IwoLA_Light
    {
    public:
    public:
        woLA_LightCollection(){childs.clear();};
       ~woLA_LightCollection();

        void   parse(Graphics::ModelParser & parser,const Position3 & pos,Graphics::effectState * effects);
        void   update(double dt);
        void   set_power(double power);
        double get_power() const;
        //////////////////////////////////////////////////////////////////////////
        const IwoLA_Light * get_child(unsigned c) const;
        IwoLA_Light       * get_child(unsigned c);
        void   set_child_power(unsigned c,double power);
        double get_child_power(unsigned c) const;
        void   push_back(IwoLA_Light * light);//light can be zero !!!
        size_t size() const { return childs.size(); };
    protected:
        lights_ex_vector childs;
    };



    class WORLDGENERAL_API woLA_LightPrototype
    { 
    public:
        woLA_LightPrototype() {}
        virtual void          read(Lua::Config & config) = 0;
        virtual IwoLA_Light * create(woLA * boss) const = 0;
    };
};
