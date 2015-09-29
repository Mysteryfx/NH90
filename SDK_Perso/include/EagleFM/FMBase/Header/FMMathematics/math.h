#ifndef __math_h__
#define __math_h__

#include <assert.h>

#include "Base.h"

#include "FMMathematics/CoreMath.h"

#include "FMMathematics/Constants.h"
#include "FMMathematics/Direction.h"
#include "Math/LinearEquations.h"
#include "Math/IIR_Filter.h"
#include "FMMathematics/Randomizer.h"
#include "FMMathematics/AbrasionCalculator.h"
#include "Math/roundDec.h"

namespace EagleFM
{
	inline void FMBASE_API calcYawPitchRollByMatrix(const Matrix3& mtr, float &yaw, float &pitch, float &roll)
	{
		yaw = Pi2;
		roll = 0.0;

		pitch = asin(__max(-1.0f, __min(mtr[0][1], 1.0f)));

		float cosPitch = cos(pitch);

		if(cosPitch > 0.0)
		{
			float A = __max(-1.0f, __min(mtr[0][0]/cosPitch, 1.0f));
			yaw = mtr[0][2] > 0 ? acos(A) : Pi2 - acos(A);
			yaw = (float)Pi2 - yaw;

			A = __max(-1.0f, __min(mtr[1][1]/cosPitch, 1.0f));
			roll = mtr[2][1] > 0 ? -acos(A) : acos(A);
		}
	}

	inline Matrix3 FMBASE_API calcMatrixByYawPitchRoll(float yaw, float pitch, float roll)
	{
		yaw = (float)Pi2 - yaw;
		Matrix3 mtr;
		float siny = sin(yaw);
		float cosy = cos(yaw);
		float sinp = sin(pitch);
		float cosp = cos(pitch);
		float sinr = sin(roll);
		float cosr = cos(roll);

		mtr[0][0] =  cosy*cosp;						mtr[0][1] =  sinp;			mtr[0][2] =  siny*cosp;
		mtr[1][0] = -siny*sinr - cosy*sinp*cosr; 	mtr[1][1] =  cosp*cosr;		mtr[1][2] =  cosy*sinr - siny*sinp*cosr;
		mtr[2][0] =  cosy*sinp*sinr - siny*cosr;	mtr[2][1] = -cosp*sinr;		mtr[2][2] =  cosy*cosr + siny*sinp*sinr;

		return mtr;
	}


	// проверка валидности:
	inline void checkValid(double &v)
	{
#ifdef _ED_HIDDEN_DEBUG 
		if(!_finite(v))
		{
			assert(!"invalid double");
			v = 0.f;
		}
#endif
	}
	// проверка валидности:
	inline void checkValid(float &v)
	{
#ifdef _ED_HIDDEN_DEBUG 
		if(!_finite(v))
		{
			assert(!"invalid float");
			v = 0.f;
		}
#endif
	}

	inline void checkValid(Vec3 &V)
	{
#ifdef _ED_HIDDEN_DEBUG 
		checkValid(V.x);
		checkValid(V.y);
		checkValid(V.z);
#endif
	}
	inline void checkValid(Math::Vec3f &V)
	{
#ifdef _ED_HIDDEN_DEBUG 
		checkValid(V.x);
		checkValid(V.y);
		checkValid(V.z);
#endif
	}

	template<typename T>
	typename T::value_type calc_poly(const T& poly, typename T::value_type x)
	{
		if(poly.empty()) return T::value_type(0.0);

		typename T::value_type res = poly[0];

		for(size_t i = 1 ; i < poly.size(); ++i)
		{
			res = res*x + poly[i];
		}

		return res;
	}

	//template<typename T> - если понадобится
	inline double interpolate(const double* x, const double* f, unsigned sz, double t, double* k = 0)
	{
		for(int i = 0; i < sz; i++)
		{
			if(t <= x[i] || i == sz - 1)
			{
				if(i > 0)
				{
					//return (f[i]*(t - x[i-1]) + f[i-1]*(x[i] - t))/(x[i] - x[i-1]);
					double k_ = (f[i] - f[i-1])/(x[i] - x[i-1]);
					if(k)
						*k = k_;
					return f[i-1] + k_*(t - x[i-1]);
				}
				return f[0];
			}
		}
		return 0.0;
	}

}

#endif
