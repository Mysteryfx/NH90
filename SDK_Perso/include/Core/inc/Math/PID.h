#ifndef _ED_Math_PID_h_
#define _ED_Math_PID_h_

namespace Math {

template <class T, class K = T> class PID_
{
public:
    T   integrator;
    T   error;
    T   out;
	bool running;
	PID_() : integrator(T(0)), error(T(0)), out(T(0)), running(false) {}
	inline void init()
	{
		running = false;
		reset_integrator();
	}
	inline void reset_integrator()
	{
		integrator = T();
	}
	inline const T & step(const T & err, K dt, K kp, K ki, K kd)
    {
		T diff = running ? (err - error)/dt : T(0);
		out = kp*err + ki*integrator + kd*diff;
		integrator += err * dt;
		error = err;
		running = true;
		return out;
    }
	inline const T & step(const T & err, K dt, const K (&k)[3])
	{
		return step(err, dt, k[0], k[1], k[2]);
	}
	inline T step_const(const T & err, K dt, K kp, K ki, K kd) const
	{
		T diff = running ? (err - error)/dt : T(0);
		return kp*err + ki*integrator + kd*diff;
	}
	inline T step_const(const T & err, K dt, const T (&k)[3]) const
	{
		return step_const(err, dt, k[0], k[1], k[2]);
	}
};
typedef PID_<double> PID;
}

#endif /* _ED_Math_PID_h_ */
