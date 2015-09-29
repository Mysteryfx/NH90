#pragma once

#include "CockpitBase.h"
#include "Avionics/avDevice.h"
#include "Utilities/ccPolarPosition.h"
#include "Avionics/avMissionTargetManager.h"
#include "Utilities/avDrawCashedObj.h"

namespace cockpit
{
    class avMLWS;
    class SightDevice;
    class avPadlock;
    enum arcade_target_type
    {
        TARGET_IS_OTHER,
        TARGET_IS_AIRBORNE,
        TARGET_IS_GROUND,
    };
    class  COCKPITBASE_API  arcade_target : public avDrawCashedObj
    {
    public:
        arcade_target()                     
        {   
            id  = 0;
            last_time = -20.0;
            friendly = false; 
            what = 0; 
            work_at_you = false; 
            update_time = -1;
            danger = 0;
            mov_ptr = 0;
        }
        arcade_target(viObject * ptr)
        {
            id        = (ptr) ? ptr->ID() : 0; 
            update();
        }
        arcade_target(const arcade_target & trg)   {     *this = trg;   }
        arcade_target & operator=(const arcade_target & t)
        {
            if (this != &t)
            {
                pos = t.pos;
                velocity = t.velocity;
                course = t.course;
                friendly = t.friendly;
                danger = t.danger;
                id = t.id;
                work_at_you = t.work_at_you;
                last_time = t.last_time;
                what = t.what;
                update_time = t.update_time;
                mov_ptr     = t.mov_ptr;
                local_pnt   = t.local_pnt; 
                reset_draw_objects();
            }
            return *this;
        }
        bool operator ==(const arcade_target & t) const   {  return id == t.id;    }
        bool operator !=(const arcade_target & t) const   {  return id != t.id;    }
       ~arcade_target();
        dVector   get_point(double dt = 0) const;
        dPosition get_position(double dt = 0) const;
        double    get_course() const;
        bool      update();
     public:
        dPosition  pos;
        dVector    velocity;
        double     course;
        bool       friendly;
        double     danger;
        unsigned   id;
        bool       work_at_you;
        double     last_time;
        unsigned   what;
        woPointer  mov_ptr;
        dVector    local_pnt;
    private:
        double     update_time;
    };
    

    class pred_nearest_by_angle : public std::less<arcade_target>
    {
    public:
        pred_nearest_by_angle(const dPosition & human_pos,const PolarPosition & o):pos(human_pos),origin(o){}
        bool operator()(const arcade_target& left, const arcade_target& right) const;
    private:
        const dPosition      & pos;
        const PolarPosition  & origin;
    };

    class pred_nearest_by_distance : public std::less<arcade_target>
    {
    public:
        pred_nearest_by_distance(const dPosition & human_pos):pos(human_pos){}
        bool operator()(const arcade_target& left, const arcade_target& right) const;
    private:
        const dPosition      & pos;
    };

    class pred_most_dangerous : public std::greater<arcade_target>
    {
    public:
        pred_most_dangerous(){}
        bool operator()(const arcade_target& left, const arcade_target& right) const
        {
            return get_danger(left) > get_danger(right);
        }
    private:
        double get_danger(const arcade_target & trg) const;
    };
    

    class arcade_target_validator : public std::unary_function<arcade_target,bool>
    {
    public:
        virtual  bool operator()(arcade_target& t) const = 0;
    };

    class pred_garbage_collector : public arcade_target_validator
    {
    public:
        pred_garbage_collector(const arcade_target & ex, const wModelTime forgetExp):exclude(ex),forgetExposure(forgetExp) {}
        bool operator()(arcade_target& t) const
        {
            if (exclude.id)
            {
                if (t == exclude)
                {
                    return false;
                }
            }
            t.update();
            if (t.id)
            {
                return wTime::GetModelTime() - t.last_time > forgetExposure;
            }
            else
            {
                return true;
            }
        }
    private:
        const arcade_target      & exclude;
        wModelTime               forgetExposure;
    };

    class pred_remove_too_far : public arcade_target_validator
    {
    public:
        pred_remove_too_far(const arcade_target & ex,double _range):exclude(ex),range(_range)
        { 
            if (human_unit().human_ptr != NULL)
                self_pos = human_unit().human_ptr->GetPoint();
        }
        bool operator()(arcade_target& t) const
        {
            if (exclude.id)
            {
                if (t == exclude)
                {
                    return false;
                }
            }
            if((t.get_point() - self_pos).length2() > range * range)
            {
                return true;
            }
            return false;
        }
    private:
        const arcade_target      & exclude;
        double                     range;
        dVector                    self_pos;
    };
    

    typedef ed::list<arcade_target> arcade_target_list;

    class avMLWS;
    class avArcadeTargetSelector;
    class avArcadeTargetSelector_timer : public avBasicTimer<avArcadeTargetSelector>
    {
    public:
        avArcadeTargetSelector_timer(avArcadeTargetSelector * _parent, wModelTime _dtime);
        void		 NextEvent();
    };

    class COCKPITBASE_API avArcadeTargetSelector 
    {
    public:
        avArcadeTargetSelector();
       ~avArcadeTargetSelector();
        
        void     set_base_point(const dVector & bp) { stab = true; base_point = bp;}
        void     left()  { move_horizontal(-1);};
        void     right() { move_horizontal( 1);};
        void     up()    { move_vertical  ( 1);};
        void     down()  { move_vertical  (-1);};
        void     stop();
        void     clear();
        void     update(double dt);
        bool     moving() const { return h_moving != 0 || v_moving != 0; }

        const dPosition & get_orientation();
        const dVector &   get_look_point()  {  get_orientation();  return look_point; }
        const dVector &   get_direction ()  {  get_orientation();  return direction; }
        double            get_range ()      {  get_orientation();  return range; }

        void     unstabilize()
        {
            clear();
            stab = false;
        }
        bool     stabilized() const { return stab; }
    protected:
        void     stabilize();

        void     move_horizontal(int dir);
        void     move_vertical(int dir);




        avArcadeTargetSelector_timer *timer;
        dVector  base_point;
        dVector  look_point;
        dVector  direction;
        double   range;
            
        int      h_moving;
        int      v_moving;
        double   h_angle;
        double   v_angle;
        double   h_speed;
        double   v_speed;
        double   speed_base;
        double   acceleration_base;

        double   update_time;
        bool     stab;
    };

    class COCKPITBASE_API avArcadeRadar : public avDevice
    {
        friend class ccArcadeRadar;
    public:
         avArcadeRadar();
        ~avArcadeRadar(){}

        void initialize(unsigned char id, const ed::string& name, const ed::string& script);
        void SetCommand(int command, float value = 0);
        void update();
        void update_current();
        
        const arcade_target & get_target() const { return target; }
              arcade_target & get_target()       { return target; }

        void  set_target(viObject * obj)
        {
            target = arcade_target(obj);
            on_change_target();
        }
        
        const arcade_target_list       & get_airborne_targets()     const { return airborne_targets;};
        const arcade_target_list       & get_ground_targets()       const { return ground_targets;};
        const arcade_target_list       & get_airborne_friendlies()  const { return airborne_friendlies;};
        const arcade_target_list       & get_ground_friendlies()    const { return ground_friendlies;};
        const avMissionTargetManager   & get_mission_targets_manager() const { return mission_targets;}
        bool                       get_show_radar() const        { return show_radar; }
        virtual double             get_search_distance() const;
        const          dPosition & get_human_position() const;
        const avArcadeTargetSelector & get_selector() const { return selector; } 

        dVector                    get_target_point();
        bool                       have_point() const;

        virtual SightDevice *      get_sight() { return 0; };
        virtual bool               get_sight_data(PolarPosition & pol);
		virtual bool               get_gun_sight_data(PolarPosition & pol) { return false; }; // for any type selected weapon it will return data for cannon

        virtual bool               get_current_waypoint(dVector & pnt) { return false; };
        virtual const wsType &     get_selected_weapon_type() const;
        virtual bool               get_gun_is_selected() const;
		virtual bool			   is_station_selected(int station) const;
		virtual int				   get_selected_weapon_count() const;
		virtual unsigned		   get_selected_ammo_type() const;
		virtual unsigned		   get_shells_count() const;
		virtual bool			   get_fire_permission() const { return true; } 
    protected:
        arcade_target_list      airborne_targets;
        arcade_target_list      airborne_friendlies;
        arcade_target_list      ground_targets;
        arcade_target_list      ground_friendlies;
        arcade_target           target;
        avMissionTargetManager  mission_targets;
        bool                    show_radar;
        
		DECLARE_DEV_LINKPTR(avMLWS   ,mlws);
        DECLARE_DEV_LINKPTR(avPadlock,padlock);

        ed::vector<double>     scales;
        unsigned                scale;
        bool                    select_next_after_destroy;

        avArcadeTargetSelector  selector;
    protected:
        virtual void  on_change_target();;

        typedef bool (*type_handler)(const wsType & type);

        static bool check_born(viObject *item);
        static bool handler_common   (viObject *item, viSearch *search, void* data);
        bool        handler_implement(const arcade_target & trg,arcade_target_list & lst);
        virtual void register_listener();

        template<class _iter> 
        _iter next(_iter it_st,_iter it_en)
        {
            _iter it  = std::find(it_st,it_en,target);
            if (it  == it_en)
            {
                return it_st;
            }
            if (++it != it_en) 
            {
                return it;
            }
            else
            {
                return it_st;
            }
        }
        
        template<class _iter>  
        void select(_iter it,_iter end_)
        {
            if (it != end_ &&
                it->id)
            {
                target = *it;
            }
            last_kill_time  = 0;
            do_select_next  = false;
            on_change_target();
        }

        void next(arcade_target_list & lst)     
        {
            collect_garbage(lst);
            select(next(lst.begin(),
                        lst.end()),
                   lst.end());
        }
        
        void previous(arcade_target_list & lst)            
        {
            collect_garbage(lst);
            select(next(lst.rbegin(),
                        lst.rend()),
                   lst.rend());
        }

        void nearest_to_hud(arcade_target_list & lst)      
        { 
            target = arcade_target();
            collect_garbage(lst);
            pred_nearest_by_angle op(get_human_position(),
                                     PolarPosition(0,0,0));

            select(std::min_element(lst.begin(),
                                    lst.end(),
                                    op),
                   lst.end());
            lst.sort(op);
        }

        void nearest_by_distance(arcade_target_list & lst) 
        {
            target = arcade_target();
            collect_garbage(lst);

            pred_nearest_by_distance op(get_human_position());

            select(std::min_element(lst.begin(),
                                    lst.end(),
                                    op),
                   lst.end());
            lst.sort(op);
        }

        void most_dangerous(arcade_target_list & lst)
        {
            target = arcade_target();
            collect_garbage(lst);
            
            pred_most_dangerous op;

            select(std::min_element(lst.begin(),
                                    lst.end(),
                                    op),
                   lst.end());
            lst.sort(op);
        }

        void collect_garbage(arcade_target_list & lst)
        {
            remove_by_predicate(lst,pred_garbage_collector(target, 20.0)); // target remains in targets list 20 seconds after last detection
        }

        void remove_too_far(arcade_target_list & lst)
        {
            remove_by_predicate(lst,pred_remove_too_far(target,get_search_distance()));
        }

        bool exist(const arcade_target & trg,const arcade_target_list & lst)
        {
            return std::find(lst.begin(),
                             lst.end(),
                             trg) != lst.end();
        }

        void insert(const arcade_target & trg,arcade_target_list & lst)
        {
            arcade_target tr = trg;
            tr.last_time = wTime::GetModelTime();
            lst.push_back(tr);
        }


        virtual void   search_targets(bool (*func)(viObject *item, viSearch *search, void *data));

        void   search_targets_by_selector();
        void   selector_command(int command);

        void   remove_by_predicate(arcade_target_list & lst,const arcade_target_validator & op)
        {
            arcade_target_list::iterator it = lst.begin();
            while(it != lst.end())
            {
                if (op(*it))
                {
                    it = lst.erase(it);
                    continue;
                }
                ++it;
            }
        }
    private:
        double     last_kill_time;
        bool       do_select_next;
    };
}