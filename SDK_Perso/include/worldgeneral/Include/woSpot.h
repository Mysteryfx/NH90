#pragma once
#include "WorldGeneral.h"
#include "dPosition.h"
#include "wMovingObject.h"
#include "WorldMail.h"
#include <ed/vector.h>
#include "wTime.h"

enum   spot_type
{
	SPOT_INFRARED,
	SPOT_LASER,
	SPOT_WP,
	SPOT_RADAR,
	SPOT_TYPE_MAX
};

#define IID_WOSPOT Common::identify<woSpot>()

class woSpot;
class dbg_obj;

namespace spot_mail {

    struct status_msg
    {
        dVector         point;
        unsigned        code;
        cVector         start_local_point;
        bool            update_start_point;
    }; 
    struct create_msg
    {
        ObjectID        id;
        status_msg      status;
        ObjectID		carrier;	// ID носителя	
        unsigned 	    type;		// тип
    };
    class mailer {
    public:
        mailer();
       ~mailer();
    };   
    typedef GenMessage<woSpotMsgID+1, Mail::CreateMsg ,create_msg>          MsgCreate;
    typedef GenMessage<woSpotMsgID+2, Mail::StatusMsg ,ObjectID,status_msg> MsgStatus;
    typedef GenMessage<woSpotMsgID+3, Mail::DestroyMsg,ObjectID>            MsgDestroy;
}; // class woCarMail

namespace Mail 
{
    Stream& operator >> (Stream& stream, spot_mail::status_msg & data);
    Stream& operator >> (Stream& stream, spot_mail::create_msg& data);
}

typedef ed::vector<woSpot*> woSpots;


template<typename Arg, typename Result>
class UnaryFunction : public std::unary_function<Arg, Result>
{
public:
	typedef UnaryFunction<Result, Arg> Reverse;
	virtual Result		operator()(const Arg & arg) const = 0;
	virtual Result		integrate(const Arg & from, const Arg & to) const = 0;
	virtual Reverse *	getReverse() const = 0;
};

class WORLDGENERAL_API woSpot : public MovingObject
{
private:
	class Timer : public wTime
 	{
	public:
		enum Events { TIMER_EVENT_DESTROY, TIMER_EVENT_EVENT, TIMER_EVENT_MAX };
	public:
		Timer();
		Timer(woSpot * pSpot_, unsigned int event_, wModelTime dt_);
		virtual void NextEvent();
	private:
		woSpot*			pSpot;
		unsigned int	event;
	};
public:
	typedef UnaryFunction<wModelTime, double> Signal;
	class ConstantSignal : Signal
	{
	public:
		ConstantSignal(double valueIn) : value(valueIn) {;}
		virtual result_type	operator()(const argument_type & arg) const
		{
			return value;
		}
		virtual result_type	integrate(const argument_type & from, const argument_type & to) const
		{
			return value * (from - to);
		}
		virtual Reverse *	getReverse() const
		{
			return NULL; //функция f(x) = const неинъективна!
		}
	private:
		double value;
	};
	 woSpot();
	~woSpot();	
    int				    Class()	{ return woClass_Point; }

	static woSpot		* create_infrared_spot(unsigned shooter, const dVector & start_local_point, const dVector & p);
	static woSpot		* create_laser_spot   (unsigned shooter, const dVector & start_local_point,const dVector & p, unsigned code);
	static woSpot		* create_smoke_spot   (unsigned shooter, const dVector & p, unsigned code, wModelTime smokePeriod);
	static woSpot		* create_radar_spot   (unsigned shooter, const dVector & p, unsigned code);

	unsigned			get_type() const			{ return sp_type;}
	unsigned			get_code() const			{ return code;}
	float				get_radius() const;

	void				set_code(unsigned new_code) { code = new_code;}
    void				set_type(unsigned new_type) { sp_type = new_type;}
	void				setDirToSpotter(cVector & dirToSpotter_) { dirToSpotter = dirToSpotter_; }
	void				setPower(float power_) { power = power_; }

    void				activate();
	void				set_point	            (const dVector   & pnt);
    void                set_start_point_local   (const dVector & pnt);//

	float				getPowerInPoint(const cPoint & point) const;

    void                Parse(Graphics::ModelParser& parser,const Position3& pos,Graphics::effectState* effects);


	// интерфейс viObject
          cPosition  &  Position(wModelTime t);	
    const cPosition  &  Position() const	    { return pos;}
    virtual dVector     GetPoint()              { return pos_p;}
    Graphics::Model  *  GetModel() { return 0; }
	const   gBox     &  LocalBox(wModelTime t);
	// интерфейс MovingHandle

	float		        GetSize();

	void				onTimeEvent(unsigned int event);

    // iwNetObject
    virtual bool		netGetCreate(Mail::Message& msg) const;
    virtual bool		netSendUpdate(int dest, int prio, int& cookie);
    static  void        netCreate(const spot_mail::create_msg & c);
    void                netUpdate(const spot_mail::status_msg & s);
    void                netDestroy();
public:	
	static bool			detectSpots(cPosition & pos, float angle, woSpots & spots, unsigned type, unsigned code = 0);
	static bool			detectLaserSpot(cPosition & pos, float angle, cVector & dir, unsigned code = 0);
	static bool			detectSpot(cPosition & pos, float angle, cVector & dir, unsigned type = SPOT_TYPE_MAX, unsigned code = 0);
protected:
    void                recalc_position(double t);
    dPosition           reverse_position(double t) const;
    dVector             start_point(double t) const;
	unsigned			sp_type;
	unsigned			code;
	woPointer			shooter;
	unsigned			shooter_id;
	cVector				dirToSpotter;
    dVector             start_local_point;
	float				power;
    float               ray_length;
    Graphics::RenderObject *   ray_;
	std::auto_ptr<Signal> signal;
private:
    static spot_mail::mailer mail;

    void                set_ray_length(double ray_length_);
};

