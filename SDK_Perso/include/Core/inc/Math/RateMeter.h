/**
 * @brief Rate meter.
 * @author Dmitry S. Baikov
 */
#ifndef _ED_Math_RateMeter_h_
#define _ED_Math_RateMeter_h_

#include "./_config.h"


namespace Math {


class RateMeter {
private:
	double m_time;
	double m_val;
	double m_invtau;

public:
		RateMeter(double tm, double val, double k) { m_invtau = 1/k; reset(tm, val); }

	void reset(double tm, double val) { m_time = tm; m_val = val; }
	double value() const { return m_val; }
	ED_MATH_EXTERN void update(double tm, double sz);
};

} // namespace Math

#endif /* _ED_Math_RateMeter_h_ */
