/**
 * @brief Linear predictor.
 * @author Dmitry S. Baikov
 */
#ifndef _ED_Math_Predictor_h_
#define _ED_Math_Predictor_h_

#include "./_config.h"


namespace Math {


#if 0
template<int N>
class Predictor {
public:
		Predictor() {}

	ED_MATH_EXTERN void init(double value, double deriv, const double k[N]);
	ED_MATH_EXTERN void stepBy(int steps, const double k[N]);
	ED_MATH_EXTERN void update(double in) { diff = in - out; }

	double res() const { return out; }
	double err() const { return diff; }

	ED_MATH_EXTERN double vel(const double k[N]) const;
	ED_MATH_EXTERN double acc(const double k[N]) const;

	void reconf(const double old_k[N], const double new_k[N])
	{
		double d[N-1];
		unpack(d, old_k);
		pack(d, new_k);
	}

private:
	double diff, out;
	double i[N];

	ED_MATH_EXTERN void unpack(double d[2], const double k[N]);
	ED_MATH_EXTERN void pack(const double d[2], const double k[N]);

};

#else

inline
double thresh(double d, double eps) { return d < -eps ? d : d > eps ? d : 0; }


class Predictor3 {
public:
		Predictor3() {}

	ED_MATH_EXTERN void init(double value, double deriv, const double k[3]);
	ED_MATH_EXTERN void step(int steps, const double k[3]);
	ED_MATH_EXTERN void update(double in) { diff = in - out; }
	ED_MATH_EXTERN void update(double in, double eps) { diff = thresh(in - out, eps); }

	double res() const { return out; }
	double err() const { return diff; }

	ED_MATH_EXTERN double vel(const double k[3]) const;
	ED_MATH_EXTERN double acc(const double k[3]) const;

	void reconf(const double old_k[3], const double new_k[3])
	{
		double d[3-1];
		unpack(d, old_k);
		pack(d, new_k);
	}

	ED_MATH_EXTERN void freeze(int i);

private:
	double diff, out;
	double i[3];

	ED_MATH_EXTERN void unpack(double d[2], const double k[3]);
	ED_MATH_EXTERN void pack(const double d[2], const double k[3]);

};


class Predictor4 {
public:
		Predictor4() {}

	ED_MATH_EXTERN void init(double value, double deriv, const double k[4]);
	ED_MATH_EXTERN void step(int steps, const double k[4]);
	ED_MATH_EXTERN void update(double in) { diff = in - out; }
	ED_MATH_EXTERN void update(double in, double eps) { diff = thresh(in - out, eps); }

	double res() const { return out; }
	double err() const { return diff; }

	ED_MATH_EXTERN double vel(const double k[4]) const;
	ED_MATH_EXTERN double acc(const double k[4]) const;

	void reconf(const double old_k[4], const double new_k[4])
	{
		double d[4-1];
		unpack(d, old_k);
		pack(d, new_k);
	}

	ED_MATH_EXTERN void freeze(int i);

private:
	double diff, out;
	double i[4];

	ED_MATH_EXTERN void unpack(double d[2], const double k[4]);
	ED_MATH_EXTERN void pack(const double d[2], const double k[4]);

};
#endif

} // namespace Math

#endif /* _ED_Math_Predictor_h_ */
